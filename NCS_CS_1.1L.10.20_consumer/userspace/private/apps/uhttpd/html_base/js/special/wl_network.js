var wirelessNetList = "<? echo(wireless_net_list)?>";
//var wirelessNetList = "0|1|1|1|ssid_name|1|6|2f:23:44;55:66:77|4||";
var listArray = wirelessNetList.split("||");
var topUserName = "<?echo(login_username)?>";

var tContent = new Array(); 
var aContent = new Array();

var s_wpawpa2mixed = "WPA/WPA2 Mixed";

window.onload=function(){
    getId("top_username").innerHTML = topUserName;  
}

//call from include_inc.js, after get xml data from include_inc.xml
function afterGetMultiLanguage(xml){
    $(xml).find("text").each(function(i){
        s_wpawpa2mixed = $(this).children("wpawpa2mixed").text();
    });

    init();
}	

function init_language()
{
    getCurLanguage();
	//alert("cur_lang="+cur_lang);
	if(cur_lang=="1")
		var surl = "xml/wl_network_en.xml";
	else
		var surl = "xml/wl_network_fr.xml";

	StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
	 tContent = ttemp;
	 aContent = atemp;

	document.getElementById("wl_pri_title").innerHTML = tContent[0]; 
	//document.getElementById("wl_pub_title").innerHTML = tContent[1]; 
	document.getElementById("wl_net_name").innerHTML = tContent[2]; 
	document.getElementById("wl_net_freq").innerHTML = tContent[3]; 
	
	document.getElementById("wl_net_mac").innerHTML = tContent[4]; 
	
	document.getElementById("wl_net_secur").innerHTML = tContent[6]; 
	document.getElementById("wl_net_action").innerHTML = tContent[7]; 
	for(var i=0;i<document.getElementsByName("wl_net_edit").length;i++)
		document.getElementsByName("wl_net_edit")[i].innerHTML = tContent[8]; 
	 
	document.getElementById("wl_net_list_title").innerHTML = tContent[9];

	if(cur_lang=="1")
	{
		get_top_bottom_en();
		wl_get_Left_en();
	}
	else
	{
		get_top_bottom_fr();
		wl_get_Left_fr();
	}

}


function init()
{
    if(listArray.length-1 ==0 )
        return;
    loadNetLists();
    init_language();	
}

function loadNetLists(){
    //vlan
    var i=0;
    var tr = "";
	   tr+= "<table cellpadding='0' cellspacing='1' border='0' width='550px'>";
	   tr+= "<tr><td colspan='5'><strong><span id='wl_pri_title' style='font-weight:bold;font-size:12px;'>Private WiFi Network</span>:</strong></td></tr>";
	   tr+="<tr>";
	   tr+="<td align='center' width='20%' bgcolor='#3f78ad'><strong class='white'><span id='wl_net_name' style='font-weight:bold;font-size:12px;'>Name</span></strong></td>";
	   tr+="<td align='center' width='20%' bgcolor='#3f78ad'><strong class='white'><span id='wl_net_freq' style='font-weight:bold;font-size:12px;'>Frequency</span></strong></td>";
       tr+="<td align='center' width='20%' bgcolor='#3f78ad'><strong class='white'><span id='wl_net_mac' style='font-weight:bold;font-size:12px;'>MAC Address</span></strong></td>";
	   tr+="<td align='center' width='30%' bgcolor='#3f78ad'><strong class='white'><span id='wl_net_secur' style='font-weight:bold;font-size:12px;'>Security Mode</span></strong></td>";
	   tr+="<td align='center' width='10%' bgcolor='#3f78ad'><strong class='white'><span id='wl_net_action' style='font-weight:bold;font-size:12px;'>Action</span></strong></td>";
	   tr+="</tr>";		
	
    for(i=0;i<listArray.length-1;i++)
    {
    	if (checkUserHideSSID(listArray[i].split("|")[1]) == 1){
    		continue;
    	}

        var index_vlan = listArray[i].split("|")[0];
		var index_ssid = listArray[i].split("|")[1];
		var ssid_enabled = listArray[i].split("|")[2];
        var radio_enabled = listArray[i].split("|")[3];
		var ssid_name = listArray[i].split("|")[4];
		var ssid_is5g = listArray[i].split("|")[5];
		var ssid_band = listArray[i].split("|")[6];		
		var ssid_mac = listArray[i].split("|")[7];
		var ssid_security_mode = listArray[i].split("|")[9];
		if(ssid_security_mode=="0")
			ssid_security_mode="OPEN";
		else if(ssid_security_mode=="1")
			ssid_security_mode="WEP";
		else if(ssid_security_mode=="2")
			ssid_security_mode="WPA";
		else if(ssid_security_mode=="3")
			ssid_security_mode="WPA2";
		else if(ssid_security_mode=="4")
			ssid_security_mode=s_wpawpa2mixed;		
			

		var bgcolor=""
	    if(ssid_is5g=="0")
			bgcolor = "#E0E5F1";
		else
			bgcolor = "#F1F3F9";
		
	    var band_value = "";
		tr += "<tr>";
		tr += "<td align=\"center\" width=\"20%\" bgcolor=\""+bgcolor+"\">" + ssid_name + "</td>";
		tr += "<td align=\"center\" width=\"20%\" bgcolor=\""+bgcolor+"\">";

		if(ssid_is5g == "0")
		{
			tr += "2.4 GHz";

			if(ssid_band == "0")
		    	band_value = "B";
			else if(ssid_band == "1")
				band_value = "G";
			else if(ssid_band == "7")
				band_value = "N";
			else if(ssid_band == "2")
				band_value = "B,G";
			else if(ssid_band == "9")
				band_value = "G,N";
			else if(ssid_band == "10")
				band_value = "B,G,N";
		}
		else if(ssid_is5g == "1")
		{
			tr += "5 GHz";
			if(ssid_band == "3")
				band_value = "A";
			else if(ssid_band == "7")
				band_value = "N";
			else if(ssid_band == "11")
				band_value = "A,N";
		}
		tr += "</td>";	

		tr += "<td align=\"center\" width=\"20%\" bgcolor=\""+bgcolor+"\">";

		tr += ssid_mac;
		tr += "</td>";
			
		tr += "<td align=\"center\" width=\"30%\" bgcolor=\""+bgcolor+"\">" + ssid_security_mode + "</td>";
		tr += "<td align=\"center\" width=\"10%\" bgcolor=\""+bgcolor+"\"><a href=\""+"wl_security.html?index_vlan=" + index_vlan + "&index_ssid=" + index_ssid + "\"" 
			+ "class=\"action\"><span name='wl_net_edit' style='font-weight:bold;font-size:12px;'>Edit</span></a></td>";
		tr += "</tr>";			
    }
	tr+="</table>";
	getId("private_span").innerHTML = tr;
}
