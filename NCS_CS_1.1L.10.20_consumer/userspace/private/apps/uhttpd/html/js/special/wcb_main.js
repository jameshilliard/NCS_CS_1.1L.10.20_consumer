var wifi_net_list ="<? echo(wireless_net_list)?>";
var Product_info = "<? echo(main_product_info)?>";
var login_success = "<?echo(wcb_main_login_success)?>";
var cur_ip_info="<? echo(main_cur_ipinfo) ?>";
var net_statation = "<? echo(main_net_station) ?>";
//cur_ip_info: ip|subnet mask|default gateway|	
//curWirelessStatus: index_vlan|index_ssid|SSID Enable|Radio Enable|SSID Name|SSID Broadcast|Security Mode|WEP KEY|WEP AUTH MODE|WPA KEY|WPA ENCRYPT MODE|Wireless band|Channel|Bandwidth|Transmit Rate|Wireless MAC|
//var wifi_net_list = vlan_index|ssid_index|ssid_enable|radio_enable|ssid_name|is5g|wireless_band|wireless_mac|chan_num|secu_mode||......;
var topUserName = "<?echo(login_username)?>";

var tContent = new Array(); 
var aContent = new Array();


window.onload=function(){
    init_language();	
    getId("top_username").innerHTML = topUserName;  
    initLoginTable();
    loadCurWiFiSettings();
	loadCurProductInfo();
	loadNetStation();
    getId("login_btn").onclick = function(){
         getId("cur_lang_stat").value = getId("lang_select").value;		
		 getId("change_lang").value = "0";
         login_submit();
    }	
    getId("lang_select").onchange = function(){
		getId("cur_lang_stat").value = getId("lang_select").value;
		getId("change_lang").value = "1";
        var form = getId("login_form");
        form.method = "Post";
        form.submit();
    }	
	
    getId("lang2_select").onchange = function(){
		getId("cur_lang_stat").value = getId("lang2_select").value;
		getId("change_lang").value = "1";
        var form = getId("login_form");
        form.method = "Post";
        form.submit();
    }		
};

function init_language()
{
    getCurLanguage();
	//alert("cur_lang="+cur_lang);
	if(cur_lang=="1")
		var surl = "xml/wcb_main_en.xml";
	else
		var surl = "xml/wcb_main_fr.xml";

	getId("lang_select").value = langstat;
	getId("lang2_select").value = langstat;
	StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
	 tContent = ttemp;
	 aContent = atemp;
	 //alert("tContent="+tContent[0]+"&"+tContent[1]);
	document.getElementById("wl_status_span").innerHTML = tContent[0]; 
	document.getElementById("wl_band24_span").innerHTML = tContent[1]; 
	document.getElementById("wl_radio24_span").innerHTML = tContent[2]; 
	document.getElementById("wl_ssid24_span").innerHTML = tContent[3]; 
	document.getElementById("wl_secu24_span").innerHTML = tContent[4]; 
	document.getElementById("wl_chan24_span").innerHTML = tContent[5]; 
	document.getElementById("wl_mac24_span").innerHTML = tContent[6];
	//document.getElementById("wl_asso24_span").innerHTML = tContent[7];
	document.getElementById("wl_band5_span").innerHTML= tContent[1];
	document.getElementById("wl_radio5_span").innerHTML = tContent[2]; 
	document.getElementById("wl_ssid5_span").innerHTML = tContent[3]; 
	document.getElementById("wl_secu5_span").innerHTML = tContent[4]; 
	document.getElementById("wl_chan5_span").innerHTML = tContent[5]; 
	document.getElementById("wl_mac5_span").innerHTML = tContent[6];
	//document.getElementById("wl_asso5_span").innerHTML = tContent[7];

    /*
	document.getElementById("moca_status_span0").innerHTML = tContent[8];
	document.getElementById("moca_chan_span0").innerHTML = tContent[9];
	document.getElementById("moca_pri_span0").innerHTML= tContent[10];
	document.getElementById("moca_mac_span0").innerHTML = tContent[11]; 
	document.getElementById("moca_status_span1").innerHTML = tContent[8]; 
	document.getElementById("moca_chan_span1").innerHTML = tContent[9]; 
	document.getElementById("moca_pri_span1").innerHTML = tContent[10];
	document.getElementById("moca_mac_span1").innerHTML = tContent[11];
    */

	document.getElementById("net_station_span").innerHTML = tContent[12];
	document.getElementById("produc_info_span").innerHTML = tContent[13]; 
	document.getElementById("model_num_span").innerHTML = tContent[14]; 
	document.getElementById("firmware_ver_span").innerHTML = tContent[15]; 
	document.getElementById("sys_time_span").innerHTML = tContent[16]; 
	document.getElementById("ip_addr_span").innerHTML = tContent[17];
	document.getElementById("sub_mask_span").innerHTML = tContent[18];
	document.getElementById("default_gate_span").innerHTML = tContent[19]; 
	

	if(cur_lang=="1")
		get_top_bottom_en();
	else
		get_top_bottom_fr();
}

function login_submit()
{
    var pass = getId("login_password").value;
    if(pass=='')
    {
        alert("Password should not be empty.");
        return;
    }
    var username = getId("login_username").value;
    if(username=='')
    {
        alert("User Name should not be empty.");
        return;
    }		
    var form = getId("login_form");
    form.method = "Post";
    form.submit();
}

function loadNetStation()
{
    if(net_statation!="")
    {
        var net_num = net_statation.split('||').length-1;
        var i=0;
        for(i=0;i<net_num;i++)
        {
	        var tr = "";
	        var interface_type = net_statation.split('||')[i].split('|')[0];
			var mac_address = net_statation.split('||')[i].split('|')[1];
			var ip_address = net_statation.split('||')[i].split('|')[2];
	        
	        tr += "<table id=\"tbl_clients\" width=\"80%%%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" class=\"status_style\" style=\"margin:0px auto 0 auto;\">\n<tr>\n";
	            
	        if(interface_type=="Wireless")
	            tr += "<td class=\"left_cell\" id=\"wrls_on\"></td>\n";
	        else
	            tr += "<td class=\"left_cell\" id=\"eth_on\"></td>\n";

	        tr += "<td class=\"mid_cell\" id=\"client_name_on\">"+mac_address+"</td>\n";
	        tr += "<td class=\"right_cell\" id=\"client_connected\">\n";
	        tr += "<p class=\"client_ip\">Connected</p>\n";
	        tr += "<p class=\"client_ip\">"+ip_address+"</p>\n";        
	        tr += "<p class=\"client_data\">Via "+interface_type+"</p>\n";
	            
	        tr += "</td>\n</tr>\n</table>\n";
			getId("clients_span").innerHTML = tr;
        }
    }
}

function initLoginTable()
{
    if(login_success=="1")
    {
        getId("login_table").style.display = "none";
        getId("user_div").style.display = "block";
    }
    else
    {
        getId("login_table").style.display = "block";
        getId("user_div").style.display = "none";
    }		
}

function loadCurProductInfo()
{
	var proinfoList = Product_info.split('|');
	var ipinfoList = cur_ip_info.split('|');
	getId("id_firmware_value").innerHTML = proinfoList[0];
	getId("id_sys_time").innerHTML = proinfoList[1];
	
	getId("id_ipaddress").innerHTML = ipinfoList[0];
	getId("id_subnet").innerHTML = ipinfoList[1];
	getId("id_gateway").innerHTML = ipinfoList[2];	
	
}

function loadCurWiFiSettings(){
    var wifi_array = wifi_net_list.split("||");
    if(wifi_array.length < 2)
        return;
	
    for(var i=0;i<wifi_array.length-1;i++)
    {
        if(wifi_array[i].split("|")[1]=="1")
        {
            //vlan
            var curWifiList = wifi_array[i].split("|");
        
            var ssid_enabled = curWifiList[2];
            if(ssid_enabled=="1")
                getId("id2_apenb").innerHTML = "Enabled";   
            else
                getId("id2_apenb").innerHTML = "Disabled";  
            getId("id2_ssid").innerHTML = curWifiList[4];

			var wifi_band = curWifiList[6];
			if(wifi_band=="0")
				getId("band2_span").innerHTML = "B";
			else if(wifi_band=="1")
				getId("band2_span").innerHTML = "G";
			else if(wifi_band=="2")
				getId("band2_span").innerHTML = "N";
			else if(wifi_band=="3")
				getId("band2_span").innerHTML = "B+G";
			else if(wifi_band=="4")
				getId("band2_span").innerHTML = "G+N";
			else if(wifi_band=="5")
				getId("band2_span").innerHTML = "B+G+N";
			
			
            if(curWifiList[9] == "0")
                getId("id2_apsecmode").innerHTML = "OPEN";
            else if(curWifiList[9] == "1")
                getId("id2_apsecmode").innerHTML = "WEP";
            else if(curWifiList[9] == "2")
                getId("id2_apsecmode").innerHTML = "WPA";
            else if(curWifiList[9] == "3")
                getId("id2_apsecmode").innerHTML = "WPA2";
            else if(curWifiList[9] == "4")
                getId("id2_apsecmode").innerHTML = "WPA/WPA2 Mixed";
            
            getId("id2_macaddr").innerHTML = curWifiList[7];
        
            var cur_channel= curWifiList[8];
            if(cur_channel == "0")
                cur_channel = "Auto";
            getId("id2_apchannel").innerHTML = cur_channel;
        }
		else if(wifi_array[i].split("|")[1]=="5")
        {
            var curWifiList = wifi_array[i].split("|");
        
            var ssid_enabled = curWifiList[2];
            if(ssid_enabled=="1")
                getId("id5_apenb").innerHTML = "Enabled";   
            else
                getId("id5_apenb").innerHTML = "Disabled";  
            getId("id5_ssid").innerHTML = curWifiList[4];

			var wifi_band = curWifiList[6];
			if(wifi_band=="6")
				getId("band5_span").innerHTML = "A+N";
			else if(wifi_band=="7")
				getId("band5_span").innerHTML = "A+N+AC";
			
			
		
            if(curWifiList[9] == "0")
                getId("id5_apsecmode").innerHTML = "OPEN";
            else if(curWifiList[9] == "1")
                getId("id5_apsecmode").innerHTML = "WEP";
            else if(curWifiList[9] == "2")
                getId("id5_apsecmode").innerHTML = "WPA";
            else if(curWifiList[9] == "3")
                getId("id5_apsecmode").innerHTML = "WPA2";
            else if(curWifiList[9] == "4")
                getId("id5_apsecmode").innerHTML = "WPA/WPA2 Mixed";
            
            getId("id5_macaddr").innerHTML = curWifiList[7];
        
            var cur_channel= curWifiList[8];
            if(cur_channel == "0")
                cur_channel = "Auto";
            getId("id5_apchannel").innerHTML = cur_channel;
        }
    }
}
