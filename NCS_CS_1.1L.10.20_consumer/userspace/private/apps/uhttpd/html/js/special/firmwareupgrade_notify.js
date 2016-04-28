var actZeroConfUpgrade = "<? echo(act_zeroconf_upgrade)?>";

window.onload=function(){
    init();
}	

function init()
{
    if(actZeroConfUpgrade=="Upgrade_Done")
		window.location.href = "main.html";
}
