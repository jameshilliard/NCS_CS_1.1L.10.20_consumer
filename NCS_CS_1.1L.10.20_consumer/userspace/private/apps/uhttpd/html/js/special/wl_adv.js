var multiple_ssid="<? echo(wireless_multiple_ssid) ?>";
//multiple_ssid: index|ifname|ssid|index_vlan|index_ssid|is5g||... ...
var adv_sets = "<? echo(wireless_advanced)?>";
var ssidList = multiple_ssid.split("||");
var topUserName = "<?echo(login_username)?>";

var tContent = new Array(); 
var aContent = new Array();

window.onload=function(){
    init_language();
	
    getId("top_username").innerHTML = topUserName;
    init();

    getId("wireless_index_ssid").onchange = function(){
        changeMultipleSSID();
    }

    getId("diy_apply_btn").onclick = function(){
        doSave();
    }

};

function init_language()
{
    getCurLanguage();
	//alert("cur_lang="+cur_lang);
	if(cur_lang=="1")
		var surl = "xml/wl_adv_en.xml";
	else
		var surl = "xml/wl_adv_fr.xml";

	StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
	 tContent = ttemp;
	 aContent = atemp;


	document.getElementById("adv_title").innerHTML = tContent[0]; 
	document.getElementById("wl_fre_span").innerHTML = tContent[2]; 
	document.getElementById("adv_protec").innerHTML = tContent[4]; 
	document.getElementById("adv_pro_enable").innerHTML = tContent[5]; 
	document.getElementById("adv_pro_disable").innerHTML = tContent[6]; 
	document.getElementById("adv_stbc_enable").innerHTML = tContent[5]; 
	document.getElementById("adv_stbc_disable").innerHTML = tContent[6]; 
	document.getElementById("adv_aggre").innerHTML = tContent[7]; 
	document.getElementById("adv_aggre_enable").innerHTML = tContent[5]; 
	document.getElementById("adv_aggre_disable").innerHTML = tContent[6]; 
	document.getElementById("adv_short_gi").innerHTML = tContent[8]; 
	document.getElementById("adv_short_enable").innerHTML = tContent[5]; 	
	document.getElementById("adv_short_disable").innerHTML = tContent[6]; 
	
	document.getElementById("wl_secu_broad").innerHTML = tContent[11]; 
	document.getElementById("wl_secu_br_enab").innerHTML = tContent[5]; 	
	document.getElementById("wl_secu_br_disa").innerHTML = tContent[6]; 
	document.getElementById("wl_secu_wmm_enab").innerHTML = tContent[5]; 	
	document.getElementById("wl_secu_wmm_disa").innerHTML = tContent[6]; 
	
	document.getElementById("adv_beacon").innerHTML = tContent[9]; 
	document.getElementById("apply_btn_span").innerHTML = tContent[10];
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
        window.location.href = "wl_adv.html?index_vlan=" + index_vlan+"&index_ssid="+ index_ssid;
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
	
	if(adv_sets.split('|')[1]!="")
		getId("wireless_index_ssid").value = adv_sets.split('|')[1];
}


function init()
{
    if(adv_sets.split("|").length< 2)
        return;
	
    loadSSID();
    loadCurSecuritySettings();
}

function loadCurSecuritySettings(){

    var advList = adv_sets.split("|");
	
    getId("wireless_index_vlan").value = advList[0];
    getId("wireless_index_ssid").value = advList[1];
	
    var protection = advList[5];
    var stbc = advList[6];
    var Aggregation = advList[7];
    var short_gi = advList[8];
    var broadcast = advList[9];
    var wmm = advList[10];
    var beancon = advList[11];
    var transmitpower = advList[12];

    if(protection == "1")
		getId("id_protectionon").checked = true;
	else
		getId("id_protectionoff").checked = true;

    if(stbc == "1")
		getId("id_stbcon").checked = true;
	else
		getId("id_stbcoff").checked = true;


    if(Aggregation == "1")
		getId("id_aggregationon").checked = true;
	else
		getId("id_aggregationoff").checked = true;

    if(short_gi == "1")
		getId("id_gion").checked = true;
	else
		getId("id_gioff").checked = true;

    if(broadcast == "1")
		getId("id_broadcaston").checked = true;
	else
		getId("id_broadcastoff").checked = true;

    if(wmm== "1")
		getId("id_wmmon").checked = true;
	else
		getId("id_wmmoff").checked = true;	

	getId("id_beacon").value = beancon;

    getId("wireless_trans_power").value = transmitpower;
}

//Click 'Save' button
function doSave(){
    var beacon = getId("id_beacon").value;

	if(!isNumber(beacon))
	{
        alert(aContent[1]);
		return;
	}

	if(beacon<20||beacon>1024)
	{
        alert(aContent[1]);
		return;
	}

    var index_ssid = getId("wireless_index_ssid").value;
    var index_vlan = findVlanBySSID(index_ssid);
    getId("wireless_index_vlan").value = index_vlan;

    getId("apply_page").value = "wl_adv.html?index_vlan=" + index_vlan+"&index_ssid="+ index_ssid;
    getId("adv_form").submit();
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

