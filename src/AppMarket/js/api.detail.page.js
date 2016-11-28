/*
*  ******************************************************************************************
* 						Define Global Variables
*  ****************************************************************************************** 
*/
var detailObj = new Object();
detailObj.service = {};
/*
* ******************************************************************************************
* 									Event handler
* ******************************************************************************************
*/

$(document).on("pagebeforeshow", "#detail", function(e, data){
	$( "#detail-popup" ).popup( "close" );
	app_pageBeforeShow();
});

$(document).on("pageshow", "#detail", function(e, data){
	console.log("#detail: pageshow");
	console.log($.mobile.pageData);
	
	if ($.mobile.pageData && $.mobile.pageData.id){
		detail_clearData();
	    console.log("Parameter id=" + $.mobile.pageData.id);
	    
	    /// Save service id
	    detailObj.serviceId = $.mobile.pageData.id;
	    
	    /// Save pageData for refresh page
	    sessionStorage.setItem("detail.pageData.id", $.mobile.pageData.id);
	    
	    /// Load data to form
	    if( ! db_services ){
		    market_refreshServices(function(){
		    	detail_loadData( $.mobile.pageData.id );
		    });
	    }else{
		    detail_loadData( $.mobile.pageData.id );	    	
	    }
	}else{
		uStorage_restoreData();
		console.error("#detail: pageData is EMPTY.");
	}
 });

$(document).on('click', '#detail-refresh', function(e){
//$('#detail-refresh').on("click", function() {
	if ($.mobile.pageData && $.mobile.pageData.id){
		detail_clearData();
	    console.log("Parameter id=" + $.mobile.pageData.id);
	    detail_loadData($.mobile.pageData.id, true);
	}
	else{
		console.log("pageData is null.");
	}
});

$(document).on('click', '#detail-button', function(e){
	console.log("#detail-button event.");
	//var status = $( this ).text();
	console.log( "detailObj.serviceId = " + detailObj.serviceId );
	
	var status = db_services[ detailObj.serviceId ].status;
	status = status.toLowerCase();
	console.log("#detail-button click event: " + status);
	switch( status ){
	case "install":
		ws_registerService( detailObj.serviceId, function ( response ) {
			if( response.result == ResponseCodes.DATA_SUCCESS ){
				detail_popupShow("Register service success.", "check");
				
				/// Update current page form
				$( '#detail-button' ).buttonMarkup({ icon: "delete" });
				//$("#detail-button ui-btn-text").text('Cancel');
				$( '#detail-button-label' ).text( "CANCEL" );
				$( "#detail" ).trigger('create');
				
				/// Refresh list service
				market_refreshServices();
				
				/// Set app change notification 
				gl_appChange(detailObj.serviceId);
			}else{
				detail_popupShow("Register service fail.", "alert");
			}
		});
		break;
	
	case "uninstall":
		ws_userRemoveService( detailObj.serviceId, function ( response ) {
			if( response.result == ResponseCodes.DATA_SUCCESS ){
				detail_popupShow("Uninstall service success.", "check");
				
				/// Update current page form
				$('#detail-button').buttonMarkup({ icon: "check" });
				$( '#detail-button-label' ).text( "INSTALL" );
				//$("#detail-button ui-btn-text").text('INSTALL');
				$('#detail').trigger('create');

				/// Refresh list service
				market_refreshServices();
				
				/// Set app change notification 
				gl_appChange(detailObj.serviceId);
			}else{
				detail_popupShow("Uninstall service fail.", "alert");
			}
		});
		break;
	
	case "cancel":
		ws_cancelRegistering( detailObj.serviceId, function ( response ) {
			if( response.result == ResponseCodes.DATA_SUCCESS ){
				detail_popupShow("Cancel register service success.", "check");
				
				/// Update current page form
				$('#detail-button').buttonMarkup({ icon: "check" });
				$( '#detail-button-label' ).text( "INSTALL" );
				//$("#detail-button ui-btn-text").text('INSTALL');
				$('#detail').trigger('create');

				/// Refresh list service
				market_refreshServices();
				
				/// Set app change notification 
				gl_appChange(detailObj.serviceId);
			}else{
				detail_popupShow("Cancel register service fail.", "alert");
			}
		});
		break;
	}
});

/*
 * ************************************************************************
 * 							API function
 * ****************************** ******************************************
*/

function detail_clearData(){
	//$("#detail-title").text("");
	$("#detail-image").html('');
	$("#detail-name").text('');
	//$("#detail-producer").text('');
	$("#detail-description").text('');
	
	/*$("#detail-container-button").show();
	$("#detail-uninstall").show();
	$("#detail-install").hide();
	$("#detail-cancel").hide();
	$("#detail-open").show();
	*/
	$('#detail-button').buttonMarkup({ icon: "delete" });
	$("#detail-button ui-btn-text").text('');
	$('#detail').trigger('create');
}

function detail_retrieveData( callback ){
	if( detail_isExistService( detailObj.serviceId ) ){
		ws_getParameters( detailObj.serviceId, function( res, isSuccess ){
			if( res && res.params && res.params.items ){
				detail_loadDataCallback(id, methodFile);
			}
			else{
				console.error("Error: ws_getRunMethodFile not finish.");
			}
		});
	}else{
		console.error("Bp:detail_loadData(): " + id +" service not found.");
	}
}

/*function detail_loadData( id, refresh ){
	if( detail_isExistService( id ) ){
		
		$.mobile.loading("show");
		
		if(refresh || sessionStorage.getItem(id) == undefined){
			console.log("Id:" + id + " = " + sessionStorage.getItem(id));
			ws_getRunMethodFile(id, function(methodFile, isSuccess){
				if( isSuccess ){
					detail_loadDataCallback(id, methodFile);
				}
				else{
					console.error("Error: ws_getRunMethodFile not finish.");
				}
			});
		}
		else {
			detail_loadDataCallback(id, sessionStorage.getItem(id));
		}
	}else{
		console.log("Bp:detail_loadData(): " + id +" service not found.");
	}
}
*/

function detail_loadData(){
	console.log("detail_loadData()");
	
	if( ( ! db_services ) || ( ! db_services [ detailObj.serviceId ] ) ){
		console.error( "Error: detail_loadData(): db_services or db_services [id] is EMPTY.");
		return;
	}
	
	/// Parse xml text
	
	/// Service info
	console.log( "Bp: detail_loadData serviceid = " + detailObj.serviceId );
	var id = detailObj.serviceId,
	name = db_services [id] . name,
	status = db_services [id] . status,
	provider = db_services [id] . provider,
	//producer = db_services [id] . producer,
	description = db_services [id] . description;
	description = ( description ? uString_escapeHtml(description) : "(No description)" );
	//$("#detail-title").text(name);
	$("#detail-image").html('<img class="detail-image" src="' + uIcon_genIconPath(id) + '" onerror="this.src=\'images/no-image.png\'" />');
	$("#detail-name").text( name ? uString_escapeHtml( name ) : "(No name)" );
	//$("#detail-producer").text( producer ? producer : "" );
	$("#detail-provider").text( provider ? provider : "" );
	$("#detail-description").text( description );
	
	/// Set icon button
	switch ( status.toLowerCase() ){
	case "install":
		$('#detail-button').buttonMarkup({ icon: "check" });
		break;
	case "uninstall":
	case "cancel":
		$('#detail-button').buttonMarkup({ icon: "delete" });
		break;
	}
	
	$( '#detail-button-label' ).text( status.toUpperCase() );
	$( "#detail" ).trigger('create');
}

function detail_isExistService(id){
	if( db_services && db_services[ id ] ){
		return true;
	}
	return false;
//	
//	services = JSON.parse(sessionStorage.services);
//	for (var i in services) {
//		if(services[i] && services[i].service && services[i].service == id)
//			return true;
//	}
//	return false;
}


/*
 **************************************
 * Popup API
 ************************************** 
 * */
function detail_popupShow(msg, icon){
	if( ! icon ) {
		icon = "check";
	}
//	msg += '<a href="#" data-rel="popup" data-role="button" class="ui-icon-alt" data-inline="true" data-transition="pop" data-icon="' + icon + '" data-theme="e" data-iconpos="notext"></a>';
	/// Error: 
	msg += '<img src="images/icons/'+ icon +'.png" height="32" width="32">';
	$( "#detail-popup" ).popup();
	$( "#detail-popup").html(msg).popup();
	$( "#detail-popup" ).popup("open");
	
}
