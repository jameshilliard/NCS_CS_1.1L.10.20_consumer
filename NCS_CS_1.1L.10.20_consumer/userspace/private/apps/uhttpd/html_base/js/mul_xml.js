	var xmlHttpMULLanageXML; 
	var bAsynch = true;
	var atemp = new Array(); 
	var ttemp = new Array();	
	function createMULLanguageXMLHttp()
	{
		if( window.ActiveXObject ) 
		{
			xmlHttpMULLanageXML = new ActiveXObject("Microsoft.XMLHTTP");
			bAsynch = false;
		}
		else if( window.XMLHttpRequest ) 
		{
			xmlHttpMULLanageXML = new XMLHttpRequest();
			bAsynch = true;
		}
	}
			
	function StartGetMULLanageXML(surl)
	{	
		createMULLanguageXMLHttp();
		//xmlHttpMULLanageXML.open("GET", surl, false);
		xmlHttpMULLanageXML.open("GET", surl, bAsynch);
		xmlHttpMULLanageXML.onreadystatechange = CallBackGetMULLanageXML;
		xmlHttpMULLanageXML.send(null);
	}
	
	function CallBackGetMULLanageXML()
	{
		if(xmlHttpMULLanageXML.readyState == 4 )
		{
			if(xmlHttpMULLanageXML.status == 200)
			{
			    //alert("55=="+xmlHttpMULLanageXML.responseXML);
				GetMULLanageXMLContent();
			}
		}			
	}
	
	function GetMULLanageXMLContent()
	{
		var result = xmlHttpMULLanageXML.responseXML;
		
		var pages = result.getElementsByTagName("page");
		var page = pages[0];
		
		/*****************Page Text**************************/
		var ts = page.getElementsByTagName("t");
		for (var i=0; i<ts.length; i++)
		{
			ttemp[i] = ts[i].firstChild.nodeValue;
		}
		
		/****************************************************/ 
		
		/*****************alert******************************/
		var ta = page.getElementsByTagName("a");
		for (var i=0; i<ta.length; i++)
		{ 
			
			atemp[i] = ta[i].firstChild.nodeValue;
			atemp[i] = MUL_ReplaceXMLEnter(atemp[i]);
			//alert("atemp[i]="+atemp[i]);
		}
		DrawHtml(atemp,ttemp);
		/****************************************************/
	}
	
	function MUL_ReplaceXMLEnter(s_aContent)
	{
		var sRes = s_aContent;
		var sFind = "\\n";
		var sRep = "\n";
		var pos = -1;
		
		pos = sRes.indexOf(sFind);
		while (pos >=0)
		{
			sRes = sRes.replace(sFind, sRep);
			pos = sRes.indexOf(sFind);
		}
		
		return sRes;
	}

	function get_top_bottom_en()
	{	
		document.getElementById("top_hi_span").innerHTML = "Hi"; 
		//document.getElementById("top_logout_span").innerHTML = "Logout"; 
		//document.getElementById("top_chan_span").innerHTML = "Change Password"; 
	}

	function get_top_bottom_fr()
	{
		document.getElementById("top_hi_span").innerHTML = "Bonjour"; 
		//document.getElementById("top_logout_span").innerHTML = "Fermer"; 
		//document.getElementById("top_chan_span").innerHTML = "Modification du mot de passe"; 
	}
	
	function wl_get_Left_en()
	{
		//document.getElementById("left_wl_title").innerHTML = "Wireless Settings"; 
		//document.getElementById("left_wl_net").innerHTML = "Wireless Network List"; 
		document.getElementById("left_wl_basic").innerHTML = "Basic Setup"; 
		document.getElementById("left_wl_secur").innerHTML = "Wireless Security"; 
		document.getElementById("left_wl_wps").innerHTML = "WPS"; 
		document.getElementById("left_wl_adv").innerHTML = "Advanced Wireless Setup"; 
		//document.getElementById("left_wl_control").innerHTML = "Wireless Access Control";
		//document.getElementById("left_wl_logout").innerHTML = "Logout";
	}

	function wl_get_Left_fr()
	{
		//document.getElementById("left_wl_title").innerHTML = "Sans fil - Réglages"; 
		//document.getElementById("left_wl_net").innerHTML = "Réseau sans fil - Liste"; 
		document.getElementById("left_wl_basic").innerHTML = "Configuration générale"; 
		document.getElementById("left_wl_secur").innerHTML = "Sans fil - Sécurité"; 
		document.getElementById("left_wl_wps").innerHTML = "WPS"; 
		document.getElementById("left_wl_adv").innerHTML = "Réseau sans fil - Configuration avancée"; 
		//document.getElementById("left_wl_control").innerHTML = "Contrôle de l'accès au réseau sans fil";
		//document.getElementById("left_wl_logout").innerHTML = "Fermer";
	}

	function moca_get_Left_en()
	{
		document.getElementById("left_moc_settings").innerHTML = "MoCA Settings"; 
		document.getElementById("left_moc_set").innerHTML = "MoCA Coax Setup"; 
		//document.getElementById("left_moc_logout").innerHTML = "Logout"; 
	}

	function moca_get_Left_fr()
	{
		document.getElementById("left_moc_settings").innerHTML = "MoCA - Configuration"; 
		document.getElementById("left_moc_set").innerHTML = "MoCA - Configuration coax"; 
		//document.getElementById("left_moc_logout").innerHTML = "Fermer"; 
	}

	function adv_get_Left_en()
	{
		document.getElementById("lft_adv_set_title").innerHTML = "Advanced Setup"; 
		document.getElementById("lft_adv_mng").innerHTML = "Management"; 
		//document.getElementById("lft_adv_chanpass").innerHTML = "Change Password"; 
		//document.getElementById("lft_adv_default").innerHTML = "Factory Default"; 
		//document.getElementById("lft_adv_restore").innerHTML = "Firmware Restore";
		document.getElementById("lft_adv_upgrade").innerHTML = "Firmware Upgrade"; 
		//document.getElementById("lft_tr69").innerHTML = "TR-069"; 
		//document.getElementById("lft_adv_ssh").innerHTML = "Local SSH";
		//document.getElementById("lft_adv_language").innerHTML = "Language Settings";
		//document.getElementById("lft_adv_logout").innerHTML = "Logout";
	}

	function adv_get_Left_fr()
	{
		document.getElementById("lft_adv_set_title").innerHTML = "Configuration avancée"; 
		document.getElementById("lft_adv_mng").innerHTML = "Gestion"; 
		//document.getElementById("lft_adv_chanpass").innerHTML = "Modifier le mot de passe"; 
		//document.getElementById("lft_adv_default").innerHTML = "Valeur par défaut"; 
		//document.getElementById("lft_adv_restore").innerHTML = "Restaurer micrologiciel";
		document.getElementById("lft_adv_upgrade").innerHTML = "Mise à niveau du micrologiciel"; 
		//document.getElementById("lft_tr69").innerHTML = "TR-069"; 
		//document.getElementById("lft_adv_ssh").innerHTML = "SSH local";
		//document.getElementById("lft_adv_language").innerHTML = "Paramètres de langues";
		//document.getElementById("lft_adv_logout").innerHTML = "Fermer";
	}

