var adv_manage_info="<? echo(adv_management) ?>";
//adv_manage_info: ip|subnet mask|default gateway|auto config
//var adv_manage_info="192.168.99.254|255.255.255.0|192.168.99.11|1|";
var topUserName = "<?echo(login_username)?>";

var tContent = new Array(); 
var aContent = new Array();

window.onload=function(){
    init_language();	
    getId("top_username").innerHTML = topUserName;
    init();

    getId("reboot_mng_btn").onclick = function(){
        doReboot();
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
		var surl = "xml/adv_manage_en.xml";
	else
		var surl = "xml/adv_manage_fr.xml";

	StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
	 tContent = ttemp;
	 aContent = atemp;


	document.getElementById("mng_title").innerHTML = tContent[0]; 
	document.getElementById("mng_ip_addr").innerHTML = tContent[1]; 
	document.getElementById("mng_sub_mask").innerHTML = tContent[2]; 
	document.getElementById("mng_def_gate").innerHTML = tContent[3]; 
	document.getElementById("mng_at_conf").innerHTML = tContent[4]; 
	document.getElementById("mng_reboot").innerHTML = tContent[5]; 
	document.getElementById("apply_btn_span").innerHTML = tContent[6];
	document.getElementById("reboot_btn_span").innerHTML = tContent[5];
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
    if(adv_manage_info=="" )
        return;
    loadCurSecuritySettings();
}

function loadCurSecuritySettings(){

    var manageList = adv_manage_info.split("|");
    getId("id_ip").value = manageList[0];
    getId("id_subnet").value = manageList[1];
	
    getId("id_gateway").value = manageList[2];
    if(manageList[3]=="1")
		getId("id_autoconfig").checked = true;
	else
		getId("id_autoconfig").checked = false;
	
}

function doReboot()
{
    //if(confirm("Do you really want to reboot?"))
    if (confirm(aContent[0]))
    {
        getId("manage_form").action = "adv_reboot.cgi";
        getId("manage_form").submit();
    }
}

//Click 'Save' button
function doSave(){
    var ip = getId("id_ip").value;
	if(trim(ip)=="")
    {
        alert(aContent[1]);
		getId("id_ip").focus();
		return;
	}
	if(!isValidIpAddress(ip))
    {
        alert(aContent[2]);
		getId("id_ip").focus();
		return;        
	}

    var subnet = getId("id_subnet").value;
	if(trim(subnet)=="")
    {
        alert(aContent[3]);
		getId("id_subnet").focus();
		return;
	}
	if(!isValidateMask(subnet))
    {
        alert(aContent[4]);
		getId("id_subnet").focus();
		return;        
	}		

    var gateway = getId("id_gateway").value;
	if(trim(gateway)=="")
    {
        alert(aContent[5]);
		getId("id_gateway").focus();
		return;
	}
	if((!isValidIpAddress(gateway) && gateway!="0.0.0.0"))
    {
        alert(aContent[5]);
		getId("id_gateway").focus();
		return;        
	}	

	if(getId("id_autoconfig").checked == true)
		getId("adv_autoconfig").value = "1";
	else
		getId("adv_autoconfig").value = "0";
    getId("manage_form").submit();
}
