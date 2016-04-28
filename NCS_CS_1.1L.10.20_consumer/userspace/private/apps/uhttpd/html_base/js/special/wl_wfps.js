var multiple_ssid="<? echo(wireless_multiple_ssid) ?>";
//multiple_ssid: index|ifname|ssid|index_vlan|index_ssid|is5g||... ...
var wl_wps = "<? echo(wireless_wps_status)?>";
//wl_wps = index_vlan|index_ssid|ssid_enable|radio_enable|ssid_name|wps_enable|security_type|key|key_encryption|wps_status|ap_pin|

var ssidList = multiple_ssid.split("||");
var topUserName = "<?echo(login_username)?>";

var tContent = new Array(); 
var aContent = new Array();

var s_configured = "Configured";
var s_unconfigured = "unConfigured";
var s_wpawpa2mixed = "WPA/WPA2 Mixed";

window.onload=function(){
    init_language();
	
    getId("top_username").innerHTML = topUserName;
    
};

//call from include_inc.js, after get xml data from include_inc.xml
function afterGetMultiLanguage(xml){
    $(xml).find("text").each(function(i){
        s_configured = $(this).children("configured").text();
        s_unconfigured = $(this).children("unconfigured").text();
        s_wpawpa2mixed = $(this).children("wpawpa2mixed").text();
    });
    

    init();

    getId("wireless_index_ssid").onchange = function(){
        changeMultipleSSID();
    }

    getId("diy_apply_btn").onclick = function(){
        getId("submit_button_name").value = "static_ap_pin";

        if(getId("id_wpson").checked == true)
            getId("wireless_wps_enable_type").value = "1";
        else
            getId("wireless_wps_enable_type").value = "0";

        var apply_pin = getId("id_appin").value;
/*                
        if(trim(apply_pin)=="")
        {
            alert("AP PIN is null!");
            return;
        }
        if(trim(apply_pin).length!==4 && trim(apply_pin).length!==8)
        {
            alert("Invalid AP PIN length! The device PIN is usually four or eight digits long!");
            return;
        }       
        if(!isNumber(apply_pin))
        {
            alert("Invalid AP PIN! AP PIN must be numeric digits!");
            return;
        }       
*/        
        getId("wireless_wps_pin").value = apply_pin;

        var index_ssid = getId("wireless_index_ssid").value;
        var index_vlan = findVlanBySSID(index_ssid);
        getId("wireless_index_vlan").value = index_vlan;
    
        getId("apply_page").value = "wl_wfps.html?index_vlan=" + index_vlan+"&index_ssid="+ index_ssid;
        getId("wfps_form").submit();        
    }

    getId("generate_btn").onclick = function(){
        getId("submit_button_name").value = "generate_ap_pin";

        if(getId("id_wpson").checked == true)
            getId("wireless_wps_enable_type").value = "1";
        else
            getId("wireless_wps_enable_type").value = "0";

        var index_ssid = getId("wireless_index_ssid").value;
        var index_vlan = findVlanBySSID(index_ssid);
        getId("wireless_index_vlan").value = index_vlan;
    
        getId("wfps_form").submit();        
    }


    getId("startpin_btn").onclick = function(){
        getId("submit_button_name").value = "use_device_pin";

        if(getId("id_wpson").checked == true)
            getId("wireless_wps_enable_type").value = "1";
        else
            getId("wireless_wps_enable_type").value = "0";

        var index_ssid = getId("wireless_index_ssid").value;
        var index_vlan = findVlanBySSID(index_ssid);
        getId("wireless_index_vlan").value = index_vlan;

        var apply_pin = getId("id_enrpin").value;
        if(trim(apply_pin)=="")
        {
            alert("Enrollee's PIN is null!");
            return;
        }
        if(trim(apply_pin).length!==4 && trim(apply_pin).length!==8)
        {
            alert("Invalid Enrollee's PIN length! The device PIN is usually four or eight digits long!");
            return;
        }       
        if(!isNumber(apply_pin))
        {
            alert("Invalid Enrollee's PIN! Enrollee's PIN must be numeric digits!");
            return;
        }       
        getId("wireless_wps_pin").value = apply_pin;        
    
        getId("wfps_form").submit();        
    }   

    getId("startpbc_btn").onclick = function(){
        getId("submit_button_name").value = "push";

        if(getId("id_wpson").checked == true)
            getId("wireless_wps_enable_type").value = "1";
        else
            getId("wireless_wps_enable_type").value = "0";

        var index_ssid = getId("wireless_index_ssid").value;
        var index_vlan = findVlanBySSID(index_ssid);
        getId("wireless_index_vlan").value = index_vlan;
    
        getId("wfps_form").submit();        
    }        
}

function init_language()
{
    getCurLanguage();
	//alert("cur_lang="+cur_lang);
	if(cur_lang=="1")
		var surl = "xml/wl_wfps_en.xml";
	else
		var surl = "xml/wl_wfps_fr.xml";

	StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
	 tContent = ttemp;
	 aContent = atemp;


	document.getElementById("wl_fre_span").innerHTML = tContent[17]; 
	document.getElementById("wfps_enable").innerHTML = tContent[1]; 
	document.getElementById("wfps_disable").innerHTML = tContent[2]; 
	document.getElementById("wfps_security").innerHTML = tContent[3]; 
	document.getElementById("wfps_encrypt").innerHTML = tContent[4]; 
	//document.getElementById("wfps_status").innerHTML = tContent[5]; 
	//document.getElementById("id_wpsstatus").innerHTML = tContent[6]; 
	//document.getElementById("wfps_stat_unconf").innerHTML = tContent[7]; 

	document.getElementById("wfps_stat2").innerHTML = tContent[5]; 
	document.getElementById("wfps_appin2").innerHTML = tContent[9]; 
	document.getElementById("wfps_genpin_btn").innerHTML = tContent[10]; 
	
	document.getElementById("wfps_enr_conf").innerHTML = tContent[11]; 
	document.getElementById("wfps_enr_pin").innerHTML = tContent[12]; 
	document.getElementById("wfps_start_pin_btn").innerHTML = tContent[13]; 
	document.getElementById("wfps_soft_push").innerHTML = tContent[14]; 
	document.getElementById("wfps_startpbc_btn").innerHTML = tContent[15]; 
	document.getElementById("apply_btn_span").innerHTML = tContent[16];
	if(cur_lang=="1")
	{
		get_top_bottom_en();
		wl_get_Left_en();
	}
	else
	{
		get_top_bottom_fr();
		wl_get_Left_fr();
	}
	
}


//Change SSID, will get the SSID information from cgi.
function changeMultipleSSID(){
    var index_ssid = getId("wireless_index_ssid").value;
    var index_vlan = findVlanBySSID(index_ssid);
    if(index_ssid != "" && index_vlan != "")
        window.location.href = "wl_wfps.html?index_vlan=" + index_vlan+"&index_ssid="+ index_ssid;
}

function loadSSID(){
    getId("wireless_index_ssid").innerHTML = "";
    for(var i=0; i<ssidList.length-1 ;i++)
    {
        var opt_show = htmlDecodeStr(ssidList[i].split("|")[2]);
        var opt_value = ssidList[i].split("|")[4];
        var is5G = ssidList[i].split("|")[5];

        opt_value = opt_value;
        if(is5G == "1") {
            opt_show = opt_show+" (5G)";
        } else {
            opt_show = opt_show+" (2.4G)";
        }
            
        getId("wireless_index_ssid")[i] = new Option(opt_show, opt_value);
    }
	
	if(wl_wps.split('|')[1]!="")
		getId("wireless_index_ssid").value = wl_wps.split('|')[1];
}


function init()
{
    if(wl_wps.split("|").length< 2)
        return;
	
    removeOthersSSID();
    loadSSID();
    loadCurSecuritySettings();
}

//only support primary SSID
function removeOthersSSID(){
    var has24G = 0;
    var has5G = 0;
    var idx = 0;
    var _list = ssidList.slice(0);
    ssidList.length = 0;    
    
    for(var i=0; i<_list.length-1 ;i++){
        var is5G = _list[i].split("|")[5];

        if (is5G == "1" && has5G == 1)  continue;
        if (is5G != "1" && has24G == 1) continue;

        if(is5G == "1") {
            has5G = 1;
        } else {
            has24G = 1;
        }
        var opt_show = htmlDecodeStr(_list[i].split("|")[2]);
        //console.log("opt_show = " + opt_show);

        ssidList[idx] = _list[i];
        idx++;
    }
    ssidList[idx] = ""; //last element is empty
    //console.log("ssidList.length = " + ssidList.length);
}

function loadCurSecuritySettings(){

    var wps_list = wl_wps.split("|");
	
    getId("wireless_index_vlan").value = wps_list[0];
    getId("wireless_index_ssid").value = wps_list[1];
	
    var wps_enable = wps_list[5];
    var sec_type = wps_list[6];
    var key_ecryption = wps_list[8];
    if(sec_type=="0")  //open
		key_ecryption = "Disabled";
	else if(sec_type=="1")
	{
        if(key_ecryption == "0")
			key_ecryption = "Open";
		else if(key_ecryption == "1")
			key_ecryption = "Shared";
		else if(key_ecryption == "2")
			key_ecryption = "Auto";
	}
	else
	{
        if(key_ecryption == "0")
			key_ecryption = "TKIP";
		else if(key_ecryption == "1")
			key_ecryption = "AES";
		else if(key_ecryption == "2")
			key_ecryption = "Auto";
	}		
		

    if(sec_type == "0")
		sec_type = "OPEN";
	else if(sec_type == "1")
		sec_type = "WEP";
	else if(sec_type == "2")
		sec_type = "WPA";
	else if(sec_type == "3")
		sec_type = "WPA2";
	else if(sec_type == "4")
		sec_type = s_wpawpa2mixed;	
	
    var wps_status = wps_list[9];
	if(wps_status=="1")
		wps_status = s_configured;
	else
		wps_status = s_unconfigured;
    var wps_appin = wps_list[10];


    if(wps_enable == "1")
		getId("id_wpson").checked = true;
	else
		getId("id_wpsoff").checked = true;

	getId("id_security").innerHTML = sec_type;
	getId("id_encryption").innerHTML = key_ecryption;	

    getId("id_wpsstatus").innerHTML = wps_status;
    
    getId("id_appin").value = wps_appin;       

    if (wps_enable == 0 && (sec_type == "OPEN" || sec_type == "WEP" || sec_type == "WPA" || key_ecryption == "TKIP")){
        getId("id_wpson").disabled = true;
        getId("id_wpsoff").disabled = true;
    } 
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


