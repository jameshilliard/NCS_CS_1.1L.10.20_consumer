var multiple_ssid="<? echo(wireless_multiple_ssid) ?>";
//multiple_ssid: index|ifname|ssid|index_vlan|index_ssid|is5g||... ...
var mac_filter = "<? echo(wireless_access_control)?>";
//mac_filter = index_vlan|index_ssid|control_mode|mac_addr_list|

var ssidList = multiple_ssid.split("||");
var tmp_macFilter_list = new Array();
var filter_ssid = "";
var filter_access = "";

var topUserName = "<?echo(login_username)?>";

var tContent = new Array(); 
var aContent = new Array();

window.onload=function(){
    init_language();
	
    getId("top_username").innerHTML = topUserName;
    init();

    getId("wireless_index_ssid").onchange = function(){
        changeMultipleSSID();
    }


    getId("diy_add_btn").onclick = function(){
        addMacRule();
    }

    getId("diy_apply_btn").onclick = function(){
        doSave();
    }
};

function init_language()
{
    getCurLanguage();
	//alert("cur_lang="+cur_lang);
	if(cur_lang=="1")
		var surl = "xml/wl_wac_en.xml";
	else
		var surl = "xml/wl_wac_fr.xml";

	StartGetMULLanageXML(surl);
}

function DrawHtml(atemp,ttemp)
{
	 tContent = ttemp;
	 aContent = atemp;


	document.getElementById("wac_title").innerHTML = tContent[0]; 
	document.getElementById("wl_fre_span").innerHTML = tContent[3]; 
	document.getElementById("wac_ctr_mode").innerHTML = tContent[4]; 
	document.getElementById("id_macenabled").options[0].innerHTML = tContent[5]; 
	document.getElementById("id_macenabled").options[1].innerHTML = tContent[6]; 
	document.getElementById("id_macenabled").options[2].innerHTML = tContent[7]; 
	document.getElementById("wac_mac").innerHTML = tContent[8]; 

	//document.getElementById("wac_comment").innerHTML = tContent[9]; 
	if(document.getElementById("wac_list_ctr2"))
		document.getElementById("wac_list_ctr2").innerHTML = tContent[13];
	if(document.getElementById("wac_macaddr2"))
		document.getElementById("wac_macaddr2").innerHTML = tContent[8];
	if(document.getElementById("wac_select2"))
		document.getElementById("wac_select2").innerHTML = tContent[10]; 

	document.getElementById("apply_btn_span").innerHTML = tContent[15];

    document.getElementById("wl_fre_span2").innerHTML = tContent[3];
    
    document.getElementById("span_add_btn").innerHTML = tContent[17];
    document.getElementById("wac_ctr_mode2").innerHTML = tContent[16];

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
	
    loadCurWacSettings();
}

//Click 'Save' button.
function doSave(){
/*
    if(!checkWPSAndMACFilter())
    {
        alert("You must disable WPS in order to use MAC address filtering.");
        return;
    }
    
*/
    var macListStr = "";
    for(var i=0; i<tmp_macFilter_list.length; i++)
    {
        macListStr += tmp_macFilter_list[i]+" ";
    }
    getId("wireless_macCount").value = tmp_macFilter_list.length;
    getId("wireless_maclist").value = macListStr;

    var index_ssid = getId("wireless_index_ssid").value;
    var index_vlan = findVlanBySSID(index_ssid);
    getId("apply_page").value = "wl_wac.html?index_vlan=" + index_vlan+"&index_ssid="+ index_ssid;
    getId("wireless_index_vlan").value = index_vlan;
    getId("wac_form").submit();
}


//Click 'Add' button to add a mac filter, unless you click the 'Save' button, it is just a temporary data in the page.
function addMacRule(){
    var total = tmp_macFilter_list.length;
    var mac = trim(getId("id_mac").value);
    //Check MAC Address.
    if(mac == "")
    {
        alert(aContent[0]);
        getId("id_mac").focus();
        return;
    }

    if(!isValidMac(mac))
    {
        alert(aContent[0]);
        getId("id_mac").focus();
        return;
    }

    //Check MAC Address is duplicate.
    for(var i= 0; i< tmp_macFilter_list.length; i++)
    {
        if(mac == tmp_macFilter_list[i])
        {
            alert(aContent[0]);
            getId("id_mac").focus();
            return;
        }
    }

    //Check the total number or MAC Filtering.
    if(total >= 32)
    {
        alert("The MAC Filter List can not be more than 32.");
        getId("manually_mac_filter").focus();
        return;
    }

    tmp_macFilter_list[total] = mac;
    getId("id_mac").value = "";
    showMacFilterList();
}

//Change SSID, will get the SSID information from cgi.
function changeMultipleSSID(){
    var index_ssid = getId("wireless_index_ssid").value;
    var index_vlan = findVlanBySSID(index_ssid);
    if(index_ssid != "" && index_vlan != "")
        window.location.href = "wl_wac.html?index_vlan=" + index_vlan+"&index_ssid="+ index_ssid;
}

function loadSSID(){
    getId("wireless_index_ssid").innerHTML = "";
    var idx = 0;
    for(var i=0; i<ssidList.length-1 ;i++)
    {
        if (checkUserHideSSID(ssidList[i].split("|")[4]) == 1){
            continue;
        }

        var opt_show = htmlDecodeStr(ssidList[i].split("|")[2]);
        var opt_value = ssidList[i].split("|")[4];
        var is5G = ssidList[i].split("|")[5];

        opt_value = opt_value;
        if(is5G == "1")
            opt_show = opt_show+" (5G)";
        else
            opt_show = opt_show+" (2.4G)";
        getId("wireless_index_ssid")[idx] = new Option(opt_show, opt_value);
        idx++;

        if (mac_filter.split('|')[1] == opt_value)  filter_ssid = opt_show;
    }
    
    if(mac_filter.split('|')[1]!="")
        getId("wireless_index_ssid").value = mac_filter.split('|')[1];
}


function init()
{
    loadSSID();
    if(mac_filter.split("|").length <= 3)
        return;	
}

function loadCurWacSettings(){
    if (mac_filter.split('|')[2] == 0)
        filter_access = tContent[5];
    else if (mac_filter.split('|')[2] == 1)
        filter_access = tContent[6];
    else if (mac_filter.split('|')[2] == 2)
        filter_access = tContent[7];

    var filter_list = mac_filter.split("|");
    
    getId("wireless_index_vlan").value = filter_list[0];
    getId("wireless_index_ssid").value = filter_list[1];
    
    var filter_mode = filter_list[2];
    getId("id_macenabled").value = filter_mode;
    
    var mac_list = trim(filter_list[3]);
    if(mac_list!="" && mac_list.split(" ").length>0 )
    {
        tmp_macFilter_list = mac_list.split(" ");
        showMacFilterList();
    }
}

//Show MAC Filter list.
function showMacFilterList(){
    var macFilterList_tab = getId("macFilterList");
    //var tr="";
    while(macFilterList_tab.rows.length>2)
        macFilterList_tab.deleteRow(-1);

    for(var i= 0; i<tmp_macFilter_list.length; i++)
    {
        var content = new Array();
        content[0] = filter_ssid;
        content[1] = filter_access;
        content[2] = tmp_macFilter_list[i];
        content[3] = '<input type="button" class="button1" id="remove_wifi_mac_btn" name="remove_wifi_mac_btn" value="'+tContent[10]+'" onclick="removeMacRule(\''+content[2]+'\')">';
        
        var _style = "background-color:#E0E5F1";
        if(i % 2 == 0)
            var _style = "background-color:#F1F3F9";

        appendMACTableRow(macFilterList_tab, "center", _style, content);
        //tr += "<tr><td align='center' bgcolor='#3f78ad'>" + content[0] + "</td>"+"<td align='center' bgcolor='#3f78ad'>" + content[1] + "</td>"+"</tr>";        
    }
    //macFilterList_tab.innerHTML = tr;
}

function appendMACTableRow(table, align, style, content){
    var tr = table.insertRow(-1);
    var i;
    for(i=0; i < content.length; i++){
        var td = tr.insertCell(-1);        
        td.style = style;        
        td.align = align;
        td.innerHTML = content[i];
    }
}

//Click 'Remove' button to delete a mac filter, unless you click the 'Apply' button, it is just a temporary data in the page.
function removeMacRule(mac){
    if(confirm("Are you sure to delete it?"))
    {
        var rm_tmp_list = new Array();
		var rm_num = 0;
        for(var i=0; i<tmp_macFilter_list.length; i++)
        {
            if(mac != tmp_macFilter_list[i])
            {
                rm_tmp_list[rm_num] = tmp_macFilter_list[i];
				rm_num++;
            }
        }
		tmp_macFilter_list = rm_tmp_list;
        showMacFilterList();
    }
}

function findVlanBySSID(ssid_index){
    var wireless_index_vlan = 1;
    for(var i=0; i<ssidList.length-1 ;i++)
    {
        var ssid = ssidList[i].split("|")[4];
        if(ssid == ssid_index)
        {
            wireless_index_vlan = ssidList[i].split("|")[3];
            break;
        }
    }
    return wireless_index_vlan;
}

//Check the SSID is 5G
function if5G(ssid_index)
{
    var is5g = 0;
    for(var i=0; i<ssidList.length-1 ;i++){
        var ssid = ssidList[i].split("|")[4];
        if(ssid == ssid_index)
        {
            is5g = ssidList[i].split("|")[5];
            break;
        }
    }
    return is5g;
}

//Check the key is ASSCII characters
function ifASSCIIKey(key_value){
    var ifAsscii = true;
    var rege=/[|]/;
    for(var i = 0; i<key_value.length; i++){
        var mat = key_value.match(rege);
        if(mat)
        {
            ifAsscii = false;
            break;
        }
    }
    return ifAsscii;
}

//Check the key is Hex characters
function ifHexKey(key_value){
    var ifHex = true;
    var rege=/[^a-fA-F0-9]/;
    for(var i = 0; i<key_value.length; i++){
        var mat = key_value.match(rege);
        if(mat)
        {
            ifHex = false;
            break;
        }
    }
    return ifHex;
}



