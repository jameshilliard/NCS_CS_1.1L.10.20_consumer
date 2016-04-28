/*
* <:copyright-BRCM:2013:DUAL/GPL:standard
* 
*    Copyright (c) 2013 Broadcom Corporation
*    All Rights Reserved
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License, version 2, as published by
* the Free Software Foundation (the "GPL").
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* 
* A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
* writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
* 
* :>
*/
/*
 * ucast object header file.
 * This header file is generated automatically. Do not edit!
 */
#ifndef _RDPA_AG_UCAST_H_
#define _RDPA_AG_UCAST_H_

/** \addtogroup ucast
 * @{
 */


/** Get ucast type handle
 *
 * This handle should be passed to bdmf_new_and_set() function in
 * order to create ucast object
 * \return ucast type handle
 */
bdmf_type_handle rdpa_ucast_drv(void);

/* ucast: Attribute types */
typedef enum {
    rdpa_ucast_attr_nflows = 0, /* nflows : R : number/4 : number of configured 5-tuple based IP flows */
    rdpa_ucast_attr_flow = 1, /* flow : RWADF : aggregate/134[16512] ip_flow_info(rdpa_ip_flow_info_t) : 5-tuple based IP flow entry */
    rdpa_ucast_attr_flow_stat = 2, /* flow_stat : RF : aggregate/8[16512] rdpa_stat(rdpa_stat_t) : 5-tuple based IP flow entry statistics */
    rdpa_ucast_attr_ipv6_table = 3, /* ipv6_table : RWADF : aggregate/36[4] ipv6_sa_da_table(rdpa_ipv6_sa_da_table_t) : IPv6 Table Entry */
} rdpa_ucast_attr_types;

extern int (*f_rdpa_ucast_get)(bdmf_object_handle *pmo);

/** Get ucast object

 * This function returns ucast object instance
 * \param[out] ucast_obj    Object handle
 * \return    0=OK or error <0
 */
int rdpa_ucast_get(bdmf_object_handle *ucast_obj);

/** Get ucast/nflows attribute 

 * Get number of configured 5-tuple based IP flows
 * \param[in]   mo_ ucast object handle or mattr transaction handle
 * \param[out]  nflows_ Attribute value
 * \return 0 or error code < 0
 */
static inline int rdpa_ucast_nflows_get(bdmf_object_handle mo_, bdmf_number *nflows_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_ucast_attr_nflows, &_nn_);
    *nflows_ = (bdmf_number)_nn_;
    return _rc_;
}


/** Get ucast/flow attribute entry 

 * Get 5-tuple based IP flow entry
 * \param[in]   mo_ ucast object handle or mattr transaction handle
 * \param[in]   ai_ Attribute array index
 * \param[out]  flow_ Attribute value
 * \return 0 or error code < 0
 */
static inline int rdpa_ucast_flow_get(bdmf_object_handle mo_, bdmf_index ai_, rdpa_ip_flow_info_t * flow_)
{
    return bdmf_attrelem_get_as_buf(mo_, rdpa_ucast_attr_flow, (bdmf_index)ai_, flow_, sizeof(*flow_));
}


/** Set ucast/flow attribute entry 

 * Set 5-tuple based IP flow entry
 * \param[in]   mo_ ucast object handle or mattr transaction handle
 * \param[in]   ai_ Attribute array index
 * \param[in]   flow_ Attribute value
 * \return 0 or error code < 0
 */
static inline int rdpa_ucast_flow_set(bdmf_object_handle mo_, bdmf_index ai_, const rdpa_ip_flow_info_t * flow_)
{
    return bdmf_attrelem_set_as_buf(mo_, rdpa_ucast_attr_flow, (bdmf_index)ai_, flow_, sizeof(*flow_));
}


/** Add ucast/flow attribute entry 

 * Add 5-tuple based IP flow entry
 * \param[in]   mo_ ucast object handle or mattr transaction handle
 * \param[in,out]   ai_ Attribute array index
 * \param[in]   flow_ Attribute value
 * \return 0 or error code < 0
 */
static inline int rdpa_ucast_flow_add(bdmf_object_handle mo_, bdmf_index * ai_, const rdpa_ip_flow_info_t * flow_)
{
    int rc;
    rc = bdmf_attrelem_add_as_buf(mo_, rdpa_ucast_attr_flow, (bdmf_index *)ai_, flow_, sizeof(*flow_));
    return rc;
}


/** Delete ucast/flow attribute entry

 * Delete 5-tuple based IP flow entry
 * \param[in]   mo_ ucast object handle
 * \param[in]   ai_ Attribute array index
 * \return 0 or error code < 0
 */
static inline int rdpa_ucast_flow_delete(bdmf_object_handle mo_, bdmf_index ai_)
{
    return bdmf_attrelem_delete(mo_, rdpa_ucast_attr_flow, (bdmf_index)ai_);
}


/** Find ucast/flow attribute entry 

 * Find 5-tuple based IP flow entry
 * \param[in]   mo_ ucast object handle or mattr transaction handle
 * \param[in,out]   ai_ Attribute array index
 * \param[in,out]   flow_ Attribute value
 * \return 0 or error code < 0
 */
static inline int rdpa_ucast_flow_find(bdmf_object_handle mo_, bdmf_index * ai_, rdpa_ip_flow_info_t * flow_)
{
    int rc;
    rc = bdmf_attrelem_find(mo_, rdpa_ucast_attr_flow, (bdmf_index *)ai_, flow_, sizeof(*flow_));
    return rc;
}


/** Get ucast/flow_stat attribute entry 

 * Get 5-tuple based IP flow entry statistics
 * \param[in]   mo_ ucast object handle or mattr transaction handle
 * \param[in]   ai_ Attribute array index
 * \param[out]  flow_stat_ Attribute value
 * \return 0 or error code < 0
 */
static inline int rdpa_ucast_flow_stat_get(bdmf_object_handle mo_, bdmf_index ai_, rdpa_stat_t * flow_stat_)
{
    return bdmf_attrelem_get_as_buf(mo_, rdpa_ucast_attr_flow_stat, (bdmf_index)ai_, flow_stat_, sizeof(*flow_stat_));
}


/** Get ucast/ipv6_table attribute entry 

 * Get IPv6 Table Entry
 * \param[in]   mo_ ucast object handle or mattr transaction handle
 * \param[in]   ai_ Attribute array index
 * \param[out]  ipv6_table_ Attribute value
 * \return 0 or error code < 0
 */
static inline int rdpa_ucast_ipv6_table_get(bdmf_object_handle mo_, bdmf_index ai_, rdpa_ipv6_sa_da_table_t * ipv6_table_)
{
    return bdmf_attrelem_get_as_buf(mo_, rdpa_ucast_attr_ipv6_table, (bdmf_index)ai_, ipv6_table_, sizeof(*ipv6_table_));
}


/** Add ucast/ipv6_table attribute entry 

 * Add IPv6 Table Entry
 * \param[in]   mo_ ucast object handle or mattr transaction handle
 * \param[in,out]   ai_ Attribute array index
 * \param[in]   ipv6_table_ Attribute value
 * \return 0 or error code < 0
 */
static inline int rdpa_ucast_ipv6_table_add(bdmf_object_handle mo_, bdmf_index * ai_, const rdpa_ipv6_sa_da_table_t * ipv6_table_)
{
    int rc;
    rc = bdmf_attrelem_add_as_buf(mo_, rdpa_ucast_attr_ipv6_table, (bdmf_index *)ai_, ipv6_table_, sizeof(*ipv6_table_));
    return rc;
}


/** Delete ucast/ipv6_table attribute entry

 * Delete IPv6 Table Entry
 * \param[in]   mo_ ucast object handle
 * \param[in]   ai_ Attribute array index
 * \return 0 or error code < 0
 */
static inline int rdpa_ucast_ipv6_table_delete(bdmf_object_handle mo_, bdmf_index ai_)
{
    return bdmf_attrelem_delete(mo_, rdpa_ucast_attr_ipv6_table, (bdmf_index)ai_);
}

/** @} end of ucast Doxygen group */




#endif /* _RDPA_AG_UCAST_H_ */
