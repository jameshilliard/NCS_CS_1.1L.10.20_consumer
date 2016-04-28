//global parameters
var g_main_page = "main.html";
var model_number = "<?echo(model_number)?>";

//Get current language status.
var langstat =  "<? echo(cur_lang_stat)?>";
var cur_lang = "0";

function getCurLanguage()
{
	if(langstat==""||langstat=="NULL")
		langstat = "0";
	if(langstat=="0")
		get_browser_lang();
	else
		cur_lang= langstat;
}

function get_browser_lang()
{
	 var Browser_Agent=window.navigator.userAgent;
	 if(Browser_Agent.indexOf("MSIE")!=-1){
	 	if(window.navigator.browserLanguage.indexOf("fr")!=-1)
			cur_lang = "2";
		else
			cur_lang= "1";
	 }
	 else{//firefox opera safari
	 	if(window.navigator.language.indexOf("fr")!=-1)
			cur_lang = "2";
		else
			cur_lang = "1";
	 }
}

/*
*************************************************************************************************
**   IPv4 functions
*************************************************************************************************
*/

/*
 * Check the digit is hex or not
 */
function isHexaDigit(digit) {
   var hexVals = new Array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                           "A", "B", "C", "D", "E", "F", "a", "b", "c", "d", "e", "f");
   var len = hexVals.length;
   var i = 0;
   var ret = false;

   for ( i = 0; i < len; i++ )
      if ( digit == hexVals[i] ) break;

   if ( i < len )
      ret = true;
   return ret;
}

function isNameUnsafe(compareChar) {
   var unsafeString = "\"<>%\\^[]`\+\$\,='#&@.: \t";
    
   if ( unsafeString.indexOf(compareChar) == -1 && compareChar.charCodeAt(0) > 32
        && compareChar.charCodeAt(0) < 123 )
      return false; // found no unsafe chars, return false
   else
      return true;
}   

// Check if a name valid
function isValidName(name) {
   var i = 0;   
   
   for ( i = 0; i < name.length; i++ ) {
      if ( isNameUnsafe(name.charAt(i)) == true )
         return false;
   }
   return true;
}

// same as is isNameUnsafe but allow spaces
function isCharUnsafe(compareChar) {
   var unsafeString = "\"<>%\\^[]`\+\$\,='#&@.:\t";
    
   if ( unsafeString.indexOf(compareChar) == -1 && compareChar.charCodeAt(0) >= 32
        && compareChar.charCodeAt(0) < 123 )
      return false; // found no unsafe chars, return false
   else
      return true;
}

// The alias must be alphanumeric 
function isAliasUnsafe(aliasChar) {
   
   var ascii;
   ascii = aliasChar.charCodeAt(0);

   if (ascii >= 48 && ascii <= 57) 
      return false;

   if (ascii >= 65 && ascii <= 90)
      return false;

   if (ascii >= 97 && ascii <= 122)
      return false;  

   return true;
}

function isValidAlias(alias) {
   var i = 0;   
   
   for ( i = 0; i < alias.length; i++ ) {
      if ( isAliasUnsafe(alias.charAt(i)) == true )
         return false;
   }
   return true;
}

/*
 * Check the subnet is the same or not
 */
function isSameSubNet(lan1Ip, lan1Mask, lan2Ip, lan2Mask) {
   var count = 0;   
   lan1a = lan1Ip.split('.');
   lan1m = lan1Mask.split('.');
   lan2a = lan2Ip.split('.');
   lan2m = lan2Mask.split('.');

   for (i = 0; i < 4; i++) {
      l1a_n = parseInt(lan1a[i]);
      l1m_n = parseInt(lan1m[i]);
      l2a_n = parseInt(lan2a[i]);
      l2m_n = parseInt(lan2m[i]);
      if ((l1a_n & l1m_n) == (l2a_n & l2m_n))
         count++;
   }
   if (count == 4)
      return true;
   else
      return false;
}

/*
 * Check the ip address is valid or not
 */
function isValidIpAddress(address) {
   var i = 0;
   var ipPattern = /^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/;
   var ipArray = address.match(ipPattern); 
   
   if (ipArray == null)
      return false;

   if ( address == '0.0.0.0' ||
        address == '255.255.255.255' )
      return false;

   addrParts = address.split('.');
   if ( addrParts.length != 4 ) return false;
   for (i = 0; i < 4; i++) {
      if (isNaN(addrParts[i]) || addrParts[i] =="")
         return false;
      num = parseInt(addrParts[i]);
      if ( num < 0 || num > 255 )
         return false;
   }
   return true;
}

/*
 * Check the subnet mask is valid or not
 */
function getLeftMostZeroBitPos(num) {
   var i = 0;
   var numArr = [128, 64, 32, 16, 8, 4, 2, 1];

   for ( i = 0; i < numArr.length; i++ )
      if ( (num & numArr[i]) == 0 )
         return i;

   return numArr.length;
}

function getRightMostOneBitPos(num) {
   var i = 0;
   var numArr = [1, 2, 4, 8, 16, 32, 64, 128];

   for ( i = 0; i < numArr.length; i++ )
      if ( ((num & numArr[i]) >> i) == 1 )
         return (numArr.length - i - 1);

   return -1;
}

//fake this function, can't cover 255.255..0, use new function isValidateMask
function isValidSubnetMask(mask) {
   var i = 0, num = 0;
   var zeroBitPos = 0, oneBitPos = 0;
   var zeroBitExisted = false;

   if ( mask == '0.0.0.0' )
      return false;

   maskParts = mask.split('.');
   if ( maskParts.length != 4 ) return false;

   for (i = 0; i < 4; i++) {
      if ( isNaN(maskParts[i]) == true )
         return false;
      num = parseInt(maskParts[i]);
      if ( num < 0 || num > 255 )
         return false;
      if ( zeroBitExisted == true && num != 0 )
         return false;
      zeroBitPos = getLeftMostZeroBitPos(num);
      oneBitPos = getRightMostOneBitPos(num);
      if ( zeroBitPos < oneBitPos )
         return false;
      if ( zeroBitPos < 8 )
         zeroBitExisted = true;
   }
   return true;
}

/*
 * Check the port is valid or not
 */
function isValidPort(port) {
   var fromport = 0;
   var toport = 100;

   portrange = port.split(':');
   if ( portrange.length < 1 || portrange.length > 2 ) {
       return false;
   }
   if ( isNaN(portrange[0]) )
       return false;
   fromport = parseInt(portrange[0]);
   if ( isNaN(fromport) )
       return false;
   
   if ( portrange.length > 1 ) {
       if ( isNaN(portrange[1]) )
          return false;
       toport = parseInt(portrange[1]);
       if ( isNaN(toport) )
           return false;
       if ( toport <= fromport )
           return false;      
   }
   
   if ( fromport < 1 || fromport > 65535 || toport < 1 || toport > 65535 )
       return false;
   
   return true;
}

/*
 * Check the nat port is valid or not
 */
function isValidNatPort(port) {
   var fromport = 0;
   var toport = 100;

   portrange = port.split('-');
   if ( portrange.length < 1 || portrange.length > 2 ) {
       return false;
   }
   if ( isNaN(portrange[0]) )
       return false;
   fromport = parseInt(portrange[0]);

   if ( portrange.length > 1 ) {
       if ( isNaN(portrange[1]) )
          return false;
       toport = parseInt(portrange[1]);
       if ( toport <= fromport )
           return false;
   }

   if ( fromport < 1 || fromport > 65535 || toport < 1 || toport > 65535 )
       return false;

   return true;
}

/*
 * Check the mac address is valid or not
 */
function isValidMacAddress(address) {
   var c = '';
   var num = 0;
   var i = 0, j = 0;
   var zeros = 0;

   addrParts = address.split(':');
   if ( addrParts.length != 6 ) return false;

   for (i = 0; i < 6; i++) {
      if ( addrParts[i] == '' )
         return false;
      for ( j = 0; j < addrParts[i].length; j++ ) {
         c = addrParts[i].toLowerCase().charAt(j);
         if ( (c >= '0' && c <= '9') ||
              (c >= 'a' && c <= 'f') )
            continue;
         else
            return false;
      }

      num = parseInt(addrParts[i], 16);
      if ( num == NaN || num < 0 || num > 255 )
         return false;
      if ( num == 0 )
         zeros++;
   }
   if (zeros == 6)
      return false;

   return true;
}

//the new mac check function, now use it, isValidMacAddress fake.
function isValidMac(mac) {
    var temp = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
    if (!temp.test(mac)){
         return false;
    }
    return true;
}


/*
 * Check the mac mask is valid or not
 */
function isValidMacMask(mask) {
   var c = '';
   var num = 0;
   var i = 0, j = 0;
   var zeros = 0;
   var zeroBitPos = 0, oneBitPos = 0;
   var zeroBitExisted = false;

   maskParts = mask.split(':');
   if ( maskParts.length != 6 ) return false;

   for (i = 0; i < 6; i++) {
      if ( maskParts[i] == '' )
         return false;
      for ( j = 0; j < maskParts[i].length; j++ ) {
         c = maskParts[i].toLowerCase().charAt(j);
         if ( (c >= '0' && c <= '9') ||
              (c >= 'a' && c <= 'f') )
            continue;
         else
            return false;
      }

      num = parseInt(maskParts[i], 16);
      if ( num == NaN || num < 0 || num > 255 )
         return false;
      if ( zeroBitExisted == true && num != 0 )
         return false;
      if ( num == 0 )
         zeros++;
      zeroBitPos = getLeftMostZeroBitPos(num);
      oneBitPos = getRightMostOneBitPos(num);
      if ( zeroBitPos < oneBitPos )
         return false;
      if ( zeroBitPos < 8 )
         zeroBitExisted = true;
   }
   if (zeros == 6)
      return false;

   return true;
}

/*
 *  Input   addr:       addr is a string, which is net address such as '192.168.10.5'   
 *          bit_val:    bit_val is an array 
 *                      length is 32 
 *                      countains the binary NetMask
 */
function changeNetAddrIntoBit(addr, bit_val)
{
    var val = addr.split('.');
    var count = 0;
    for (j = 0; j < 4; j++)
    {
        var bit8_val = parseInt(val[3-j]);
        for (i = 0; i < 8; i ++)
        {
            bit_val[31-count] = bit8_val%2;
            if (bit_val[31-count] == 0)
                bit8_val = bit8_val/2;
            else
                bit8_val = (bit8_val -1)/2;
            count++;
        }
    }
}

/*
 *  Input:  Mask_bit   Mask_bit is an array 
 *                     length is 32 
 *                     countains the binary NetMask
 */
function getMaskBitOneNum(Mask_bit)
{
    for (i = 0; i<32; i++)
    {
        if ( Mask_bit[i] == 0)
            return i;             
    }
    return 32;
}

/*
 *  Return: 1       lan1 is a subnet of lan2
 *          0       lan1 is the same as lan2
 *          -1      lan2 is a subnet of lan1
 *          100    lan1 and lan2 don't have any relationship    
 *             
 */
function compareSubNet(lan1Ip, lan1Mask, lan2Ip, lan2Mask) 
{
    var lan1Ip_bit = new Array();
    var lan1Mask_bit = new Array();
    var lan2Ip_bit = new Array();
    var lan2Mask_bit = new Array();
    changeNetAddrIntoBit(lan1Ip, lan1Ip_bit);
    changeNetAddrIntoBit(lan1Mask, lan1Mask_bit);
    changeNetAddrIntoBit(lan2Ip, lan2Ip_bit);
    changeNetAddrIntoBit(lan2Mask, lan2Mask_bit);
    var lan1_Mask_len = getMaskBitOneNum(lan1Mask_bit);
    var lan2_Mask_len = getMaskBitOneNum(lan2Mask_bit);
    if (lan1_Mask_len < lan2_Mask_len)
    {
        for (i = 0; i<lan1_Mask_len; i++)
        {
            if (lan1Ip_bit[i] != lan2Ip_bit[i])
                return 100;
        }
        return -1;
    }
    else if(lan1_Mask_len > lan2_Mask_len)
    {
        for (i = 0; i<lan2_Mask_len; i++)
        {
            if (lan1Ip_bit[i] != lan2Ip_bit[i])
                return 100;
        }
        return 1;
    }
    else
    {
        for (i = 0; i<lan1_Mask_len; i++)
        {
            if (lan1Ip_bit[i] != lan2Ip_bit[i])
                return 100;
        }
        return 0;

    }
}

/*
 *  Input   IpAddr: ip's address such as "192.168.0.1"
 *  Return  1:  the ip is in the range: 0~127.255.255.255
 *          2:  the ip is in the range: 128~192.255.255.255
 *          3:  the ip is in the range: 193~238.255.255.255
 *          4:  the ip is in the range: 225~240.255.255.255  muticast ip Address
 *          5:  the ip is in the range: 240~255.255.255.255  broadcast ip Address
 */
function checkIPType(IpAddr)
{
    var val = IpAddr.split('.');
    var byte1_val = parseInt(val[0]);
    if (byte1_val < 128)
        return 1;
    else if (byte1_val < 192)
        return 2;
    else if (byte1_val < 224)
        return 3;
    else if (byte1_val < 240)
        return 4;
    else
        return 5;
}

/*For QoS IP check.*/
function judgeIPAddr(IPAddr)
{
    var ipPart = new Array();

    if (IPAddr == '' || IPAddr == 'undefined')
    return ;

    if (IPAddr != '0.0.0.0')
    {
    ipPart = IPAddr.split(".");

    if (ipPart[3] == '0')
        return true;
    else
        return false;
    }
}

function checkNetIPAddr(ip, netIP, subMask)
{
    var ipAddrNum = IPAddressToNumber(ip);
    var nIPAddr = IPAddressToNumber(netIP);
    var nSubMask = IPAddressToNumber(subMask);
    var nNetAddr = nSubMask & nIPAddr;
    var nBroAddr = nNetAddr | ~nSubMask;

    if (ipAddrNum >= nNetAddr && ipAddrNum <= nBroAddr) 
    return true;
    else
    return false;
}

function IPAddressToNumber(IPAddress)
{
    var v = IPAddress.split(".");
    var Number = 0;

    for (var i = 0; i < v.length; i++)
    {
    Number <<= 8;
    Number += parseInt(v[i]);
    }
    return Number;
}

function isValidPrivateIPAddress(IPAddress)
{
    if(((IPAddressToNumber(IPAddress)>IPAddressToNumber("10.0.0.0")&&IPAddressToNumber(IPAddress)<IPAddressToNumber("10.255.255.255"))
      ||(IPAddressToNumber(IPAddress)>IPAddressToNumber("172.16.0.0")&&IPAddressToNumber(IPAddress)<IPAddressToNumber("172.31.255.255"))
      ||(IPAddressToNumber(IPAddress)>IPAddressToNumber("192.168.0.0")&&IPAddressToNumber(IPAddress)<IPAddressToNumber("192.168.255.255"))
     )&&IPAddress.split(".")[3]!="0"&&IPAddress.split(".")[3]!="255")
        return true;
    else
        return false;   
}


function isValidPublicIPAddress(IPAddress)
{
    if(((IPAddressToNumber(IPAddress)>IPAddressToNumber("1.0.0.0")&&IPAddressToNumber(IPAddress)<IPAddressToNumber("126.255.255.255"))
      ||(IPAddressToNumber(IPAddress)>IPAddressToNumber("128.0.0.0")&&IPAddressToNumber(IPAddress)<IPAddressToNumber("191.255.255.255"))
      ||(IPAddressToNumber(IPAddress)>IPAddressToNumber("192.0.0.0")&&IPAddressToNumber(IPAddress)<IPAddressToNumber("223.255.255.255"))
      ||isValidPrivateIPAddress(IPAddress))&&IPAddress.split(".")[3]!="0"&&IPAddress.split(".")[3]!="255"&&!(IPAddress.split(".")[0]=="169"&&IPAddress.split(".")[1]=="254"))
        return true;
    else
        return false;
    
    
}
/*
*************************************************************************************************
**   IPv6 functions
*************************************************************************************************
*/

/*
 * Check the ipv6 address is valid or not
 */

function isValidIpAddress6(address) {
   var i = 0, num = 0;

   addrParts = address.split(':');
   if (addrParts.length < 3 || addrParts.length > 8)
      return false;
   for (i = 0; i < addrParts.length; i++) {
      if ( addrParts[i] != "" )
         num = parseInt(addrParts[i], 16);
      if ( i == 0 ) {
//         if ( (num & 0xf000) == 0xf000 )
//            return false; //can not be link-local, site-local or multicast address
      }
      else if ( (i + 1) == addrParts.length) {
         if ( num == 0 || num == 1)
            return false;   //can not be unspecified or loopback address
      }
      if ( num != 0 )
         break;
   }
   return true;
}

/*
 * Check the ipv6 address is valid or not
 */

function isipv6(ip)
{
    var perlipv6regex = "^\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:)))(%.+)?\s*$";
    var aeronipv6regex = "^\s*((?=.*::.*)(::)?([0-9A-F]{1,4}(:(?=[0-9A-F])|(?!\2)(?!\5)(::)|\z)){0,7}|((?=.*::.*)(::)?([0-9A-F]{1,4}(:(?=[0-9A-F])|(?!\7)(?!\10)(::))){0,5}|([0-9A-F]{1,4}:){6})((25[0-5]|(2[0-4]|1[0-9]|[1-9]?)[0-9])(\.(?=.)|\z)){4}|([0-9A-F]{1,4}:){7}[0-9A-F]{1,4})\s*$";

    var regex = "/"+perlipv6regex+"/";
    return (/^\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:)))(%.+)?\s*$/.test(ip));
}

/*
 * Check the ipv6 address length is valid or not
 */

function isValidIPv6AddrLen(len)
{
    if (len.length < 1 || isNaN(len) || parseInt(len) > 128 || parseInt(len) < 16 /*|| parseInt(len)%8!=0*/)
    {
        return false;
    }
    return true;
}

/*
 * Check the ipv6 prefix length is valid or not
 */

function isValidIPv6PrefixLen(len)
{
    if (len.length < 1 || isNaN(len) || parseInt(len) >= 64 || parseInt(len) <= 0 /*|| parseInt(len)%8!=0*/)
    {
        return false;
    }
    return true;
}

/*
 * Check the ipv4 address length is valid or not
 */

function isValidIPv4Len(len)
{
    if (len.length < 1 || isNaN(len) || parseInt(len) > 32 || parseInt(len) < 0 /*|| parseInt(len)%8!=0*/)
    {
        return false;
    }
    return true;
}

/* 2000::1  =>  2000:0:0:0:0:0:0:1 
 * Double colon parsed into single colon. 
 */

function switchIPv6Address(address)
{       
    if(address.indexOf("/") != -1) {
        address = address.split("/")[0];        
    }

    var index = address.indexOf("::");
    if(index != -1) {
        var pre_address = address.substring(0, index);
        var suf_address = address.substring(index+2, address.length);

        var pre_colon = 0;
        var suf_colon = 0;
        if(pre_address.indexOf(":") != -1) {
            pre_colon = pre_address.split(":").length - 1;
        }
        if(suf_address.indexOf(":") != -1) {
            suf_colon = suf_address.split(":").length - 1;
        }
        var added_colon = 6 - pre_colon - suf_colon;
        for(var i = 0; i<added_colon; i++) {
            pre_address += ":0"; 
        }
        pre_address += ":";
        return pre_address + suf_address;
    } else {
        return address;
    }
}

/* 2000:0:0:0:0:0:0:1/48  => 2000:0:0
 * Cut IPv6 address according to prefix.
 */

function cutIPv6Address(address)
{
    var length = 0;
    var cutAddress = "";
    if(address.indexOf("/") != -1) {
        length = address.split("/")[1];     
    }
    var size = length / 16;
    address = switchIPv6Address(address);

    var address_array = address.split(":");
    for(var i = 0; i < size; i++) {
        cutAddress += address_array[i] + ":";
    }
    return cutAddress.substring(0, cutAddress.length - 1);
}

/*
 * Check the binary digit of IPv6 address. 
 */

function checkBitIPv6Address(address)
{
    var index = address.indexOf("::");
    if(index != -1) {
        address = address.substring(0, index);      
    } 
    return getAddressBit(address);
}

function getAddressBit(address)
{
    var address_array = address.split(":");
    var address_2 = "";
    var address_sum = "";
    var size = 0;
    for(var i = 0; i < address_array.length; i++)
    {
        if(address_array[i] != "") {        
            address_2 = parseInt(address_array[i], 16).toString(2);
            if(address_2.length < 16) {
                var length = 16 - address_2.length;
                
                for(var j = 0; j < length; j++) {
                    address_2 = "0" + address_2;
                }       
            }
            address_sum += address_2;
        }
    }   
    return address_sum.lastIndexOf("1") + 1;
}

/*
 * Ignore the zero prefix of number.
 */

function splitZeroPrefix(str)
{
     var re = /0*(\d+)/;
     str = switchCase(str);
     return str.replace(re, "$1");
}

function switchCase(str)
{
    return str.toLowerCase();
}

function trim(str) {
    return str.replace(/^\s\s*/, '').replace(/\s\s*$/, '');
}

function encrypt_query(instr,key)
{
    return instr;
}

function isNumber(str) 
{
    var re = /^[0-9]*$/;
    var flag = true;
    if(!re.test(str)) {
        flag = false;
    }   
    return flag;
}

function isHexNumber(str) 
{
    var re = /^[A-Fa-f0-9]*$/;
    var flag = true;
    if(!re.test(str)) {
        flag = false;
    }   
    return flag;
}

function isURL(str)
{
    var re = /[a-zA-z]+:\/\/[^s]*/;
    var flag = true;
    if(!re.test(str)) {
        flag = false;
    }   
    return flag;
}

function appendTableRow(table, align, content){
    var tr = table.insertRow(-1);
    var i;
    for(i=0; i < content.length; i++){
        var td = tr.insertCell(-1);
        td.align = align;
        td.innerHTML = content[i];
    }
}

function appendTableEmptyRow(table, align, colspan, content){
    var tr = table.insertRow(-1);
    var td = tr.insertCell(-1);
    if (isNaN(colspan))
        colspan = 1;
    td.align = align;
    td.colSpan = colspan;
    td.innerHTML = content;
}

/*
function appendTableRow(table, align, content){
    var innerHTML = "<tr>";
    for(var i=0; i < content.length; i++){
        innerHTML += "<td align='" + align + "'>" + content[i] + "</td>";
    }
    innerHTML += "</tr>";
    table.append(innerHTML);
}

function appendTableEmptyRow(table, align, colspan, content){
    table.append("<tr><td align='" + align + "' colspan='" + colspan + "'>" + content + "</td></tr>");
}
*/

function postSubmit(lochead){
    
    var action = lochead.substr(0,lochead.indexOf("?"));
    var paramStr =  lochead.substr(lochead.indexOf("?")+1,lochead.length);
    var paramArray = paramStr.split("&");
    var tempForm = document.createElement("form");
    tempForm.name = "postForm";
    tempForm.action = action;
    tempForm.method = "post";
    document.body.appendChild(tempForm);
    if(paramArray.length>0){
        for(var i=0;i<paramArray.length;i++){
            var paramArrayValue = paramArray[i];
            //deal with "&" which in the value
            if(paramArrayValue){
                var regeFir = /\%260.6117115231341502*/;
                var matFir = paramArrayValue.match(regeFir);
                if (matFir)
                {
                    var reFir = /\%260.6117115231341502/g;
                    paramArrayValue = paramArrayValue.replace(reFir, "\&");
                }
            }
            //paramArrayValue = unescape(paramArrayValue)
            if(paramArrayValue!=""){
                var param = paramArrayValue.split("=");
                var paramName = param[0];
                var paramValue = paramArrayValue.substr((paramName.length)+1,paramArrayValue.length);
                var tempInput = document.createElement("input");
                tempInput.type="hidden";
                tempInput.name=paramName;  
                tempInput.value=paramValue;
                tempForm.appendChild(tempInput);
            }
        }   
    }
    tempForm.submit();
}


/*  1:bridge
    2:lan eth
    3:wan eth
    4:coax
    5:wireless
    6:ppp
    7:vlan interface
*/
function getDeviceType(ifname)
{
    var dtype = 0;
    var l_ifname = ifname.length;   

    if (l_ifname <= 2)
        return dtype;

    var pre_ifname = ifname.substr(0, 2);
    
    if (ifname == NETWORK_INTERFACE_ETH10)
        dtype = 3;
    else if (ifname == NETWORK_INTERFACE_ETH2)
        dtype = 4;
    else if (ifname.indexOf(".") != -1)
        dtype = 7;
    else if (pre_ifname == "et")
        dtype = 2;
    else if (pre_ifname == "br")
        dtype = 1;
    else if (pre_ifname == "at")
        dtype = 5;
    else if (pre_ifname == "pp")
        dtype = 6;

    return dtype;
}


function in_array(v, a){
    for(key in a){
        if(a[key] == v)
            return true;
    }
    return false;
}

function pop_up(str, flag, callback)
{
    if(flag == 0 || flag == "0") 
    {
        alert(str);     
    }
    else if(flag == 1 || flag == "1")
    {
        if(confirm(str))
        {
            callback();
        }
    }
}


////////////////////////////////////////////////
// source:  Format -> "1|name||2|value||"
// key: string
// keyID, resID: number
// example: key = "2", keyID = 0, resID = 1
//          return = "value"
function findResultMultiple(source, key, keyID, resID) {

    if (source == "" || key == "" || keyID < 0 || resID < 0)
        return "";

    var sourceArray = source.split("||");
    for (var i=0; i<sourceArray.length; i++) {

        if (sourceArray[i] == "") continue;
    
        var lineArray = sourceArray[i].split("|");
        
        if (resID > lineArray.length-1) 
            return "";
        
        if (key == lineArray[keyID])
            return lineArray[resID];
    }       

    return "";  
}

////////////////////////////////////////////////
// source:  Format -> "1|5|12|13|101"
// key: string
// keyID, resID: number
// example: key = "1"
//          return = "1"
function findResultSingle(source, key) {

    if (source == "" || key == "" )
        return "";

    var lineArray = source.split("|");
    for (var i=0; i<lineArray.length; i++) {
        if (key == lineArray[i])
            return lineArray[i];
    }       

    return "";  
}

function initHint(Y)
{
    Y.one("body").appendChild("<div id='hint_box_div'></div>");
}

function showHint(Y, str, left, top, width) 
{   
    var hintBox = Y.one("#hint_box_div");
    hintBox.setX(left);
    hintBox.setY(top);
    hintBox.setContent(str);        
    hintBox.setStyle("width", width);
    hintBox.setStyle("visibility", "visible");                 
}

function hideHint(Y) {
    var hintBox = Y.one("#hint_box_div");
    hintBox.setStyle("visibility", "hidden");   
}

function getValue(v)
{
    if (v && v != "NULL")
        return v;
    else
        return "";
}

function getIntValue(v)
{
    if (v && v != "NULL")
        return v;
    else
        return 0;
}

function getStringValue(v)
{
    if (v && v != "NULL")
        return v;
    else
        return "";
}

function replaceNULL(str, s)
{
    var s_tmp = str;
    var r0 = /NULL/g;
    s_tmp = s_tmp.replace(r0, s);

    return s_tmp;
}

function getValueFromQueryString(query_string, key)
{
    var a_query_string = query_string.split("&");
    for (var i=0; i<a_query_string.length; i++)
    {
        var a_list = a_query_string[i].split("=");
        for (var j=0; j<a_list.length; j++)
        {
            if (a_list[0] == key)   return a_list[1];
        }
    }
    
    return "";
}

function trimQuotes(str)
{
    return str.substr(1, str.length - 2);
}

/***
change ip to hex
**/
function ip2hex(ip)
{
    var ip4 = ip.split(".");
    var p,i;
    var hexStr = "0x";
    for(i=0; i<ip4.length; i++)
    {
        p = new Number(ip4[i]);
        hexStr += p<16?("0"+p.toString(16)):p.toString(16);
    }
    return(hexStr);
}


/**Check if ip1 and ip2 have the same subnetmask**/
function isSubNetIpV4(ip1, subnetmask, ip2)
{
    var a_ip1 = ip1.split(".");
    if (a_ip1.length != 4)  return 0;
        
    var a_ip2 = ip2.split(".");
    if (a_ip2.length != 4)  return 0;
    
    ip1 = ip2hex(ip1);
    subnetmask = ip2hex(subnetmask);
    ip2 = ip2hex(ip2);
    return((ip1&subnetmask) == (ip2&subnetmask));
}

function isInIPRange(start, end, ip)
{
    var a_start = start.split(".");
    if (a_start.length != 4)    return 0;
        
    var a_end = end.split(".");
    if (a_end.length != 4)  return 0;
    
    var a_ip = ip.split(".");
    if (a_ip.length != 4)   return 0;
    
    if (parseInt(a_ip[3]) >= parseInt(a_start[3]) && parseInt(a_ip[3]) <= parseInt(a_end[3]))
        return true;
        
    return false;
}

function getIPObj(str)
{
    if(str == "NULL" || str == "")
        str = "0.0.0.0";

    var strArray = str.split(".");
    var obj = new Object();

    obj.ip0 = strArray[0];
    obj.ip1 = strArray[1];
    obj.ip2 = strArray[2];
    obj.ip3 = strArray[3];

    return obj;
}

function getIPStr(ip0, ip1, ip2, ip3)
{
    return ip0.get("value") + "." + ip1.get("value") + "." 
         + ip2.get("value") + "." + ip3.get("value");
}

function getMacObj(str)
{
    if(str == "NULL" || str == "")
        str = "00:00:00:00:00:00";

    var strArray = str.split(":");
    var obj = new Object();

    obj.mac0 = strArray[0];
    obj.mac1 = strArray[1];
    obj.mac2 = strArray[2];
    obj.mac3 = strArray[3];
    obj.mac4 = strArray[4];
    obj.mac5 = strArray[5];

    return obj;
}

function getMacStr(mac0, mac1, mac2, mac3, mac4, mac5)
{
    return mac0.get("value") + ":" + mac1.get("value") + ":" 
         + mac2.get("value") + ":" + mac3.get("value") + ":" 
         + mac4.get("value") + ":" + mac5.get("value");
}

function deleteMessage(single, module)
{
    var result = "";
    var str = " this ";
    if(!single)
        str = " these ";

    result += "Are you sure you want to delete" + str + module;

    if(!single)
        result += "(s)";

    result += "?";
    return result;
}

/*#########################################################
#
# functions for JS get element and set style
#
##########################################################*/
function getId(id){
    return document.getElementById(id);
}

function ifhideTab(id, flag)
{
    if(flag)
        getId(id).style.display = "none";
    else
        getId(id).style.display = "";
}

function ifdisableTab(id, flag)
{
    if(flag)
        getId(id).disabled = true;
    else
        getId(id).disabled = false;
}

/*#########################################################
#
# functions for html data  decode
#
##########################################################*/
function htmlDecodeStr(str) {
    if (str.search("&#") == -1)
        return str;

    var re = /&#[1-9][0-9]{0,2};/g;
    var s_des = str.replace(re, function($0){
                                    var s_tmp = $0;
                                    var r0 = /&#/g;
                                    s_tmp = s_tmp.replace(r0, "");
                                    var r1 = /;/g;
                                    s_tmp = s_tmp.replace(r0, "");
                                    s_tmp = String.fromCharCode(parseInt(s_tmp));
                                return(s_tmp)});

    return s_des;
}
//Decode the input text value
function htmlDecode() {
    for (var n=0; n<(document.forms.length); n++)
    {
        var form = document.forms[n];
        for (var i = 0; i < form.elements.length; i++) {
            if (form.elements[i].type == "text" && form.elements[i].value != "")
                form.elements[i].value = htmlDecodeStr(form.elements[i].value);
        }
    }
}
//Decode the input hidden value
function htmlDecode_hidden() {
    for (var n=0; n<(document.forms.length); n++)
    {
        var form = document.forms[n];
        for (var i = 0; i < form.elements.length; i++) {
            if (form.elements[i].type == "hidden" && form.elements[i].value != "")
                form.elements[i].value = htmlDecodeStr(form.elements[i].value);
        }
    }
}
//check if has microsoft special characters<>"'%;()&+ 
function hasSpecialChar(str)
{
    var re = /^[^<>()"'%&+;]+$/;
    return !re.test(str);
}
//check if has html tags
function hasHTMLTags(str){
    var ifHasHTMLTgs= false;
    var htmlTagsArray = ["<applet","<input","<body","<embed","<frame","<script","<javascript","<img","<iframe","<style","<link","<ilayer","<meta","<object","</script","<tr","<td","<th","<table","</table","<form","</form"];
    for(var i=0;i<htmlTagsArray.length;i++){
        if(str.indexOf(htmlTagsArray[i])!=-1){
            ifHasHTMLTgs = true;
            break;
        }
    }
    return ifHasHTMLTgs;
}
//support all characters
function hasQWESTChar(str)
{
    var re = /^[0-9a-zA-Z `~!@#\$%\^&\*\(\)\-_=\+\[\{\]\}\\\|;:'",<\.>\/\?]+$/;
    return re.test(str);
}

function htmlEncodeStr(str)
{
    var re = /<|>|"|'|%|;|\(|\)|&|\+/gi;
    var s_des = str.replace(re, function($0){
                                    i_tmp = String.charCodeAt($0);
                                return("&#" + i_tmp + ";")});
    return s_des;                               
}

function appendTableRow(table, align, content){
    var tr = table.insertRow(-1);
    var i;
    for(i=0; i < content.length; i++){
        var td = tr.insertCell(-1);
        td.align = align;
        td.innerHTML = content[i];
    }
}

//set the even row to be different color.
function setEvenTrColor(tab_id)
{
    var logTab = document.getElementById(tab_id);
    var rowsLen = logTab.rows.length;
    var ifTwoTh = false;
    var i=1;
    for(i; i<rowsLen; i++)
    {
        if((ifTwoTh && i%2 != 0) || (!ifTwoTh && i%2 == 0))
            logTab.rows[i].className ="even";
    }
}

//replace 'NULL' and '' to 'N/A'
function replaceNull(src){
    var des = src;
    if(des == "NULL" || des == "")
        des = "N/A";
    return des;
}
//Interface: br0/br0:1/eth0/eth0:1/ath0/ath0:1/wifi0/wifi0:1/host0/host0:1
function isValidInterface(str) 
{
    var result = true;
    var re = /^((eth|br|ath|wifi|host){1})([0-9]+)(([:][0-9]+)?)$/;
    
    if(!re.test(str)) {
        result = false;
    }   
    return result;
}

//For partial refresh.
var xmlhttp = null;

if(window.XMLHttpRequest)  //Mozilla
{
  xmlhttp = new XMLHttpRequest();
}
else if(window.ActiveXObject) //IE
{
  try
  {
    xmlhttp = new ActiveXObject("Msxml2.XMLHTTP"); //new IE
  }
  catch(e)
  {
    xmlhttp = new ActiveXObject("Microsoft.XMLHTTP"); //old IE
  }
}
if(!xmlhttp)
{
  window.alert("Your broswer not support XMLHttpRequest!");
}

/**
* isValidateMask
* check submask
* submask:  eg: 255.255.255.192
**/
function isValidateMask(submask)
{
  var IPPattern = /^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$/
  if(!IPPattern.test(submask))return false;

  var IPArray = submask.split(".");
  var ip1 = parseInt(IPArray[0]);
  var ip2 = parseInt(IPArray[1]);
  var ip3 = parseInt(IPArray[2]);
  var ip4 = parseInt(IPArray[3]);
  if ( ip1<0 || ip1>255 || ip2<0 || ip2>255 || ip3<0 || ip3>255 || ip4<0 || ip4>255 ) {
    return false;    
  }

  var ip_binary = fomartIP(ip1) + fomartIP(ip2) + fomartIP(ip3) + fomartIP(ip4);

  if(-1 != ip_binary.indexOf("01")) return false;
  return true;
}


/**
* fomartIP
* return binary value
* ip: int, eg: 0-255
* return: binary value of ip,(eg：ip=255, return 11111111; ip=1, return 00000001)
**/
function fomartIP(ip)
{
  return (ip+256).toString(2).substring(1);
}

//1: WCB  0:WEB
function getModelType(model_number){
  return (model_number.length > 3 && model_number.substr(0, 3) == "WCB") ? 1 : 0;
}


//1:low   2:medium  3:high
function getPasswordStrength(pwd){
    var p1= (pwd.search(/[a-zA-Z]/)!=-1) ? 1 : 0;
    var p2= (pwd.search(/[0-9]/)!=-1) ? 1 : 0;
    var p3= (pwd.search(/[^A-Za-z0-9]/)!=-1) ? 1 : 0;
    var pa=p1+p2+p3;

    return pa;
}

function checkPassword(pwd, id){
    var _level = getPasswordStrength(pwd);

    switch (_level){
        case 1:
            getId(id).innerHTML = '<font style=" font-weight:bold; color:#FF0000">Low</font>';
            break;
        case 2:
            getId(id).innerHTML = '<font style=" font-weight:bold; color: #FFD700">Medium</font></span>';
            break;
        case 3:
            getId(id).innerHTML = '<font style=" font-weight:bold; color:#008000">High</font>';
            break;
        default:
            getId(id).innerHTML = '';
            break;
    }
}
