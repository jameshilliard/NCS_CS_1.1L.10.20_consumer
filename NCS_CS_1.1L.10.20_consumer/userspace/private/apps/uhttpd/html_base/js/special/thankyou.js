var g_apply_page = htmlDecodeStr("<? echo(g_apply_page) ?>");
var g_action_page = htmlDecodeStr("<? echo(g_action_page) ?>");
var redirect_ip="<? echo(redirect_ip) ?>";

var advanced_tr69_reboot="<? echo(advanced_tr69_reboot) ?>";

var xmlHttpCheck;
var bAsynchCheck = true;
var timeout = 0;
var interval_id = "";
var tContent = new Array(); 
var aContent = new Array();

var topUserName = "<?echo(login_username)?>";
    
function createCheckXMLHttp()
{
    //all browser use new XMLHTttpRequest()
    //if( window.ActiveXObject ) 
    if (0)
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

    if(g_action_page.indexOf("adv_reboot.act")!=-1 || g_action_page.indexOf("adv_default.act")!=-1 || g_action_page.indexOf("diagnostic_reset_conn.act")!=-1 || g_action_page.indexOf("adv_swap.act")!=-1){        
        xmlHttpCheck.timeout = 200 * 1000;
        xmlHttpCheck.ontimeout = function(){ window.location.href = g_main_page; };
    } else if(g_action_page.indexOf("adv_upgrade.act") != -1){
        xmlHttpCheck.timeout = 200 * 1000;
        xmlHttpCheck.ontimeout = function(){ window.location.href = g_main_page; };
    } else if(g_action_page.indexOf("adv_tr69.act") != -1 && advanced_tr69_reboot == "1"){
        xmlHttpCheck.timeout = 150 * 1000;
        xmlHttpCheck.ontimeout = function(){ window.location.href = g_main_page; };
    } else {
        xmlHttpCheck.timeout = 90 * 1000;
        xmlHttpCheck.ontimeout = function(){ window.location.href = g_apply_page; };
    }

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
    //console.log("--GetCheckContent result => " + result);
    if (result == "0")
    {
        timeout = 3000;
        //alert("applypage="+g_apply_page+"@actionpage="+g_action_page);

        if(g_action_page.indexOf("adv_reboot.act")!=-1 || g_action_page.indexOf("adv_default.act")!=-1 || g_action_page.indexOf("diagnostic_reset_conn.act")!=-1 || g_action_page.indexOf("adv_swap.act")!=-1 || (g_action_page.indexOf("adv_tr69.act")!=-1 && advanced_tr69_reboot == "1"))
        {
           timeout = 200;
		   if(g_action_page.indexOf("adv_reboot.act")!=-1 || g_action_page.indexOf("adv_swap.act")!=-1 || (g_action_page.indexOf("adv_tr69.act")!=-1 && advanced_tr69_reboot == "1"))
		   {
    		   if(cur_lang=="1")
    		   {
                    //console.log("--GetCheckContent switch 1 ");
        		   getId("thank1_note").innerHTML = "The Device is rebooting.";
        		   getId("thank2_note").innerHTML = "Please wait <B><span id='show_sec'>"+timeout+"</span></B>&nbsp;seconds ...";
    		   }
    		   else
    		   {
                    //console.log("--GetCheckContent switch 2");
        		   getId("thank1_note").innerHTML = "L'appareil redémarre.";
        		   getId("thank2_note").innerHTML = "Veuillez attendre <B><span id='show_sec'>"+timeout+"</span></B>&nbsp;secondes ...";
    		   }
		   }
		   else if(g_action_page.indexOf("adv_default.act")!=-1 )
		   {

                /*
    		   if(cur_lang=="1")
    		   {
                    //console.log("--GetCheckContent switch 3 ");
        		   getId("thank1_note").innerHTML = "The Device is restoring to default setting.";
        		   getId("thank2_note").innerHTML = "Please wait <B><span id='show_sec'>"+timeout+"</span></B>&nbsp;seconds ...";
    		   }
    		   else
    		   {
                    //console.log("--GetCheckContent switch 4 ");
        		   getId("thank1_note").innerHTML = "L'appareil retourne aux valeurs par défaut.";
        		   getId("thank2_note").innerHTML = "Veuillez attendre <B><span id='show_sec'>"+timeout+"</span></B>&nbsp;secondes ...";
    		   }
               */
               getId("thank1_note").innerHTML = tContent[3];
               getId("thank2_note").innerHTML = tContent[4] + "<B><span id='show_sec'>"+timeout+"</span></B>" + tContent[5];
		   }
		   interval_id=window.setInterval("show_sec_time()", 1000);
        }
		else{
            /*
            if(cur_lang=="2")
            {
                //console.log("--GetCheckContent switch 5 ");
                getId("thank1_note").innerHTML = "Veuillez attendre pendant que nous appliquons les nouveaux réglages à votre appareil.";
                getId("thank2_note").innerHTML = "";
            }*/			
            setTimeout(function(){
                window.location.href = g_apply_page;
            }, timeout);
	    }
    } else {
        if(g_action_page.indexOf("adv_upgrade.act") != -1){
            window.parent.getUpgradeResult(result);
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
        //window.location.href = 'http://' + redirect_ip;
        window.location.href = g_main_page;
    }
}

function init_language()
{
    getCurLanguage();
	//alert("cur_lang="+cur_lang);
	if(cur_lang=="1")
		var surl = "xml/thankyou_en.xml";
	else
		var surl = "xml/thankyou_fr.xml";

	StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
	tContent = ttemp;
	aContent = atemp;

    //console.log("--DrawHtml thank_title => " + tContent[0]);
    document.getElementById("thank_title").innerHTML = tContent[0]; 
	document.getElementById("thank1_note").innerHTML = tContent[1]; 
	//document.getElementById("thank2_note").innerHTML = tContent[2]; 

	if(cur_lang=="1")
	{
		get_top_bottom_en();
	}
	else
	{
		get_top_bottom_fr();
	}
	
    StartCheckXMLHttp(g_action_page);
}

window.onload=function(){
    init_language();	
    getId("top_username").innerHTML = topUserName;  
    
}
