/*
*  ******************************************************************************************
* 						Define Global Variables
*  ****************************************************************************************** 
*/

var appObj = new Object();
appObj.isOtherServices= { "#app": false, "#app-submit": false, "#app-detail": false };
appObj.isOtherService = false;
appObj.isChangeTab = false;
appObj.isBackPage = false;
//appObj.resFindServices = [];
//appObj.resRunMethodFile = {};
appObj.resGetService = {};
appObj.resGetParameters = {};	/// List request params of service
appObj.resGetJobsByService = {};
appObj.loaded = false;
appObj.interval;
appObj.hasRunningJob = false;

/*
* ******************************************************************************************
* 									Event handler
* ******************************************************************************************
*/
$(document).on("pagebeforeshow", "#app", function(event){
	console.log("#app: pagebeforeshow");
	
	app_pageBeforeShow();
	
	appObj.isChangeTab = gl_isChangeTabEvent();
	appObj.isBackPage = gl_isBackEvent();
	appObj.isOtherService = app_isOtherService();
	if( appObj.isOtherService 
			|| ( ( ! appObj.isBackPage ) && ( ! appObj.isChangeTab ) )){
		app_clearData();
	}
//	if ($.mobile.pageData && $.mobile.pageData.id){
//		//app_clearData();
//	    console.log("Parameter id=" + $.mobile.pageData.id);
//	    
//	    /// Save service id
//	    appObj.serviceId = $.mobile.pageData.id;
//	    
//	    /// Save pageData for refresh page
//	    sessionStorage.setItem("app.pageData.id", $.mobile.pageData.id);
//	}else{
//		if( sessionStorage.getItem("app.pageData.id") != undefined ){
//			if ( $.mobile.pageData == undefined ) {
//				$.mobile.pageData = new Object();
//			}
//			if( $.mobile.pageData.id == undefined ) {
//				$.mobile.pageData.id = sessionStorage.getItem("app.pageData.id");
//				/// Save service id
//			    appObj.serviceId = $.mobile.pageData.id;
//				
//			    appObj.f5 = true;
//				console.log("Bp: #app: pagebeforeshow F5");
//			}
//		}else{
//			//console.error("Page load: Can not load this page, please back to home and try again.");
//			console.error("Page load: app.pageData.id not exist.");
//		}
//	}
});

$( document ).on( "pageshow", "#app", function(event) {
	console.log("#app: pageshow: pageData:");
	console.log($.mobile.pageData);
	
	loading_show();
	
	/// Reset flag
	appObj.hasRunningJob = false;
	
	if( appObj.serviceId 
			&& appObj.resGetParameters [ appObj.serviceId ]
			&& appObj.resGetService[ appObj.serviceId ]
			//&& appObj.resFindServices 
			//&& appObj.resRunMethodFile[ appObj.serviceId  ]
			&& appObj.resGetJobsByService[ appObj.serviceId ]
			&& ( ! gl_isNeedReloadData() ) ){
		if( ( appObj.isBackPage || appObj.isChangeTab ) 
				&& appObj.loaded 
				&& ( ! appObj.isOtherService ) ){
		/*if( ( appObj.isBackPage || appObj.isChangeTab ) && appObj.loaded ){*/
			console.log("#app: pageshow: Is back page event do not need reload data.");
			document.title = $('#app-title').text();
			appObj.changeTab = false;
			appObj.isChangeTab = false;
			loading_hide();
			app_setIntervalRefreshStatus();
			return;
		}

		//app_tabMenu($.mobile.pageData.id, "#app" );
		app_tabMenu( appObj.serviceId, "#app" ); 
		console.log("Bp: pageshow, #app dont need reload data.");
		/// Fill data to form
		app_loadData();
		
		appObj.loaded = true;
		loading_hide();
		app_setIntervalRefreshStatus();
	}else{
		//app_tabMenu($.mobile.pageData.id, "#app" );
		app_tabMenu( appObj.serviceId , "#app" );
		app_retrieveData( appObj.serviceId, function ( result ) {
			if( result == 0){
				/// Fill data to form
				app_loadData();
				
				/// Set flag for dont need load again 
				appObj.loaded = true;
				
				loading_hide();
				
				app_setIntervalRefreshStatus();
			}else{
				console.error("Error: #app pageshow: app_retrieveData has problem. result = " + result);;
			}
		});
	}	
 });

$( document ).on( "pagehide", "#app", function(event) {
	console.log("pagehide");
	app_clearInterval();
});

$(document).on('click', '#app-btn-refresh', function(e){
	console.log( "Bp: refresh onclick." );
	
	loading_show();
	
	if( appObj.serviceId ){
		app_refreshJobsStatus( function(result){
			if ( result == 0 ){
				app_loadJobsStatus();
				loading_hide();
			}else{
				console.error ("Error: #app-btn-refresh: app_refreshJobsStatus data fail.");
				loading_hide();
			}
		} );
	}else{
		console.error ("Error: #app-btn-refresh: appObj.serviceId is UNDEFINED.");
		loading_hide();
	}
});

$(document).on('click', '.app-menu-btn-remove', function(e){
	console.log("Bp: remove app now");
	
	if( appObj.serviceId ){
		console.log('Bp: $(this).attr(data-tab)');
		console.log($(this).attr('data-tab'));
		var tab = $(this).attr('data-tab');
		var tab_popupShow = app_popupShow;
		var app_tab_menu = "#app-status-menu";
		switch(tab){
		case "status":
			app_tab_menu = "#app-status-menu";
			tab_popupShow = app_popupShow;
			break;
		case "submit":
			app_tab_menu = "#app-submit-menu";
			tab_popupShow = submit_popupShow;
			break;
		case "detail":
			app_tab_menu = "#app-detail-menu";
			tab_popupShow = detail_popupShow;
			break;
		}
		//console.log("Bp: .app-menu-btn-remove: click: app_tab_menu = " + app_tab_menu + ", tab_popupShow = " + tab_popupShow);

		$( app_tab_menu ).popup( "close" );
				
		ws_userRemoveService( appObj.serviceId, function(res){
			switch(  res.result ){
			
			case ResponseCodes.DATA_SUCCESS:
				
				/// Remove app success
				setTimeout(function() { 
					tab_popupShow("Remove app success !", "check");
				}, 500);
				setTimeout(function() { 
					gl_appRemove(appObj.serviceId);
					
					$.mobile.changePage("#home");
				}, 2000);
				break;
			case ResponseCodes.DATA_SELECT_EMPTY:
				console.log("No permission");
				setTimeout(function() { 
					gl_appRemove(appObj.serviceId);
					
					$.mobile.changePage("#home");
				}, 2000);
				break;
			default: 
				msg = "Remove app fail !";
				break;
			}
			if( msg.length > 0 ){
				/// Remove app fail.
				setTimeout(function() { 
					tab_popupShow( msg, "alert");
				}, 500);
			}
			/*if( res.valid == 1 ){
				/// Remove app success
				setTimeout(function() { 
					tab_popupShow("Remove app success !", "check");
				}, 500);
				setTimeout(function() { 
					gl_appRemove(appObj.serviceId);
					
					$.mobile.changePage("#home");
				}, 2000);
				
			}else{
				/// Remove app fail.
				setTimeout(function() { 
					tab_popupShow("Remove app fail !", "alert");
				}, 500);
			}*/
		});
	}
	
});

/*
 * ************************************************************************
 * 							API function
 * ************************************************************************
*/
function app_clearData(){
	$("#app-title").text('');
	$( "#app-list-message" ).html('');
	$( "#app-list-jobs" ).empty();
	$( "#app" ).trigger("create");
}

function app_loadJobsStatus(){
	$( "#app-list-message" ).html('');
	
	var resJobs = appObj.resGetJobsByService[ appObj.serviceId ];
	
	if( resJobs && resJobs.jobs && resJobs.jobs.items && resJobs.jobs.items.length > 0 ){
		$( "#app-list-jobs" ).empty();
		$.each( resJobs.jobs.items, function (index){
			jobId = this.job;
			name = this.jobName;
			name = (name != undefined && name.trim() != "") ? unescape( name ) : "(No name)";
			
			status = '';
			if( this.status ){
				status = gl_jobStatusToString(this.status);
			}
			/// Notification to setinterval refresh status
			if( status.toLowerCase() == "running" || status.toLowerCase() == "initializing" ){
				appObj.hasRunningJob = true;
			}
			
			start = this.start;
			end = this.end;
			duration = this.duration;
			
			
			html = '';
			html += '<li>';
			html += '	<a href="#job-detail?id=' + jobId + '&service=' + appObj.serviceId + '">';
			html += '    	<h2>' + name + '</h2>';
			html += '    	<p>Start: &nbsp;&nbsp; <strong>' + start + '</strong></p>';
			html += '    	<p class="ui-li-aside">Duration&nbsp;&nbsp;<strong>' + duration + '</strong></p>';
			html += '    </a>';
			html += '    <span class="ui-li-count">' + status + '</span>';
			html += '</li>';
				
			$( "#app-list-jobs" ).append(html);
		} );
		$( "#app-list-jobs" ).listview( "refresh" );
		$( "#app" ).trigger("create");
	} else {
		$( "#app-list-jobs" ).empty();
		$( "#app-list-message" ).html("No such jobs.");
	}
}

function app_refreshJobsStatus( callback ){
	//appObj.resJobs = undefined;
	ws_getJobsByService( appObj.serviceId, function( resJobs ){
		if( resJobs.jobs && resJobs.jobs.items ){
			if( resJobs.jobs && resJobs.jobs.items ){
				/// Save response message
				//appObj.resJobs = resJobs;
				if( ! appObj.resGetJobsByService ){
					appObj.resGetJobsByService = {};
				}
				appObj.resGetJobsByService[ appObj.serviceId] = resJobs; 
				if( callback ){
					callback( 0 ); /// Success
				}	
			}else{
				console.error("Error: app_refreshJobsStatus(): ws_getJobsByService: response resJobs.jobs.items tag is missing.");
				callback( -4 );
			}
		}else{
			console.error("Error: app_refreshJobsStatus(): ws_getJobsByService: response resJobs.jobs.items tag is missing.");
			callback(false);
		}
	} );
}

function app_loadData(){
	var id = appObj.serviceId,
	
	service = appObj.resGetService[ id ];

	document.title = service.name;
	
	$("#app-title").text( service.name );
	
	$("#app").trigger('create');

	/// Fill jobs list
	app_loadJobsStatus();
}
/*
function app_loadData2(){
	var id = appObj.serviceId, 
		methodFile = appObj.resRunMethodFile[id];
	
	/// Fill service info
	if( ! methodFile || methodFile == ''){
		console.log("Error: app_loadData(id, methodFile): methodFile is UNDEFINED.");
		return;
	}
	var xmlDoc = $.parseXML( methodFile );
	var xml = $( xmlDoc );
	$( xml ).find('Service').each(function(){
		
		var name = $(this).attr('name'),
			//producer = $(this).attr('producer'),
			description = $(this).children('Description').text();
		name = ( name ) ? name : id;

		document.title = name;
		
		$("#app-title").text(name);
		
		$("#app").trigger('create');

		/// Save description file
		sessionStorage.setItem(id, methodFile);
		return false;
	});
	
	/// Fill jobs list
	app_loadJobsStatus();
}*/

/*
function app_loadData(id, methodFile){
	
	if( ! methodFile || methodFile == ''){
		console.log("Error: app_loadData(id, methodFile): methodFile is UNDEFINED.");
		return;
	}
		
	var xmlDoc = $.parseXML( methodFile );
	var xml = $( xmlDoc );
	$( xml ).find('Service').each(function(){
		
		var name = $(this).attr('name'),
			//producer = $(this).attr('producer'),
			description = $(this).children('Description').text();
		name = ( name ) ? name : id;
//	name = $( xml ).find('Service').attr('name'),
//	producer = $( xml ).find('Service').attr('producer');
//	description = $( xml ).find('Description').text();
			document.title = name;
			
			$("#app-title").text(name);
			
			$("#app").trigger('create');
			
			$.mobile.loading("hide");
			
			/// Save description file
			sessionStorage.setItem(id, methodFile);
	});
}
*/


/*
 ********************************************************************************************
 * 						API for 3 tab: status, detail, submit
 ********************************************************************************************
 * */

function app_pageBeforeShow(){

	if ($.mobile.pageData && $.mobile.pageData.id){
		//app_clearData();
	    console.log("Parameter id=" + $.mobile.pageData.id);
	    
	    /// Check other service selected -> need fill new data
	    if( appObj.serviceId != $.mobile.pageData.id ){
	    	appObj.isOtherServices [ "#app" ] = true;
	    	appObj.isOtherServices [ "#app-submit" ] = true;
	    	appObj.isOtherServices [ "#app-detail" ] = true;
	    }
	    
	    /// Save service id
	    appObj.serviceId = $.mobile.pageData.id;
	    
	    /// Save pageData for refresh page
	    sessionStorage.setItem("app.pageData.id", $.mobile.pageData.id);
	}else{
		if( sessionStorage.getItem("app.pageData.id") != undefined ){
			if ( $.mobile.pageData == undefined ) {
				$.mobile.pageData = new Object();
			}
			if( $.mobile.pageData.id == undefined ) {
				$.mobile.pageData.id = sessionStorage.getItem("app.pageData.id");
				/// Save service id
			    appObj.serviceId = $.mobile.pageData.id;
				
			    appObj.f5 = true;
				console.log("Bp: #app: pagebeforeshow F5");
			}
		}else{
			//console.error("Page load: Can not load this page, please back to home and try again.");
			console.error("Page load: app.pageData.id not exist.");
		}
	}
	return appObj;
//
//	if ($.mobile.pageData && $.mobile.pageData.id){
//		//app_clearData();
//	    console.log("Parameter id=" + $.mobile.pageData.id);
//	    
//	    /// Save service id
//	    appObj.serviceId = $.mobile.pageData.id;
//	    
//	    /// Save pageData for refresh page
//	    sessionStorage.setItem("app.pageData.id", $.mobile.pageData.id);
//	}else{
//		if( sessionStorage.getItem("app.pageData.id") != undefined ){
//			if ( $.mobile.pageData == undefined ) {
//				$.mobile.pageData = new Object();
//			}
//			if( $.mobile.pageData.id == undefined ) {
//				$.mobile.pageData.id = sessionStorage.getItem("app.pageData.id");
//				
//				/// Save service id
//			    appObj.serviceId = $.mobile.pageData.id;
//				
//			    appObj.f5 = true;
//				console.log("Bp: #app: pagebeforeshow F5");
//			}
//		}else{
//			//console.error("Page load: Can not load this page, please back to home and try again.");
//			console.error("Page load: app.pageData.id not exist.");
//		}
//	}
//	return appObj;
}

function app_retrieveData( serviceid, callback ){
	if( ! serviceid ){
		console.error("Error: app_retrieveData( serviceid, callback ): serviceId is EMPTY. Cannot load service !");
		callback( -1 ); /// Service is null
	}
	
	ws_getService( serviceid, function( res ){
		if( res.service ){
			appObj.resGetService[ serviceid ] = res;
			ws_getParameters( serviceid, function( res ){
				if( res.params && res.params.items ){
					appObj.resGetParameters[ serviceid ] = res;
					ws_getJobsByService( serviceid, function( resJobs ){
						console.log("Bp: ws_getJobsByService: resJobs: ");
						console.log(resJobs);
						if( resJobs.jobs && resJobs.jobs.items ){
							/// Save response message
							//appObj.resJobs = resJobs;
							if( ! appObj.resGetJobsByService ){
								appObj.resGetJobsByService = {};
							}
							appObj.resGetJobsByService[serviceid] = resJobs; 
							if( callback ){
								callback( 0 ); /// Success
								uStorage_backupData();
							}	
						}else{
							console.error("Error: app_refreshJobsStatus(): ws_getJobsByService: response resJobs.jobs.items tag is missing.");
							callback( -4 );
						}
					} );					
				}
			} );
		} else {
			console.log("Unknow response. res = ");
			console.log(res);
			alert("Unknow");
		}
	});
} 

function app_retrieveData2( serviceid, callback ){
	
	if( ! serviceid ){
		console.error("Error: app_retrieveData( serviceid, callback ): serviceId is EMPTY. Cannot load service !");
		callback( -1 ); /// Service is null
	}
	ws_findServices( function ( items ){
		appObj.resFindServices = items;
		var isUseable = false;
		$.each( items, function (index){
			if( this.service == serviceid ){
				isUseable = true;
				return false;	// break loop
			}
		});
		if( isUseable ){
			ws_getRunMethodFile( serviceid, function ( methodFile, isSuccess ){
				console.log("bP: app_retrieveData, methodFile = ");
				console.log(methodFile);
				if( isSuccess ){
					if( ! appObj.resRunMethodFile ){
						appObj.resRunMethodFile = {};
					}
					appObj.resRunMethodFile [serviceid] = methodFile;
					ws_getJobsByService( serviceid, function( resJobs ){
						console.log("Bp: ws_getJobsByService: resJobs: ");
						console.log(resJobs);
						if( resJobs.jobs && resJobs.jobs.items ){
							/// Save response message
							//appObj.resJobs = resJobs;
							if( ! appObj.resGetJobsByService ){
								appObj.resGetJobsByService = {};
							}
							appObj.resGetJobsByService[serviceid] = resJobs; 
							if( callback ){
								callback( 0 ); /// Success
							}	
						}else{
							console.error("Error: app_refreshJobsStatus(): ws_getJobsByService: response resJobs.jobs.items tag is missing.");
							callback( -4 );
						}
					} );
				}
				else{
					callback( -3 ); /// Cannot get description file.
				}
			});
		}else{
			return callback( -2 ); /// Service not avaiable to use.
		}
	});
}

function app_tabMenu( id , page ){
	console.log("app_tabMenu( id , page )" + id + ", " + page);
	var appActive = '',	submitActive = '', detailsActive = '';
	switch ( page ){
	case "#app":
		appActive = 'class="ui-btn-active ui-state-persist"';
		break;
	case "#app-submit":
		submitActive = 'class="ui-btn-active ui-state-persist"';
		break;
	case "#app-detail":
		detailsActive = 'class="ui-btn-active ui-state-persist"';
		break;
	default:
		console.log("Error: " + page + " page not exist.");
		break;
	}
	
	var html = '';
	html += '<div data-role="navbar" data-theme="b">';
	html += '<ul>';
	html += '   <li><a href="#app?id=' + id + '" ' + appActive + ' >Status</a></li>';
	html += '   <li><a href="#app-submit?id=' + id + '" ' + submitActive + '>Submit Job</a></li>';
	html += '   <li><a href="#app-detail?id=' + id + '" ' + detailsActive + '>Details</a></li>';
	html += '</ul>';
	html += '</div>';

	header = page + ' [data-role="header"]';
	navbar = page + ' [data-role="navbar"]';

	//console.log("Bp: app_tabMenu: header=" + header + ', navbar=' + navbar);
	
	if( $( header ) && $( header ).html() ){
		if( $( navbar ) && $( navbar ).html() ){
			$( navbar ).replaceWith( html );
			console.log("Bp: app_tabMenu( id , page ) REPLACE");
		}else{
			console.log("Bp: app_tabMenu( id , page ) APPEND");
			$( header ).append( html );
		}
		$( '[data-role="navbar"]' ).navbar();
//		$( '[data-role="navbar"]' ).navbar("refresh");
	}
	else{
		console.log("app_tabMenu:Not found." + header);
	}
	$( page ).trigger("pagecreate");
}

//function app_isExistService(id){
//	if( ! sessionStorage.services ){
//		return false;
//	}
//	services = JSON.parse(sessionStorage.services);
//	for (var i in services) {
//		if(services[i] && services[i].service && services[i].service == id)
//			return true;
//	}
//	return false;
//}

function app_isOtherService(){
	var current = gl_getCurrentPage();
	console.log("bp: app_isOtherService: current: " + current);
	console.log("bp: app_isOtherService: appObj.isOtherService" , appObj.isOtherServices);
	if( appObj.isOtherServices[current] ){
		appObj.isOtherServices[current] = false;
		console.log("bp: app_isOtherService: TRUE");
		return true;
	}
	console.log("bp: app_isOtherService: FALSE");
	return false;
}

function app_setIntervalRefreshStatus ( ){
	if( ! appObj.hasRunningJob ){
		return;
	}
	appObj.interval = setInterval(function(){
		app_refreshJobsStatus( function(result){
			if ( result == 0 ){

				appObj.hasRunningJob = false;
				
				app_loadJobsStatus(); 
				
				if( ! appObj.hasRunningJob ){
					app_clearInterval();
				}
				
			}else{
				console.error ("Error: app_setIntervalRefreshStatus: app_refreshJobsStatus data fail.");
				app_clearInterval();
			}
		} );
	}, 3000);
};

function app_clearInterval(){
	clearInterval(appObj.interval);
};
/*
 ********************************************************
 * 						Popup API
 ********************************************************
 * */
function app_popupShow( msg, icon){
	if( ! icon ){
		icon = "check";
	}

	msg += '<img src="images/icons/'+ icon +'.png" height="32" width="32">';
	$( "#app-popup").popup({ positionTo: "window" });
	$( "#app-popup").html(msg).popup("option", "positionTo", "window");
	
	$( "#app-popup" ).popup( "open" );
}
 
