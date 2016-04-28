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
 *   MODULE   :  ais_block.c
 *   DATE     :  2015-02-12
 *   Author   :  Ken Hu (khu@actiontec.com)
 *   PURPOSE  :  Functions for AEI sign block operation.
 *
 ***********************************************************************/
#include <stdio.h>
#include <string.h>
#include "aei_img_sign/ais_block.h"
#include "crc32.h"

static U8 aei_img_mark[] = AEI_IMG_MARK;
void init_ais_block (aeiImgSign *sign_block)
{
    if (sign_block) {
        memset(sign_block, 0, sizeof(aeiImgSign));
        memcpy(sign_block->aei_sign_mark, aei_img_mark,
                sizeof(sign_block->aei_sign_mark));
        sign_block->major_ver = AEI_IMG_SIGN_MAJOR_VER;
        sign_block->minor_ver = AEI_IMG_SIGN_MINOR_VER;
    }
}

/*Return 0 for non ais mark, else return 1.*/
static int is_ais_mark(U8 *buffer, U32 len)
{
    int i;
    if (NULL == buffer || len < AEI_IMG_MARK_LEN) {
        return 0;
    }
    for (i=0;i<AEI_IMG_MARK_LEN;i++) {
        if (buffer[i] != aei_img_mark[i]) {
            return 0;
        }
    }
    return 1;
}

int ais_get_native_endian (void)
{
    int i = 0x00010203;
    unsigned char *c;

    c = (unsigned char *)&i;
    if (c[0] == 0) {
        return AEI_IMG_BIG_ENDIAN;
    } else if (c[0] == 0x03){
        return AEI_IMG_SMALL_ENDIAN;
    } else {
        return AEI_IMG_UNKNOWN_ENDIAN;
    }
}

inline U32 ais_revert_endian_U32 (U32 i)
{
    unsigned char *c;

    c = (unsigned char *) &i;
    c[0] ^= c[3];
    c[3] ^= c[0];
    c[0] ^= c[3];
    c[1] ^= c[2];
    c[2] ^= c[1];
    c[1] ^= c[2];

    return i;
}

void create_ais_signature (U8 *data, U32 data_len, aeiImgSign *sign_block)
{
    U32 data_crc32;
    U32 signature_crc32;
    U8 aei_sign_extra_key[] = {0x55, 0xaa,0xff,0x00, 0x01, 0xe0, 0xff};
    U8 *dyn_key;

    data_crc32 = getCrc32(data, data_len, CRC32_INIT_V);
    signature_crc32 = getCrc32(aei_sign_extra_key, sizeof(aei_sign_extra_key),
            data_crc32);

    /*Use convert data_crc32 to small endian buffer, and cal its CRC.*/
    if (AEI_IMG_BIG_ENDIAN == ais_get_native_endian()) {
        data_crc32 = ais_revert_endian_U32(data_crc32);
    }
    dyn_key = (U8*)&data_crc32;
    signature_crc32 = getCrc32(dyn_key, sizeof(data_crc32), signature_crc32);

    signature_crc32 = getCrc32((U8*)sign_block, (U32)((void *)((aeiImgSign *)0)->signature - (void *)0),
                               signature_crc32);

    if (sign_block->endian != ais_get_native_endian()) {
        signature_crc32 = ais_revert_endian_U32(signature_crc32);
    }
    memcpy(sign_block->signature, &signature_crc32, sizeof(sign_block->signature));

}

/*
 * If this buffer is signed by AEI, then return 0 and output the valid
 * AEI image sign block to sign if sign is not NULL.
 * Otherwise return non-zero value.
 * */
AIS_ERR_CODE ais_get_sign_block_from_buffer (U8 *buf, U32 buf_len, aeiImgSign *sign)
{
    int ret = AIS_E_OK;
    int i;
    do {
        aeiImgSign block_to_verify, *p_sign;
        if (buf_len <= AEI_IMG_SIGN_BLOCK_LEN) {
            ret = AIS_E_WRONG_LEN;
            break;
        }
        if (!is_ais_mark(buf+buf_len-AEI_IMG_SIGN_BLOCK_LEN, AEI_IMG_SIGN_BLOCK_LEN)) {
            ret = AIS_E_MARK_NOT_FOUND;
            break;
        }
        p_sign = (aeiImgSign *)(buf+buf_len-AEI_IMG_SIGN_BLOCK_LEN);
        if (p_sign->major_ver > AEI_IMG_SIGN_MAJOR_VER){
            ret = AIS_E_VER_NOT_MATCH;
            break;
        }
        memcpy(&block_to_verify, p_sign, sizeof(block_to_verify));

        create_ais_signature(buf, buf_len-AEI_IMG_SIGN_BLOCK_LEN, &block_to_verify);

        for (i = 0; i < AEI_IMG_SIGNATURE_LEN; i++) {
            if (p_sign->signature[i] != block_to_verify.signature[i]) {
                break;
            }
        }
        if (i != AEI_IMG_SIGNATURE_LEN) {
            ret = AIS_E_SIGN_FAKE;
            break;
        }

        if (sign) {
            memcpy(sign, p_sign, sizeof(aeiImgSign));
        }

        ret = AIS_E_OK;
    }while (0);

    return ret;
}
