var multiple_ssid="<? echo(wireless_multiple_ssid) ?>";
//multiple_ssid: index|ifname|ssid|index_vlan|index_ssid|is5g||... ...

var channels = "<? echo(wireless_channel_list)?>";
var securityStatus = "<? echo(wireless_security_status)?>";
//securityStatus : index_vlan|index_ssid|SSID Enable|Radio Enable|SSID Name|Security mode|Key|Key Type|WEP Key Length|RADIUS IP|RADIUS Port|RADIUS password
/*
var multiple_ssid = "0|wl0|0000001A|0|1|0||1|wl0.1|ATTsgMWZqs|0|2|0||2|wl0.2|ATTsgMWZqs_Guest|0|3|0||3|wl0.3|vATTvYMtUHNN|0|4|0||4|wl1|00000027|0|5|1||5|wl1.1|0000002B|0|6|1||6|wl1.2|0000002E|0|7|1||7|wl1.3|00000030|0|8|1||";;
var channels = "20:1|2|3|4|5|6|7|8|9|10|11|/40:1|2|3|4|5|6|7|8|9|10|11|/80:/";
var securityStatus = "0|2|1|1|ATTsgMWZqs|4|agde9qswmw4tepxk||0|";
*/
var ssidList = multiple_ssid.split("||");
var topUserName = "<?echo(login_username)?>";
var isQuantennaCard = "<?echo(isQuantennaCard)?>";

var m_is5G = 0;

var tContent = new Array(); 
var aContent = new Array();

window.onload=function(){
    getId("top_username").innerHTML = topUserName;
    init();
    init_language();
    
    getId("wireless_index_ssid").onchange = function(){
        changeMultipleSSID();
    }
    
    getId("diy_apply_btn").onclick = function(){
        doSave();
    }

    getId("id_secopen").onchange = function(){ changeSecMode("0"); }
    getId("id_secwep").onchange = function(){ changeSecMode("1"); }
    getId("id_secwpa").onchange = function(){ changeSecMode("2"); }

    getId("id_auth1").onclick = function(){ switchAuthMode(1); }
    getId("id_auth2").onclick = function(){ switchAuthMode(2); }
};

function init_language()
{
    getCurLanguage();
    //alert("cur_lang="+cur_lang);
    if(cur_lang=="1")
        var surl = "xml/wl_security_en.xml";
    else
        var surl = "xml/wl_security_fr.xml";

    StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
     tContent = ttemp;
     aContent = atemp;


    document.getElementById("wl_secu_title").innerHTML = tContent[0]; 
    document.getElementById("wl_fre_span").innerHTML = tContent[2]; 
    document.getElementById("wl_secu_setting").innerHTML = tContent[10];    
    document.getElementById("wl_secu_set_open").innerHTML = tContent[11]; 
    document.getElementById("wl_no_secu_app").innerHTML = tContent[12]; 
    document.getElementById("wl_secu_sim_wep").innerHTML = tContent[13]; 

    document.getElementById("wl_secu_mode_open").innerHTML = tContent[11]; 
    document.getElementById("wl_secu_mode_share").innerHTML = tContent[14]; 
    document.getElementById("wl_secu_key").innerHTML = tContent[15]; 

    document.getElementById("wl_secu_enhan_secu").innerHTML = tContent[32]; 
    document.getElementById("wl_secu_cypher").innerHTML = tContent[21];
    
    document.getElementById("wl_secu_auth_mode").innerHTML = tContent[22]; 
    document.getElementById("wl_secu_enterpri").innerHTML = tContent[23];   
    document.getElementById("wl_secu_person").innerHTML = tContent[24]; 
    document.getElementById("secu_radio_ip").innerHTML = tContent[25]; 
    document.getElementById("secu_radio_port").innerHTML = tContent[26];
    document.getElementById("secu_radio_passwd").innerHTML = tContent[27];
    
    document.getElementById("apply_btn_span").innerHTML = tContent[29];
    document.getElementById("back_btn_span").innerHTML = tContent[30];

    document.getElementById("span_pskkey").innerHTML = tContent[31];
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


function changeSecMode(mode) {    
    switch (mode){
        case "0":{
            //console.log(mode);
            setWepDisabled(true);
            setWpaDisabled(true);
            break;
        }
        case "1":{
            setWepDisabled(false);
            setWpaDisabled(true);
            break;
        }
        case "2":
        case "3":
        case "4":
        case "5":
        case "6":
        case "7":
        {
            setWepDisabled(true);
            setWpaDisabled(false);
            break;
        }
    }
}

function setWepDisabled(flag) {
    getId("id_wep0").disabled = flag;
    getId("id_wep1").disabled = flag;
    getId("id_wep2").disabled = flag;
    getId("id_wepkey").disabled = flag;
    getId("wep_key_len").disabled = flag;
}

function setWpaDisabled(flag) {
    getId("id_wpamode0").disabled = flag;
    getId("id_wpamode1").disabled = flag;
    getId("id_wpamode2").disabled = flag;
    getId("id_cypher0").disabled = flag;
    getId("id_cypher1").disabled = flag;
    getId("id_cypher2").disabled = flag;
    getId("id_psk").disabled = flag;

    getId("id_auth1").disabled = flag;
    getId("id_auth2").disabled = flag;
    getId("id_serverip").disabled = flag;
    getId("id_serverport").disabled = flag;
    getId("id_serverpassword").disabled = flag;
}


//Change SSID, will get the SSID information from cgi.
function changeMultipleSSID(){
    var index_ssid = getId("wireless_index_ssid").value;
    var index_vlan = findVlanBySSID(index_ssid);
    if(index_ssid != "" && index_vlan != "")
        window.location.href = "wl_security.html?index_vlan=" + index_vlan+"&index_ssid="+ index_ssid;
}

function changeRadio()
{
    var ssidEnable = securityStatus.split('|')[2];
    var radioEnable = securityStatus.split('|')[3];
    var security_mode = securityStatus.split("|")[5];

    if (ssidEnable == "1" && radioEnable == "1"){
        enableElements();
        changeSecMode(security_mode);
    } else {
        disableElements();
    }
}

function enableElements()
{
       getId("id_secopen").disabled = false;
       getId("id_secwep").disabled = false;
       getId("id_wep0").disabled = false;
       getId("id_wep1").disabled = false;
       getId("id_wep2").disabled = false;
       getId("id_wepkey").disabled = false;
       
       getId("id_secwpa").disabled = false;
       getId("id_wpamode0").disabled = false;
       getId("id_wpamode1").disabled = false;
       getId("id_wpamode2").disabled = false;
       getId("id_cypher0").disabled = false;
       getId("id_cypher1").disabled = false;       
       getId("id_cypher2").disabled = false;
       getId("id_psk").disabled = false;

       getId("id_auth1").disabled = false;
       getId("id_auth2").disabled = false;
       getId("id_serverip").disabled = false;
       getId("id_serverport").disabled = false;
       getId("id_serverpassword").disabled = false;
}


function disableElements()
{
       getId("id_secopen").disabled = true;
       getId("id_secwep").disabled = true;
       getId("id_wep0").disabled = true;
       getId("id_wep1").disabled = true;
       getId("id_wep2").disabled = true;
       getId("id_wepkey").disabled = true;
       
       getId("id_secwpa").disabled = true;
       getId("id_wpamode0").disabled = true;
       getId("id_wpamode1").disabled = true;
       getId("id_wpamode2").disabled = true;
       getId("id_cypher0").disabled = true;
       getId("id_cypher1").disabled = true;    
       getId("id_cypher2").disabled = true;
       getId("id_psk").disabled = true;

       getId("id_auth1").disabled = true;
       getId("id_auth2").disabled = true;
       getId("id_serverip").disabled = true;
       getId("id_serverport").disabled = true;
       getId("id_serverpassword").disabled = true;
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
    
    if(securityStatus.split('|')[1]!="")
        getId("wireless_index_ssid").value = securityStatus.split('|')[1];
}


function init()
{
    if(ssidList.length-1 ==0 )
        return;
    if(securityStatus == "")
        return;
    var index_ssid = securityStatus.split("|")[1];

    loadSSID();
    loadCurSecuritySettings();
    changeRadio();

    m_is5G = if5G(securityStatus.split('|')[1]);
    if (if5G(securityStatus.split('|')[1]) == 1 && isQuantennaCard == 1){
        getId("area_auth_mode").style.display = "none";
        getId("server_table").style.display = "none";

        getId("psk_table").style.display = "";

        //getId("securitymode_wep").style.display = "none";
    }
    getId("securitymode_wep").style.display = "none";

    if (m_is5G == 1){
        $("#span_secwpa2").html("WPA2");
        getId("wl_secu_enhan_secu").style.display = "none";
        getId("tr_mode").style.display = "none";
        getId("tr_cypher").style.display = "none";
    }
}

function loadCurSecuritySettings(){

    var secuList = securityStatus.split("|");
    getId("wireless_index_vlan").value = secuList[0];
    getId("wireless_index_ssid").value = secuList[1];
    
    getId("wireless_ssid_enable").value = secuList[2];
    getId("wireless_radio_enable").value = secuList[3];
    
    var security_mode = secuList[5];
    var wireless_key = htmlDecodeStr(secuList[6]);
    var wepauth_encrypt_mode = secuList[7];
    var wep_key_length = secuList[8];
    
    if(security_mode=="0")
        getId("id_secopen").checked = true;
    else if(security_mode=="1")
    {
        getId("id_secwep").checked = true;
        if(wepauth_encrypt_mode == "0")
        {
            getId("id_wep0").checked = true;
        }
        else if(wepauth_encrypt_mode == "1")
        {
            getId("id_wep1").checked = true;
        }
        else if(wepauth_encrypt_mode == "2")
        {
            getId("id_wep2").checked = true;
        }
        
        getId("id_wepkey").value = wireless_key;
        getId("wep_key_len").value = wep_key_length;
    }
    else 
    {
        if(security_mode=="2" || security_mode=="5")
        {
            getId("id_secwpa").checked = true;
            getId("id_wpamode0").checked = true;
        }
        else if(security_mode=="3" || security_mode=="6")
        {
            getId("id_secwpa").checked = true;
            getId("id_wpamode1").checked = true;
        }
        else if(security_mode=="4" || security_mode=="7")
        {
            getId("id_secwpa").checked = true;
            getId("id_wpamode2").checked = true;
        }
        
        if(wepauth_encrypt_mode == "0")
        {
            getId("id_cypher0").checked = true;
        }
        else if(wepauth_encrypt_mode == "1")
        {
            getId("id_cypher1").checked = true;
        }
        else if(wepauth_encrypt_mode == "2")
        {
            getId("id_cypher2").checked = true;
        }
        
        if (security_mode=="2" || security_mode=="3" || security_mode=="4") {
            getId("id_auth2").checked = true;
            switchAuthMode(2);
            getId("id_psk").value = wireless_key;

        } else if (security_mode=="5" || security_mode=="6" || security_mode=="7") {
            getId("id_auth1").checked = true;
            switchAuthMode(1);

            getId("id_serverip").value = secuList[9];
            getId("id_serverport").value = secuList[10];
            getId("id_serverpassword").value = secuList[11];
        }
    }
}


function switchAuthMode(mode){

    if(mode == 1) {
        getId("psk_table").style.display = "none";
        getId("server_table").style.display = "";
    }
    else if(mode == 2) {
        getId("psk_table").style.display = "";
        getId("server_table").style.display = "none";
    }
}

//Click 'Save' button
function doSave(){
    if (m_is5G == 1){
        getId("id_wpamode1").checked = true;
        getId("id_cypher1").checked = true;
    }

    var ssidEnable = securityStatus.split('|')[2];
    var radioEnable = securityStatus.split('|')[3];
    if (!(ssidEnable == 1 && radioEnable == 1)) return;

    //Check wep/wpa key

    var secMode = "";
    var key = "";
    var key_type = "";
    
    if(getId("id_secopen").checked == true)
    {
        getId("wireless_security_type").value = "0";
        if (!confirm(aContent[5]))  return;
    }
    else if(getId("id_secwep").checked == true)
    {
        getId("wireless_security_type").value = "1";
        key = trim(getId("id_wepkey").value);
        if(key=="")
        {
            alert(aContent[21]);
            getId("id_wepkey").focus();
            return;
        }
        getId("wireless_key").value = trim(getId("id_wepkey").value);

        if (!confirm(aContent[5]))  return;
    }
    else if(getId("id_secwpa").checked == true)
    {
        if (getId("id_auth1").checked == true){
            if(getId("id_wpamode0").checked == true)
                getId("wireless_security_type").value = "5";
            else if(getId("id_wpamode1").checked == true)
                getId("wireless_security_type").value = "6";
            else if(getId("id_wpamode2").checked == true)
                getId("wireless_security_type").value = "7";
        } else if (getId("id_auth2").checked == true){
            if(getId("id_wpamode0").checked == true)
                getId("wireless_security_type").value = "2";
            else if(getId("id_wpamode1").checked == true)
                getId("wireless_security_type").value = "3";
            else if(getId("id_wpamode2").checked == true)
                getId("wireless_security_type").value = "4";

            key = trim(getId("id_psk").value);  
            if(key=="")
            {
                alert(aContent[19]);
                getId("id_psk").focus();
                return;
            }
            getId("wireless_key").value = trim(getId("id_psk").value);
        }                 
    }   
        

    secMode = getId("wireless_security_type").value;
    if(secMode == "1"){
        //Check WEP Key
        var key_length = key.length;


        if((getId("wep_key_len").value == "10")&&key_length!=10)
        {
            alert(aContent[22]);
            getId("id_wepkey").focus();    
            return;
        }

        if((getId("wep_key_len").value == "26")&&key_length!=26)
        {
            alert(aContent[22]);
            getId("id_wepkey").focus();   
            return;
        }

        if(key_length!=10&&key_length!=26)
        {
            alert(aContent[22]);
            getId("id_wepkey").focus();   
            return;            
        }
        

        if(key_length == "10"){
            //10 Hex Characters
            if(!ifHexKey(key))
            {
                alert(aContent[21]);
                getId("id_wepkey").focus();
                return;
            }
        }

        if(key_length == "26"){
            //26 Hex Characters
            if(!ifHexKey(key))
            {
                alert(aContent[21]);
                getId("id_wepkey").focus();
                return;
            }
        }
    }
    else if(secMode == "2" || secMode == "3" || secMode == "4")
    {
        //Check WPA Key

        if(key.length < 8 || key.length > 64)
        {
            alert(aContent[23]);
            getId("wireless_key").focus();
            return;
        }
    } else if(secMode == "5" || secMode == "6" || secMode == "7") {
        if(trim(getId("id_serverip").value) == "")
        {
            alert(aContent[7]);
            getId("id_serverip").focus();
            return;
        }
        else
        {
            if(!isValidIpAddress(getId("id_serverip").value))
            {
                alert(aContent[8]);
                getId("id_serverip").focus();
                return;
            }
        }

        if(trim(getId("id_serverport").value) == "")
        {
            alert(aContent[9]);
            getId("id_serverport").focus();
            return;
        }
        else 
        {
            if(!isValidPort(getId("id_serverport").value))
            {
                alert(aContent[10]);
                getId("id_serverport").focus();
                return;
            }
        }

        if(trim(getId("id_serverpassword").value) == "")
        {
            alert(aContent[11]);
            getId("id_serverpassword").focus();
            return;
        }
        else if(trim(getId("id_serverpassword").value).indexOf("|") != -1)
        {
            alert(aContent[12]);
            getId("id_serverpassword").focus();
            return;
        }
    }
  

    var index_ssid = getId("wireless_index_ssid").value;
    var index_vlan = findVlanBySSID(index_ssid);
    getId("wireless_index_vlan").value = index_vlan;


    if(getId("id_secwpa").checked == true) {
        if(getId("id_wpamode0").checked == true || getId("id_cypher0").checked == true){
            if (!confirm(aContent[16])) return;
        }
    }

    getId("apply_page").value = "wl_security.html?index_vlan=" + index_vlan+"&index_ssid="+ index_ssid;
    getId("security_form").submit();
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
