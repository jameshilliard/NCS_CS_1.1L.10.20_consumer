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
 *   MODULE   :  tr69_aei_uas_grant.h
 *   DATE     :  2015-03-18
 *   Author   :  Ken Hu (khu@actiontec.com)
 *   PURPOSE  :  Define common data structures and APIs of AEI tr69
 *               User Access Source (UAS) grant.
 *
 ***********************************************************************/
#ifndef _TR69_AEI_UAS_GRANT_H_
#define _TR69_AEI_UAS_GRANT_H_

#ifndef TR69_AEI_UAS_KEYSTR_WEB
#define TR69_AEI_UAS_KEYSTR_WEB "web"
#endif
#ifndef TR69_AEI_UAS_KEYSTR_CONSOLE
#define TR69_AEI_UAS_KEYSTR_CONSOLE "console"
#endif
#ifndef TR69_AEI_UAS_KEYSTR_SSH
#define TR69_AEI_UAS_KEYSTR_SSH "ssh"
#endif
#ifndef TR69_AEI_UAS_KEYSTR_TELNET
#define TR69_AEI_UAS_KEYSTR_TELNET "telnet"
#endif

/* --------------------------------------------------------------------
 * @brief Function : tr69_aei_uasGranted
 *
 * @param[in] grant_value points to access resources granted string.
 * @param[in] uas_keystr points to key part string of UAS, should be one of above
 *            TR69_AEI_UAS_KEYSTR_XXX macro.
 * @param[in] uas_argv points to argv part string of UAS.
 * @return    1 for granted, 0 for NOT.
 *
 * grant_value is a string defined in TR69 protype.xml,
 * it indicates what access sources are granted.
 * Format:
 *      uaskeystr1[-uasargv1];uaskeystr2[-uasargv2];
 * -------------------------------------------------------------------- */
int tr69_aei_uasGranted(const char *grant_value,
                        const char *uas_keystr, const char *uas_argv);
#endif
