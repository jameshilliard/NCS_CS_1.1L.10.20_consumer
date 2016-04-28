var tr69_info = "<?echo(adv_tr69)?>";
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

    getId("advanced_tr69_enable").onclick = enableTR69;
    getId("advanced_tr69_disable").onclick = disableTR69;
};

function init_language()
{
    getCurLanguage();
    //alert("cur_lang="+cur_lang);
    if(cur_lang=="1")
        var surl = "xml/adv_tr69_en.xml";
    else
        var surl = "xml/adv_tr69_fr.xml";

    StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
     tContent = ttemp;
     aContent = atemp;


    document.getElementById("advanced_tr69_url").innerHTML = tContent[0]; 
    document.getElementById("advanced_tr69_username").innerHTML = tContent[1]; 
    document.getElementById("advanced_tr69_password").innerHTML = tContent[2]; 
    document.getElementById("tr_peri_info_enable").innerHTML = tContent[3]; 
    document.getElementById("tr_enable").innerHTML = tContent[4]; 
    document.getElementById("tr_disable").innerHTML = tContent[5];

    document.getElementById("tr_peri_inf_interval").innerHTML = tContent[6];
    document.getElementById("tr_backoff").innerHTML = tContent[7];
    document.getElementById("tr_inf_time").innerHTML = tContent[8];
    document.getElementById("tr_req_url").innerHTML = tContent[9];
    document.getElementById("tr_rq_username").innerHTML = tContent[10];
    document.getElementById("tr_rq_passwd").innerHTML = tContent[11];
    document.getElementById("apply_btn_span").innerHTML = tContent[12];

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
    var tr69_list = tr69_info.split("|");
    if(tr69_list.length < 10)   return;

    if(tr69_list[9] == 1)
    {
        getId("advanced_tr69_enable").checked = true;
        enableTR69();
    }
    else
    {
        getId("advanced_tr69_disable").checked = true;
        disableTR69();
    }

    getId("advanced_tr69_url").value = getValue(tr69_list[0]);
    getId("advanced_tr69_username").value = getValue(tr69_list[1]);
    getId("advanced_tr69_password").value = getValue(tr69_list[2]);
    
    if(tr69_list[3] == "1")
        getId("advanced_tr69_informenable").checked = true;
    else
        getId("advanced_tr69_informdisable").checked = true;

    getId("advanced_tr69_informinterval").value = getValue(tr69_list[4]);
    getId("advanced_tr69_informtime").value = getValue(tr69_list[5]);
    
    getId("advanced_tr69_connection_url").value = getValue(tr69_list[6]);
    getId("advanced_tr69_connection_username").value = getValue(tr69_list[7]);
    getId("advanced_tr69_connection_password").value = getValue(tr69_list[8]);
}

function enableTR69() {
    getId("advanced_tr69_url").disabled = false;
    getId("advanced_tr69_username").disabled = false;
    getId("advanced_tr69_password").disabled = false;
    getId("advanced_tr69_informenable").disabled = false;
    getId("advanced_tr69_informdisable").disabled = false;

    getId("advanced_tr69_informinterval").disabled = false;
    getId("advanced_tr69_informtime").disabled = false;
    getId("advanced_tr69_connection_url").disabled = false;
    getId("advanced_tr69_connection_username").disabled = false;
    getId("advanced_tr69_connection_password").disabled = false;
}

function disableTR69() {
    getId("advanced_tr69_url").disabled = true;
    getId("advanced_tr69_username").disabled = true;
    getId("advanced_tr69_password").disabled = true;
    getId("advanced_tr69_informenable").disabled = true;
    getId("advanced_tr69_informdisable").disabled = true;
    
    getId("advanced_tr69_informinterval").disabled = true;
    getId("advanced_tr69_informtime").disabled = true;
    getId("advanced_tr69_connection_url").disabled = true;
    getId("advanced_tr69_connection_username").disabled = true;
    getId("advanced_tr69_connection_password").disabled = true;
}

function doSave()
{   
    if(getId("advanced_tr69_url").value.indexOf("|")>=0)
    {
        alert(aContent[0]);
        return;
    }
    
    if(getId("advanced_tr69_username").value.indexOf("|")>=0||getId("advanced_tr69_username").value.indexOf(" ")>=0)
    {
        alert(aContent[1]);
        return;
    }
    if(getId("advanced_tr69_password").value.indexOf("|")>=0||getId("advanced_tr69_password").value.indexOf(" ")>=0)
    {
        alert(aContent[2]);
        return;
    }

    if(getId("advanced_tr69_connection_username").value.indexOf("|")>=0||getId("advanced_tr69_connection_username").value.indexOf(" ")>=0)
    {
        alert(aContent[3]);
        return;
    }
    if(getId("advanced_tr69_connection_password").value.indexOf("|")>=0||getId("advanced_tr69_connection_password").value.indexOf(" ")>=0)
    {
        alert(aContent[4]);
        return;
    }

    if(isNaN(parseInt(getId("advanced_tr69_informinterval").value,10)))
    {
        alert(aContent[5]);
        return;
    }
    
    var tr69_list = tr69_info.split("|");
    var _enable = (getId("advanced_tr69_enable").checked) ? 1 : 0;
    if (getId("advanced_tr69_url").value != tr69_list[0] ||
        getId("advanced_tr69_username").value != tr69_list[1] ||
        getId("advanced_tr69_password").value != tr69_list[2] ||
        getId("advanced_tr69_connection_username").value != tr69_list[7] ||
        getId("advanced_tr69_connection_password").value != tr69_list[8] ||
        _enable != tr69_list[9]){
        
        if(!confirm("changed TR-069 enable or ACS URL or Username or Password or ACS_ConnectionRequestUsername or ACS_ConnectionRequestPassword. must reboot DUT, are you sure?"))
            return;

        getId("advanced_tr69_reboot").value = 1;
    }


    getId("tr69_form").submit();
}
