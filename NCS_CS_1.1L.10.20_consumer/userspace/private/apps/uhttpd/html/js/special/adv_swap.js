var adv_swapimageversion="<? echo(adv_swapimageversion) ?>";

var topUserName = "<?echo(login_username)?>";

var tContent = new Array(); 
var aContent = new Array();

window.onload=function(){
    init_language();    
    getId("top_username").innerHTML = topUserName;
    init();

    getId("swap_btn").onclick = function(){
        doSwap();
    }
};

function init_language()
{
    getCurLanguage();
    //alert("cur_lang="+cur_lang);
    if(cur_lang=="1")
        var surl = "xml/adv_swap_en.xml";
    else
        var surl = "xml/adv_swap_fr.xml";

    StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
     tContent = ttemp;
     aContent = atemp;


    document.getElementById("restore_title").innerHTML = tContent[0]; 
    document.getElementById("restore_active_vers").innerHTML = tContent[1]; 
    document.getElementById("rest_back_ver").innerHTML = tContent[2]; 
    document.getElementById("restr_swap").innerHTML = tContent[3]; 
    document.getElementById("swap_set_span").innerHTML = tContent[4]; 

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
    if(adv_swapimageversion == "")
        return;
    
    var a_swapimageversion = adv_swapimageversion.split("|");
    var currentIdx = a_swapimageversion[0];
    var active_version = (currentIdx == 1) ? a_swapimageversion[1] : a_swapimageversion[2];
    var backup_version = (currentIdx == 1) ? a_swapimageversion[2] : a_swapimageversion[1];

    getId("active_version").innerHTML = active_version;
    getId("inactive_version").innerHTML = backup_version;

    getId("swap_currentversion").value = currentIdx;
}

function doSwap()
{
    if (confirm(aContent[0]))
    {        
        getId("swap_form").submit();
    }
}


