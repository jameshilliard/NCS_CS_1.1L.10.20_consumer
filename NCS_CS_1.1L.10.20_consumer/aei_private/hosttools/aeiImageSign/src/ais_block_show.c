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
*/
/***********************************************************************
 *
 *   MODULE   :  ais_block_show.c
 *   DATE     :  2015-02-12
 *   Author   :  Ken Hu (khu@actiontec.com)
 *   PURPOSE  :  Show Functions for AEI sign block.
 *
 ***********************************************************************/
#include <stdio.h>
#include "aei_img_sign/ais_block.h"

#define REVERT_U32_IF_NEED(x,need) (need?ais_revert_endian_U32(x):x)
#define PRINT_MEM_BYTE(m,byte_c)                        \
          {                                             \
              int _i;                                   \
              unsigned char *_p = (unsigned char *)m;   \
              printf("[ ");                             \
              for (_i=0;_i<byte_c;_i++) {               \
               printf("0x%02x ",_p[_i]);                \
              }                                         \
              printf("]");                              \
          }                                             \

#define SHOW_AIS_BLOCK_NAME_FMT "\t%-16s: "

static void show_ais_img_update_verify (aei_img_update_verifer *v, int revert_endian)
{
    int i;
    printf(SHOW_AIS_BLOCK_NAME_FMT , "Grant Products");
    U32 flag;

    flag = REVERT_U32_IF_NEED(v->flag,revert_endian);
    if (flag & AEI_IMG_UPDATE_FLG_PRODUCT) {
        for (i = 0;i < v->grant_products_num; i++) {
            printf("%s%d", i==0?"":", ",
                REVERT_U32_IF_NEED(v->grant_product_id[i],revert_endian));
            PRINT_MEM_BYTE(&v->grant_product_id[i], sizeof(v->grant_product_id[i]));
        }
    } else {
        printf("Any");
    }
    printf("\n");

    printf(SHOW_AIS_BLOCK_NAME_FMT , "Grant Customers");
    if (flag & AEI_IMG_UPDATE_FLG_CUSTOMER) {
        for (i = 0;i < v->grant_customers_num; i++) {
            printf("%s%d", i==0?"":", ",
                REVERT_U32_IF_NEED(v->grant_customer_id[i],revert_endian));
            PRINT_MEM_BYTE(&(v->grant_customer_id[i]), sizeof(v->grant_customer_id[i]));
        }
    } else {
        printf("Any");
    }
    printf("\n");

    printf(SHOW_AIS_BLOCK_NAME_FMT , "DownUpGrade");
    if (flag & AEI_IMG_UPDATE_FLG_ONLY_DOWN_OR_UP) {
        printf("Only %s allowed !!!\n",
                v->down_or_up==AEI_IMG_DOWNGRADE?"downgrade":"upgrade");
    } else {
        printf("Both downgrade and upgrade allowed.\n");
    }
}

void show_ais_block (aeiImgSign *sign_block)
{
    int i;
    int native_endian;
    int revert_endian;
    if (sign_block) {
        native_endian = ais_get_native_endian();
        if (native_endian == AEI_IMG_UNKNOWN_ENDIAN){
            printf("Warnning: Native endian is unknown !!!\n");
            printf("Warnning: Assume native endian is big!!!\n");
            printf("Warnning: Some members of AEI signed image signnative block may not be correctly shown!!!\n");
            native_endian = AEI_IMG_BIG_ENDIAN;
        }
        revert_endian = !(native_endian == sign_block->endian);

        printf("------- AEI signed image signature block -------\n");
        printf(SHOW_AIS_BLOCK_NAME_FMT , "Sign mark");
        for (i=0;i<sizeof(sign_block->aei_sign_mark); i++) {
            printf(" 0x%02x", sign_block->aei_sign_mark[i]);
        }
        printf("\n");
        printf(SHOW_AIS_BLOCK_NAME_FMT "%d.%d\n",
                "Signature ver",
                sign_block->major_ver, sign_block->minor_ver);
        printf(SHOW_AIS_BLOCK_NAME_FMT, "Target endian");
        switch (sign_block->endian) {
            case AEI_IMG_BIG_ENDIAN:
                printf("Big");
                break;
            case AEI_IMG_SMALL_ENDIAN:
                printf("Small");
                break;
            default:
                printf("Unknown");
                break;
        }
        printf("\n");

        printf(SHOW_AIS_BLOCK_NAME_FMT "%d", "Product ID",
                REVERT_U32_IF_NEED(sign_block->product_id,revert_endian));
        PRINT_MEM_BYTE(&sign_block->product_id, sizeof(sign_block->product_id));
        printf("\n");

        printf(SHOW_AIS_BLOCK_NAME_FMT "%d", "Customers ID",
                REVERT_U32_IF_NEED(sign_block->customer_id,revert_endian));
        PRINT_MEM_BYTE(&sign_block->customer_id, sizeof(sign_block->customer_id));
        printf("\n");

        printf(SHOW_AIS_BLOCK_NAME_FMT "%s\n", "Software ver", sign_block->sw_ver);
        printf(SHOW_AIS_BLOCK_NAME_FMT "%s\n", "Signed date", sign_block->signed_date);
        show_ais_img_update_verify(&sign_block->up_verifer, revert_endian);
        printf(SHOW_AIS_BLOCK_NAME_FMT, "Sigature");
        for (i=0;i<sizeof(sign_block->signature); i++) {
            printf("0x%02x ", sign_block->signature[i]);
        }
        printf("\n");

        printf("----------------------------------------\n");
    }
}
