//status|FreqCapabilityMask|BeaconPowerLimit|PreferredNC|PrivacyEnabledSetting|KeyPassphrase|mac|
var moca_info = "<? echo(moca_info) ?>";
//var moca_info = "Up|0x0000000000100000|21|1|1|99999999988888888|xx|";

var topUserName = "<?echo(login_username)?>";

var tContent = new Array(); 
var aContent = new Array();

window.onload=function(){
    init_language();
    
    getId("top_username").innerHTML = topUserName;
    init();

    getId("diy_apply_btn").onclick = function(){
        doSave();
    }
};

function init_language()
{
    getCurLanguage();

    if(cur_lang=="1")
        var surl = "xml/moca_setup_en.xml";
    else
        var surl = "xml/moca_setup_fr.xml";

    StartGetMULLanageXML(surl);

}

function DrawHtml(atemp,ttemp)
{
    tContent = ttemp;
    aContent = atemp;

    document.getElementById("moca_setup_title").innerHTML = tContent[0]; 
    document.getElementById("moca_set_status").innerHTML = tContent[1]; 
    document.getElementById("moca_set_channel").innerHTML = tContent[2];
    document.getElementById("moca_bean_power").innerHTML = tContent[3]; 
    document.getElementById("moca_net_ctr").innerHTML = tContent[4]; 
    document.getElementById("moca_set_enable").innerHTML = tContent[5];

    document.getElementById("moca_set_disable").innerHTML = tContent[6];     
    document.getElementById("moca_privacy").innerHTML = tContent[7]; 
    document.getElementById("moca_net_passwd").innerHTML = tContent[8];     
    document.getElementById("moca_min_max").innerHTML = tContent[9];
    document.getElementById("moca_show_passwd").innerHTML = tContent[10];

    document.getElementById("net_ctr_mac").innerHTML = tContent[11]; 
    document.getElementById("moca_link_status").innerHTML = tContent[12];
    document.getElementById("moca_vie_span").innerHTML = tContent[13]; 
    document.getElementById("diy_apply_btn").innerHTML = tContent[14]; 

    if(cur_lang=="1")
    {
        get_top_bottom_en();
        moca_get_Left_en();
    }
    else
    {
        get_top_bottom_fr();
        moca_get_Left_fr();
    }
}


function init()
{
    showMocaInfo();

    getId("id_mocaprivacyon").onclick = doMocaOnoff;
    getId("id_mocaprivacyoff").onclick = doMocaOnoff;

    getId("id_mocashowpwd").onclick = doShowPwd;
    getId("moca_vie_span").onclick = function(){window.top.location.href = "moca_status.html";};

    doMocaOnoff();
    doShowPwd();
}

function showMocaInfo(){
    var moca_info_list = moca_info.split("|");
    //console.log("moca_info_list length => " + moca_info_list.length);

    var _status = getStringValue(moca_info_list[0]);
    var _freqcapabilitymask = getStringValue(moca_info_list[1]);
    var _beaconpowerlimit = getStringValue(moca_info_list[2]);
    var _preferrednc = getStringValue(moca_info_list[3]);
    var _privacyenable = getStringValue(moca_info_list[4]);
    var _keypass = getStringValue(moca_info_list[5]);
    var _mac = trim(getStringValue(moca_info_list[6]));

    getId("mocastatus_span").innerHTML = (_status.toUpperCase() == "UP") ? "Connected" : "Disconnected";
    getId("id_mocachannel").value = _freqcapabilitymask;
    getId("id_mocabeacon").value = _beaconpowerlimit;
    
    if (_preferrednc == 1)
        getId("id_controlleron").checked = true;
    else
        getId("id_controlleroff").checked = true;

    if (_privacyenable == 1)
        getId("id_mocaprivacyon").checked = true;
    else
        getId("id_mocaprivacyoff").checked = true;

    //getId("id_mocachannel").value = _privacyenable;
    getId("id_mocapwd").value = _keypass;

    getId("moca_nc_mac").innerHTML = _mac;



}

function doMocaOnoff()
{    
    if (getId("id_mocaprivacyon").checked) 
        getId("id_mocapwd").disabled = false;
    else 
        getId("id_mocapwd").disabled = true;  
}

function doShowPwd()
{
    if(getId("id_mocashowpwd").checked)
    {
        getId("id_mocapwd").type = "text";
    }
    else
    {
        getId("id_mocapwd").type = "password";
    }
}


//Click 'Save' button.
function doSave(){
    if(trim(id_mocapwd.value) == "") {
        alert(aContent[0]);
        getId("id_mocapwd").focus();
        return;
    } else if (getId("id_mocapwd").value.length < 12 || getId("id_mocapwd").value.length > 17){
        alert(aContent[1]);
        getId("id_mocapwd").focus();
        return;
    } else if(!isNumber(id_mocapwd.value)) {
        alert(aContent[2]);
        getId("id_mocapwd").focus();
        return;
    } else if(trim(id_mocapwd.value).indexOf("|") != -1) {
        alert(aContent[3]);
        getId("id_mocapwd").focus();
        return;
    }


    if (getId("id_controlleron").checked == true)
        setup_form.lan_moca_enable.value = 1;

    if (getId("id_mocaprivacyon").checked == true)
        setup_form.lan_moca_privacy.value = 1;


    getId("setup_form").submit();
}
