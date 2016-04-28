var change_pwd_old_error = "<?echo(change_pwd_old_error)?>";
var xmlHttpCheck;
var bAsynchCheck = true;
var topUserName = "<?echo(login_username)?>";

var tContent = new Array(); 
var aContent = new Array();

window.onload=function(){
    init_language();	
	
    getId("top_username").innerHTML = topUserName;
    

    getId("diy_apply_btn").onclick = function(){
        doSave();
    }
};

function init_language()
{
    getCurLanguage();
	//alert("cur_lang="+cur_lang);
	if(cur_lang=="1")
		var surl = "xml/adv_password_en.xml";
	else
		var surl = "xml/adv_password_fr.xml";

	StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
	 tContent = ttemp;
	 aContent = atemp;


	document.getElementById("chan_pass_title").innerHTML = tContent[0]; 
	document.getElementById("chan_cur_ps").innerHTML = tContent[1]; 
	document.getElementById("chan_new_ps").innerHTML = tContent[2]; 
	document.getElementById("chan_re_pw").innerHTML = tContent[3];
	document.getElementById("apply_btn_span").innerHTML = tContent[4]; 
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

    init();
}
    
function createCheckXMLHttp()
{
    if( window.ActiveXObject ) 
    {
        xmlHttpCheck = new ActiveXObject("Microsoft.XMLHTTP");
        bAsynchCheck = false;
    }
    else if( window.XMLHttpRequest ) 
    {
        xmlHttpCheck = new XMLHttpRequest();
        bAsynchCheck = true;
    }
}
        
function StartCheckXMLHttp(surl)
{   
    createCheckXMLHttp();
    
    //alert(surl);
    //xmlHttpMULLanageXML.open("GET", surl, false);
    xmlHttpCheck.open("POST", surl, bAsynchCheck);
    xmlHttpCheck.onreadystatechange = CallBackCheckXMLHttp;
    xmlHttpCheck.send(null);
}

function CallBackCheckXMLHttp()
{
    if(xmlHttpCheck.readyState == 4 )
    {
        if(xmlHttpCheck.status == 200)
        {
            GetCheckContent();
        }
    }           
}
    
function GetCheckContent()
{       
    var result = xmlHttpCheck.responseText;
    //alert("result="+result);
    if (result == "0")
    {
        
    }
}

function init()
{
    getId("id_currentpass").value="";
	getId("id_newpass").value="";
	getId("id_renewpass").value="";

	if(change_pwd_old_error=="1")
    {
        StartCheckXMLHttp("adv_pwd_err_clear.act");
        alert(aContent[0]);
		getId("id_currentpass").focus();
		return;		
	}
}

//Click 'Save' button
function doSave(){
    if(trim(getId("id_currentpass").value)=="")
    {
        alert(aContent[2]);
		getId("id_currentpass").focus();
		return;
	}

    if(trim(getId("id_newpass").value)=="")
    {
        alert(aContent[4]);
		getId("id_newpass").focus();
		return;
	}

    if(getId("id_newpass").value!=getId("id_renewpass").value)
    {
        alert(aContent[6]);
		getId("id_renewpass").focus();
		return;
	}	
	getId("password_form").submit();
}

