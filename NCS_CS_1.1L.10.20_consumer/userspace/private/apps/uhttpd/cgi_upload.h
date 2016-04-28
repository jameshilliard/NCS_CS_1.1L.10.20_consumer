/*
 * session control
 *
 */
#ifndef CGI_UPLOAD_H
#define CGI_UPLOAD_H


/*******************************************************************
 *  [FUNCTION NAME]:
 *          init_uhttpd_handle
 *
 *  [DESCRIPTION]:
 *      init uhttpd handle for process bcm configuration
 *
 *  [RETURN]
 *              == 0        :         SUCCESS
 *              other       :         FAIL
 *
 *****************************************************************/
int init_uhttpd_handle();


/*******************************************************************
 *  [FUNCTION NAME]:
 *          flash_update_from_file
 *
 *  [DESCRIPTION]:
 *      update from RAM file, and active it
 *
 *  [PARAMETER]:
 *          filename[in]    : RAM filename
 *
 *  [RETURN]
 *              >= 0        :         SUCCESS
 *              other       :         FAIL
 *
 *  [History]
 *          @2011.9.15  jerome.zhao: create this function
 *          @2014.2.20 Sharko: modify for WCB6000
 *****************************************************************/

int flash_image_from_file(const char *filename);


#endif
