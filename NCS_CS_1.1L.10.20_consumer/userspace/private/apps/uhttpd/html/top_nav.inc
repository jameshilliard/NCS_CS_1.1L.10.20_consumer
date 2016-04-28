<script language="javascript">
	getCurLanguage();
</script>
<div id="logo">
	<script language="Javascript">
	    document.write('<img src="images/logo.jpg" />');
	</script>		
</div>
<div id="top_nav">

	<div id="user_div">
		<ul id="userToolbar" class="on">
			<li class="first-child"><span id="top_hi_span">Hi</span> <span id="top_username"></span></li>
			<li><a href="logout.cgi"><span id="top_logout_span">Logout</span></a></li>
			<li id="top_chanpass_li"><a href="adv_password.html"><span id="top_chan_span">Change Password</span></a></li>
		</ul>
	</div>

	<div id="top_nav_header"></div>
	<div id="top_nav_content">
		<ul id="navigation">
			<script  language="javascript">				
			        //alert("cur_lang="+cur_lang);
			    var isWCB = getModelType(model_number);
				if(cur_lang=="2")
				{
					document.write('<li class="fr_Status"><a id="top_status" href="main.html"></a></li>');
					document.write('<li class="fr_WirelessSetup" id="top_wireless_li"><a id="top_wireless" href="wl_network.html"></a></li>');
					if (isWCB == 1) 	document.write('<li class="fr_MocaSetup"><a id="top_moca" href="moca_setup.html"></a></li>');
					document.write('<li class="fr_AdvancedSetup"><a id="top_advanced" href="adv_manage.html"></a></li>');
				}
				else
				{
					document.write('<li class="Status"><a id="top_status" href="main.html"></a></li>');
					document.write('<li class="WirelessSetup" id="top_wireless_li"><a id="top_wireless" href="wl_network.html"></a></li>');
					if (isWCB == 1) 	document.write('<li class="MocaSetup"><a id="top_moca" href="moca_setup.html"></a></li>');
					document.write('<li class="AdvancedSetup"><a id="top_advanced" href="adv_manage.html"></a></li>');
				}

				var icon_count = 3;
				if (isWCB == 1) 	icon_count = 4;
				
				//910:menu total length; 90:icon body width;  30: icon padding left  31: icon padding right  130: icon width
                var left_width = Math.floor((910 - 90*icon_count) / (icon_count+1));
                var marginleft = left_width - 30;
                var icon_width = left_width - 30 - 31 + 130;
                //console.log("left_width = " + left_width + ", marginleft = " + marginleft + ", icon_width = " + icon_width);
                document.getElementById("navigation").style.marginLeft = left_width+"px";

				var list_li = getId("navigation").getElementsByTagName('li');
				var li_len = list_li.length;
				for (var i=0; i<li_len; i++){
					list_li[i].style.width = icon_width+"px";
				}
			</script>
		</ul>
	</div>
	<div id="top_nav_footer"></div>
</div>

