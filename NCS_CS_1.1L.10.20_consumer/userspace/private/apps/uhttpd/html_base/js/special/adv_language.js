var topUserName = "<?echo(login_username)?>";

var tContent = new Array(); 
var aContent = new Array();

window.onload=function(){
    init_language();	
	
    getId("top_username").innerHTML = topUserName;  
    getId("diy_apply_btn").onclick = function(){
        getId("cur_lang_stat").value = getId("lang_select").value;		
        var form = getId("language_form");
        form.method = "Post";
        form.submit();
	}
};

function init_language()
{
    getCurLanguage();
	//alert("cur_lang="+cur_lang);
	if(cur_lang=="1")
		var surl = "xml/adv_language_en.xml";
	else
		var surl = "xml/adv_language_fr.xml";

	getId("lang_select").value = langstat;
	StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
	 tContent = ttemp;
	 aContent = atemp;


	document.getElementById("language_title").innerHTML = tContent[0]; 
	document.getElementById("lang_lang").innerHTML = tContent[1]; 
	document.getElementById("lang_select").options[0].text = tContent[2]; 
	document.getElementById("lang_select").options[1].text = tContent[3]; 
	document.getElementById("lang_select").options[2].text = tContent[4]; 	
	document.getElementById("apply_btn_span").innerHTML = tContent[5]; 
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
