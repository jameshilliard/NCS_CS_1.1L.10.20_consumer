#include <linux/module.h>
#include <linux/interrupt.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>

#include "nl_kernel.h"

#define MILISECONDS_TO_JIFFIES(x) ((x*HZ)/1000)
#define JIFFIES_TO_MILISECONDS(x) ((x*1000)/HZ)

typedef struct _led_info {
    struct timer_list blink_timer;
    struct timer_list logic_timer;
    unsigned int duration_on;
    unsigned int duration_off;
    unsigned int duration_stop;
    unsigned int blink_cycles;
    unsigned int mode   :8;
    unsigned int current_state  :1;//0-off, 1-on
    unsigned int reserved_state :1;//0-off, 1-on
    unsigned int stop   :1;//0-blink, 1-stop
    unsigned int initialized    :1;
} led_info;

typedef struct _button_info {
    struct timer_list release_timer;
    unsigned int initialized    :1;
    nl_button_state state;
} button_info;

typedef struct _wifi_event_node {
    wifi_event_info info;
    struct list_head list;
} wifi_event_node_t;

static struct sock *g_nl_sk = NULL;
static unsigned int g_nl_pid = 0;
static unsigned int g_nl_group = 1;
static led_op_cb g_led_op_cb = {0};
static led_info g_led_info[NL_LED_COUNT] = {
    [NL_LED_ID_WPS_GREEN]={.mode=NL_LED_OFF,.duration_on=0,.duration_off=0,.duration_stop=0,.blink_cycles=0,.initialized=0},
    [NL_LED_ID_WPS_RED]={.mode=NL_LED_OFF,.duration_on=0,.duration_off=0,.duration_stop=0,.blink_cycles=0,.initialized=0},
    [NL_LED_ID_2G]={.mode=NL_LED_OFF,.duration_on=0,.duration_off=0,.duration_stop=0,.blink_cycles=0,.initialized=0},
    [NL_LED_ID_5G]={.mode=NL_LED_OFF,.duration_on=0,.duration_off=0,.duration_stop=0,.blink_cycles=0,.initialized=0},
};
static nl_button_id g_current_button_id = NL_BUTTON_COUNT;
static button_op_cb g_button_op_cb = {0};
static button_info g_button_info[NL_BUTTON_COUNT] = {
    [NL_BUTTON_ID_RESET]    ={.initialized=0},
    [NL_BUTTON_ID_WPS]      ={.initialized=0},
};
static spinlock_t wifi_lock;
static LIST_HEAD(wifi_event_list);

static int nl_get_user_address(struct sock **sk, unsigned int *pid, unsigned int *group)
{
    if(NULL==g_nl_sk)   return -1;

    *sk = g_nl_sk;
    *pid = g_nl_pid;
    *group = g_nl_group;

    return 0;
}

static int send_to_user(unsigned char type, unsigned char subtype, void* data, unsigned short len)
{
    int ret = 0;
    struct sock *nl_sk = NULL;
    unsigned int nl_pid = 0;
    unsigned int nl_group = 0;
    struct nlmsghdr *nlh = NULL;
    unsigned char *p = NULL;
    struct sk_buff *skb = NULL;
    static unsigned int seq = 0;
    tlv_hdr tlv = {type, subtype, len};
    unsigned int size = NLMSG_SPACE(len+sizeof(tlv_hdr)+sizeof(struct nlmsghdr));
    gfp_t allocation = GFP_ATOMIC;

    do {
        if(nl_get_user_address(&nl_sk, &nl_pid, &nl_group)) {
            ret = -1;
            break;
        }

        if(nl_group) {
            if (!netlink_has_listeners(nl_sk, nl_group)) {
                ret = -ESRCH;
                break;
            }
        }

        skb = alloc_skb(size, allocation);
        if(!skb) {
            ret = -ENOMEM;
            break;
        }

        nlh = NLMSG_PUT(skb, 0, seq++, NLMSG_DONE, size-sizeof(struct nlmsghdr));

        p = NLMSG_DATA(nlh);

        memcpy(p, &tlv, sizeof(tlv));
        if(NULL!=data&&len>0) {
            memcpy(p+sizeof(tlv), data, len);
        }

        NETLINK_CB(skb).pid = 0;    //from kernel
        NETLINK_CB(skb).dst_group = nl_group;

        if(nl_group) {
            ret = netlink_broadcast(nl_sk, skb, 0, nl_group, allocation);
        } else {
            ret = netlink_unicast(nl_sk, skb, nl_pid, MSG_DONTWAIT);
        }
    } while(0);

    return ret;

nlmsg_failure:
    kfree_skb(skb);
    return -EINVAL;

}

static void set_led(unsigned int led_id, unsigned int mode,
                    unsigned int on_ms, unsigned int off_ms, unsigned int cycles, unsigned int stop_ms)
{   //set led mode
    const int min_interval = 50;
#ifdef _DEBUG
    printk("\n==>\t led_id=%d, mode=%d, on:%d, off:%d, cycles=%d, stop_ms=%d\n", led_id, mode, on_ms, off_ms, cycles, stop_ms);
#endif
    if(led_id>=NL_LED_COUNT) return;
    if(!g_led_info[led_id].initialized) return;

    switch(mode) {
    case NL_LED_OFF://off
        del_timer(&g_led_info[led_id].logic_timer);
        if(off_ms) {
            g_led_info[led_id].duration_off = MILISECONDS_TO_JIFFIES(off_ms);
            mod_timer(&g_led_info[led_id].blink_timer, jiffies + g_led_info[led_id].duration_off);
        } else {
            del_timer(&g_led_info[led_id].blink_timer);
        }
        g_led_info[led_id].mode = mode;
        g_led_info[led_id].current_state = LED_STATE_OFF;
        g_led_op_cb.set_led_gpio(led_id, g_led_info[led_id].current_state);
        return;
    case NL_LED_ON://on
        del_timer(&g_led_info[led_id].logic_timer);
        if(on_ms) {
            g_led_info[led_id].duration_on = MILISECONDS_TO_JIFFIES(on_ms);
            mod_timer(&g_led_info[led_id].blink_timer, jiffies + g_led_info[led_id].duration_on);
        } else {
            del_timer(&g_led_info[led_id].blink_timer);
        }
        g_led_info[led_id].mode = mode;
        g_led_info[led_id].current_state = LED_STATE_ON;
        g_led_op_cb.set_led_gpio(led_id, g_led_info[led_id].current_state);
        return;
    case NL_LED_BLINK://blink
        if(on_ms<min_interval) {
            on_ms = min_interval;
        }
        if(off_ms<min_interval) {
            off_ms = min_interval;
        }
        if(cycles) {
            mod_timer(&g_led_info[led_id].logic_timer, jiffies
                      + (g_led_info[led_id].duration_on+g_led_info[led_id].duration_off)*cycles);
        } else {
            del_timer(&g_led_info[led_id].logic_timer);
        }
        g_led_info[led_id].mode = mode;
        g_led_info[led_id].reserved_state = g_led_info[led_id].current_state;
        g_led_info[led_id].blink_cycles = cycles;
        g_led_info[led_id].duration_on = MILISECONDS_TO_JIFFIES(on_ms);
        g_led_info[led_id].duration_off = MILISECONDS_TO_JIFFIES(off_ms);
        g_led_info[led_id].duration_stop = 0;
        mod_timer(&g_led_info[led_id].blink_timer, jiffies);
        return;
    case NL_LED_BLINK_STOP://blink-stop, blink-stop, ...
        if(!cycles || !stop_ms) {
            del_timer(&g_led_info[led_id].logic_timer);
            return;
        }
        if(on_ms<min_interval) {
            on_ms = min_interval;
        }
        if(off_ms<min_interval) {
            off_ms = min_interval;
        }
        g_led_info[led_id].mode = mode;
        g_led_info[led_id].reserved_state = g_led_info[led_id].current_state;
        g_led_info[led_id].blink_cycles = cycles;
        g_led_info[led_id].duration_on = MILISECONDS_TO_JIFFIES(on_ms);
        g_led_info[led_id].duration_off = MILISECONDS_TO_JIFFIES(off_ms);
        g_led_info[led_id].duration_stop = MILISECONDS_TO_JIFFIES(stop_ms);
        g_led_info[led_id].stop = 0;
        mod_timer(&g_led_info[led_id].logic_timer, jiffies
                  + (g_led_info[led_id].duration_on+g_led_info[led_id].duration_off)*cycles);
        mod_timer(&g_led_info[led_id].blink_timer, jiffies);
        break;
    default:
        return;
    }

}

void wlan_set_2g_led(unsigned int mode, unsigned int on_ms, unsigned int off_ms, unsigned int cycles)
{
    set_led(NL_LED_ID_2G, mode, on_ms, off_ms, cycles, 0);
}
EXPORT_SYMBOL(wlan_set_2g_led);

void wlan_set_5g_led(unsigned int mode, unsigned int on_ms, unsigned int off_ms, unsigned int cycles)
{
    set_led(NL_LED_ID_5G, mode, on_ms, off_ms, cycles, 0);
}
EXPORT_SYMBOL(wlan_set_5g_led);

static int parse_payload(unsigned char type, unsigned char subtype, void* data, unsigned short len)
{
    int ret = 0;
    unsigned char* p = data;
    unsigned char mode=0;
    unsigned int on_ms=0;
    unsigned int off_ms=0;
    unsigned int cycles=0;
    unsigned int stop_ms=0;

    switch(type) {
    case NL_TYPE_GROUPS:
        g_nl_group = subtype;
        break;
    case NL_TYPE_LED:
        memcpy(&mode, p, sizeof(unsigned char));
        p += sizeof(unsigned char);
        memcpy(&on_ms, p, sizeof(unsigned int));
        p += sizeof(unsigned int);
        memcpy(&off_ms, p, sizeof(unsigned int));
        p += sizeof(unsigned int);
        memcpy(&cycles, p, sizeof(unsigned int));
        p += sizeof(unsigned int);
        memcpy(&stop_ms, p, sizeof(unsigned int));
        p += sizeof(unsigned int);

        set_led(subtype, mode, on_ms, off_ms, cycles, stop_ms);
        break;
    default:
    case NL_TYPE_BUTTON:
    case NL_TYPE_WIFI_EVENT:
#ifdef _DEBUG
        printk("[%s:%d]netlink attribute type [%d] is not supported.\n", __func__, __LINE__, type);
#endif
        ret = -1;
        break;
    }

    return ret;
}

static void nl_recv_msg(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = NULL;
    tlv_hdr *tlv = NULL;

    if(!skb) return;

    nlh = (struct nlmsghdr*)skb->data;
    if(!nlh) return;

    g_nl_pid = nlh->nlmsg_pid;

    tlv = (tlv_hdr*)nlmsg_data(nlh);

    parse_payload(tlv->type, tlv->subtype, tlv->data, tlv->len);
}

static void led_logic_timeout(unsigned long led_id)
{
#ifdef _DEBUG
    printk("\n==> \t led=%d, mode=%d, stop=%d\n", led_id, g_led_info[led_id].mode, g_led_info[led_id].stop);
#endif
    switch(g_led_info[led_id].mode) {
    case NL_LED_BLINK://used to control led to end temporary blinking state
        del_timer(&g_led_info[led_id].blink_timer);
        g_led_op_cb.set_led_gpio(led_id, g_led_info[led_id].reserved_state);
        break;
    case NL_LED_BLINK_STOP:
        g_led_info[led_id].stop = !g_led_info[led_id].stop;
        if(g_led_info[led_id].stop) { //led works from blink to stop mode
            del_timer(&g_led_info[led_id].blink_timer);
            g_led_op_cb.set_led_gpio(led_id, g_led_info[led_id].reserved_state);
            mod_timer(&g_led_info[led_id].logic_timer, jiffies + g_led_info[led_id].duration_stop);
        } else { //led works from stop to blink mode
            mod_timer(&g_led_info[led_id].logic_timer, jiffies
                      + (g_led_info[led_id].duration_on+g_led_info[led_id].duration_off)
                      *g_led_info[led_id].blink_cycles);
            mod_timer(&g_led_info[led_id].blink_timer, jiffies);
        }
        break;
    default:
        break;
    }
}

static void led_blink_timeout(unsigned long led_id)
{   //used to control led blinking (on-off-on-off-...)
    if(NL_LED_ON==g_led_info[led_id].mode) {
        if(g_led_info[led_id].duration_on) {
            g_led_info[led_id].current_state = LED_STATE_OFF;
            g_led_op_cb.set_led_gpio(led_id, g_led_info[led_id].current_state);
        }
    } else if(NL_LED_OFF==g_led_info[led_id].mode) {
        if(g_led_info[led_id].duration_off) {
            g_led_info[led_id].current_state = LED_STATE_ON;
            g_led_op_cb.set_led_gpio(led_id, g_led_info[led_id].current_state);
        }
    } else if(NL_LED_BLINK==g_led_info[led_id].mode
              ||NL_LED_BLINK_STOP==g_led_info[led_id].mode) {
        g_led_info[led_id].current_state = !(g_led_info[led_id].current_state);
        if(g_led_info[led_id].current_state) {
            mod_timer(&g_led_info[led_id].blink_timer, jiffies + g_led_info[led_id].duration_on);
        } else {
            mod_timer(&g_led_info[led_id].blink_timer, jiffies + g_led_info[led_id].duration_off);
        }
        g_led_op_cb.set_led_gpio(led_id, g_led_info[led_id].current_state);
    } else { //do nothing
    }
}

static void init_led(unsigned int led_id)
{
    if(!g_led_op_cb.set_led_gpio) return;

    init_timer(&g_led_info[led_id].blink_timer);
    g_led_info[led_id].blink_timer.data = led_id;
    g_led_info[led_id].blink_timer.function = &led_blink_timeout;

    init_timer(&g_led_info[led_id].logic_timer);
    g_led_info[led_id].logic_timer.data = led_id;
    g_led_info[led_id].logic_timer.function = &led_logic_timeout;

    g_led_info[led_id].initialized = 1;

    set_led(led_id, g_led_info[led_id].mode, g_led_info[led_id].duration_on, g_led_info[led_id].duration_off, g_led_info[led_id].blink_cycles, g_led_info[led_id].duration_stop);
}

void act_set_led_callback(led_op_cb* op_cb)
{
    if(op_cb) {
        memcpy(&g_led_op_cb, op_cb, sizeof(g_led_op_cb));
    }
}
EXPORT_SYMBOL(act_set_led_callback);

void act_set_button_callback(button_op_cb* op_cb)
{
    if(op_cb) {
        memcpy(&g_button_op_cb, op_cb, sizeof(g_button_op_cb));
    }
}
EXPORT_SYMBOL(act_set_button_callback);

static void button_release_timer_func(unsigned long button_id)
{
    if(NL_BUTTON_KEYUP==g_button_op_cb.get_button_gpio(button_id)) {
        act_button_notify(button_id, NL_BUTTON_KEYUP);
    } else { //NL_BUTTON_KEYDOWN
        mod_timer(&g_button_info[button_id].release_timer, jiffies+MILISECONDS_TO_JIFFIES(10));
    }
}

static void init_button(unsigned int button_id)
{
    if(!g_button_op_cb.get_button_gpio) return;

    init_timer(&g_button_info[button_id].release_timer);
    g_button_info[button_id].release_timer.data = button_id;
    g_button_info[button_id].release_timer.function = &button_release_timer_func;

    g_button_info[button_id].initialized = 1;
}

void do_button_tasklet(unsigned long data)
{
	unsigned char b = g_button_info[g_current_button_id].state;
    send_to_user(NL_TYPE_BUTTON, g_current_button_id, &b, 1);
}

DECLARE_TASKLET(button_tasklet, do_button_tasklet, 0);

void act_button_notify(nl_button_id button_id, nl_button_state button_state)
{
    if(button_id>=NL_BUTTON_COUNT) return;
    if(button_state==g_button_info[button_id].state) return;
    g_current_button_id = button_id;
    g_button_info[button_id].state = button_state;
    tasklet_schedule(&button_tasklet);
    if(NL_BUTTON_KEYDOWN==button_state) {
        mod_timer(&g_button_info[button_id].release_timer, jiffies+MILISECONDS_TO_JIFFIES(10));
    }
}
EXPORT_SYMBOL(act_button_notify);

void do_wifi_tasklet(unsigned long data)
{
    wifi_event_node_t *node = NULL;
    wifi_event_node_t *prev = NULL;

    spin_lock(&wifi_lock);
    list_for_each_entry(node, &wifi_event_list, list) {
        if(prev) {
            list_del(&prev->list);
            kfree(prev);
            prev=NULL;
        }
        send_to_user(NL_TYPE_WIFI_EVENT, node->info.event, &node->info, sizeof(wifi_event_info));
        prev = node;
    }
    if(prev) {
        list_del(&prev->list);
        kfree(prev);
        prev=NULL;
    }
    spin_unlock(&wifi_lock);
}

DECLARE_TASKLET(wifi_tasklet, do_wifi_tasklet, 0);

void act_wifi_notify(wifi_event_info* event_info)
{
    wifi_event_node_t *node = NULL;
    spin_lock(&wifi_lock);
    node = kmalloc(sizeof(wifi_event_node_t), GFP_KERNEL);
    if(node) {
        memcpy(&node->info, event_info, sizeof(wifi_event_info));
        INIT_LIST_HEAD(&node->list);
        list_add(&node->list, &wifi_event_list);
    }
    spin_unlock(&wifi_lock);
    tasklet_schedule(&wifi_tasklet);
}
EXPORT_SYMBOL(act_wifi_notify);


static int __init actiontec_init(void)
{
    unsigned int led_id = 0;
    unsigned int button_id = 0;

    printk("actiontec netlink_kernel module loaded.\n");

    g_nl_sk = netlink_kernel_create(NULL, NETLINK_USER, 0, nl_recv_msg, NULL, THIS_MODULE);
    if(!g_nl_sk) {
        printk(KERN_ALERT "Error creating socket.\n");
        return -1;
    }

    for(led_id=0; led_id<NL_LED_COUNT; led_id++)    init_led(led_id);
    for(button_id=0; button_id<NL_BUTTON_COUNT; button_id++)    init_button(button_id);

    spin_lock_init(&wifi_lock);

    return 0;
}

static void __exit actiontec_exit(void)
{
    unsigned int led_id = 0;
    unsigned int button_id = 0;

    for(led_id=0; led_id<NL_LED_COUNT; led_id++) {
        if(g_led_info[led_id].initialized) {
            del_timer(&g_led_info[led_id].blink_timer);
            del_timer(&g_led_info[led_id].logic_timer);
        }
    }
    for(button_id=0; button_id<NL_BUTTON_COUNT; button_id++) {
        if(g_button_info[button_id].initialized) {
            del_timer(&g_button_info[button_id].release_timer);
        }
    }

    netlink_kernel_release(g_nl_sk);
    printk("actiontec netlink_kernel module exit.\n");
}

module_init(actiontec_init);
module_exit(actiontec_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Denis Gong");
MODULE_DESCRIPTION("actiontec netlink_kernel module used to communicate with userspace program");


