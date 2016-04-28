//enable|timeout|username|password|port|
//var ssh_info = "0|43200|admin|JDEkU2pBL3daeGEkNmxoTnVySmRycW5lN2pQNXcuYnhuMA==|22|";
var ssh_info = "<?echo(adv_localssh)?>";
var topUserName = "<?echo(login_username)?>";
var g_default_pwd = "ssh*****";

var tContent = new Array(); 
var aContent = new Array();

window.onload=function(){
    init_language();    
    getId("top_username").innerHTML = topUserName;
    init();

    getId("diy_apply_btn").onclick = function(){
        doSave();
    }

    getId("advanced_ssh_enable").onclick = enableSsh;
    getId("advanced_ssh_disable").onclick = disableSsh;
    getId("sel_ssh_timeout").onchange = changeTimeout;
};

function init_language()
{
    getCurLanguage();
    //alert("cur_lang="+cur_lang);
    if(cur_lang=="1")
        var surl = "xml/adv_localssh_en.xml";
    else
        var surl = "xml/adv_localssh_fr.xml";

    StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
     tContent = ttemp;
     aContent = atemp;


    getId("ssh_title").innerHTML = tContent[0];     
    getId("sh_local").innerHTML = tContent[0]; 
    getId("sh_enable").innerHTML = tContent[1];
    getId("sh_disable").innerHTML = tContent[2];

    getId("sh_username").innerHTML = tContent[3];     
    getId("sh_passwd").innerHTML = tContent[4]; 
    getId("sh_port").innerHTML = tContent[5]; 
    getId("sh_idle").innerHTML = tContent[6]; 
    getId("apply_btn_span").innerHTML = tContent[7]; 
    getId("seconds_span").innerHTML = tContent[8];

    getId("sel_ssh_timeout").options[1].text = tContent[9]; 
    getId("sel_ssh_timeout").options[2].text = tContent[10];
    getId("sel_ssh_timeout").options[3].text = tContent[11];
    getId("sel_ssh_timeout").options[4].text = tContent[12];

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
    //enable|timeout|username|password|port|    
    var ssh_list = ssh_info.split("|");    
    if(ssh_list.length < 5)   return;    

    if(ssh_list[0] == 1)
    {
        getId("advanced_ssh_enable").checked = true;
        enableSsh();
    }
    else
    {
        getId("advanced_ssh_disable").checked = true;
        disableSsh();
    }

    getId("advanced_ssh_username").value = getValue(ssh_list[2]);
    getId("advanced_ssh_password").value = g_default_pwd;
    getId("advanced_ssh_port").value = ssh_list[4];

    if((ssh_list[1]!="1800") && (ssh_list[1]!="43200") && (ssh_list[1]!="86400") && (ssh_list[1]!="604800")) {
        getId("sel_ssh_timeout").value = "custom";
        getId("custom_td").style.display = "block";
        getId("custom_ssh_timeout").value = ssh_list[1];  
    } else
    {
        getId("custom_td").style.display = "none";
        getId("sel_ssh_timeout").value = ssh_list[1];
    }
}


function enableSsh() {
    getId("sel_ssh_timeout").disabled = false;
    getId("advanced_ssh_username").disabled = false;
    getId("advanced_ssh_password").disabled = false;
    getId("advanced_ssh_port").disabled = false;
    getId("custom_ssh_timeout").disabled = false;
}

function disableSsh() {
    getId("sel_ssh_timeout").disabled = true;
    getId("advanced_ssh_username").disabled = true;
    getId("advanced_ssh_password").disabled = true;
    getId("advanced_ssh_port").disabled = true;
    getId("custom_ssh_timeout").disabled = true;
}

function changeTimeout() {
    if(getId("sel_ssh_timeout").value == "custom") {
        getId("custom_td").style.display = "block";
    } else
    {
        getId("custom_td").style.display = "none";
    }   
}


function doSave()
{
    if(getId("advanced_ssh_username").value.indexOf("|")>=0||getId("advanced_ssh_username").value.indexOf(" ")>=0||getId("advanced_ssh_username").value=="")
    {
        alert(aContent[0]);
        return;
    }
    if(getId("advanced_ssh_password").value.indexOf("|")>=0||getId("advanced_ssh_password").value.indexOf(" ")>=0||getId("advanced_ssh_password").value=="")
    {
        alert(aContent[1]);
        return;
    }

    if(isNaN(getId("advanced_ssh_port").value)||parseInt(getId("advanced_ssh_port").value)>65535||parseInt(getId("advanced_ssh_port").value)<1)
    {
        alert(aContent[2]);
        return;
    }
    if( isInUse(getId("advanced_ssh_port").value) ) {
        alert(aContent[3]);
        return;
    }


    if(getId("sel_ssh_timeout").value == "custom")
    {
        var custom_time_value = getId("custom_ssh_timeout").value;
        if(custom_time_value ==""|| !isNumber(trim(custom_time_value)))
        {
            alert(aContent[4]);
            return;
        }
        else    
            getId("advanced_ssh_timeout").value = custom_time_value;
    }
    else
        getId("advanced_ssh_timeout").value = getId("sel_ssh_timeout").value;

    if (getId("advanced_ssh_password").value == g_default_pwd){
        getId("advanced_ssh_password").value = "";
    }
    
    getId("ssh_form").submit();
}

function isInUse(port)
{
    if( (port == "80") || (port=="4567") || (port=="7547")) {
        return true;
    }

}

