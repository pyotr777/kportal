var padetailObj = new Object();
padetailObj.service = new Object();


/*
* ******************************************************************************************
* 									Event handler
* ****************************** ***********************************************************
*/
$(document).on("beforepagehide", "#provider-app-detail", function(e, data){
	$("#padetail-public").hide();
	$("#padetail-unpublic").hide();
	$("#padetail-save").hide();
	
	/// Clear message
	$("#padetail-msg").html("");
});
/*
$(document).on("beforepageshow", "#provider-app-detail", function(e, data){
	console.log("#provider-app-detail: beforepageshow");
	$("#padetail-public").hide();
	$("#padetail-unpublic").hide();
	$("#padetail-save").hide();
});*/
	
$(document).on("pageshow", "#provider-app-detail", function(e, data){
	console.log("#provider-app-detail: pageshow");
//	console.log($.mobile.pageData);
/*	//just test
	$("#padetail-unpublic").show();
	$("#padetail-public").hide();
*/	

	/// Clear message
	$("#padetail-msg").html("");
	
	if ( $.mobile.pageData && /*$.mobile.pageData.id && */$.mobile.pageData.from ){
		
		padetail_clearData();
	    console.log("Parameter id=" + $.mobile.pageData.id);
	    
	    /// Save service id
	    padetailObj.serviceId = $.mobile.pageData.id;
	    
	    /// Save from tab
	    padetailObj.fromTab = $.mobile.pageData.from;
	    
	    if ( padetailObj.fromTab ){
	    	padetail_getData(padetailObj.fromTab);
	    } else {
	    	console.error("From parameter unknow");
	    }
	    
	    console.log("Bp: padetailObj.service = ");
	    console.log(padetailObj.service);
	    
	    /// Save pageData for refresh page
	    //sessionStorage.setItem("detail.pageData.id", $.mobile.pageData.id);
	    
	    /// Load data to form
	   /* if( ! db_services ){
		    market_refreshServices(function(){
		    	detail_loadData( $.mobile.pageData.id );
		    });
	    }else{
		    detail_loadData( $.mobile.pageData.id );
	    }*/
	    //padetail_loadData(padetailObj.service);
	} else {
		console.log("#detail: pageData is EMPTY.");
		pu_restoreData();
		pu_setF5();
		//alert("padetailObj.fromTab " + padetailObj.fromTab);
		//var files = $('input#provider-newapp-icon').prop("files");
		//console.log("files");
		//console.log(files);
		//alert("BP 01");
//		if( padetailObj.fromTab == "newapp" 
//			&& ( padetailObj.service.iconfile == undefined 
//					|| padetailObj.service.iconfile.name == undefined))
//		{
//			history.back();
//			return;
//		}
	}
	padetail_loadData(padetailObj.service);
});

$(document).on( "click", '#padetail-backtoedit', function(e){
	switch ( padetailObj.fromTab ){
	case "apps":
		$.mobile.changePage( '#provider-app?id=' + padetailObj.serviceId + "&from=apps", { transition: "slideup", reverse: false });
		break;
	case "editapp":
		$.mobile.changePage( '#provider-app?id=' + padetailObj.serviceId + "&from=editapp", { transition: "slideup", reverse: false});
		break;
	case "newapp":
		window.history.back();
		//alert("historyback");
		//$.mobile.changePage( '#provider?from=newapp', { transition: "slideup", reverse: false, changeHash: false});
		break;
	default:
		break;
	}
});

$(document).on( "click", '#padetail-public', function(e){
	padetail_submitApp( true );
});

$(document).on( "click", '#padetail-save', function( e ){
	///This will prevent event triggering more then once
	/*if(e.handled !== true) {
        e.handled = true;
        return true;
    }*/
	console.log("Save button is clicked");
	padetail_submitApp();
});

$(document).on( "click", '#padp-ok', function( e ){
	///This will prevent event triggering more then once
/*	if(e.handled !== true) {
        e.handled = true;
        return true;
    }*/
	console.log("Un-publish");
	padetail_submitApp( false );
});


$(document).on( "click", '#paddp-delete-btn', function( e ){
	///This will prevent event triggering more then once
/*	if(e.handled !== true) {
        e.handled = true;
        return true;
    }*/
	console.log("#paddp-delete-btn: click ");
	padetail_deleteApp();
});

/*
 * ************************************************************************
 * 							API function
 * ****************************** ******************************************
*/
function padetail_deleteApp(){
	ws_deleteService( padetailObj.serviceId, function ( res ){
		//alert("ws_deleteService");

		if( res.result == ResponseCodes.DATA_SUCCESS ){
			//$.mobile.changePage( '#provider');
			$("#provider-app-detail-delete-popup").popup("close");
			padetail_showMessage(); // Clear;
			$.mobile.changePage( '#provider?id=' + providerObj.service.id + "&from=apps", { transition: 'slide', reverse: false });
		} else {
			padetail_showMessage("Delete app fail!");
			return;
		}
		
	} );
}

function padetail_submitApp( publish ){
  loading_show();
	
  /// Send request create service to server
  switch ( padetailObj.fromTab ){
	
  case "newapp":
    if( publish ){
      padetailObj.service.status = true;
    } else {
      padetailObj.service.status = false;
    }
    //alert("from newapp: localStorage.email = " + localStorage.email);
    ws_createService( padetailObj.service, function( res ){
      if( res.uploadFile != undefined && res.uploadFile == padetailObj.service.iconfile.name ){
        ws_sendFile(res.requestId, padetailObj.service.iconfile, function(res){
          loading_hide();
          if(res.result == ResponseCodes.DATA_SUCCESS){
            alert("Create service success !");
            /// Change to list apps tab (provider page) when success
            $.mobile.changePage( '#provider?from=newapp', { /*reloadPage:true,*/ transition: "slideup" });
          } else if(res.result == ResponseCodes.FILE_ACTION_ERROR || res.result == ResponseCodes.DATA_IS_EXISTED){
            alert("Upload service icon fail !");
          } else {
            alert("Create service fail!");
          }
        });
      } else {
        var msg = "Create service fail.";
        switch( res.result ){
        case ResponseCodes.DATA_SUCCESS:
          if(res.service) {
            loading_hide();
            $.mobile.changePage( '#provider?id=' + providerObj.service.id + "&from=newapp", { transition: "slideup" });
            return;
          }
          break;
        case ResponseCodes.ERROR_SERVICE_INVALID_NAME:
          msg = "The name is already being used."
          break;
        case ResponseCodes.ERROR_SERVICE_EXEPATH_NOTFOUND:
          msg = "The executable file not found in docker image.";
          break;
        case ResponseCodes.ERROR_SERVICE_SHPATH_NOTFOUND:
          msg = "The template job script file not found in docker image.";
          break;
        case ResponseCodes.ERROR_SERVICE_SLAVEDAEMON_NOTFOUND:
          msg = "The slavedaemon file not found in docker image.";
          break;
        case ResponseCodes.ERROR_SERVICE_STAGEINDIR_NOTFOUND:
          msg = "The stage-in directory not found in docker image.";
          break;
        case ResponseCodes.ERROR_SERVICE_PRESHPATH_NOTFOUND:
          msg = "The pre-processing script file not found in docker image.";
          break;
        case ResponseCodes.ERROR_SERVICE_POSTSHPATH_NOTFOUND:
          msg = "The pre-processing script file not found in docker image.";
          break;

        default:
          break;
        }
        padetail_showMessage(msg);
        loading_hide();
     }
    });
    break;
  case "apps":
  case "editapp":
    if( publish !== undefined && publish !== padetailObj.service.status ){
      padetailObj.service.status = publish;
      if ( ! padetailObj.service.updates ){
        padetailObj.service.updates = [];
      }
      padetailObj.service.updates.push ("status");
      /*if ( padetailObj.service.updates.length > 0 ){
	padetailObj.service.updates.push ("status");
	} else {
	console.error("padetailObj.service.updates is empty.");
	console.log(padetailObj.service)
	return;
      }*/
    } else {
      console.log("No update status");
    }
		
    /// Check updates
    if ( ( padetailObj.service.updates == undefined || padetailObj.service.updates.length == 0 ) 
 	&& ( padetailObj.service.params == undefined || Object.size( padetailObj.service.params ) == 0 )) 
    {
      console.error("padetailObj.service.updates is empty.");
      console.log(padetailObj.service)
      return;
    }
    ws_updateService( padetailObj.service, function( res ){
      if( res.uploadFile != undefined && res.uploadFile == padetailObj.service.iconfile.name ){
        ws_sendFile(res.requestId, padetailObj.service.iconfile, function(res){
          loading_hide();
          if(res.result == ResponseCodes.DATA_SUCCESS){
	    alert("Update service success !");
	    /// Set notify for update change
            gl_providerChangeService( res.service );
            /// Change to list apps tab (provider page) when success
            $.mobile.changePage( '#provider?id=' + res.service + "&from=apps", { transition: "slideup", reverse: false});//, changeHash: false });
          } else if(res.result == ResponseCodes.FILE_ACTION_ERROR || res.result == ResponseCodes.DATA_IS_EXISTED){
             alert("Upload service icon fail !");
          } else {
            alert("Update image fail!");
	  }
       });
      }
      else //if( res.result === ResponseCodes.DATA_SUCCESS)// ||  res.result == ResponseCodes.ACTION_WAIT_ICON)// && res.service )
      {
        var msg = "Update service fail.";
        switch( res.result ){
        case ResponseCodes.DATA_SUCCESS:
          loading_hide();
          alert("Update service success !");
          /// Set notify for update change
          gl_providerChangeService( res.service );
          $.mobile.changePage( '#provider?id=' + res.service + "&from=editapp", { transition: "slideup", reverse: false});//, changeHash: false });
          return;
          break;
        case ResponseCodes.ERROR_SERVICE_INVALID_NAME:
          msg = "The name is already being used."
          break;
        case ResponseCodes.ERROR_SERVICE_EXEPATH_NOTFOUND:
          msg = "The executable file not found in docker image.";
          break;
        case ResponseCodes.ERROR_SERVICE_SHPATH_NOTFOUND:
          msg = "The template job script file not found in docker image.";
          break;
        case ResponseCodes.ERROR_SERVICE_SLAVEDAEMON_NOTFOUND:
          msg = "The slavedaemon file not found in docker image.";
          break;
        case ResponseCodes.ERROR_SERVICE_STAGEINDIR_NOTFOUND:
          msg = "The stage-in directory path not found in docker image.";
          break;
        case ResponseCodes.ERROR_SERVICE_PRESHPATH_NOTFOUND:
          msg = "The pre-processing script file not found in docker image.";
          break;
        case ResponseCodes.ERROR_SERVICE_POSTSHPATH_NOTFOUND:
          msg = "The pre-processing script file not found in docker image.";

        default:
          break;
        }
        padetail_showMessage(msg);
	loading_hide();
      }
    });
    break;
  default:		
    console.log("Unknow from tag.");
    break;
  }
}

function padetail_clearData(){
	
	//$("#detail-title").text("");
	//$("#padetail-image").html('');
	$("#padetail-name").text('');
	//$("#padetail-producer").text('');
	$("#padetail-provider").text('');
	$("#padetail-description").text('');
	$("#padetail-message").html('');
	
	/*$("#detail-container-button").show();
	$("#detail-uninstall").show();
	$("#detail-install").hide();
	$("#detail-cancel").hide();
	$("#detail-open").show();
	*/
	//$('#padetail-button').buttonMarkup({ icon: "delete" });
	//$("#padetail-button ui-btn-text").text('');
	$('#provider-app-detail').trigger('create');
}

function padetail_loadData( service ){
	/// Icon
	if ( service.iconfile  && service.iconfile.name ) {
		ufile_displayAsImage(service.iconfile, "padetail-image");
	} else {
		var src;
		if( service.id == undefined || service.id == ""){
			src = "images/no-image.png";
		} else {
			src = uIcon_genIconPath(service.id)
		}
		$( "#padetail-image" ).attr( "src", src );
	}
	
	//$("#detail-title").text(name);
	//$("#padetail-image").html('<img class="detail-image" src="images/' + id + '.png" onerror="this.src=\'images/service_default.png\'" />');
	var name = service.name ? uString_escapeHtml( service.name ) : "(No name)";
	var description = ( service.descript ? uString_escapeHtml(service.descript) : "(No description)" );
	$("#padetail-name").text( name );
	//$("#padetail-producer").text(service.producer);
	$("#padetail-provider").text( service.provider ? service.provider : localStorage.email);
	$("#padetail-description").text(description);
	
	/// Button / icon
	$("#padetail-backtoedit").show();
	
	switch( padetailObj.fromTab ){
	case "newapp":
		$("#padetail-unpublic").hide();
		$("#padetail-public").show();
		$("#padetail-save").show();
		$("#padetail-delete").hide();
		break;
	case "editapp":
		$("#padetail-save").show();
		$("#padetail-delete").show();
		if( padetailObj.service.status ){
			$("#padetail-public").hide();
			$("#padetail-unpublic").show();
		} else {
			$("#padetail-public").show();
			$("#padetail-unpublic").hide();
		}
		break;
	case "apps":
		$("#padetail-save").hide();
		$("#padetail-delete").show();
		if( padetailObj.service.status ){
			$("#padetail-public").hide();
			$("#padetail-unpublic").show();
		} else {
			$("#padetail-public").show();
			$("#padetail-unpublic").hide();
		}
		break;
	default:
		console.error("Unknow padetailObj.fromTab page: " + padetailObj.fromTab);
		break;
	}
	$('#provider-app-detail').trigger('create');
}
/*
 * Get/recieve data from sender tab
 */
function padetail_getData( tab ){
	switch (tab){
	case "newapp":
		padetailObj.service = providerObj.service;
		break;
	case "editapp":
		padetailObj.service = pappObj.service;
		break;
	case "apps":
		padetailObj.service = providerObj.services[ padetailObj.serviceId ];
		break;
	default:
		console.error("Unkow from name !");
		break;
	}
	pu_backupData();
}

/*
 * Show message error
 */
function padetail_showMessage( msg ){
	$("#padetail-msg").html( msg ? ( "<i>" + msg + "</i>" ) : "" );
}
