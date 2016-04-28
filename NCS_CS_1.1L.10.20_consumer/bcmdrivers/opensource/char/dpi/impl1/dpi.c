#if defined(CONFIG_BCM_KF_DPI) && defined(CONFIG_BRCM_DPI)
/*
<:copyright-BRCM:2014:DUAL/GPL:standard 

   Copyright (c) 2014 Broadcom Corporation
   All Rights Reserved

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation (the "GPL").

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

:>
*/
/******************************************************************************
 Filename:       dpi.c
*****************************************************************************/

#include <linux/module.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/if_ether.h>

#include <linux/devinfo.h>
#include <linux/urlinfo.h>
#include <linux/dpi_engine.h>
#include <linux/dpi_appdb.h>
#include <linux/dpi_ctk.h>

#if defined(CONFIG_BCM_KF_BLOG)
#include <linux/blog.h>
#endif

static struct nf_hook_ops forward_hook_ops_fwfilter;
static struct nf_hook_ops forward_hook_ops_infirst;



#if defined(CC_DPI_SUPPORT_DEBUG)
#include <linux/bcm_colors.h>
#define dpi_print(fmt, arg...)                                           \
    if ( dpi_dbg )                                                       \
        printk( CLRc "DPI %s :" fmt CLRnl, __FUNCTION__, ##arg )
#define dpi_assertv(cond)                                                \
    if ( !cond ) {                                                       \
        printk( CLRerr "DPI ASSERT %s : " #cond CLRnl, __FUNCTION__ );   \
        return;                                                          \
    }
#define dpi_assertr(cond, rtn)                                           \
    if ( !cond ) {                                                       \
        printk( CLRerr "DPI ASSERT %s : " #cond CLRnl, __FUNCTION__ );   \
        return rtn;                                                      \
    }
#define DPI_DBG(debug_code)    do { debug_code } while(0)
#else
#define dpi_print(fmt, arg...) DPI_NULL_STMT
#define dpi_assertv(cond) DPI_NULL_STMT
#define dpi_assertr(cond, rtn) DPI_NULL_STMT
#define DPI_DBG(debug_code) DPI_NULL_STMT
#endif

int dpi_dbg = 0;

inline unsigned int conntrack_get_appid( const struct sk_buff *skb )
{
    return (((struct nf_conn *)skb->nfct)->dpi.app_id);
}

inline void conntrack_set_appid( struct sk_buff *skb, unsigned int id )
{
    (((struct nf_conn *)skb->nfct)->dpi.app_id) = id;
}

inline uint16_t conntrack_get_dpi_status( const struct sk_buff *skb,
                                          uint16_t mask )
{
    return (((((struct nf_conn *)skb->nfct)->dpi.flags) & mask) == mask);
}

inline void conntrack_set_dpi_status( struct sk_buff *skb, uint16_t status )
{
    (((struct nf_conn *)skb->nfct)->dpi.flags) |= status;
}

inline uint16_t conntrack_get_devid( const struct sk_buff *skb )
{
    return (((struct nf_conn *)skb->nfct)->dpi.dev_key);
}

inline void conntrack_set_devid( struct sk_buff *skb, uint16_t id )
{
    (((struct nf_conn *)skb->nfct)->dpi.dev_key) = id;
}

inline void conntrack_set_urlid( struct sk_buff *skb, uint16_t id )
{
    (((struct nf_conn *)skb->nfct)->dpi.url_id) = id;
}

static inline void devid_in_handle( uint16_t dev_idx, 
                                    struct sw_struct *param )
{
    dpi_print("devIdx<%u> SW_DEVID<%d>", dev_idx, param->type & SW_DEVID);

    if (param->type & SW_DEVID)
    {
        DevInfoEntry_t entry;

        devinfo_get(dev_idx, &entry);

        if (entry.flags != DEVINFO_DONE)
        {
            entry.vendor_id = param->u.s.devid.vendor_id;
            entry.os_id = param->u.s.devid.name_id;
            entry.class_id = param->u.s.devid.class_id;
            entry.type_id = param->u.s.devid.cat_id;
            entry.flags = DEVINFO_DONE;

            devinfo_set(&entry);
        }
    }

    return;
}

static inline void devid_fw_handle( struct sk_buff *skb, 
                                    struct sw_struct *param )
{
    /* device identification is only for LAN device */
    if (!conntrack_get_dpi_status(skb, DEVID_STATUS_NOMORE) &&
        !(skb->dev->priv_flags & IFF_WANDEV))
    {
        uint16_t dev_idx;
        DevInfoEntry_t entry;

        dpi_print("IN: nfct<0x%08x> status<0x%08x> devID<%u> SW_DEVID<%u>",
                  (int)skb->nfct, ((struct nf_conn *)skb->nfct)->dpi.flags,
                  (((struct nf_conn *)skb->nfct)->dpi.dev_key),
                  param->type & SW_DEVID);

        dev_idx = conntrack_get_devid(skb);

        if (dev_idx == DEVINFO_IX_INVALID)
        {
            uint8_t *ptr = eth_hdr(skb)->h_source;

            dpi_print("mac<%02x:%02x:%02x:%02x:%02x:%02x>",
                      ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);

            dev_idx = devinfo_lookup(ptr);

            if (dev_idx != DEVINFO_IX_INVALID)
                conntrack_set_devid(skb, dev_idx);
            else
            {
                dpi_print("fail to alloc devid?");
                return;
            }
        }

        devinfo_get(dev_idx, &entry);

        dpi_print("Got entry: idx<%u> flag<%u>", entry.idx, entry.flags);
        if (entry.flags == DEVINFO_DONE)
        {
            conntrack_set_dpi_status(skb, DEVID_STATUS_NOMORE);
            dpi_print(" -> status<%08x>",
                      ntohl(((struct nf_conn *)skb->nfct)->dpi.flags));
            return;
        }
        else
        {
            if (param->type & SW_DEVID)
            {
                entry.vendor_id = param->u.s.devid.vendor_id;
                entry.os_id = param->u.s.devid.name_id;
                entry.class_id = param->u.s.devid.class_id;
                entry.type_id = param->u.s.devid.cat_id;
                entry.flags = DEVINFO_DONE;

                devinfo_set(&entry);
            
                conntrack_set_dpi_status(skb, DEVID_STATUS_NOMORE);

                dpi_print(" -> status<%08x>\n"
                          "    vendor<%u> os<%u> class<%u> type<%u>",
                          ntohl(((struct nf_conn *)skb->nfct)->dpi.flags),
                          entry.vendor_id, entry.os_id,
                          entry.class_id, entry.type_id);

                return;
            }
        }
    }
}

static inline void dpi_result_update( struct sk_buff *skb, 
                                      struct sw_struct *param )
{
    uint32_t behinst;

    if (param->type & SW_APP)
    {
        /*
         * application classification result.
         */
        uint32_t ct_appid;

#if 0
        behinst = appdb_get_behinst_id(param->u.s.app.cat_id,
                  param->u.s.app.app_id, param->u.s.app.beh_id);
#else
        behinst = appdb_get_appinst_id(param->u.s.app.cat_id,
                  param->u.s.app.app_id);
#endif

        ct_appid = conntrack_get_appid(skb);
        if (ct_appid != behinst)
        {
            if (appdb_beh2appinst(behinst) != appdb_beh2appinst(ct_appid))
            {
                /* app id of this ctk entry is changed. Add a resync bit. */
                dpi_print("appid: from 0x%08x to 0x%08x", ct_appid, behinst);
            }

            conntrack_set_appid(skb, behinst);
        }

        conntrack_set_dpi_status(skb, APPID_STATUS_IDENTIFIED);

        if (param->flags & SW_FG_FINAL)
        {
            conntrack_set_dpi_status(skb, APPID_STATUS_FINAL);
        }

#ifdef DPI_URL_RECORD
        /* url record */
        if (param->u.s.url.domain && param->u.s.url.domain_len > 0)
        {
            if (param->u.s.url.domain_len > URLINFO_MAX_HOST_LEN)
            {
                printk("URL<%s> is greater than %d\n", 
                       param->u.s.url.domain, param->u.s.url.domain_len);
            }
            else
            {
                UrlInfoEntry_t url;
                uint16_t url_idx;

                url.hostlen = param->u.s.url.domain_len;
                strncpy(url.host, param->u.s.url.domain, 
                        param->u.s.url.domain_len);

                url_idx = urlinfo_lookup(&url);

                if (url_idx != URLINFO_IX_INVALID)
                    conntrack_set_urlid(skb, url_idx);
                else
                    dpi_print("fail to alloc urlidx?");
            }
        }

#if 0  /* Now we only record host. We may record more in the future */
        if (param->u.s.url.path && param->u.s.url.path_len > 0)
        {
           char url[1024]={0};

           strncpy(url, param->u.s.url.path, param->u.s.url.path_len);
           printk("!!path(%d): %s\n", param->u.s.url.path_len, url);
        }

        if (param->u.s.url.referer && param->u.s.url.referer_len > 0)
        {
           char url[1024]={0};

           strncpy(url, param->u.s.url.referer, param->u.s.url.referer_len);
           printk("##referer(%d): %s\n\n", param->u.s.url.referer_len, url);
        }
#endif
#endif

#if 0
        if ((param->flags & SW_FG_NOMORE) ||
            conntrack_max_dpi_pkt((struct nf_conn *)skb->nfct, DPI_MAX_PKT))
#else
        if (param->flags & SW_FG_NOMORE)
#endif
        {
            conntrack_set_dpi_status(skb, APPID_STATUS_NOMORE);

            dpi_print("  Cat:%u(%s) App:%u(%s) Beh:%u(%s)\n"
                      "  Final<%d> Nomore<%d> NOTIA<%d> APPID<%u> status<%08x>",
                    param->u.s.app.cat_id, param->u.s.app.cat_name,
                    param->u.s.app.app_id, param->u.s.app.app_name,
                    param->u.s.app.beh_id, param->u.s.app.beh_name,
                    (param->flags & SW_FG_FINAL),
                    (param->flags & SW_FG_NOMORE),
                    (param->flags & SW_FG_NOTIA),
                    ((struct nf_conn *)skb->nfct)->dpi.app_id,
                    ntohl(((struct nf_conn *)skb->nfct)->dpi.flags));
        }
        else
        {
#if defined(CONFIG_BCM_KF_BLOG)
            blog_skip(skb);
#endif
        }
    }
    else
    {
        if (!conntrack_get_dpi_status(skb, APPID_STATUS_IDENTIFIED))
        {
            /*
             * Unknown app.
             */
            dpi_print("unknown app");
            conntrack_set_appid(skb, appdb_get_unknown_behinst_id());
        }

#if 0
        if ((param->flags & SW_FG_NOTIA) || (param->flags & SW_FG_NOMORE) || 
             conntrack_max_dpi_pkt((struct nf_conn *)skb->nfct, DPI_MAX_PKT))
#else
        if ((param->flags & SW_FG_NOTIA) || (param->flags & SW_FG_NOMORE)) 
#endif
        {
            conntrack_set_dpi_status(skb, APPID_STATUS_NOMORE);
            dpi_print("  App NOMORE: APPID<%u> status<%08x>",
                      ((struct nf_conn *)skb->nfct)->dpi.app_id,
                      ntohl(((struct nf_conn *)skb->nfct)->dpi.flags));
        }
        else
        {
#if defined(CONFIG_BCM_KF_BLOG)
            blog_skip(skb);
#endif
        }
    }

    devid_fw_handle(skb, param);
}

static inline void dpi_classify( struct sk_buff *skb )
{
    extern int ips_check(struct sw_struct *param);

    int ret;
    struct sw_struct param;

    memset(&param, 0x00, sizeof(param));

    param.skb = skb;
	
    if (conntrack_get_dpi_status(skb, DEVID_STATUS_NOMORE))
        param.req_flag = SW_APP;
    else
        param.req_flag = SW_APP | SW_DEVID;

    ret = ips_check(&param);
    switch (ret)
    {
    case BW_OK:
        dpi_result_update(skb, &param);
        break;
    case BW_ERR:
        dpi_print("Cannot scan this packet");
        break;
    default:
        dpi_print("Unexpected return value (fixme)");
        break;
    }
}

static inline void devid_classify( uint16_t dev_idx, struct sk_buff *skb )
{
	extern int ips_check(struct sw_struct *param);

	int ret;
	struct sw_struct param;

	memset(&param, 0x00, sizeof(param));

	param.skb = skb;
	param.req_flag = SW_DEVID;

	ret = ips_check(&param);
	switch (ret)
	{
	case BW_OK:
		devid_in_handle(dev_idx, &param);
		break;
	case BW_ERR:
		dpi_print("Cannot scan this packet");
		break;
	case BW_QUEUE:
		/* Not possible in this project. */
		dpi_print("Packet stolen (Not possible, fixme)");
		break;
	default:
		// do nothing
		dpi_print("Unexpected return value (fixme)");
		break;
	}
}

static int is_udp_pkt( const struct sk_buff *skb )
{
    switch (ntohs(skb->protocol))
    {
        case ETH_P_IP:
            return (ip_hdr(skb)->protocol == IPPROTO_UDP);
        case ETH_P_IPV6:    //FIXME: parse extention header?
            return (ipv6_hdr(skb)->nexthdr == IPPROTO_UDP);
        default:
            break;
    }

    return 0;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24))
static uint32_t forward_hook_local_devid(
        uint32_t hooknum, struct sk_buff *skb, const struct net_device *in_dev,
        const struct net_device *out_dev, int (*okfn)(struct sk_buff *) )
#else
uint32_t forward_hook_local( 
        uint32_t hooknum, struct sk_buff **pskb,
        const struct net_device *in_dev, const struct net_device *out_dev,
        int (*okfn)(struct sk_buff *))
#endif
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
	struct sk_buff *skb = *pskb;
#endif

	if (unlikely(skb == NULL || skb->dev == NULL))
	{
		dpi_assertr((skb != NULL), NF_ACCEPT); /* If skb is NULL, let's panic */
		return NF_ACCEPT;
	}

    /* Assume only UDP packets (DHCP) contain device info */
    if (!(skb->dev->priv_flags & IFF_WANDEV) && is_udp_pkt(skb))
	{
        uint16_t dev_idx = devinfo_lookup(eth_hdr(skb)->h_source);
        DevInfoEntry_t entry;

        if (dev_idx == DEVINFO_IX_INVALID)
        {
            dpi_print("fail to alloc devid?");
            return NF_ACCEPT;
        }

        devinfo_get(dev_idx, &entry);

        if (entry.flags != DEVINFO_DONE)
		    devid_classify(dev_idx, skb);
	}

	return NF_ACCEPT;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24))
static uint32_t forward_hook_fwfilter( 
        uint32_t hooknum, struct sk_buff *skb, const struct net_device *in_dev,
        const struct net_device *out_dev, int (*okfn)(struct sk_buff *) )
#else
uint32_t forward_hook_fwfilter(	
        uint32_t hooknum, struct sk_buff **pskb,
        const struct net_device *in_dev, const struct net_device *out_dev,
        int (*okfn)(struct sk_buff *))
#endif
{
    uint32_t verdict = NF_ACCEPT;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
    struct sk_buff *skb = *pskb;
#endif

    if (unlikely(skb == NULL || skb->dev == NULL || skb->nfct == NULL))
    {
		dpi_assertr((skb != NULL), NF_ACCEPT); /* If skb is NULL, let's panic */
        return NF_ACCEPT;
    }

    /* 
     * Assumption: For one conntrack, device identification should be done
     * before application identification
     */
    if (likely(!conntrack_get_dpi_status(skb, APPID_STATUS_NOMORE)))
    {
        dpi_classify(skb);
    }
    else if ((conntrack_get_devid(skb) == DEVINFO_IX_INVALID) &&
             !(skb->dev->priv_flags & IFF_WANDEV))
    {
        uint16_t dev_idx;
        uint8_t *ptr = eth_hdr(skb)->h_source;

        dpi_print("found mac after APP_NOMORE<%02x:%02x:%02x:%02x:%02x:%02x>",
                   ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);

        dev_idx = devinfo_lookup(ptr);

        if (dev_idx != DEVINFO_IX_INVALID)
            conntrack_set_devid(skb, dev_idx);
        else
        {
            dpi_print("fail to alloc devid?");
        }
    }

    return verdict;
}

static int __register_hook( struct nf_hook_ops *ops, unsigned int hooknum, 
                            u_int8_t pf, int prio, nf_hookfn *cb )
{
	memset(ops, 0x00, sizeof(*ops));

	ops->hooknum = hooknum;
	ops->pf = pf;
	ops->priority = prio;

	ops->hook = (nf_hookfn *) cb;

	if (nf_register_hook(ops) != 0)
	{
		return -1;
	}

	return 0;
}

static int __init dpi_init( void )
{
    DPI_DBG( printk( "DPI dpi_dbg<0x%08x> = %d\n", (int)&dpi_dbg, dpi_dbg ); );

    if (__register_hook(&forward_hook_ops_fwfilter,
        NF_INET_FORWARD, PF_INET, NF_IP_PRI_FILTER, forward_hook_fwfilter) < 0)
    {
        dpi_print("Cannot register forward_hook_ops_fwfilter");
        return -1;
    }

   	if (__register_hook(&forward_hook_ops_infirst,
		NF_INET_LOCAL_IN, PF_INET, NF_IP_PRI_FILTER,
        forward_hook_local_devid) < 0)
	{
		dpi_print("Cannot register forward_hook_ops_infirst");
		return -1;
	}

    devinfo_init();
    urlinfo_init();

    return 0;
}

static void __exit dpi_fnit(void)
{
    nf_unregister_hook(&forward_hook_ops_fwfilter);
    nf_unregister_hook(&forward_hook_ops_infirst);
}

module_init(dpi_init);
module_exit(dpi_fnit);

MODULE_LICENSE("GPL");
#endif /*if defined(CONFIG_BCM_KF_DPI) && defined(CONFIG_BRCM_DPI) */
