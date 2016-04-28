var update_progress = '<?echo(update_progress)?>';
var Product_info = "<? echo(main_product_info)?>";
//var curVer = '<?echo(currentFirmVer)?>';
var bar = 0;
var upload_refresh_time=0;

var topUserName = "<?echo(login_username)?>";
var timeout = 0;
var interval_id = "";


var tContent = new Array(); 
var aContent = new Array();

window.onload=function(){
    init_language();	
	
    getId("top_username").innerHTML = topUserName;  
    init(); 

    getId('update').onclick = function(){
		//alert("Upgrade Now!!");
        var uploadFileName = getId("filename").value;
        if(uploadFileName == "")
        {
            //alert("Please selecting a image file.");
            alert(aContent[0]);
            return;
        }

        var file = getId("filename");
        var form_upload = getId("upload_form");

        getId("percent").innerHTML = "0%";
        //bar=window.setInterval("http_refresh()", 2000);

        form_upload.removeChild(form_upload.filename);
        form_upload.appendChild(file);

		timeout = 230;
		if(cur_lang=="1")
		{
            getId("uploadImageDiv").innerHTML = '<input type="file" id="filename" name="filename" size="30"/>';
	        getId("percent").innerHTML = "<span id='upgrade_msg'>Please wait <B><span class='green' id='show_sec'>"+timeout+"</span></B>&nbsp;seconds ...</span>";
	    }
		else
		{
            getId("uploadImageDiv").innerHTML = '<input type="file" id="filename" name="filename" size="30"/>';
	        getId("percent").innerHTML = "<span id='upgrade_msg'>Veuillez attendre <B><span class='green' id='show_sec'>"+timeout+"</span></B>&nbsp;secondes ...</span>";
	    }
		getId("filename").disabled = true;
        interval_id=window.setInterval("show_sec_time()", 1000);
		
		form_upload.submit();
    }

};


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
        window.location.href = "main.html";
    }       
}

function init_language()
{
    getCurLanguage();
	//alert("cur_lang="+cur_lang);
	if(cur_lang=="1")
		var surl = "xml/adv_upgrade_en.xml";
	else
		var surl = "xml/adv_upgrade_fr.xml";

	StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
	 tContent = ttemp;
	 aContent = atemp;


	document.getElementById("upgrade_title").innerHTML = tContent[0]; 
	document.getElementById("up_cur_ver").innerHTML = tContent[1]; 
	document.getElementById("up_select").innerHTML = tContent[2]; 
	document.getElementById("upgrade_btn_span").innerHTML = tContent[3];

    document.getElementById("update_span").innerHTML = tContent[5];
	if(cur_lang=="1")
	{
		get_top_bottom_en();
		adv_get_Left_en();
	}
	else
	{
		get_top_bottom_fr();
		adv_get_Left_fr();
		document.getElementById("please_wait_span").innerHTML = "Veuillez attendr";
	}
}

function http_refresh_upload()
{
    upload_refresh_time++;
    var upload_percent = 10 * upload_refresh_time;
    if(bar!=0)
    {
        if(upload_percent >= 100)
            getId("percent").innerHTML = "100%";
        else
            getId("percent").innerHTML = upload_percent+"%";
    }   
}

function init()
{
    //Show Current firmware version.
    var proinfoList = Product_info.split('|');
    var curVer = proinfoList[0];
    getId("cur_ver").innerHTML = curVer;
}

//Auto firmware update progress.
function http_refresh()
{
    var statusval = "";
    var update_progress = 0;
    if(xmlhttp != null)
    {
        xmlhttp.open("GET","../../adv_upgrade_refresh.html",true);
        xmlhttp.onreadystatechange =function()
        {
            if(xmlhttp.readyState==4)
            {
                if(xmlhttp.status == 200)
                {
                    statusval = xmlhttp.responseText;
                    if(statusval == '')
                    {
                        clearInterval(bar);
                        getId("percent").innerHTML = "<span class='green'>Firmware upgradting...Please wait 2 minutes.</span>";                    
                        setTimeout(function(){
                            window.location.href = "login.html";
                        }, 120000);                    
                        return;
                    }
                    statusval = statusval.substr(1, statusval.length - 2);
                    update_progress = statusval;

                    //Show update progress.
                    if(update_progress  == "-1")
                        getId("percent").innerHTML = "<span class='red'>Image uploading failed. Failed to open the image.</span>";
                    else if(update_progress  == "-2")
                        getId("percent").innerHTML = "<span class='red'>Image uploading failed. The image size is wrong.</span>";
                    else if(update_progress  == "-3")
                        getId("percent").innerHTML = "<span class='red'>Image upgrade failed. The image is wrong.</span>";
                    else if(update_progress  == "-4")
                        getId("percent").innerHTML = "<span class='red'>Firmware upgrade is rejected. Device is operating on battery.</span>";
                    else if(update_progress  == "100")
                    {
                        clearInterval(bar);
                        getId("percent").innerHTML = "<span class='green'>Firmware upgrade is complete. The modem is rebooting.</span>";
                        setTimeout(function(){
                            window.location.href = "login.html";
                        }, 60000);
                    }
                    else{
                        getId("percent").innerHTML = update_progress + "%";
                    }
                }
            }
        }
        xmlhttp.send(null);
    }
}

function getUpgradeResult(result){
    //console.log("getUpgradeResult result = " + result);
    //upgrade status, -9:init;  9:upload finished.  -1:image error; 0:success
    if (result == -1){
        //console.log("image error");
        window.clearInterval(interval_id);
        if(cur_lang=="1")
            getId("upgrade_msg").innerHTML = "Firmware Upgrade failed, please reboot then retry. ";
        else
            getId("upgrade_msg").innerHTML = "Échec de la mise à niveau du micrologiciel, veuillez redémarrer puis réessayer. ";
    }
}
