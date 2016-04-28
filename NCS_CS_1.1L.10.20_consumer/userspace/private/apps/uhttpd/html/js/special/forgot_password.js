var device_serial_num = "<? echo(dev_serial_num) ?>";
var serial_number, new_password,re_enter_password;
var change_btn,cancel_btn;
var forgotpasswd_form;

var tContent = new Array(); 
var aContent = new Array();

document.onkeydown = function(e){

	var keycode;
	
	if(navigator.appName == "Microsoft Internet Explorer")
	{
		keycode = event.keyCode;
	}
	else
	{
		keycode = e.which;
	}

	switch (keycode)
	{
		case 13: 	// Enter
			changeThePassword();
			return;
			break;
		default:
			break;
	}
}

window.onload=function(){
    init_language();	
	
	serial_number = getId("serial_number");
	new_password = getId("new_password");
	re_enter_password= getId("re_enter_password");
	change_btn = getId("change_btn");
	cancel_btn = getId("cancel_btn");
	forgotpasswd_form = getId("forgotpasswd_form");

	change_btn.onclick = changeThePassword;
	cancel_btn.onclick = backToHomepage;
	
	serial_number.focus();
	serial_number.value = "";
	new_password.value = "";
	re_enter_password.value = "";
}

function init_language()
{
    getCurLanguage();
	//alert("cur_lang="+cur_lang);
	if(cur_lang=="1")
		var surl = "xml/forgot_password_en.xml";
	else
		var surl = "xml/forgot_password_fr.xml";

	StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
	 tContent = ttemp;
	 aContent = atemp;


	document.getElementById("reset_title").innerHTML = tContent[0]; 
	document.getElementById("reset_note").innerHTML = tContent[1]; 
	document.getElementById("reset_seril").innerHTML = tContent[2]; 
	document.getElementById("reset_pass").innerHTML = tContent[3]; 
	document.getElementById("reset_repasswd").innerHTML = tContent[4]; 
	document.getElementById("res_app").innerHTML = tContent[5]; 
	document.getElementById("res_cancel").innerHTML = tContent[6]; 
}

function backToHomepage()
{
	window.location.href = "main.html";
}


function changeThePassword()
{
	if(serial_number.value == "")
	{
		alert(aContent[0]);
		serial_number.focus();
		return;
	}

	if(serial_number.value != device_serial_num)
	{
		alert(aContent[1]);
		serial_number.focus();
		return;
	}

	if(new_password.value == "")
	{
		alert(aContent[2]);
		new_password.focus();
		return;
	}

	if(re_enter_password.value == "")
	{
		alert(aContent[3]);
		new_password.focus();
		return;
	}

	if(re_enter_password.value != new_password.value)
	{
		alert(aContent[4]);
		re_enter_password.focus();
		return;
	}
	
	forgotpasswd_form.submit();
}
