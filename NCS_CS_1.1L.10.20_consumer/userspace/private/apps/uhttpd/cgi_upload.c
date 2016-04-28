//Remove all the code: duplicate with dal_upload.c
//TODO: Remove the file from Repo
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "cgi_upload.h"
#include "cms.h"
#include "cms_util.h"
#include "cms_boardcmds.h"
#include "cms_mdm.h"
#include "cms_msg.h"
#include "../../../../shared/opensource/include/bcm963xx/bcm_hwdefs.h"
//#include "httpd.h"
