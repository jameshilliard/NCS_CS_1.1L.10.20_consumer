var topUserName = "<?echo(login_username)?>";

var tContent = new Array(); 
var aContent = new Array();

window.onload=function(){
    init_language();	
	
    getId("top_username").innerHTML = topUserName;  
    getId("restore_btn").onclick = function(){
        //if(confirm('Do you really want to reset the current settings to default?'))
        if (confirm(aContent[0]))
		  getId("default_form").submit();
    }
};

function init_language()
{
    getCurLanguage();
	//alert("cur_lang="+cur_lang);
	if(cur_lang=="1")
		var surl = "xml/adv_default_en.xml";
	else
		var surl = "xml/adv_default_fr.xml";

	StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
     tContent = ttemp;
     aContent = atemp;


    document.getElementById("adv_default_title").innerHTML = tContent[0]; 
    document.getElementById("adv_def_setting").innerHTML = tContent[1]; 
    document.getElementById("restore_btn_span").innerHTML = tContent[2];
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