//status|FreqCapabilityMask|BeaconPowerLimit|PreferredNC|PrivacyEnabledSetting|KeyPassphrase|mac|
var moca_info = "<? echo(moca_info) ?>";
//var moca_info = "Up|0x0000000000100000|21|1|0|99999999988888888|xx|";

//1. spilt with "||", the first element, node count|self node id|self mac
//others element: TX|RX|MAC
var moca_status = "<? echo(moca_status) ?>";
//var moca_status = "3 |0 |12:34:56:78 || 234 Mbps  | 245 Mbps  | 1  10:9F:A9:EB:3C:88 || 334 Mbps  | 345 Mbps  | 2  20:9F:A9:EB:3C:88 ||";


var topUserName = "<?echo(login_username)?>";

var tContent = new Array(); 
var aContent = new Array();

var channelList = {
    "0x0000000015554000": "Automatic",
    "0x0000000000004000": "D1(1150 MHz)",
    "0x0000000000010000": "D2(1200 MHz)",
    "0x0000000000040000": "D3(1250 MHz)",
    "0x0000000000100000": "D4(1300 MHz)",
    "0x0000000000400000": "D5(1350 MHz)",
    "0x0000000001000000": "D6(1400 MHz)",
    "0x0000000004000000": "D7(1450 MHz)",
    "0x0000000010000000": "D8(1500 MHz)",
};

var privacyList = {
    "0":"Disabled",
    "1":"Enabled",
};

window.onload=function(){
    init_language();
    
    getId("top_username").innerHTML = topUserName;
    init();
};

function init_language()
{
    getCurLanguage();

    if(cur_lang=="1")
        var surl = "xml/moca_status_en.xml";
    else
        var surl = "xml/moca_status_fr.xml";

    StartGetMULLanageXML(surl);

}

function DrawHtml(atemp,ttemp)
{
    tContent = ttemp;
    aContent = atemp;

    document.getElementById("moca_status_title").innerHTML = tContent[0]; 
    document.getElementById("stat_note1").innerHTML = tContent[1]; 
    document.getElementById("status_note2").innerHTML = tContent[2];
    document.getElementById("stat_channel").innerHTML = tContent[3]; 
    document.getElementById("stat_privacy").innerHTML = tContent[4]; 
    document.getElementById("stat_passwd").innerHTML = tContent[5];

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
    showMocaStatus();
}

function showMocaInfo(){
    var moca_info_list = moca_info.split("|");

    var _freqcapabilitymask = getStringValue(moca_info_list[1]);
    var _privacyenable = getStringValue(moca_info_list[4]);
    var _keypass = getStringValue(moca_info_list[5]);

    getId("id_mocachannel").innerHTML = channelList[_freqcapabilitymask];
    getId("id_mocaprivacy").innerHTML = privacyList[_privacyenable];
    if (_privacyenable == 1)
        getId("id_mocapassword").innerHTML = _keypass;
}

function showMocaStatus(){
    //moca_status = "3 |0 |12:34:56:78 | 1 || 234 Mbps  | 245 Mbps  | 1  10:9F:A9:EB:3C:88 || 334 Mbps  | 345 Mbps  | 2  20:9F:A9:EB:3C:88 ||";
    var list_status = moca_status.split("||");
    if (list_status.length < 2)     return;

    var list_adapter = list_status[0].split("|");
    if (list_adapter.length < 2)    return;

    getId("adapter_mac").innerHTML = trim(list_adapter[2]);

    var i_node_id = parseInt(trim(list_adapter[1]), 10);
    var nc_node_id = parseInt(trim(list_adapter[3]), 10);
    if(nc_node_id == i_node_id)    getId("adapter_name").innerHTML = getId("adapter_name").innerHTML + "*";

    var _id = "";
    var iNodeCount = parseInt(trim(list_adapter[0]), 10);
    for (var i=1; i<iNodeCount; i++){
        var list_device = list_status[i].split("|");
        if (list_device.length < 3)    continue;

        _id = "d" + i + "_name";
        var i_node_id = parseInt(trim(list_device[2]), 10);
        if(nc_node_id == i_node_id)    getId(_id).innerHTML = getId(_id).innerHTML + "*";

        _id = "d" + i +"_mac";
        getId(_id).innerHTML = trim(trim(list_device[2]).split("  ")[1]);

        _id = "adapter_d" + i;
        getId(_id).innerHTML = trim(list_device[0]);
        _id = "d" + i + "_adapter";
        getId(_id).innerHTML = trim(list_device[1]);
    }
}

