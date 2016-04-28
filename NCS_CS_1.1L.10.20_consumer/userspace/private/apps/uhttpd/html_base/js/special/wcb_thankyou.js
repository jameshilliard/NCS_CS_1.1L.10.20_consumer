var g_apply_page = htmlDecodeStr("<? echo(g_apply_page) ?>");
var g_action_page = htmlDecodeStr("<? echo(g_action_page) ?>");
var redirect_ip="<? echo(redirect_ip) ?>";

var xmlHttpCheck;
var bAsynchCheck = true;
var timeout = "";
var interval_id = "";
var tContent = new Array(); 
var aContent = new Array();

var topUserName = "<?echo(login_username)?>";
    
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
    
    if (result == "0")
    {
        timeout = "3000";
        //alert("applypage="+g_apply_page+"@actionpage="+g_action_page);
		
        if(g_action_page.indexOf("adv_reboot.act")!=-1 || g_action_page.indexOf("adv_default.act")!=-1 || g_action_page.indexOf("diagnostic_reset_conn.act")!=-1)
        {
           timeout = "90";
		   getId("thank1_note").innerHTML = "Do not turn off or reboot the Device during this time.";
		   getId("thank2_note").innerHTML = "Please wait <B><span id='show_sec'>"+timeout+"</span></B>&nbsp;seconds ...";
		   interval_id=window.setInterval("show_sec_time()", 1000);
        }
		else{
            setTimeout(function(){
                window.location.href = g_apply_page;
            }, timeout);
	    }
    }
}

function show_sec_time()
{
    timeout--;
    if(timeout >= 1)
    {
        getId("show_sec").innerHTML = timeout;  
    }
    else
    {
        window.clearInterval(interval_id);
        window.location.href = 'http://' + redirect_ip;
    }       
}

function init_language()
{
    getCurLanguage();
	//alert("cur_lang="+cur_lang);
	if(cur_lang=="1")
		var surl = "xml/wcb_thankyou_en.xml";
	else
		var surl = "xml/wcb_thankyou_fr.xml";

	StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
	 tContent = ttemp;
	 aContent = atemp;


	document.getElementById("thank_title").innerHTML = tContent[0]; 
	document.getElementById("thank1_note").innerHTML = tContent[1]; 
	document.getElementById("thank2_note").innerHTML = tContent[2]; 

	if(cur_lang=="1")
	{
		get_top_bottom_en();
	}
	else
	{
		get_top_bottom_fr();
	}
	
}

window.onload=function(){
    init_language();	
    getId("top_username").innerHTML = topUserName;  
    StartCheckXMLHttp(g_action_page);
}
