/*
   Copyright (c) 2015 Actiontec Electronic Inc
   All Rights Reserved

 This program is the proprietary software of Actiontec Electronic Inc (AEI) and/or its
 licensors, and may only be used, duplicated, modified or distributed pursuant
 to the terms and conditions of a separate, written license agreement executed
 between you and AEI (an "Authorized License").  Except as set forth in
 an Authorized License, AEI grants no license (express or implied), right
 to use, or waiver of any kind with respect to the Software, and AEI
 expressly reserves all rights in and to the Software and all intellectual
 property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE
 NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY
 AEI AND DISCONTINUE ALL USE OF THE SOFTWARE.

 Except as expressly set forth in the Authorized License,

 1. This program, including its structure, sequence and organization,
    constitutes the valuable trade secrets of AEI, and you shall use
    all reasonable efforts to protect the confidentiality thereof, and to
    use this information only in connection with your use of AEI
    integrated circuit products.

 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
    AND WITH ALL FAULTS AND AEI MAKES NO PROMISES, REPRESENTATIONS OR
    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
    RESPECT TO THE SOFTWARE.  AEI SPECIFICALLY DISCLAIMS ANY AND
    ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT,
    FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
    COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE
    TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR
    PERFORMANCE OF THE SOFTWARE.

 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL AEI OR
    ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
    INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY
    WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
    IF AEI HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES;
    OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
    SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
    SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY
    LIMITED REMEDY.
:>

*/
/***********************************************************************
 *
 *   MODULE   :  ais_common.h
 *   DATE     :  2015-02-11
 *   Author   :  Ken Hu (khu@actiontec.com)
 *   PURPOSE  :  Define common data structures and APIs of AEI image
 *               signature.
 *
 ***********************************************************************/
#ifndef _AEI_IMAGE_SIGN_COMMON_H_
#define _AEI_IMAGE_SIGN_COMMON_H_

#define AEI_IMG_SIGN_MAJOR_VER 1
#define AEI_IMG_SIGN_MINOR_VER 0

#define AEI_IMG_MARK_LEN 4
#define AEI_IMG_MARK  {'A', 'E', 'I', 'S'} //Should match with AEI_IMG_MARK_LEN
#define AEI_IMG_SW_VER_STR_LEN 16
#define AEI_IMG_DATE_STR_LEN 20 //Support non-ascii date string
#define AEI_IMG_SIGNATURE_LEN 4 // Don't change to other value

#ifndef U8
#define U8 unsigned char
#endif
#ifndef U32
#define U32 unsigned int
#endif

/*Need to check if current running product is in grant_product_id[],
 * only matched products go with sw upgrade. */
#define AEI_IMG_UPDATE_FLG_PRODUCT 0x1
/*Need to check if current running customer is in grant_customer_id[],
 * only matched customers go with sw upgrade. */
#define AEI_IMG_UPDATE_FLG_CUSTOMER 0x2
/*Need to check if only downgrade or upgrade is allowed.*/
#define AEI_IMG_UPDATE_FLG_ONLY_DOWN_OR_UP 0x4
typedef struct __attribute__((packed)) {
    U32 flag;
#define AEI_IMG_MAX_GRANT_PRODUCTS_NUM 4
    U8 grant_products_num;
    U32 grant_product_id[AEI_IMG_MAX_GRANT_PRODUCTS_NUM];
#define AEI_IMG_MAX_GRANT_CUSTOMERS_NUM 8
    U8 grant_customers_num;
    U32 grant_customer_id[AEI_IMG_MAX_GRANT_CUSTOMERS_NUM];
#define AEI_IMG_DOWNGRADE 0
#define AEI_IMG_UPGRADE   1
    U8 down_or_up;
}aei_img_update_verifer ;

typedef struct __attribute__((packed)) {
    U8 aei_sign_mark[AEI_IMG_MARK_LEN];
    U8 major_ver;
    U8 minor_ver;
#define AEI_IMG_BIG_ENDIAN 0
#define AEI_IMG_SMALL_ENDIAN 1
#define AEI_IMG_UNKNOWN_ENDIAN 2
    U8 endian;
    U32 product_id;
    U32 customer_id;
    char sw_ver[AEI_IMG_SW_VER_STR_LEN + 1];
    char signed_date[AEI_IMG_DATE_STR_LEN + 1];
    aei_img_update_verifer up_verifer;
    U8 signature[AEI_IMG_SIGNATURE_LEN]; /*Signature should be the last member of this structure. */
} aeiImgSign ;

#define AEI_IMG_SIGN_BLOCK_LEN sizeof(aeiImgSign)

typedef enum {
    AIS_E_OK = 0,
    AIS_E_WRONG_LEN,
    AIS_E_MARK_NOT_FOUND,
    AIS_E_VER_NOT_MATCH,
    AIS_E_SIGN_FAKE,
} AIS_ERR_CODE;

#endif
