/*
* ******************************
* Define Global Variables
* ****************************** 
*/
adetailObj = new Object();
adetailObj.loaded = false;
/*
* ******************************
* Event handler
* ****************************** 
*/
$(document).on("pagebeforeshow", "#app-detail", function(event){
	console.log("#app: pagebeforeshow");
	app_pageBeforeShow();
	appObj.isChangeTab = gl_isChangeTabEvent();
	appObj.isBackPage = gl_isBackEvent();
	appObj.isOtherService = app_isOtherService();
	if( appObj.isOtherService 
			|| ( ( ! appObj.isBackPage ) && ( ! appObj.isChangeTab ) )){
		appdetail_clearData();
	}
});

$(document).on("pageshow", "#app-detail", function(e, data){
	
	console.log("#app-detail: pageshow: pageData:");
	console.log($.mobile.pageData);
	$.mobile.loading("show");
	loading_show();
	
	if( appObj.serviceId 
			//&& appObj.resFindServices 
			//&& appObj.resRunMethodFile[ appObj.serviceId ]
			//&& appObj.resGetJobsByService[ appObj.serviceId ]
			&& appObj.resGetService[ appObj.serviceId ]
			//&& ( ! gl_isNeedReloadData() ) )
	){
//		if( gl_isBackEvent() ){
//			console.log("#app: pageshow: Is back page event do not need reload data.");
//			return;
//		}
		if( ( appObj.isBackPage || appObj.isChangeTab ) 
				&& adetailObj.loaded 
				&& ( ! appObj.isOtherService ) ){
			console.log("#app: pageshow: Is back page event do not need reload data.");
			document.title = $("#app-detail-title").text();
			appObj.changeTab = false;
			appObj.isChangeTab = false;
			loading_hide();
			return;
		}

		app_tabMenu( appObj.serviceId, "#app-detail" );
		
		console.log("Bp: pageshow, #app-detail dont need reload data.");
		appdetail_loadData( appObj.serviceId );
		adetailObj.loaded = true;
		loading_hide();
	}else{

		app_tabMenu($.mobile.pageData.id, "#app-detail" );
		
		app_retrieveData( appObj.serviceId, function ( result ) {
			if( result == 0){
				/// Fill data to form
				appdetail_loadData(appObj.serviceId);
				adetailObj.loaded = true;
				loading_hide();
			}else{
				console.error("Error: #app-detail pageshow: app_retrieveData has problem. result = " + result);;
			}
		});
	}
	
 });

$(document).on('click', '#app-detail-btn-refresh', function(e){
	console.log( "Bp: refresh onclick." );
	loading_show();
	if( appObj.serviceId ){
		ws_getService(appObj.serviceId, function( resService ){
			if( resService.result ){
				console.error();
				return;
			}
			appObj.resGetService [appObj.serviceId] = resService;
			appdetail_loadData();
			loading_hide();
		});
		/*
		 * 
		 ws_getRunMethodFile(appObj.serviceId, function(methodFile, isSuccess){
			if( isSuccess ){
				appObj.resRunMethodFile [appObj.serviceId] = methodFile;
				appdetail_loadData();
				loading_hide();
			}
		});
		*/		
	}
});

/*
 * ******************************
 * API function
 * ****************************** 
*/
function appdetail_clearData(){
	$("#app-detail-title").text("");
	$("#app-detail-image").html('');
	$("#app-detail-name").text('');
	//$("#app-detail-producer").text('');
	$("#app-detail-provider").text('');
	$("#app-detail-description").text('');
	$('#app-detail-button').buttonMarkup({ icon: "delete" });
	$("#app-detail-button ui-btn-text").text('');
	$("#app-detail").trigger("pagecreate");
}

function appdetail_loadData(){
	console.log("Bp appdetail_loadData(): appObj.serviceId = " + appObj.serviceId);
	var resService = appObj.resGetService[ appObj.serviceId ];
	console.log(resService);
	document.title = resService.name;
	$("#app-detail-title").text(resService.name);
	$("#app-detail-image").html('<img class="detail-image" src="' + uIcon_genIconPath(resService.service) + '" onerror="this.src=\'images/no-image.png\'" alt="' + resService.name +'"/>');
	$("#app-detail-name").text(resService.name);
	$("#app-detail-title").text(resService.name);
	$("#app-detail-service-id").text(resService.service);
	//$("#app-detail-producer").text(resService.producer);
	$("#app-detail-provider").text(resService.provider);
	$("#app-detail-description").text(resService.description);
	
	//$('#app-detail-button').buttonMarkup({ icon: "delete" });
	//$("#app-detail-button ui-btn-text").text(status);
	
	//console.log("Name: " + resService.name + ", producer: " + resService.producer);
	$("#app-detail").trigger("pagecreate");
}
/*
function appdetail_loadData2(){
	var serviceid = appObj.serviceId, 
		methodFile = appObj.resRunMethodFile[serviceid];
	 	resFindServices = appObj.resFindServices;
	if( ! methodFile )
		return;
	
	var xmlDoc = $.parseXML( methodFile );
	var xml = $( xmlDoc );
	$( xml ).find('Service').each(function(){
		var $service = $(this);
		var name = $(this).attr('name'),
			//producer = $(this).attr('producer'),
			description = $(this).children('Description').text(),
			provider ;
		console.log("Bp: appdetail_loadData(): appObj = ");
		console.log(resFindServices);
		if (  resFindServices && $.isArray( resFindServices )){
			$.each(resFindServices, function (index){
				if( this.service == serviceid){
					provider = this.provider;
					return false; /// break loop
				}
			});
			
		}else{
			console.error("Error: appdetail_loadDataCallback(id, methodFile): ")
		}
		
		document.title = name;
		$("#app-detail-title").text(name);
		$("#app-detail-image").html('<img class="detail-image" src="images/' + serviceid + '.png" onerror="this.src=\'images/service_default.png\'" alt="' + name +'"/>');
		$("#app-detail-name").text(name);
		$("#app-detail-title").text(name);
		$("#app-detail-service-id").text(serviceid);
		//$("#app-detail-producer").text(producer);
		$("#app-detail-provider").text(provider);
		$("#app-detail-description").text(description);
		
		//$('#app-detail-button').buttonMarkup({ icon: "delete" });
		$("#app-detail-button ui-btn-text").text(status);
		
		console.log("Name: " + name + ", producer: " + producer);
				
		return false;
	});
	$("#app-detail").trigger("pagecreate");
}

*/
/*
 **************************************
 * Popup API
 ************************************** 
 * */
function detail_popupShow( msg, icon){
	if( ! icon ){
		icon = "check";
	}

	msg += '<img src="images/icons/'+ icon +'.png" height="32" width="32">';
	$( "#detail-popup").popup({ positionTo: "window" });
	$( "#detail-popup").html(msg).popup("option", "positionTo", "window");
	$( "#detail-popup" ).popup( "open" );
}
 