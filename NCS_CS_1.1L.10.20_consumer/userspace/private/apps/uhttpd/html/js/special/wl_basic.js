var curWirelessStatus = "<? echo(wireless_basic_status)?>";
//curWirelessStatus: index_vlan|index_ssid|SSID Enable|Radio Enable|SSID Name|SSID Broadcast|Security Mode|WEP KEY|WEP AUTH MODE|WPA KEY|WPA ENCRYPT MODE|Wireless band|Channel|Bandwidth|Transmit Rate|Wireless MAC|

var multiple_ssid = "<? echo(wireless_multiple_ssid) ?>";
var channels = "<? echo(wireless_channel_list)?>";
//var multiple_ssid = "0|wifi0|ATT7AIfhVP|1|1|1||1|wifi1|ATT7AIfhVP_Guest|1|2|1||2|ath0|ATTa8QvYUd|1|5|0||3|ath1|ATTa8QvYUd_Guest|1|6|0||";
//var channels = "20:1|2|3|4|5|6|7|8|9|10|11|/40:1|2|3|4|5|6|7|8|9|10|11|/80:/";
//var curWirelessStatus = "1|5|1|1|ATTa8QvYUd|1|4|ny8huydjqnrr2mxk|2|1212121212121212|2|1|0|20|2|3f:22:11:11:11:11";

var topUserName = "<?echo(login_username)?>";
var isQuantennaCard = "<?echo(isQuantennaCard)?>";

var ssidList = multiple_ssid.split("||");

var tContent = new Array(); 
var aContent = new Array();

var s_on = "On";
var s_off = "Off";
var s_automatic = "Automatic";
var s_wpawpa2mixed = "WPA/WPA2 Mixed";

window.onload=function(){
    getId("top_username").innerHTML = topUserName;
    
};

//call from include_inc.js, after get xml data from include_inc.xml
function afterGetMultiLanguage(xml){
    $(xml).find("text").each(function(i){
        s_on = $(this).children("on").text();
        s_off = $(this).children("off").text();
        s_automatic = $(this).children("automatic").text();
        s_wpawpa2mixed = $(this).children("wpawpa2mixed").text();
    });

    document.getElementById("wireless_radio_enable").options[0].text = s_on;
    document.getElementById("wireless_radio_enable").options[1].text = s_off; 
    document.getElementById("wireless_ssid_enable").options[0].text = s_on; 
    document.getElementById("wireless_ssid_enable").options[1].text = s_off; 
    
    init();
    init_language();    
    
    getId("wireless_index_ssid").onchange = function(){
        changeMultipleSSID();
    }

    getId("wireless_security_type").onchange = function(){
        changeSecuriy();
    }

    getId("wireless_dot11_mode").onchange = function(){
        changeMode();
    }

    getId("wireless_channelwidth").onchange = function(){
        changeBandwidth();
    }


    getId("wireless_radio_enable").onchange = function(){
        changeRadioEnable();
    }

    getId("wireless_ssid_enable").onchange = function(){
        changeSSIDEnable();
    }

    getId("diy_apply_btn").onclick = function(){
        doSave();
    }   
}

function init_language()
{
    getCurLanguage();
	//alert("cur_lang="+cur_lang);
	if(cur_lang=="1")
		var surl = "xml/wl_basic_en.xml";
	else
		var surl = "xml/wl_basic_fr.xml";

	StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
	 tContent = ttemp;
	 aContent = atemp;


	document.getElementById("wl_basic_title_span").innerHTML = tContent[0]; 
	document.getElementById("wl_fre_span").innerHTML = tContent[1]; 
    document.getElementById("wl_band2_span").innerHTML = tContent[3]; 
	document.getElementById("wl_radio2_span").innerHTML = tContent[4];
	document.getElementById("wl_ssid2_enable").innerHTML = tContent[5]; 
	document.getElementById("wl_name2_span").innerHTML = tContent[6]; 
	document.getElementById("wl_width2_span").innerHTML = tContent[7]; 
	document.getElementById("wl_number2_span").innerHTML = tContent[8]; 
    document.getElementById("wl_power2_span").innerHTML = tContent[9]; 
	document.getElementById("wl_secur2_span").innerHTML = tContent[10]; 
	document.getElementById("wl_pass2_span").innerHTML = tContent[11]; 
	
	document.getElementById("apply_btn_span").innerHTML = tContent[14];
    document.getElementById("wl_secu_broad").innerHTML = tContent[15]; 
    document.getElementById("wl_secu_br_enab").innerHTML = tContent[12];
    document.getElementById("wl_secu_br_disa").innerHTML = tContent[13];

	if(cur_lang=="1")
	{
		get_top_bottom_en();
		wl_get_Left_en();
	}
	else
	{
		get_top_bottom_fr();
		wl_get_Left_fr();
		//document.getElementById("wireless_security_type").options[0].text = "Ouvert ";
		//document.getElementById("wireless_security_type").options[4].text = "WPA/WPA2 Mixte ";
		//document.getElementById("wireless_channel").options[0].text = "Automatique ";
	}
	
}


//Change SSID Enable.
function changeSSIDEnable(){
    var ssid_Enable = getId("wireless_ssid_enable").value;
    if(ssid_Enable == "0"){
        //Disabled
        getId("wireless_ssid").disabled = true;
        getId("wireless_security_type").disabled = true;
        getId("wireless_key").disabled = true;
        getId("wireless_dot11_mode").disabled = true;
        getId("wireless_channelwidth").disabled = true;
        getId("wireless_channel").disabled = true;
        getId("wep_key_mode").disabled = true;
        getId("id_broadcaston").disabled = true;
        getId("id_broadcastoff").disabled = true;
        getId("wireless_trans_power").disabled = true;
    }else{
        //Enabled
        getId("wireless_ssid").disabled = false;
        getId("wireless_security_type").disabled = false;
        getId("wireless_key").disabled = false;
        getId("wireless_dot11_mode").disabled = false;
        getId("wireless_channelwidth").disabled = false;
        getId("wireless_channel").disabled = false;
        getId("wep_key_mode").disabled = false;
        getId("id_broadcaston").disabled = false;
        getId("id_broadcastoff").disabled = false;
        getId("wireless_trans_power").disabled = false;
        //Set disable/enable of key len or key value
        changeSecuriy();
    }
}

//Change radio enable
function changeRadioEnable(){
    var radioEnable = getId("wireless_radio_enable").value;
    if(radioEnable == "0"){
        //Disabled
        getId("wireless_ssid_enable").disabled = true;
        getId("wireless_ssid").disabled = true;
        getId("wireless_security_type").disabled = true;
        getId("wireless_key").disabled = true;
        getId("wireless_dot11_mode").disabled = true;
        getId("wireless_channelwidth").disabled = true;
        getId("wireless_channel").disabled = true;
        getId("wep_key_mode").disabled = true;
        getId("id_broadcaston").disabled = true;
        getId("id_broadcastoff").disabled = true;
        getId("wireless_trans_power").disabled = true;
    }else{
        //Enabled
        getId("wireless_radio_enable").disabled = false;        
        getId("wireless_ssid_enable").disabled = false;
        changeSSIDEnable();
    }
}

//Change Security
function changeSecuriy(){
    var secType = getId("wireless_security_type").value;
    if(secType == "0"){
        //off
        getId("wireless_key").disabled = true;
    }
    else if(secType == "1"){
        //wep
        getId("wireless_key").disabled = false;
    }else{
        //wpa, wpa2, wpa/wpa2
        getId("wireless_key").disabled = false;
    }
}

//Change Mode.
function changeMode(){
    var mode = getId("wireless_dot11_mode").value;

    if(mode == "7")
        loadBandwidth(1);
    else if(mode == "6")
        loadBandwidth(3);
    else if(mode == "0" || mode == "1" || mode == "3")
        loadBandwidth(2);
    else
        loadBandwidth(0);
}

//Change SSID, will get the SSID information from cgi.
function changeMultipleSSID(){
    var index_ssid = getId("wireless_index_ssid").value;
    var index_vlan = findVlanBySSID(index_ssid);
    if(index_ssid != "" && index_vlan != "")
        window.location.href = "wl_basic.html?index_vlan=" + index_vlan+"&index_ssid="+ index_ssid;
}

//Change Channel by bandwidth.
function changeBandwidth(){
    var bw = getId("wireless_channelwidth").value;
    loadChannel(bw);
}

function loadMode(flag){
    getId("wireless_dot11_mode").innerHTML = "";
    if(flag == 1)
    {
        //5G
        getId("wireless_dot11_mode")[0] = new Option("5GHz (A,N)", "6");
        getId("wireless_dot11_mode")[1] = new Option("5GHz (A,N,AC)", "7");
    }else{
        //2.4G
        getId("wireless_dot11_mode")[0] = new Option("2.4GHz (B)", "0");
        getId("wireless_dot11_mode")[1] = new Option("2.4GHz (G)", "1");
        getId("wireless_dot11_mode")[2] = new Option("2.4GHz (N)", "2");
        getId("wireless_dot11_mode")[3] = new Option("2.4GHz (B,G)", "3");
        getId("wireless_dot11_mode")[4] = new Option("2.4GHz (G,N)", "4");
        getId("wireless_dot11_mode")[5] = new Option("2.4GHz (B,G,N)", "5");
    }
}
//5G AC: 20MHZ, 40MHZ, 80 MHZ; 5G A,N: 20MHZ, 40MHZ; 2.4G B/G/B+G 20MHZ; 2.4G N/B+N/B+G+N: 20MHZ, 40MHZ;
function loadBandwidth(flag){
    getId("wireless_channelwidth").innerHTML = "";
    if(flag == 1)
    {
        //5G: A,N, AC
        getId("wireless_channelwidth")[0] = new Option("20MHZ", "20");
        getId("wireless_channelwidth")[1] = new Option("40MHZ", "40");
        getId("wireless_channelwidth")[2] = new Option("80MHZ", "80");
        getId("wireless_channelwidth").selectedIndex = 2;
    }
    else if(flag == 3)
    {
        //5G A,N
        getId("wireless_channelwidth")[0] = new Option("20MHZ", "20");
        getId("wireless_channelwidth")[1] = new Option("40MHZ", "40");
        getId("wireless_channelwidth").selectedIndex = 1;
    }
    else if(flag == 0)
    {
        //2.4G: N/G+N/B+G+N
        getId("wireless_channelwidth")[0] = new Option("20MHZ", "20");
        getId("wireless_channelwidth")[1] = new Option("40MHZ", "40");
        getId("wireless_channelwidth").selectedIndex = 0;
    }
    else if(flag == 2)
    {
        //2.4G: B/G/B+G, No 40MHZ
        getId("wireless_channelwidth")[0] = new Option("20MHZ", "20");
    }
}

function loadSSID(){
    getId("wireless_index_ssid").innerHTML = "";
    var idx = 0;
    for(var i=0; i<ssidList.length-1 ;i++)
    {
        if (checkUserHideSSID(ssidList[i].split("|")[4]) == 1){
            continue;
        }
        
        var opt_show = htmlDecodeStr(ssidList[i].split("|")[2]);
        var opt_value = ssidList[i].split("|")[4];
        var is5G = ssidList[i].split("|")[5];

        opt_value = opt_value;
        if(is5G == "1")
            opt_show = opt_show+" (5G)";
        else
            opt_show = opt_show+" (2.4G)";
        getId("wireless_index_ssid")[idx] = new Option(opt_show, opt_value);
        idx++;
    }
}

function loadChannel(bandwidth){
    if(channels == "" ||bandwidth == "")
        return;
    var channels_str = "";
    var channels_bandwidth_list = channels.split("/");
    for(var i= 0; i<channels_bandwidth_list.length-1; i++){
        var bw = channels_bandwidth_list[i].split(":")[0];
        if(bw == bandwidth)
        {
            channels_str = channels_bandwidth_list[i].split(":")[1];
            break;
        }
    }

    if(channels_str == "")
        return;   
    var channelList = channels_str.split("|");
    getId("wireless_channel").innerHTML = "";
    getId("wireless_channel")[0] = new Option(s_automatic, "0");
    for(var i=0; i<channelList.length-1; i++)
    {
        getId("wireless_channel")[i+1] = new Option(channelList[i], channelList[i]);
    }
}

//Load Security type
function loadSecurity(flag){
    getId("wireless_security_type").innerHTML = "";
    var idx = 0;
    if(flag == 1)
    {
        //5G
        getId("wireless_security_type")[idx++] = new Option("Open", "0");
        
        if (isQuantennaCard == 1){
            //getId("wireless_security_type")[idx++] = new Option("WPA", "2");
            getId("wireless_security_type")[idx++] = new Option("WPA2", "3");
            //getId("wireless_security_type")[idx++] = new Option(s_wpawpa2mixed, "4");            
        } else {
            ////getId("wireless_security_type")[idx++] = new Option("WEP", "1");
            //getId("wireless_security_type")[idx++] = new Option("WPA", "2");
            getId("wireless_security_type")[idx++] = new Option("WPA2", "3");
            //getId("wireless_security_type")[idx++] = new Option(s_wpawpa2mixed, "4");
        }

    }else{
        //2.4G
        getId("wireless_security_type")[idx++] = new Option("Open", "0");
        //getId("wireless_security_type")[idx++] = new Option("WEP", "1");
        getId("wireless_security_type")[idx++] = new Option("WPA", "2");
        getId("wireless_security_type")[idx++] = new Option("WPA2", "3");
        getId("wireless_security_type")[idx++] = new Option(s_wpawpa2mixed, "4");
    }
}

function init()
{
    if(ssidList.length-1 ==0 )
        return;
    if(curWirelessStatus == "")
        return;
    var index_ssid = curWirelessStatus.split("|")[1];
    var type = if5G(index_ssid);
    loadMode(type);
    loadSSID();
    //loadChannel();
    loadSecurity(type);
    loadCurWiFiSettings();
    changeSecuriy();
    changeRadioEnable();
}

function loadCurWiFiSettings(){
    //vlan
    var curWifiList = curWirelessStatus.split("|");
    getId("wireless_index_vlan").value = curWifiList[0];
    getId("wireless_index_ssid").value = curWifiList[1];
    getId("wireless_ssid_enable").value = curWifiList[2];
    getId("wireless_radio_enable").value = curWifiList[3];
    getId("wireless_ssid").value = htmlDecodeStr(curWifiList[4]);
    //getId("wireless_ssid_broadcast").value = curWifiList[5];
    getId("wireless_security_type").value = curWifiList[6];
	var get_sec_type = getId("wireless_security_type").value;
	if(get_sec_type=="1")
	{
        getId("wireless_key").value = htmlDecodeStr(curWifiList[7]);
        getId("wep_key_len").value = getId("wireless_key").value.length;		
    }
	else if(get_sec_type=="2"||get_sec_type=="3"||get_sec_type=="4")
        getId("wireless_key").value = htmlDecodeStr(curWifiList[9]);
	else
		getId("wireless_key").value = "";
	
    getId("wireless_dot11_mode").value = curWifiList[11];

    if(curWifiList[11] == "7"){
        loadBandwidth(1);
        var _v = (curWifiList[13] == 20 || curWifiList[13] == 40 || curWifiList[13] == 80) ? curWifiList[13] : 20;
        getId("wireless_channelwidth").value = _v;
    }        
    else if(curWifiList[11] == "0" || curWifiList[11] == "1" || curWifiList[11] == "3"){
        loadBandwidth(2);
        var _v = (curWifiList[13] == 20) ? curWifiList[13] : 20;
        getId("wireless_channelwidth").value = _v;
    }
    else {
        loadBandwidth(0);
        var _v = (curWifiList[13] == 20 || curWifiList[13] == 40) ? curWifiList[13] : 20;
        getId("wireless_channelwidth").value = _v;
    }
    
    //Load current channels.
    if(curWifiList[13] != "")
    {
        loadChannel(curWifiList[13]);
        getId("wireless_channel").value = curWifiList[12];
    }

    if(curWifiList[5] == "1")
        getId("id_broadcaston").checked = true;
    else
        getId("id_broadcastoff").checked = true;

    getId("wireless_trans_power").value = curWifiList[14];
}

//Click 'Save' button
function doSave(){
    if(getId("wireless_ssid_enable").value == "1"&&getId("wireless_radio_enable").value == "1")
    {
        //Check wep/wpa key
        var secType = getId("wireless_security_type").value;
        var key = getId("wireless_key").value;

		
        if(secType == "1"){
            //Check WEP Key
            var key_length = key.length;

            if(key == ""){
                alert(aContent[2]);
                getId("wireless_key").focus();
                return;
            }
			
            if(key_length!=10&&key_length!=26)
            {
                alert(aContent[4]);
                getId("wireless_key").focus();   
    			return;            
    		}

            if(key_length == "10"){
                //10 Hex Characters
                if(!ifHexKey(key))
                {
                    alert(aContent[3]);
                    getId("wireless_key").focus();
                    return;
                }
            }
    
            if(key_length == "26"){
                //26 Hex Characters
                if(!ifHexKey(key))
                {
                    alert(aContent[3]);
                    getId("wireless_key").focus();
                    return;
                }
            }

			getId("wep_key_len").value = key.length;
			var wep_auth = curWirelessStatus.split('|')[8];
			getId("wep_auth_mode").value = wep_auth;

            if (!confirm(aContent[21])) return;
        }
        else if(secType == "2" || secType == "3" || secType == "4")
        {
            //Check WPA Key
            if(key == ""){
                alert(aContent[2]);
                getId("wireless_key").focus();
                return;
            }
    
            if(key.length < 8 || key.length > 64)
            {
                alert(aContent[5]);
                getId("wireless_key").focus();
                return;
            }
			var wpa_encrypt = curWirelessStatus.split('|')[10];
			getId("wpa_encrypt_mode").value = wpa_encrypt;			
        } else if (secType == "0") {
            if (!confirm(aContent[21])) return;
        }
    
    }

    var index_ssid = getId("wireless_index_ssid").value;
    var index_vlan = findVlanBySSID(index_ssid);
    getId("wireless_index_vlan").value = index_vlan;

    //primary ssid changed ,need set prissidchanged to 1, cgi will disable Auto Configure 
    if ((index_ssid == 1 || index_ssid == 5) && getId("wireless_ssid").value != htmlDecodeStr(curWirelessStatus.split("|")[4])){
        getId("prissidchanged").value = 1;
    }

    if (secType == "2" && index_ssid == 1){
        if (!confirm(aContent[18])) return;
    } else if (secType == "2" && index_ssid == 5){
        if (!confirm(aContent[19])) return;
    }

    getId("apply_page").value = "wl_basic.html?index_vlan=" + index_vlan+"&index_ssid="+ index_ssid;
    getId("wireless_index_vlan").value = index_vlan;
    getId("basic_form").submit();
}

function findVlanBySSID(ssid_index){
    var wireless_index_vlan = 1;
    for(var i=0; i<ssidList.length-1 ;i++)
    {
        var ssid = ssidList[i].split("|")[4];
        if(ssid == ssid_index)
        {
            wireless_index_vlan = ssidList[i].split("|")[3];
            break;
        }
    }
    return wireless_index_vlan;
}

//Check the SSID is 5G
function if5G(ssid_index)
{
    var is5g = 0;
    for(var i=0; i<ssidList.length-1 ;i++){
        var ssid = ssidList[i].split("|")[4];
        if(ssid == ssid_index)
        {
            is5g = ssidList[i].split("|")[5];
            break;
        }
    }
    return is5g;
}

//Check the key is ASSCII characters
function ifASSCIIKey(key_value){
    var ifAsscii = true;
    var rege=/[|]/;
    for(var i = 0; i<key_value.length; i++){
        var mat = key_value.match(rege);
        if(mat)
        {
            ifAsscii = false;
            break;
        }
    }
    return ifAsscii;
}

//Check the key is Hex characters
function ifHexKey(key_value){
    var ifHex = true;
    var rege=/[^a-fA-F0-9]/;
    for(var i = 0; i<key_value.length; i++){
        var mat = key_value.match(rege);
        if(mat)
        {
            ifHex = false;
            break;
        }
    }
    return ifHex;
}
