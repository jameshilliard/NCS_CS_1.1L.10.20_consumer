//enable|ip|
var system_info = "<?echo(adv_systemlog)?>";
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

    getId("systemlog_enable").onclick = enablesystemlog;
    getId("systemlog_disable").onclick = disablesystemlog;
};

function init_language()
{
    getCurLanguage();
    //alert("cur_lang="+cur_lang);
    if(cur_lang=="1")
        var surl = "xml/adv_systemlog_en.xml";
    else
        var surl = "xml/adv_systemlog_fr.xml";

    StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
     tContent = ttemp;
     aContent = atemp;


    getId("systemlog_title").innerHTML = tContent[0];     
    getId("span_enabledisablename").innerHTML = tContent[1]; 
    getId("span_enable").innerHTML = tContent[2];
    getId("span_disable").innerHTML = tContent[3];

    getId("span_ip").innerHTML = tContent[4];     

    getId("apply_btn_span").innerHTML = tContent[5]; 

    if(cur_lang=="1")
    {
        get_top_bottom_en();
        adv_get_Left_en();
    }
    else
    {
        get_top_bottom_fr();
        adv_get_Left_fr();
    }
    
}


function init()
{
    //enable|ip|    
    var systemlog_list = system_info.split("|");    
    if(systemlog_list.length < 2)   return;    

    if(systemlog_list[0] == 1)
    {
        getId("systemlog_enable").checked = true;
        enablesystemlog();
    }
    else
    {
        getId("systemlog_disable").checked = true;
        disablesystemlog();
    }

    getId("adv_systemlog_ip").value = getValue(systemlog_list[1]);
}


function enablesystemlog() {
    getId("adv_systemlog_ip").disabled = false;
}

function disablesystemlog() {
    getId("adv_systemlog_ip").disabled = true;
}


function doSave()
{
    if(trim(getId("adv_systemlog_ip").value) == "")
    {
        alert(aContent[0]);
        getId("adv_systemlog_ip").focus();
        return;
    }
    else if(!isValidIpAddress(getId("adv_systemlog_ip").value))
    {
        alert(aContent[1]);
        getId("adv_systemlog_ip").focus();
        return;
    }

    
    getId("systemlog_form").submit();
}
