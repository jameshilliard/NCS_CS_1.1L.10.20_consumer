var hanplog_info = "<? echo(hanplog_info) ?>";
var hnaplog_status = "0";     //0:disable; 1:enable
var hanplog_content = "<? echo(hanplog_content) ?>";
var topUserName = "<?echo(login_username)?>";

var tContent = new Array(); 
var aContent = new Array();

window.onload=function(){
    init_language();	
    getId("top_username").innerHTML = topUserName;
    init();

    getId("btn_enable").onclick = function(){
        doAction("enable");
    }

    getId("btn_clear").onclick = function(){
        doAction("clear");
    }

    getId("btn_saveas").onclick = function(){
        doAction("saveas");
    }
};

function init_language()
{
    getCurLanguage();
	//alert("cur_lang="+cur_lang);
	if(cur_lang=="1")
		var surl = "xml/hnap_log_en.xml";
	else
		var surl = "xml/hnap_log_fr.xml";

	StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
	 tContent = ttemp;
	 aContent = atemp;


	document.getElementById("hnaplog_title").innerHTML = tContent[0]; 
	
    if (hnaplog_status == "0")
        document.getElementById("btn_enable").innerHTML = tContent[1];
    else
        document.getElementById("btn_enable").innerHTML = tContent[2];
	
    document.getElementById("btn_clear").innerHTML = tContent[3]; 
	document.getElementById("btn_saveas").innerHTML = tContent[4]; 
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
    if(trim(hanplog_info) == "")
        return;
    
    hnaplog_status = hanplog_info;
    drawHnapLogList();
}

function drawHnapLogList(){
    var list_content = hanplog_content.split("||||");
    //console.log("content length = " + list_content.length);

    var hnaplog_list = document.getElementById("hnaplog_list"); 
    var newTr = null;
    var newTd = null;

    for (var i=0; i<list_content.length; i++){
        if (trim(list_content[i]) == "")  continue;
        newTr = hnaplog_list.insertRow();
        
        if (i % 2)
            newTr.style = "background-color:#e0e5f1; height:20px;";
        else
            newTr.style = "background-color:#ffffff; height:20px;";

        newTd = newTr.insertCell();
        newTd.align="left";
        newTd.valign="top";
        newTd.innerHTML = list_content[i]; 
    }
}

function doAction(actionType)
{
    
    switch (actionType){
        case "enable":
            if (hnaplog_status == "0")
                getId("hnaplog_action").value = "enable";
            else
                getId("hnaplog_action").value = "disable";
            break;
        case "clear":
            getId("hnaplog_action").value = "clear";
            break;
        case "clear":
            getId("hnaplog_action").value = "saveas";
            break;
    }

    getId("hnaplog_form").submit();
}
