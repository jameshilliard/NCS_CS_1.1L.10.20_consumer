$(document).ready(function(){
	if(cur_lang=="1")
		var surl = "xml/include_inc_en.xml";
	else
		var surl = "xml/include_inc_fr.xml";

	$.ajax( {
	    url: surl,
	    type: 'get',
	    dataType: 'xml',
	    success: function(xml){	    	
			$(xml).find("text").each(function(i){
				$("#wcb_footer_span").html($(this).children("copyright").text());
				$("#top_logout_span").html($(this).children("logout").text());
				$("#left_wl_logout").html($(this).children("logout").text());
				$("#left_moc_logout").html($(this).children("logout").text());
				$("#lft_adv_logout").html($(this).children("logout").text());

				$("#left_wl_title").html($(this).children("wirelesstitle").text());
				$("#lft_adv_default").html($(this).children("factorydefault").text());
				$("#lft_adv_restore").html($(this).children("firmwarerestore").text());
				$("#lft_adv_ssh").html($(this).children("localssh").text());
				$("#lft_adv_language").html($(this).children("languagesetting").text());

				$("#left_wl_net").html($(this).children("wlnetworklist").text());
				$("#left_wl_control").html($(this).children("wlwac").text());				
				$("#top_chan_span").html($(this).children("changepassword").text());
				$("#lft_adv_chanpass").html($(this).children("changepassword").text());
 			});

 			if(typeof afterGetMultiLanguage==="function"){
				afterGetMultiLanguage(xml);
			}
	    },
	    error: function(xml){
	    	console.log("error: " + xml);
	    },
	}); 

});

