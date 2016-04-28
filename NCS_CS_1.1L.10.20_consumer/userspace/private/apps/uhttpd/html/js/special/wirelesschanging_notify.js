var actZeroConfAutoSync = "<? echo(act_zeroconf_autosync)?>";

window.onload=function(){
    init();
}

function init()
{
    if(actZeroConfAutoSync=="AutoSync_Done")
		window.location.href = "main.html";
}
