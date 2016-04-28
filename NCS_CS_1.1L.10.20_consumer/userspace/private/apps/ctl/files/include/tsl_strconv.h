
#ifndef _TSL_STRCONV_H
#define _TSL_STRCONV_H

#include <arpa/inet.h>  /* mwang_todo: should not include OS dependent file */

#include "tsl_common.h"


/*!\file cms_strconv.h
 * \brief Header file for the application to convert value from string to other formats.
 * This is in the cms_util library.
 */

/** Macro to determine if a string parameter is empty or not */
#define IS_EMPTY_STRING(s)    ((s == NULL) || (*s == '\0'))

#if 0
/** convert vpiVciStr (eg. "0/35") to vpi=2 and vci=35
 * @param vpiVciStr (IN) vpi/vci string
 * @param vpi (OUT) vpi in tsl_int_t format 
 * @param vci (OUT) vpi in tsl_int_t format
 * @return CmsRet enum.
 */
CmsRet tsl_atmVpiVciStrToNum(const char *vpiVciStr, tsl_int_t *vpi, tsl_int_t *vci);

/**
 * @param vpiVciStr (IN) vpi/vci string
 * @param vpi (OUT) vpi in tsl_int_t format 
 * @param vci (OUT) vpi in tsl_int_t format
 * @return CmsRet enum.
 */
CmsRet tsl_atmVpiVciNumToStr(const tsl_int_t vpi, const tsl_int_t vci, char *vpiVciStr);

/** convert macStr to macNum (array of 6 bytes)
 *  Ex: 0a:0b:0c:0d:0e:0f -> 0a0b0c0d0e0f
 *
 * @param macStr (IN) macStr to be converted.
 * @param macNum (OUT) macNum must point to a buffer of at least 6 bytes which the
 *                     caller provides.
 * @return CmsRet enum.
 */
CmsRet tsl_macStrToNum(const char *macStr, UINT8 *macNum);

/** convert macNum (array of 6 bytes) to macStr.
 *  Ex: 0a0b0c0d0e0f -> 0a:0b:0c:0d:0e:0f
 * 
 * @param macNum (IN) macNum array to be converted.
 * @param macStr (OUT) macStr must point to a buffer of at least MAC_STR_LEN+1 (17+1)
 *                     to store the results.
 * @return CmsRet enum.
 */
CmsRet tsl_macNumToStr(const UINT8 *macNum, char *macStr);


/** convert string to signed 32 bit integer, similar to unix strtol
 *
 * @param str (IN)        string to convert
 * @param endptr (IN/OUT) optional, if provided, will point to the first
 *                        character which caused conversion error.
 * @param base (IN)       radix to use for conversion.  If 0, then the
 *                        function will decide the radix based on input string.
 * @param val (OUT)       if conversion was successful, the resulting value.
 * @return CmsRet enum.
 */
CmsRet tsl_strtol(const char *str, char **endptr, tsl_int_t base, tsl_int_t *val);


/** convert string to unsigned 32 bit integer, similar to unix strtoul
 *
 * @param str (IN)        string to convert.  A leading - sign is not allowed.
 * @param endptr (IN/OUT) optional, if provided, will point to the first
 *                        character which caused conversion error.
 * @param base (IN)       radix to use for conversion.  If 0, then the
 *                        function will decide the radix based on input string.
 * @param val (OUT)       if conversion was successful, the resulting value.
 * @return CmsRet enum.
 */
CmsRet tsl_strtoul(const char *str, char **endptr, tsl_int_t base, tsl_uint_t *val);


/** convert string to signed 64 bit integer, similar to unix strtoll
 *
 * @param str (IN)        string to convert
 * @param endptr (IN/OUT) optional, if provided, will point to the first
 *                        character which caused conversion error.
 * @param base (IN)       radix to use for conversion.  If 0, then the
 *                        function will decide the radix based on input string.
 * @param val (OUT)       if conversion was successful, the resulting value.
 * @return CmsRet enum.
 */
CmsRet tsl_strtol64(const char *str, char **endptr, tsl_int_t base, SINT64 *val);


/** convert string to unsigned 64 bit integer, similar to unix strtoull
 *
 * @param str (IN)        string to convert.  A leading - sign is not allowed.
 * @param endptr (IN/OUT) optional, if provided, will point to the first
 *                        character which caused conversion error.
 * @param base (IN)       radix to use for conversion.  If 0, then the
 *                        function will decide the radix based on input string.
 * @param val (OUT)       if conversion was successful, the resulting value.
 * @return CmsRet enum.
 */
CmsRet tsl_strtoul64(const char *str, char **endptr, tsl_int_t base, UINT64 *val);


/** convert string to lowercase string
 *
 * @param string (IN/OUT) the upper case of the characters in the string will be 
 *                        converted to lower case characters.
 */
void tsl_strToLower(char *string);


/*!\enum UrlProto
 * \brief URL protocols returned by tsl_parseUrl().
 */
typedef enum 
{
   URL_PROTO_HTTP=0, /**< http */
   URL_PROTO_HTTPS=1, /**< https */
   URL_PROTO_FTP=2,   /**< ftp */
   URL_PROTO_TFTP=3   /**< tftp */
} UrlProto;


/** Parse an URL and return is components.
 *
 * @param url    (IN) url to parse.
 * @param proto (OUT) If not NULL, and if URL is well formed, it will be
 *                    set to the URL protocol (URL_PROTO_HTTPD, URL_PROTO_FTP, etc.)
 * @param addr  (OUT) If not NULL, and if URL is well formed, it will be 
 *                    set to the address portion of the URL.  Caller is responsible
 *                    for freeing addr string.
 * @param port  (OUT) If not NULL, and if URL is well formed, it will be 
 *                    set to the port portion of the URL.  If no port number
 *                    is specified, then port will be set to 0.
 * @param path  (OUT) If not NULL, and if URL is well formed, it will be 
 *                    set to the path portion of the URL.  Caller is responsible for
 *                    freeing path string.
 *
 * @return CmsRet enum, specifically, CMSRET_SUCCESS if URL is well formed,
 *                      CMSRET_INVALID_ARGUMENTS if URL is not well formed.
 */
CmsRet tsl_parseUrl(const char *url, UrlProto *proto, char **addr, UINT16 *port, char **path);
 


/** Return the path to the CommEngine build directory in the given buffer.
 *
 * This function should only be used for unit testing on the Linux desktop,
 * but we define the symbol for modem builds as well to reduce the number
 * of ifdef's in the code.
 *
 * @param pathBuf   (OUT) Contains the pathBuf.
 * @param pathBufLen (IN) Length of the buffer.  If the buffer is not large
 *                        enough to hold the path, an error will be returned.
 * @return CmsRet enum.
 */
CmsRet tsl_getBaseDir(char *pathBuf, tsl_uint_t pathBufLen);


/** Return the Primary and Secondary DNS sever string.
 *
 *
 * @param str (IN)        comma seperated DNS Servers string to be converted
 * @param str (OUT)       if conversion was successful, Primary DNS string.
 * @param str (OUT)       if conversion was successful, Secondary DNS string.

 * @return CmsRet enum.
 */
CmsRet tsl_parseDNS(const char *inDsnServers, char *outDnsPrimary, char *outDnsSecondary);


/** Return a syslog mode integer corresponding to the mode string.
 *
 * Mode refers to where the log goes.
 * The mode numbers are hard coded in logconfig.html, so there is no point in
 * defining #def's for them.
 * 1 = local circular buffer in shared mem
 * 2 = remote
 * 3 = local cirulcar buffer in shared mem and remote
 * 4 = local file (not clear if software actually supports this)
 * 5 = local file and remote (not clear if software actually supports this)
 *
 * @param modeStr (IN) mode string.
 *
 * @return mode number.
 */
tsl_int_t tsl_syslogModeToNum(const char *modeStr);


/** Given a syslog mode number, return the associated string.
 *
 * See tsl_syslogModeToNum for listing of valid mode numbers.
 *
 * @param mode (IN) mode number.
 *
 * @return mode string, this string is a pointer to a static string, so it
 * must not be modified or freed.
 */
char * tsl_numToSyslogModeString(tsl_int_t mode);


/** Return true if the given number is a valid and supported syslog mode.
 *
 * See tsl_syslogModeToNum for listing of valid mode numbers.
 * Only modes 1, 2, and 3 are supported.
 *
 * @param mode (IN) mode number.
 *
 * @return true if the given mode number is a valid and supported syslog mode.
 */
tsl_bool tsl_isValidSyslogMode(const char *modeStr);


/** Given a syslog level string, return equivalent log level.
 *
 * The log level numbers are defined in /usr/include/sys/syslog.h.
 *
 * @param levelStr (IN) The level in string format.
 *
 * @return the equivalent syslog level number.
 */
tsl_int_t tsl_syslogLevelToNum(const char *levelStr);


/** Given a syslog level number, return the equivalent log level string.
 *
 * The log level numbers are defined in /usr/include/sys/syslog.h.
 *
 * @param level (IN) The log level number.
 *
 * @return the log level string, this string is a pointer to a static string, so it
 * must not be modified or freed.
 */
char * tsl_numToSyslogLevelString(tsl_int_t level);


/** Return true if the given number is a valid syslog level.
 *  This is designed for the cli menu input validation.  But I put this
 *  function here to keep all the syslog level conversions together.
 *
 * @param level (IN) The log level number in string format.
 * 
 * @return TRUE if the given number is a valid syslog level.
 */
tsl_bool tsl_isValidSyslogLevel(const char *levelStr);


/** Return true if the string is a valid syslog level string.
 *
 * @param level (IN) The log level string.
 * 
 * @return TRUE if the given string is a valid syslog level string.
 */
tsl_bool tsl_isValidSyslogLevelString(const char *level);

#ifdef DMP_X_ACTIONTEC_COM_IPV6_1 /* aka SUPPORT_IPV6 */
/** Convert an IPv6 address string to its standard format returned
 *  from inet_ntop().
 *
 * @param address (IN) IPv6 address in CIDR notation.
 * @param address (OUT) standard format IPv6 address in CIDR notation.
 *
 * @return CmsRet enum.
 */
CmsRet tsl_standardizeIp6Addr(const char *address, char *stdAddr);

/** Given an IPv6 address string, return true if it is a valid IPv6 global
 *  unicast address.
 *
 * @param address (IN) IPv6 address string.
 *
 * @return TRUE if the string is a valid IPv6 global unicast address.
 */
tsl_bool tsl_isGlobalUnicastAddress(const char *address);

/** Return the Primary and Secondary DNS sever string.
 *
 * @param str (IN)        comma seperated DNS Servers string to be converted
 * @param str (OUT)       if conversion was successful, Primary DNS string.
 * @param str (OUT)       if conversion was successful, Secondary DNS string.
 *
 * @return CmsRet enum.
 */
CmsRet tsl_getAddrPrefix(const char *address, tsl_uint_t plen, char *prefix);

/** Return the Primary and Secondary DNS sever string.
 *
 * @param str (IN)        comma seperated DNS Servers string to be converted
 * @param str (OUT)       if conversion was successful, Primary DNS string.
 * @param str (OUT)       if conversion was successful, Secondary DNS string.
 *
 * @return CmsRet enum.
 */
CmsRet tsl_replaceEui64(const char *address1, char *address2);

/** Return the Primary and Secondary DNS sever string.
 *
 * @param str (IN)        comma seperated DNS Servers string to be converted
 * @param str (OUT)       if conversion was successful, Primary DNS string.
 * @param str (OUT)       if conversion was successful, Secondary DNS string.
 *
 * @return CmsRet enum.
 */
CmsRet tsl_parsePrefixAddress(const char *prefixAddr, char *address, tsl_uint_t *plen);

#endif


/*!
 * \brief defines for ppp auto method in number
 */
#define PPP_AUTH_METHOD_AUTO        0
#define PPP_AUTH_METHOD_PAP         1
#define PPP_AUTH_METHOD_CHAP        2
#define PPP_AUTH_METHOD_MSCHAP      3


/** Given a ppp  auth method string, return the equivalent auth method number.
 *
 *
 * @param authStr (IN) The auth method string.
 *
 * @return the  auth method number
 */
tsl_int_t tsl_pppAuthToNum(const char *authStr);

/** Given a ppp auth method number, return the equivalent auth method string.
 *
 *
 * @param authNum (IN) The auth method number.
 *
 * @return the  auth method string, this string is a pointer to a static string, so it
 * must not be modified or freed.
 */
char * tsl_numToPppAuthString(tsl_int_t authNum);


/** Given a log level string, return the equivalent CmsLogLevel enum.
 *
 * @param logLevel (IN) The log level in string form.
 * @return CmsLogLevel enum.
 */
CmsLogLevel tsl_logLevelStringToEnum(const char *logLevel);


/** Given a log destination string, return the equivalent CmsLogDestination enum.
 *
 * @param logDest (IN) The log destination in string form.
 * @return CmsLogDestination enum.
 */
CmsLogDestination tsl_logDestinationStringToEnum(const char *logDest);


/** Given an IP address string, return true if it is a valid IP address.
 *
 * @param af      (IN) address family, either AF_INET or AF_INET6.
 * @param address (IN) IP address string.
 *
 * @return TRUE if the string is a valid IP address.
 */
tsl_bool tsl_isValidIpAddress(tsl_int_t af, const char* address);


/** Given an IP address string, return true if it is a valid IPv4 address.
 *
 * @param address (IN) IP address string.
 *
 * @return TRUE if the string is a valid IPv4 address.
 */
tsl_bool tsl_isValidIpv4Address(const char* address);


/** Given a MAC address string, return true if it is a valid mac address
 *  string.  Note the string must be in the following format:
 *  xx:xx:xx:xx:xx:xx where x is a hex digit.
 *
 * @param macAddr (IN) mac address string.
 *
 * @return TRUE if the string is a valid mac address.
 */
tsl_bool tsl_isValidMacAddress(const char* macAddress);


/** Given a port number string, return true if it is a valid port number.
 *
 * @param portNumberStr (IN) port number string.
 *
 * @return TRUE if the string is a valid port number.
 */
tsl_bool tsl_isValidPortNumber(const char* portNumberStr);

#endif

/** Compare two strings.  Similar to strcmp except that NULL string will be
 *  treated as zero length string.
 *
 * @param s1 (IN) the first string.
 * @param s2 (IN) the second string.
 *
 * @return same return value as strcmp
 */
tsl_int_t tsl_strcmp(const char *s1, const char *s2);


/** Compare two strings disregarding case.  Similar to strcasecmp except that
 *  NULL string will be treated as zero length string.
 *
 * @param s1 (IN) the first string.
 * @param s2 (IN) the second string.
 *
 * @return same return value as strcasecmp
 */
tsl_int_t tsl_strcasecmp(const char *s1, const char *s2);


/** Compare two strings.  Similar to strncmp except that NULL string will be
 *  treated as zero length string.
 *
 * @param s1 (IN) the first string.
 * @param s2 (IN) the second string.
 * @param n  (IN) number of characters to compare.
 *
 * @return same return value as strncmp
 */
tsl_int_t tsl_strncmp(const char *s1, const char *s2, tsl_int_t n); 


/** Compare two strings disregarding case.  Similar to strncasecmp except that
 *  NULL string will be treated as zero length string.
 *
 * @param s1 (IN) the first string.
 * @param s2 (IN) the second string.
 * @param n  (IN) number of characters to compare.
 *
 * @return same return value as strcmp
 */
tsl_int_t tsl_strncasecmp(const char *s1, const char *s2, tsl_int_t n); 


/** locate a substring.  Similar to strstr except that NUll string will be
 *  treated as zero length string.
 *
 * @param s1 (IN) the first string.
 * @param s2 (IN) the second string.
 *
 * @return same return value as strstr
 */
char *tsl_strstr(const char *s1, const char *s2);

/* Copies the string form source string(src) to destination
 *  buffer(dest). If the src string is larger than destination
 *  buffer then atmost dlen-1 bytes are copied into destination
 *  and also the dest is null terminated.
 *
 * @param dest(OUT)- destination buffer 
 * @param src (IN) the source string.
 * @param dlen(IN)- destination buffer size 
 *
 * @return destination address(dest)
 */
char *tsl_strncpy(char *dest, const char *src, tsl_int_t dlen);

/** locate a sub-option string in a string of sub-options separated by commas.
 * 
 * This function is useful if you have a parameter string that looks like:
 * AAAA, AAAABBB, CCC, DD
 * and you want to know if sub-option AAAA is present.  Note that strstr cannot
 * be used here because if your parameter string was
 * AAAABBB, CCC, DD
 * and you used strstr to look for option AAAA, then strstr would return a pointer
 * to AAAABBB, which is not the sub-option string you were really looking for.
 * This is required by the DSL code in dealing with the AdslModulationCfg parameter.
 *
 * @param s1 (IN) the full sub-option string.  Sub-options can be separated by any
 *                number of spaces and commas.
 * @param s2 (IN) the sub-option string.
 *
 * @return TRUE if the subOption string was found in the fullOptionString.
 */


/** Return the length of the source string.  Similar to strlen except that NUll string will be
 *  treated as zero length string.
 *
 * @param src (IN) the source string.
 *
 * @return same return value as strlen if NULL, return 0.
 */
tsl_int_t tsl_strlen(const char *src);


/* Copies the string form source string(src) to destination
 *  buffer(dest). If the src string is larger than destination
 *  buffer then atmost dlen-1 bytes are copied into destination
 *  and also the dest is null terminated.
 *
 * @param dest(OUT)- destination buffer 
 * @param src (IN) the source string.
 * @param dlen(IN)- destination buffer size 
 *
 * @return destination address(dest)
 */
char *tsl_strncat(char *dest, const char *src, tsl_int_t dst_len);

/* Copies the string form source string(src) to destination
 *  buffer(dest). If the src string is larger than destination
 *  buffer then atmost dlen-1 bytes are copied into destination
 *  and also the dest is null terminated.
 *
 * @param dest(OUT)- destination buffer 
 * @param src (IN) the source string.
 * @param dlen(IN)- destination buffer size 
 * @param n(IN)-  n  bytes  of  src
 *
 * @return destination address(dest)
 */

char* tsl_strncat_n(char *dest, const char *src, tsl_int_t dst_len, size_t n);


/* Copies the string form source string(src) to destination
 *  buffer(dest). If the src string is larger than destination
 *  buffer then atmost dlen-1 bytes are copied into destination
 *  and also the dest is null terminated.
 *
 * @param dest(OUT)- destination buffer 
 * @param src (IN) the source string.
 * @param dlen(IN)- destination buffer size 
 * @param n(IN)- n  bytes  of  src
 *
 * @return destination address(dest)
 */
char *tsl_strncpy_n(char *dest, const char *src, tsl_int_t dlen, size_t n);


//0x01 0xFF 0x02 ==> "01FF02"
tsl_uint_t tsl_hex2str(const tsl_uchar_t *hex, const tsl_uint_t hex_len, tsl_char_t *hexString);

//"01FF02" ==> 0x01 0xFF 0x02
tsl_uint_t tsl_str2hex(const tsl_char_t *str, const tsl_uint_t str_len, tsl_uchar_t *hex);

#if 0
tsl_bool tsl_isSubOptionPresent(const char *fullOptionString, const char *subOption);


/** Retrieve WAN protocol name of the given protocol ID
 *
 * @param prtcl     (IN) network protocol
 * @param portId  (OUT) name - protocol name for html
 *
 * @return None.
 */
void tsl_getWanProtocolName(UINT8 protocol, char *name) ;


/** Convert a linear array of DHCP Vendor Ids into a single comma separated string.
 *
 * @param vendorIds (IN) This is a sequence of DHCP Vendor Ids laid out as a single linear buffer.
 *                       There are MAX_PORTMAPPING_DHCP_VENDOR_IDS (5) chunks in the buffer.
 *                       Each chunk is DHCP_VENDOR_ID_LEN + 1 bytes long.  Within each chunk
 *                       there may be a single DHCP vendor id.
 *
 * @return comma separated string of vendor ids.  The caller is responsible for freeing
 *         this buffer.
 */
char *tsl_getAggregateStringFromDhcpVendorIds(const char *vendorIds);

/** Convert a comma separated string of vendor ids into a linear array of DHCP vendor ids.
 *
 * @param aggregateString (IN) This is a sequence of up to MAX_PORTMAPPING_DHCP_VENDOR_IDS(5) comma 
 *                             separated dhcp vendor id strings.
 *
 * @return a linear buffer of vendor ids.  The caller is responsible for freeing this buffer.
 */
char *tsl_getDhcpVendorIdsFromAggregateString(const char *aggregateString);


/** Get number of DHCP vendor ids in the aggregate string
 * 
 * This is not implemented yet because it is not needed.  But it seems like a
 * useful function to have.
 * 
 * @param aggregateString (IN) This is a sequence of up to MAX_PORTMAPPING_DHCP_VENDOR_IDS(5) comma 
 *                             separated dhcp vendor id strings.
 * 
 * @return number of DHCP vendor ids.
 */
tsl_uint_t tsl_getNumberOfDhcpVendorIds(const char *aggregateString);


#define DSL_LINK_DESTINATION_PREFIX_PVC   "PVC:"
#define DSL_LINK_DESTINATION_PREFIX_SVC   "SVC:"


/** Convert connection mode string to a ConnectionModeType number
 *
 * @param connModeStr (IN) MDMVS_ string defined in mdm 
 *
 * @return a ConnectionModeType number
 */
ConnectionModeType tsl_connectionModeStrToNum(const char *connModeStr);
#endif

#endif /* _TSL_STRCONV_H */


