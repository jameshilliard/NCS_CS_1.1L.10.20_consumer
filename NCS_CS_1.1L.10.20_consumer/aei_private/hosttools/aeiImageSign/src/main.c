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
 *   MODULE   :  main.c
 *   DATE     :  2015-02-11
 *   Author   :  Ken Hu (khu@actiontec.com)
 *   PURPOSE  :  Main function of app to generate AEI signed image.
 *
 ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <time.h>
#include "aei_img_sign/ais_common.h"
#include "aei_img_sign/ais_block.h"
#include "aei_img_sign/ais_block_show.h"
#include "crc32.h"

#define _TO_STR(x) #x
#define TO_STR(x) _TO_STR(x)
#define KBYTES 1024
#define DEFAULT_OUTF_SUF ".ais"
#define DEFAULT_PADDING_BYTE 0xff
#define DEFAULT_SW_VER    "Unknown"
#define DEFAULT_FLASH_B_SIZE (128 * KBYTES)

/*To decouple module dependency, copy from bcmTag.h*/
typedef struct _WFI_TAG {
    unsigned int wfiCrc;
    unsigned int wfiVersion;
    unsigned int wfiChipId;
    unsigned int wfiFlashType;
    unsigned int wfiFlags;
} WFI_TAG, *PWFI_TAG;

#define WFI_TAG_LEN sizeof(WFI_TAG)

typedef struct {
    aeiImgSign aei_signed_block;
    char *in_f;
    char *out_f;
    U32 flash_block_size;
    U8 is_help;
    U8 is_version;
    U8 check_aei_sign; /*Only check if the input already signed by AEI.*/

   /*for internal use*/
    U8 native_endian;
    U8 is_BCM_wfi; /*Indicate if input image is broadcom whole flash image*/
    WFI_TAG orig_bcm_wfi_tag;
    U8 revert_wfi_crc;
    U32 in_f_size;
    /*original image true length which may abstract tailing len from in_f_size*/
    U32 orig_img_true_len;
    /*current image true length which may add padding len and AEI image sign block len.*/
    U32 crt_img_true_len;
    U32 f_buffer_size; /*The buffer for reading input file and writing output file*/
    U8 *f_buffer; /*The buffer for reading input file and writing output file*/
    U32 out_f_size;
}AEIImgSignHelper;

static struct option longopts[] =
{
    { "target-endian",    required_argument, 0, 't'},
    { "product-id",    required_argument, 0, 'p'},
    { "customer-id", required_argument, 0, 'c'},
    { "sw-ver",    required_argument, 0, 's'},
    { "sign-date",    required_argument, 0, 'd'},
    { "grant-products-id",    required_argument, 0, 'b'},
    { "grant-customers-id",    required_argument, 0, 'u'},
    { "only-allow-upgrade",    no_argument, 0, 'g'},
    { "only-allow-downgrade",  no_argument, 0, 'w'},
    { "flash-block-size",    required_argument, 0, 'z'},
    { "input-file",  required_argument, 0, 'i'},
    { "output-file", required_argument, 0, 'o'},
    { "check-aei-sign", no_argument, 0, 'a'},
    { "help",       no_argument, 0, 'h'},
    { "version",    no_argument, 0, 'v'},
    { 0, 0, 0, 0 }
};
#define OPTIONS_STR_FMT "t:p:c:s:d:b:u:gwz:i:o:ahv"

static char version[] = "AEI Flash Image Signature version " \
                         TO_STR(AEI_IMG_SIGN_MAJOR_VER) \
                         "." \
                         TO_STR(AEI_IMG_SIGN_MINOR_VER) \
                         "\n" \
                         ;

static char usage_str[] =
"Usage:\t%s [OPTIONS] [-h | --help] [-v | --version]\n"
"OPTIONS:\n"
"\t[-t | --target-endian] <big | small>\n"
"\t\tSpecify target endian of this image, default is big endian.\n"

"\t[-p | --product-id] <product_id>\n"
"\t\tId number of product for this image to sign.\n"
"\t\tDefault is 0.\n"

"\t[-c | --customer-id] <customer_id>\n"
"\t\tId number of customer for this image to sign.\n"
"\t\tDefault is 0.\n"

"\t[-s | --sw-ver] <software_version_string>\n"
"\t\tSoftware version string for this image to sign.\n"
"\t\tDefault is \"" DEFAULT_SW_VER "\" string if not specifed.\n"

"\t[-d | --sign-date] <date_time_string>\n"
"\t\tDate time string for this image to sign.\n"
"\t\tDefault is current host date time if not specified\n"

"\t[-b | --grant-products-id] <products_id_list | any>\n"
"\t\tGrant image updating permit to the specified products.\n"
"\t\tMost " TO_STR(AEI_IMG_MAX_GRANT_PRODUCTS_NUM) " can be specified, separated by ','\n"
"\t\tDefault equals to product-id if not specified\n"

"\t[-u | --grant-customers-id] <customers_id_list | any>\n"
"\t\tGrant image updating permit to the specified customers.\n"
"\t\tMost " TO_STR(AEI_IMG_MAX_GRANT_CUSTOMERS_NUM) " can be specified, separated by ','\n"
"\t\tDefault equals to customer-id if not specified\n"

"\t[-g | --only-allow-upgrade] | [-w | --only-allow-downgrade]\n"
"\t\tGrant this image only can upgrade or downgrade from previous image\n"
"\t\tDefault allows both if not specified\n"
"\n"

"\t[-z | --flash-block-size] <flash_block_size>\n"
"\t\tTarget flash block size in bytes.\n"
"\t\tSupport format 'nnnK', nnn is decimal number, K means 1024.\n"
"\t\teg, 128K equals to 131072.\n"
"\t\tDefault is " TO_STR(DEFAULT_FLASH_B_SIZE) "bytes if not specified\n"

"\t[-i | --input-file] <input_file_name>\n"
"\t\tInput file name of the image which not signed by AEI\n"
"\t\tMust be explictly specified\n"

"\t[-o | --output-file] <output_file_name>\n"
"\t\tOuput file name of image which will be signed by AEI\n"
"\t\tDefault value is <input_file_name>" DEFAULT_OUTF_SUF " if not specified\n"

"\t[-a | --check-aei-sign]\n"
"\t\tCheck the input file if it's AEI signed image.\n"
"\t\tThe input file is specified through [-i | --input-file]\n"

"\t[-h | --help] print this help information\n"
"\t[-v | --version] print version information\n"
;

static char *progname;
static AEIImgSignHelper aei_sign_hlp;

static void usage (void)
{
    printf(usage_str, progname);
}

static void show_sign_hlper (AEIImgSignHelper *hlp)
{
#define SHOW_NAME_FMT "\t%-16s: "
    if (hlp) {
        printf("=================== ActionTec Electronic Inc Image Signature Tool ===================\n");
        printf("\t\t%s",version);
        printf("\n");
        printf("-------------------------------------------------------------------------------------\n");
        printf(SHOW_NAME_FMT "%s\n", "Native endian",
                hlp->native_endian == AEI_IMG_BIG_ENDIAN?"Big":"Small");
        if (!hlp->check_aei_sign) {
            printf(SHOW_NAME_FMT "%d", "Flash blk size", hlp->flash_block_size);
            if (hlp->flash_block_size % KBYTES == 0) {
                printf("/%dK", hlp->flash_block_size/KBYTES);
            }
            printf(" bytes\n");
        }

        printf(SHOW_NAME_FMT "%s", "Input file", hlp->in_f);
        if (hlp->is_BCM_wfi) {
            printf(" [Broadcom whole flash image (WFI)]");
        }
        printf("\n");
        if (!hlp->check_aei_sign) {
            printf(SHOW_NAME_FMT "%s\n", "Output file", hlp->out_f);
        }
        printf("-------------------------------------------------------------------------------------\n");
        printf("\n");

        show_ais_block(&hlp->aei_signed_block);
    }
}

static void set_cur_time_str (char *date_str, int size)
{
    time_t t;
    struct tm *tmp;

    t = time(NULL);
    tmp = localtime(&t);
    if (tmp == NULL) {
        strncpy(date_str, "2014-11-16 20:36:00", size);
    } else {

        strftime(date_str, size, "%Y-%m-%d %H:%M:%S", tmp);
    }
}

static void init_sign_hlper (AEIImgSignHelper *hlp)
{
    if (hlp) {
        memset(hlp, 0, sizeof(AEIImgSignHelper));
        hlp->flash_block_size = DEFAULT_FLASH_B_SIZE;

        hlp->native_endian = ais_get_native_endian();
        if (AEI_IMG_UNKNOWN_ENDIAN == hlp->native_endian){
            printf("%s()@%s::%d:Fail to detect native endian\n",
                    __FUNCTION__, __FILE__, __LINE__);
            exit(1);
        }

        init_ais_block(&hlp->aei_signed_block);
        hlp->aei_signed_block.up_verifer.flag |=
            (AEI_IMG_UPDATE_FLG_PRODUCT|AEI_IMG_UPDATE_FLG_CUSTOMER);
        strncpy(hlp->aei_signed_block.sw_ver, DEFAULT_SW_VER,
                sizeof(hlp->aei_signed_block.sw_ver));

        /*Init sign date to current time as default value*/
        set_cur_time_str(hlp->aei_signed_block.signed_date,
                         sizeof(hlp->aei_signed_block.signed_date));
    }
}

/*
 * Convert the number list string "n1,n2,n3,..." to array
 *
 * num_count is IN&OUT parameter,
 * IN: indicated the array number count;
 * OUT: indicate how many number found in the list, or negative value for error.
 * */
static void num_list_str_to_array (const char *list, U32 *array, int *num_count)
{
   const char *ps = list;
   char *num_end;
   int out_value = 0, array_size;
   U32 number;

   array_size = *num_count;

   do {
       number = strtoul(ps, &num_end, 0);
       if (*num_end != ',' && *num_end != '\0') {
           /* number list string is not following the format rule*/
           out_value = -1;
           break;
       }
       out_value++;
       if (array_size) {
           *array = number;
           array_size--;
           array++;
       }
       ps = strchr(ps,',');
       if (ps)
           ps++;
   }while (ps);

   *num_count =out_value;
}

#define OPT_STR_IS_LONGER_THAN(_maxLen) ( strlen(optarg) > (_maxLen) )
#define OPT_GET_ID_WITH_CHECK(v, optend, err_s, ret) \
{ \
  if (optarg[0] != '\0')  {\
      v = strtoul(optarg, &optend, 0); \
      if (*optend != '\0') { \
         printf("%s", err_s); \
         ret = 1; \
      } \
  } else { \
    printf("%s", err_s); \
    ret = 1; \
  } \
} \

#define OPT_GET_BYTES_WITH_CHECK(v, optend, err_s, ret) \
{ \
  if (optarg[0] != '\0')  {\
      v = strtoul(optarg, &optend, 0); \
      if (*optend == 'K') { \
          v *= KBYTES; \
      } else if (*optend != '\0') {\
        printf("%s", err_s); \
        ret = 1; \
      }\
  } else { \
    printf("%s", err_s); \
    ret = 1; \
  } \
}

#define _CHECK_F_BUFFER_LEN(v, file, fun, line) \
        if ( (v) > hlp->f_buffer_size) {   \
            printf("[%s()@%s::%d]:File buffer size (%d bytes) is not enough for %lu bytes request!!!\n",\
                    fun, file, line, \
                    hlp->f_buffer_size, (unsigned long)(v)); \
            printf("Please report a bug to writer!!!\n"); \
            return 1; \
        }
#define CHECK_F_BUFFER_LEN(v) _CHECK_F_BUFFER_LEN(v, __FILE__,__FUNCTION__,__LINE__)

/*Return 0 for success, other for failure.*/
static int parse_options_input (AEIImgSignHelper *hlp, int argc, char **argv)
{
    int optc;
    int option_index = 0;
    char *opt_end;
    int ret = 0;

    /* Parse command line options. */
    progname = argv[0];

    while ((optc = getopt_long (argc, argv, OPTIONS_STR_FMT, longopts, &option_index)) != EOF) {
        switch (optc)
        {
            case 't':
                /*target-endian*/
                if (0 == strcmp(optarg,"small")) {
                    hlp->aei_signed_block.endian = AEI_IMG_SMALL_ENDIAN;
                } else if (0 == strcmp(optarg,"big")) {
                    hlp->aei_signed_block.endian = AEI_IMG_BIG_ENDIAN;
                } else {
                    printf("Wrong input for target-endian\n");
                }

                break;
            case 'p':
                /*product-id*/
                OPT_GET_ID_WITH_CHECK(hlp->aei_signed_block.product_id, opt_end, "Wrong input for product-id\n", ret);
                break;
            case 'c':
                /*customer-id*/
                OPT_GET_ID_WITH_CHECK(hlp->aei_signed_block.customer_id, opt_end, "Wrong input for customer-id\n", ret);
                break;
            case 's':
                /*sw-ver*/
                if (OPT_STR_IS_LONGER_THAN(AEI_IMG_SW_VER_STR_LEN)) {
                    printf("Specifed sofware version string length (%zu) is longer than %d\n",
                            strlen(optarg),AEI_IMG_SW_VER_STR_LEN);
                    ret = 1;
                } else {
                    strncpy(hlp->aei_signed_block.sw_ver, optarg, AEI_IMG_SW_VER_STR_LEN);
                    hlp->aei_signed_block.sw_ver[AEI_IMG_SW_VER_STR_LEN] = '\0';
                }
                break;
            case 'd':
                /*sign-date*/
                if (OPT_STR_IS_LONGER_THAN(AEI_IMG_DATE_STR_LEN)) {
                    printf("Specifed sign date string length (%zu) is longer than %d\n",
                            strlen(optarg), AEI_IMG_DATE_STR_LEN);
                    ret = 1;
                } else {
                    strncpy(hlp->aei_signed_block.signed_date, optarg, AEI_IMG_DATE_STR_LEN);
                    hlp->aei_signed_block.signed_date[AEI_IMG_DATE_STR_LEN] = '\0';
                }
                break;
            case 'b':
                /*grant-products-id*/
                if (0 == strcmp(optarg, "any")) {
                    hlp->aei_signed_block.up_verifer.flag &= ~AEI_IMG_UPDATE_FLG_PRODUCT;
                } else {
                    int count;
                    count = AEI_IMG_MAX_GRANT_PRODUCTS_NUM;
                    hlp->aei_signed_block.up_verifer.flag |= AEI_IMG_UPDATE_FLG_PRODUCT;
                    num_list_str_to_array(optarg, hlp->aei_signed_block.up_verifer.grant_product_id,
                                          &count);
                    if (count > AEI_IMG_MAX_GRANT_PRODUCTS_NUM) {
                        printf("Can't specify more than %d granted products, %d is too many\n",
                                AEI_IMG_MAX_GRANT_PRODUCTS_NUM, count);
                        count = AEI_IMG_MAX_GRANT_PRODUCTS_NUM;
                        ret = 1;
                    } else if (count <= 0) {
                        printf("Please check the format of granted products list string\n");
                        count = 0;
                        hlp->aei_signed_block.up_verifer.flag &= ~AEI_IMG_UPDATE_FLG_PRODUCT;
                        ret = 1;
                    }
                    hlp->aei_signed_block.up_verifer.grant_products_num = count;
                }
                break;
            case 'u':
                /*grant-customer-id*/
                if (0 == strcmp(optarg, "any")) {
                    hlp->aei_signed_block.up_verifer.flag &= ~AEI_IMG_UPDATE_FLG_CUSTOMER;
                } else {
                    int count;
                    count = AEI_IMG_MAX_GRANT_CUSTOMERS_NUM;
                    hlp->aei_signed_block.up_verifer.flag |= AEI_IMG_UPDATE_FLG_CUSTOMER;
                    num_list_str_to_array(optarg, hlp->aei_signed_block.up_verifer.grant_customer_id,
                                          &count);
                    if (count > AEI_IMG_MAX_GRANT_CUSTOMERS_NUM) {
                        printf("Can't specify more than %d granted customers, %d is too many\n",
                                AEI_IMG_MAX_GRANT_CUSTOMERS_NUM, count);
                        count = AEI_IMG_MAX_GRANT_CUSTOMERS_NUM;
                        ret = 1;
                    } else if (count <= 0) {
                        printf("Please check the format of granted customers list string\n");
                        count = 0;
                        hlp->aei_signed_block.up_verifer.flag &= ~AEI_IMG_UPDATE_FLG_CUSTOMER;
                        ret = 1;
                    }
                    hlp->aei_signed_block.up_verifer.grant_customers_num = count;
                }
                break;
            case 'g':
            case 'w':
                /*only-allow-upgrade or only-allow-downgrade*/
                if (hlp->aei_signed_block.up_verifer.flag & AEI_IMG_UPDATE_FLG_ONLY_DOWN_OR_UP) {
                    printf("Please only specify only-allow-upgrade or only-allow-downgrade, not both\n");
                    ret = 1;
                } else {
                    hlp->aei_signed_block.up_verifer.flag |= AEI_IMG_UPDATE_FLG_ONLY_DOWN_OR_UP;
                    hlp->aei_signed_block.up_verifer.down_or_up
                                    = (optc == 'g'?AEI_IMG_UPGRADE:AEI_IMG_DOWNGRADE);
                }
                break;

            case 'z':
                /*flash-block-size*/
                OPT_GET_BYTES_WITH_CHECK(hlp->flash_block_size, opt_end, "Wrong input for flash-block-size\n", ret);
                break;
            case 'i':
                /*input-file*/
                if (hlp->in_f) {
                    printf("Please only specify input file once\n");
                    ret = 1;
                } else if (NULL == (hlp->in_f = strdup(optarg))) {
                    printf("Fail to dup string for input file name\n");
                    ret = 1;
                }
                break;
            case 'o':
                /*output-file*/
                if (hlp->out_f) {
                    printf("Please only specify output file once\n");
                    ret = 1;
                } else if (NULL == (hlp->out_f = strdup(optarg))) {
                    printf("Fail to dup string for output file name\n");
                    ret = 1;
                }
                break;
            case 'a':
                /*check-aei-sign*/
                hlp->check_aei_sign = 1;
                break;
            case 'v':
                hlp->is_version = 1;
                break;
            case 'h':
                hlp->is_help = 1;
                break;
            default:
                ret = 1;
                break;
         }
    }
    return ret;
}

/*
 * Apply some default values which can't be determined at
 * hlp initialization phase.*/
static void apply_default_value_post_config (AEIImgSignHelper *hlp)
{
    aeiImgSign *sign_block;

    if (NULL == hlp->in_f && NULL != hlp->out_f) {
        hlp->in_f = strdup(hlp->out_f);
    } else  if (NULL == hlp->out_f && NULL != hlp->in_f) {
        int out_f_name_size;
        out_f_name_size = strlen(hlp->in_f)+strlen(DEFAULT_OUTF_SUF) + 1;
        hlp->out_f = malloc(out_f_name_size);
        if (hlp->out_f) {
            strncpy(hlp->out_f,hlp->in_f, out_f_name_size);
            strncat(hlp->out_f,DEFAULT_OUTF_SUF, out_f_name_size - strlen(hlp->in_f));
            hlp->out_f[out_f_name_size - 1] = '\0';
        }

    }

    sign_block = &hlp->aei_signed_block;
    if (sign_block->up_verifer.flag & AEI_IMG_UPDATE_FLG_PRODUCT
            && sign_block->up_verifer.grant_products_num == 0) {
        sign_block->up_verifer.grant_products_num = 1;
        sign_block->up_verifer.grant_product_id[0] = sign_block->product_id;
    }
    if (sign_block->up_verifer.flag & AEI_IMG_UPDATE_FLG_CUSTOMER
            && sign_block->up_verifer.grant_customers_num == 0) {
        sign_block->up_verifer.grant_customers_num = 1;
        sign_block->up_verifer.grant_customer_id[0] = sign_block->customer_id;
    }
}

#define SELF_REV_ENDIAN_U32(x) (x) = ais_revert_endian_U32(x)
static void revert_ais_block_endian_if_need (AEIImgSignHelper *hlp)
{
    if (hlp->aei_signed_block.endian != ais_get_native_endian()) {
        aeiImgSign *sign;
        aei_img_update_verifer *v;
        int i;

        sign = &hlp->aei_signed_block;
        SELF_REV_ENDIAN_U32(sign->product_id);
        SELF_REV_ENDIAN_U32(sign->customer_id);

        v = &sign->up_verifer;
        SELF_REV_ENDIAN_U32(v->flag);
        for(i=0;i<v->grant_products_num;i++) {
            SELF_REV_ENDIAN_U32(v->grant_product_id[i]);
        }
        for(i=0;i<v->grant_customers_num;i++) {
            SELF_REV_ENDIAN_U32(v->grant_customer_id[i]);
        }
    }
}

/* Configure helper by command line options
 * Return 0 for success, other for failure */
static int config_sign_hlp (AEIImgSignHelper *hlp, int argc, char **argv)
{
    int ret = 0;
    do {
        if (parse_options_input(hlp, argc, argv) ) {
            printf("Parameter input error!!!\n");
            printf("Suggest run '%s -h' to get more help info\n", progname);
            ret = 1;
        }

        if (0 == ret) {
            apply_default_value_post_config(hlp);
        }

        revert_ais_block_endian_if_need(hlp);

    }while(0);

    return ret;
}

/* Loading input file to memory buffer and close the file.
 * Return 0 for success, other for error. */
static int load_in_file(AEIImgSignHelper *hlp)
{
    int ret = 1;
    struct stat in_stat;
    char *f_name;
    FILE *fp = NULL;
    U32 buf_max_size_need = 0;

    f_name = hlp->in_f;
    do {
        if (NULL == f_name) {
            printf("Input file not specified!!!\n");
            break;
        }

        if (stat(f_name, &in_stat) != 0)         {
            printf( "Fail to get stat of file: %s\n", f_name);
            break;
        }

        buf_max_size_need = in_stat.st_size;
        buf_max_size_need += AEI_IMG_SIGN_BLOCK_LEN;
        if (buf_max_size_need % hlp->flash_block_size) {
            buf_max_size_need = (buf_max_size_need/hlp->flash_block_size + 1)
                                * hlp->flash_block_size;
        }
        buf_max_size_need += WFI_TAG_LEN;
        hlp->f_buffer = malloc(buf_max_size_need);
        if (NULL == hlp->f_buffer) {
            printf( "No memory for file buffer: %d bytes\n",
                    buf_max_size_need);
            break;
        }
        hlp->in_f_size = in_stat.st_size;
        hlp->orig_img_true_len = hlp->in_f_size; /*Set it equal to in file size as default.*/
        hlp->f_buffer_size = buf_max_size_need;

        if (NULL == (fp = fopen(f_name, "rb" ))) {
            printf( "Fail to open file for reading: %s\n", f_name);
            break;
        }

        CHECK_F_BUFFER_LEN(hlp->in_f_size);
        if (fread( hlp->f_buffer, sizeof(char),
                   hlp->in_f_size, fp) != hlp->in_f_size) {
            printf( "Fail to read file: %s\n", f_name);
            break;
        }
        ret = 0;
    }while (0);

    if (NULL != fp) {
        fclose(fp);
    }
    return ret;
}

static int create_output_file (AEIImgSignHelper *hlp)
{
    int ret = 1;
    char *f_name;
    FILE *fp = NULL;

    f_name = hlp->out_f;
    do {
        if (NULL == f_name) {
            printf("Output file not specified!!!\n");
            break;
        }
        if (NULL == (fp = fopen(f_name, "wb" ))) {
            printf( "Fail to open file for writing: %s\n", f_name);
            break;
        }

        if (fwrite(hlp->f_buffer, sizeof(char),
                   hlp->crt_img_true_len, fp) != hlp->crt_img_true_len) {
            printf( "Fail to write file: %s\n", f_name);
            break;
        }

        ret = 0;
    }while(0);

    if (NULL != fp) {
        fclose(fp);
    }
    return ret;
}

static int fill_padding (AEIImgSignHelper *hlp)
{
    U32 padding_byte_count = 0;

    hlp->crt_img_true_len = hlp->orig_img_true_len;

    padding_byte_count = (hlp->orig_img_true_len + AEI_IMG_SIGN_BLOCK_LEN)
                             % hlp->flash_block_size;
    if (padding_byte_count) {
        padding_byte_count = hlp->flash_block_size - padding_byte_count;

        CHECK_F_BUFFER_LEN(hlp->orig_img_true_len + padding_byte_count);
        memset(&hlp->f_buffer[hlp->orig_img_true_len], DEFAULT_PADDING_BYTE, padding_byte_count);
        hlp->crt_img_true_len += padding_byte_count;
    }

    return 0;
}

static void check_in_file_is_bcm_wfi(AEIImgSignHelper *hlp)
{
    U32 wfi_crc = CRC32_INIT_V;
    PWFI_TAG p_wfi_tag;

    if (hlp->f_buffer && hlp->in_f_size > WFI_TAG_LEN) {
        p_wfi_tag = (PWFI_TAG)(hlp->f_buffer + hlp->in_f_size - WFI_TAG_LEN);
        wfi_crc = getCrc32(hlp->f_buffer,
                           hlp->in_f_size - WFI_TAG_LEN,
                           CRC32_INIT_V);
        if ( wfi_crc == p_wfi_tag->wfiCrc
                || ais_revert_endian_U32(wfi_crc) == p_wfi_tag->wfiCrc) {
            hlp->is_BCM_wfi = 1;
            hlp->orig_img_true_len = hlp->in_f_size - WFI_TAG_LEN;
            memcpy(&hlp->orig_bcm_wfi_tag,
                    p_wfi_tag, sizeof(WFI_TAG));
            if (wfi_crc != p_wfi_tag->wfiCrc) {
                hlp->revert_wfi_crc = 1;
            }
        }
    }
}

static int re_add_bcm_wfi_tag (AEIImgSignHelper *hlp)
{
    U32 crc32;
    if (hlp->is_BCM_wfi) {
        crc32 = getCrc32(hlp->f_buffer, hlp->crt_img_true_len, CRC32_INIT_V);
        if (hlp->revert_wfi_crc) {
            crc32 = ais_revert_endian_U32(crc32);
        }
        hlp->orig_bcm_wfi_tag.wfiCrc = crc32;
        CHECK_F_BUFFER_LEN(hlp->crt_img_true_len+WFI_TAG_LEN);
        memcpy(&hlp->f_buffer[hlp->crt_img_true_len],
               &hlp->orig_bcm_wfi_tag, WFI_TAG_LEN);
        hlp->crt_img_true_len += WFI_TAG_LEN;
    }
    return 0;
}

static int apply_aei_signature (AEIImgSignHelper *hlp)
{
    create_ais_signature(hlp->f_buffer, hlp->crt_img_true_len, &hlp->aei_signed_block);

    CHECK_F_BUFFER_LEN(hlp->crt_img_true_len + AEI_IMG_SIGN_BLOCK_LEN);
    memcpy(&hlp->f_buffer[hlp->crt_img_true_len], &hlp->aei_signed_block, AEI_IMG_SIGN_BLOCK_LEN);
    hlp->crt_img_true_len += AEI_IMG_SIGN_BLOCK_LEN;
    return 0;
}

static int do_check_aei_sign(AEIImgSignHelper *hlp)

{
    int ret = 0;

    ret = ais_get_sign_block_from_buffer(
                hlp->f_buffer, hlp->orig_img_true_len, &hlp->aei_signed_block);
    return ret;
}

static int process_sign_hlp (AEIImgSignHelper *hlp)
{
    int ret = 0;
    do {
        /* Print version or help info*/
        if (hlp->is_version) {
            printf("%s",version);
        }
        if (hlp->is_help) {
            usage();
        }
        if (hlp->is_version || hlp->is_help) {
            return ret;
        }

        if ((ret = load_in_file(hlp))) {
            break;
        }

        check_in_file_is_bcm_wfi(hlp);

        if (hlp->check_aei_sign) {
            ret = do_check_aei_sign(hlp);
            break;
        } else {
            /*The following steps are for AEI image signature*/
            if ((ret = fill_padding(hlp))) {
                break;
            }
            if ((ret = apply_aei_signature(hlp))) {
                break;
            }

            /*Re-calculate wfi crc and add wfi tag to tail if need.*/
            if ((ret = re_add_bcm_wfi_tag(hlp))) {
                break;
            }

            if ((ret = create_output_file(hlp))) {
                break;
            }
        }

    }while (0);

    if (0 == ret) {
        show_sign_hlper(hlp);
    }
    if (hlp->check_aei_sign) {
        printf("AEI signed image checking .... ");
    }else {
        printf("AEI signed image generating .... ");
    }
    printf("[%s]\n", ret?"FAIL":"OK");
    return ret;
}

#define SAFE_FREE(x) if (x) {free(x); x = NULL;}
static void release_sign_hlp (AEIImgSignHelper *hlp)
{
    if (hlp) {
        SAFE_FREE(hlp->in_f);
        SAFE_FREE(hlp->out_f);
        SAFE_FREE(hlp->f_buffer);
    }
}

/*************************************************************
 * Function Name: main
 * Description  : Program entry point that parses command line parameters
 *                and calls a function to create the image.
 * Returns      : 0
 ***************************************************************************/
int main (int argc, char **argv)
{
    int ret_code = 0;
    AEIImgSignHelper *hlp = &aei_sign_hlp;

    init_sign_hlper(hlp);
    do {
        if ((ret_code = config_sign_hlp(hlp,argc, argv))) {
            break;
        }

        if ((ret_code = process_sign_hlp(hlp))) {
            break;
        }

    } while (0);

    release_sign_hlp(hlp);
    return ret_code;
} /* main */
