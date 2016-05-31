//HISTORY
//2013/09/06    EH0001  longnm  Add api for client

var providerObj = new Object();

providerObj.services = new Object(); 			/// List service object
providerObj.resGetServices; 	/// Save getServices response of get services by provider request 

providerObj.isLoaded = false; 			/// load data flag
providerObj.currentTab = "newapp";		/// Current focus tab, default newapp tab
providerObj.service = new Object();
providerObj.service.params = {};

providerObj.f5 = false; /// Flag refresh page
providerObj.isBack;	/// Set back flag to do'nt clear form : Note: no set default value

providerObj.isEnableBtn = false; /// Flag check enable button
providerObj.interval = 160;
providerObj.timeResourceTimer;
/*
* ******************************************************************************************
* 									Event handler
* ******************************************************************************************
*/
$(document).on("pageshow", "#provider", function(e, data){
//$( document ).on("pageshow", "#provider", function(event){
	console.log('#provider: pageshow.');
	//alert("Page show");
	loading_show( 1500 );
	/*
	if( providerObj.isBack == true ){
		providerObj.isBack = undefined;
	} else {
		//document.getElementById("provider-newapp-form").reset();
	}*/
	/// Clean 
	$("#provider-submit-msg").html("");
	
	/// 
	//provider_startInterval();
	
	/// Set tab focus
	if ( $.mobile.pageData && ( $.mobile.pageData.id || $.mobile.pageData.from )){
		providerObj.from = $.mobile.pageData.from ? $.mobile.pageData.from : undefined;
		providerObj.id = $.mobile.pageData.id ? $.mobile.pageData.id : undefined;
		document.getElementById("provider-newapp-form").reset();
		
	} else {
		if ( providerObj.f5 ){
			//alert("restore");
			pu_restoreData();
//			console.log("providerObj = ");
//			console.log(providerObj);
			providerObj.f5 = false;
			
			/// Reset form			
			document.getElementById("provider-newapp-form").reset(); 
		} else {
			provider_refreshParamTable();
			provider_refreshCommandPreview(providerObj.service);
		}
	}
	
	//if ( providerObj.from != undefined ){
		console.log( "providerObj.from " + providerObj.from );
		switch ( providerObj.from ){
		case "waitlist":
			providerObj.currentTab = "waitlist";
			break;
	    case "apps":
	    case "newapp":
	    case "editapp":
	    	providerObj.currentTab = "apps";
			if(providerObj.id == undefined){
				providerObj.currentTab = "newapp";
			}
	    	break;
	    default:
	    	console.error('Unknow providerObj.from =' + providerObj.from);
	    	break;
	    }
	//} else {
	//	providerObj.currentTab = "apps";
	//	console.log("providerObj.from is unknow");
	//}
	//alert('providerObj.from = ' + providerObj.from + ", providerObj.currentTab = " + providerObj.currentTab);
    //provider_setTabFocus( providerObj.currentTab );
    
    /// Load login name
    uLogin_success();
    
    if( ( providerObj.from == "newapp" && providerObj.id != undefined )
    	&& providerObj.from !== "editapp"
		&& providerObj.services  
		&& providerObj.resGetServices
		&& ( ! providerObj.isNeedReloadData ( { "service" : providerObj.id } )) )
    {
    	if ( providerObj.isLoaded ) {
			//alert("loaded");
        	loading_hide();
        	return;
    	}
    	//console.log('Bp: "#provider: pageshow: Do not need reload data.');
		//provider_refreshParamTable(providerObj.service.params);
		//provider_refreshCommandPreview(providerObj.service);
		//provider_loadServicesList(providerObj.resGetServices);
		
		provider_setTabFocus( providerObj.currentTab );
		
    	//provider_loadData( providerObj.services );
		providerObj.isLoaded = true;
		loading_hide();
    }
    else{
    	//alert("call provider_retrieveData");
    	//load_servicesOfProvider();
    	provider_retrieveData( function(res) {
    		provider_loadServicesList( res );
			provider_setTabFocus( providerObj.currentTab );
			loading_hide();
    	});
    }
});

var pagehideed = false;
$( document ).on( "pagehide", "#provider", function(event) {
	//alert("page hide")
	provider_clearInterval();
	pagehideed = true;
});

function provider_startInterval(){
	pu_loadTimeResources(function(){
		providerObj.timeResourceTimer = setInterval(function() {
			clearInterval(providerObj.timeResourceTimer);
			provider_startInterval();
		}, Math.abs(providerObj.interval) * 1000);
	});
}

function provider_clearInterval(){
	if (providerObj.timeResourceTimer !== undefined) {
        clearInterval(providerObj.timeResourceTimer);
        providerObj.timeResourceTimer = undefined;
    }
}

function provider_loadQuotaTime(){
	ws_getProviderTimeResource(function(res){
		$("#ptq-remain-kpserver").val(res.remainkp != undefined ? res.remainkp : 0);
		$("#ptq-total-kpserver").val(res.totalkp != undefined  ? res.totalkp : 0);
		$("#ptq-remain-kdesk").val(res.remainkd != undefined  ? res.remainkd : 0);
		$("#ptq-total-kdesk").val(res.totalkd != undefined  ? res.totalkd : 0);
		$("#provider-publickey").val(res.publicKey != undefined  ? res.publicKey : "");
		
		// Update min/max for maxElapsedTime field
		//$("#provider-newapp-elapsed").attr({ "min": 1, "max": res.totalkp != undefined  ? res.totalkp * 1000000 : 0});
	});
}

function provider_loadServicesList( resServices ){
	//var listName = ( isAppsTab ? "provider-apps-list" : "provider-wait-list" ),
	var appshtml = "",
	waithtml = "";
	
	/// Clean list
	$( "#provider-apps-list" ).empty();
	$( "#provider-wait-list" ).empty();
	
	/// Init
	$( "#provider-apps-list" ).listview();
	$( "#provider-wait-list" ).listview();
	
	//console.log("resServices = ");
	//console.log(resServices);
	//alert("provider_loadServicesList( resServices )");
	if( resServices && resServices.services && resServices.services.items ){
		if( resServices.services.items.length > 0 ){
			$.each( resServices.services.items, function (index){
				if  ( ! ( this.service != undefined && this.status != undefined && this.numberOfUsers != undefined && this.numberOfWaitUsers != undefined ) ){
					console.log("Ingore");
					return true; // ~ continue
				}
				
				var service = {};
				service.id = this.service;
				service.icon = uIcon_genIconPath( service.id );//( service.id ? ( uIcon_genIconPath( service.id ) ) : '' );
				service.name = ( ( this.name && this.name !== "") ? this.name : "(No name)" );
				service.path = this.path;
				service.shpath = this.shpath;
				service.image = this.image;
				//service.producer = ( this.producer ? this.producer : '' );
				service.noUsers = ( this.numberOfUsers ? this.numberOfUsers : 0 );
				service.noWaitUsers = ( this.numberOfWaitUsers ? this.numberOfWaitUsers : 0 );
				service.status = this.status;
				service.statusStr = this.status ? "public" : "private";
				service.noNodes = this.numberOfNodes ? this.numberOfNodes : 0;
				service.maxElapsedTime = this.maxElapsedTime ? this.maxElapsedTime : 0;
				service.descript = this.description;
				service.stageinDirs = this.stageinDirs;
				service.stageoutDirs = this.stageoutDirs;
				/// Append to list services
				if ( service.id ){
					providerObj.services [ service.id ] = service;
				}
				
				appshtml += '\
<li><a href="#provider-app-detail?id=' + service.id + '&from=apps" data-transition="slidefade" >\
    <img src="' + service.icon + '" onerror="this.src=\'images/no-image.png\'" /> \
    <h2>' + service.name + '</h2>\
    <!--p>' + service.producer + '</p-->\
    <p class="ui-li-count price">' + service.statusStr + '</p>\
    <span class="ui-li-aside count">' + service.noUsers + '</span>\
    </a>\
</li>';
				if ( parseInt( service.noWaitUsers ) > 0 ) {
					waithtml += '\
<li><a href="#provider-app?id=' + service.id + '&from=waitlist">\
    <img src="' + service.icon + '" onerror="this.src=\'images/no-image.png\'" />\
    <h2>' + service.name + '</h2>\
    <!--p>' + service.producer + '</p-->\
    <span class="ui-li-count "><strong>' + service.noWaitUsers + '</strong></span>\
</li>';
				}
			});
			if( waithtml.length > 0 ){
				$( "#provider-wait-list" ).empty();
				$( "#provider-wait-list" ).append( waithtml ).listview( "refresh" );
			} else {
				$( "#provider-wait-list" ).html('<center>No user waiting to register</center>').listview( "refresh" );
			}
			$( "#provider-apps-list" ).empty();
			$( "#provider-apps-list" ).append( appshtml ).listview( "refresh" );
			
		} else {
			$( "#provider-apps-list" ).html('<center>No such service </center>').listview( "refresh" );
			$( "#provider-wait-list" ).html('<center>No waitting user</center>').listview( "refresh" );
			return false;
		}
	} else {
		console.error("Response message tags is missing.");
	}
}

$(document).on( "click", '.provider-tab-btn', function(e){
	///This will prevent event triggering more then once
	if(e.handled !== true) {
        e.handled = true;
        return true;
    }
	console.log(".provider-tab-btn: click ");
	provider_setTabFocus( $(this).attr( "data-tab" ) );
});

/*
$(document).on('click', '#provider-btn-login', function(e){
	///This will prevent event triggering more then once
	if(e.handled !== true){
        e.handled = true;
        return true;
    }
	if($( "#provider-signinText" ).text() === "Login"){
		console.log("Bp: login now !");
		homeObj.currentPage = "#home";
		home_login( home_oa2LoginCallback );
	}else{
		console.log("Bp: logout now !");
		home_logout();
	}
});
*/

function provider_retrieveData( callback ){
    ws_getServices( function( res ){
        providerObj.resGetServices = res;
        if( callback ){
        	pu_backupData();
        	callback ( res );
        }
    });
}

$(document).on('submit', '#provider-newapp-form', function( e ){
	
	///This will prevent event triggering more then once
	if(e.handled !== true) {
        e.handled = true;
        return true;
    }
	
	var params = providerObj.service.params;
	/// Check parameter empty
	var noparam = Object.size(params);
		noparam2 = Object.keys(params).length;
		
	/// Prevent default submit  
	e.preventDefault();
	console.log("Bp: provider-newapp-form: Submit job occur !");
	
	/// Parse parameter submit
	dataString = $(this).serialize();
	console.log( "Bp: provider-newapp-form: onSubmit: " + dataString );
	var serArr = $(this).serializeArray();
	
	for ( var i in serArr ){
		var value = serArr[ i ].value.trim();
		var msg = "";
		/*
		if( ! value ) {
			continue;
		}*/
		//console.log("name = " + serArr[ i ].name + ", value = " + value);
		switch ( serArr[ i ].name ){
		
		//case "provider-newapp-id":
		//	providerObj.service.id = value;
		//	break;
		case "pnaf-name":
			providerObj.service.name = value;
			if( value == "" ){
				msg = "Name without only space.";
			}
			break;			
		case "pnaf-image":
			providerObj.service.image = value;
			break;
		case "pnaf-path":
			providerObj.service.path = value;
			if( value == "" ){
				msg = "Path without only space.";
			}
		case "pnaf-shpath":
			providerObj.service.shpath = value;
			/*if( value == "" ){
				msg = "Path without only space.";
			}*/
		case "pnaf-stgin":
			providerObj.service.stageinDirs = [];
			providerObj.service.stageinDirs.push({"path":value});
			break;
			
		case "pnaf-stgout":
			providerObj.service.stageoutDirs = [];
			providerObj.service.stageoutDirs.push({"path":value})
			break;

/*		case "pnaf-icon":
			providerObj.service.icon = value;
			break;*/
		case "pnaf-elapsed":
			//providerObj.service.maxElapsedTime = value;
			try{
				var val = parseInt(value);
				if( val > 0 && val <= 65535){
					providerObj.service.maxElapsedTime = val;
				} else {
					msg = "Number value from 0 to 65,535 nodes";
				}
			}catch(e){
				msg = "Number is invalid";
			}
			break;
		case "pnaf-nodes":
			//providerObj.service.noNodes = value;
			try{
				var val = parseInt(value);
				if( val > 0 && val <= 255){
					providerObj.service.noNodes = val;
				} else {
					msg = "Number value from 0 to 255 nodes";
				}
			}catch(e){
				msg = "Number is invalid";
			}
			break;
			
		case "pnaf-descript":
			providerObj.service.descript = value;
			break;
		case "pnaf-publish":
			providerObj.service.publish = publish;
			break;
		default:
			console.log("serArr[ i ].name = " + serArr[ i ].name + " is unknowed.");
			continue;
			break;
		}
		
		if( msg != "" ){
			var idTag = serArr[ i ].name;
			//alert("input#" + idTag);
			//$.mobile.activePage.find("input#" + idTag).setCustomValidity( msg );
			document.getElementById(idTag).setCustomValidity(msg);
			$.mobile.activePage.find( "#" + idTag ).focus();
			return true; /// Break submit
		}
	}
	
	/// Icon image
	var files = $('input#pnaf-icon').prop("files");
	if ( files && files.length > 0 ){
		if( files[0].size > 104857600 ) {
			uValid_setCustomValidity("provider-editapp-icon", "File size maximum is 100M.");
			return true;
		} else {
			providerObj.service.iconfile = files[0];
			providerObj.service.icontag = "pnaf-icon";
		}
	} else {
		providerObj.service.iconfile = undefined;
		console.log("No select icon file");
	}
		
	/// Change to detail page to confirm public service
	/// Pass data
	//padetailObj.service = providerObj.service;
	//padetailObj.fromTab = "newapp"
	
	/// Clear from info
	providerObj.from = undefined;
	
	/// Just test
	providerObj.service.id = "";	/// No service id
	
	/*
	/// Check path before change page 
	ws_checkPathOfService( providerObj.service.path, function(res){
		if( res.result == ResponseCodes.DATA_SUCCESS ){
			$("#provider-submit-msg").html("");
			$.mobile.changePage( '#provider-app-detail?id=' + providerObj.service.id + "&from=newapp", { transition: "slideup" });
			return true;
		} else {
			document.getElementById("pnaf-path").setCustomValidity("Application path is already registered");
			$("#provider-submit-msg").html("Application path is already registered");
			$("#provider-submit-msg").focus();
			return true;
		}
	});*/
	
	$.mobile.changePage( '#provider-app-detail?id=' + providerObj.service.id + "&from=newapp", { transition: "slideup" });
	return true;
});
/*
$(document).on('click', '#provider-newapp-reset-btn', function(e){
	providerObj.service.params = {};
	provider_refreshParamTable(providerObj.service.params);
	provider_refreshCommandPreview(providerObj.service);
});*/

$(document).on('reset', '#provider-newapp-form', function(e){
	providerObj.service.params = {};
	provider_refreshParamTable(providerObj.service.params);
	provider_refreshCommandPreview(providerObj.service);
	$( "#provider-newapp-submit-btn" ).button("disable");
	$( "#provider-newapp-reset-btn" ).button("disable");
	providerObj.isEnableBtn = false;
});



/*$(document).on("change", "input[type=file]#pnaf-icon", function(e) {
	var file = this.files[0];
	providerObj.service.icon = {};
	providerObj.service.iconfile = file;
});
*/
$(document).on("change", "input[type=text]#pnaf-path", function(e) {
	provider_refreshCommandPreview();
	e.target.setCustomValidity("");
	$("#provider-submit-msg").html("");
});

$(document).on("change", "input[type=number]", function(e) {
	e.target.setCustomValidity("");
});

$(document).on("change", "#provider-newapp-form input", function(e) {
	provider_inputChange();
});

function provider_inputChange(){
	
	/// Check require fields
	if($("#pnaf-path").val().trim() == "" || $("#pnaf-image").val().trim() == "" ){
		$( "#provider-newapp-submit-btn" ).button('disable'); 
		$( "#provider-newapp-reset-btn" ).button('disable'); 
		providerObj.isEnableBtn = false;
		return;
	}
	if ( providerObj.isEnableBtn ){
		return;
	}
	providerObj.isEnableBtn = true;
	$( "#provider-newapp-submit-btn" ).button("enable");
	$( "#provider-newapp-reset-btn" ).button("enable");
}

/*
 **********************************************
 * Event parameter popup
 * 
 **********************************************
 * */

/*
 * Handler when click delete button 
 */
function provider_deleteParamCallback( paramId ){
	console.log('providerObj.service.params[ paramId ]: ' + providerObj.service.params[ paramId ]);
	delete providerObj.service.params[ paramId ];
	console.log('providerObj.service.params[ paramId ]: ' + providerObj.service.params[ paramId ]);
	provider_refreshParamTable();
	provider_refreshCommandPreview(providerObj.service);
	provider_inputChange();
};

function provider_addEditParamCallback ( param ){
	switch ( param.action ){
	case "newparam":
		$.mobile.activePage.find( "#provider-newparam-popup" ).popup("close");
		/// Generate param
		param.id = pu_genParamId( providerObj.service.params );
		providerObj.service.params[param.id] = param;
		provider_refreshParamTable(providerObj.service.params);
		provider_refreshCommandPreview(providerObj.service);
		
		break;
	case "editparam":
		console.log("param = ");
		console.log(param);
		providerObj.service.params[param.id] = param;
		$.mobile.activePage.find ( "#provider-editparam-popup" ).popup("close");
		provider_refreshParamTable(providerObj.service.params);
		provider_refreshCommandPreview(providerObj.service);
		break;	
	default:
		console.error("Unkow action !");
		break;
	}
	provider_inputChange();
};

/*
 * *****************************************************************************************
 * 									API function
 * ****************************** **********************************************************
*/


/*
 * Refresh preview textbox
 */
function provider_refreshCommandPreview(){
	pu_refreshCommandPreview( 'provider-newapp-preview', 'pnaf-path', providerObj.service );
}

/*
 * Refresh parameter table
 */
function provider_refreshParamTable(){
	pu_refreshParamTable( 'newapp', 'provider-newapp-params-table', providerObj.service.params );
}

function provider_setTabFocus( tabname ){
	$( '.provider-tab-btn').removeClass("ui-btn-active ui-state-persist");
	$( '#provider-tab-btn-' + tabname ).addClass("ui-btn-active ui-state-persist");
	switch ( tabname ){
	case "quotatime":
		$( '#provider-tab-quotatime' ).show();
		$( '#provider-tab-apps' ).hide();
		$( '#provider-tab-waitlist' ).hide();
		$( '#provider-tab-newapp' ).hide();
		if(providerObj.currentTab != "quotatime"){
			provider_loadQuotaTime(providerObj.resGetServices);
		}
		/// Save current tab
		providerObj.currentTab = tabname;
		break;
	case "apps":
		$( '#provider-tab-quotatime' ).hide();
		$( '#provider-tab-apps' ).show();
		$( '#provider-tab-waitlist' ).hide();
		$( '#provider-tab-newapp' ).hide();
		if(providerObj.currentTab == "waitlist"){
			provider_loadServicesList(providerObj.resGetServices);
		}
		/// Save current tab
		providerObj.currentTab = tabname;
		break;
	case "waitlist":
		$( '#provider-tab-quotatime' ).hide();
		$( '#provider-tab-apps' ).hide();
		$( '#provider-tab-waitlist' ).show();
		$( '#provider-tab-newapp' ).hide();
		if(providerObj.currentTab == "apps"){
			provider_loadServicesList(providerObj.resGetServices);
		}
		/// Save current tab
		providerObj.currentTab = tabname;
		break;
	case "newapp":
		$( '#provider-tab-quotatime' ).hide();
		$( '#provider-tab-apps' ).hide();
		$( '#provider-tab-waitlist' ).hide();
		$( '#provider-tab-newapp' ).show();
		/// Save current tab
		providerObj.currentTab = tabname;
		
		/// Get list public images for new app
		ws_findImages(0, 1, 1, "", function(res){
			$("#pnaf-image").empty();
			if(res.images && res.images.length){
				providerObj.resImages = res.images;
				$.each(res.images, function(index, imageObj) {
					var optTemp = '<option value="' + imageObj.image + '">' + imageObj.name + '</option>';            
					$("#pnaf-image").append(optTemp)
				});
			}
			$("#pnaf-image").selectmenu();
			$("#pnaf-image").selectmenu('refresh', true);
		});
		
		provider_refreshParamTable(providerObj.service.params);
		provider_refreshCommandPreview(providerObj.service);
		break;
	default:
		console.log("Unknow!");
		break;
	}
}

