/*
*  ******************************************************************************************
* 						Define Global Variables
*  ****************************************************************************************** 
*/

var pappObj = new Object();
pappObj.currentTab = "users";		/// Current focus tab, default newapp tab
pappObj.service = {};
pappObj.service.updates = [];	/// List update service info
pappObj.resService = {}; 	/// Save service sql
pappObj.resUsers = new Object();
pappObj.resParams = new Object(); /// List response message getParameters
pappObj.resWaitUsers = new Object();
pappObj.resImages = new Object();

pappObj.loaded = false;
pappObj.isBackPage = false;
pappObj.f5 = false; /// Flag after refresh page ;
pappObj.isOtherService = false;

/*
* ******************************************************************************************
* 									Event handler
* ******************************************************************************************
*/

$(document).on("pagebeforeshow", "#provider-app", function(event){
	
	console.log("#provider-app: pagebeforeshow");
	pappObj.isBackPage = gl_isBackEvent();
	
	if( sessionStorage.getItem("provider-app.pageData.id") != undefined
			&& sessionStorage.getItem("provider-app.pageData.from") != undefined ){
		if ( $.mobile.pageData == undefined ) {
			$.mobile.pageData = new Object();
		}
		if( $.mobile.pageData.id == undefined && $.mobile.pageData.from == undefined ){
			$.mobile.pageData.id = sessionStorage.getItem("provider-app.pageData.id");			
			$.mobile.pageData.from = sessionStorage.getItem("provider-app.pageData.from");
			
			pu_restoreData();

			pappObj.f5 = true;
			pu_setF5();
			
			console.log("Bp: pappObj = ");
			console.log(pappObj);
		}
	}else{
		pappObj.f5 = false;
		console.log("#provider-app: pagebeforeshow: provider-app.pageData.idd & provider-app.pageData.from is UNDEFINED.")
	}
	/// Clear
	$( '#papp-tab-users' ).hide();
	$( '#papp-tab-waitlist' ).hide();
	$( '#papp-tab-details' ).hide();
});

$( document ).on("pageshow", "#provider-app", function(event){
    console.log('#provider-app: pageshow.');
    
    /// Clear message
    $("#papp-submit-msg").html("");
    $("#papp-adduser-msg").html('');
    
    console.log("pappObj.isOtherService = " + pappObj.isOtherService + ", pappObj.loaded = " + pappObj.loaded);
    if ( $.mobile.pageData && $.mobile.pageData.id &&  $.mobile.pageData.from){
		//detail_clearData();
    	
	    console.log("Parameter id=" + $.mobile.pageData.id);

	    /// Check new service is another service
	    pappObj.isOtherService = ( pappObj.id !== $.mobile.pageData.id );
	    
	    /// Save service id
	    pappObj.id = $.mobile.pageData.id;
	    pappObj.from = $.mobile.pageData.from;
	    
	    /// Save pageData for refresh page
	    sessionStorage.setItem("provider-app.pageData.id", $.mobile.pageData.id);
	    sessionStorage.setItem("provider-app.pageData.from", $.mobile.pageData.from);
	    //alert("id = " +  sessionStorage.getItem("provider-app.pageData.id") + ", from = " + sessionStorage.getItem("provider-app.pageData.from"));
	    
	    /// Focus tab
	    if ( pappObj.f5 
	    		&& sessionStorage.getItem("provider-app.pappObj.currentTab")){
	    	pappObj.currentTab = sessionStorage.getItem("provider-app.pappObj.currentTab");
	    } else {
		    switch ( pappObj.from ){
		    case "waitlist":
		    	pappObj.currentTab = "waitlist";
		    	break;
		    case "apps":
		    	//pappObj.currentTab = "users";
		    	//break;
		    case "editapp":
		    	pappObj.currentTab = "details";
		    	break;
		    default:
		    	console.error('Unknow pappObj.from');
		    	break;
		    }
	    }
	    uLogin_success();
	    papp_setTabFocus(pappObj.currentTab);
	    
	    loading_show();
		
	    /*
	    console.log("pappObj.resUsers[ pappObj.id ] = ");
	    console.log( pappObj.resUsers[ pappObj.id ] );
	    console.log("pappObj.resWaitUsers[ pappObj.id ] = ");
	    console.log( pappObj.resWaitUsers[ pappObj.id ]);
	    */
	    if( /*pappObj.resService[pappObj.id] &&*/
	    	pappObj.resUsers[ pappObj.id ] && 
	    	pappObj.resWaitUsers[ pappObj.id ] && 
	    	pappObj.resParams[ pappObj.id ] &&	
	    	//appObj.isBackPage && 
	    	( ! gl_isNeedReloadData( { "service" : pappObj.id  } ) )
	    ) {
	    	console.log("pappObj.isOtherService = " + pappObj.isOtherService + ", pappObj.loaded = " + pappObj.loaded + ", pappObj.f5 = " + pappObj.f5);
	    	if ( pappObj.loaded && ( ! pappObj.isOtherService ) && ( ! pappObj.f5 ) ) {
	    		loading_hide();
	    		return;
	    	}
	    	pappObj.isOtherService = false;
	    	papp_loadData();
	    	loading_hide();
	    } else {
	    	papp_retrieveData( function( result ) {
	    		if( result == 0 ){
		    		papp_loadData();
		    		loading_hide();
	    		}else{
	    			//jdetail_popupShow("Get <strong>" + jobDetailObj.jobId + "</strong> job from server has been problem.", "alert");
	    		}
	    	});
	    }
	}else if($.mobile.pageData.from == "newapp"){
		pappObj.currentTab = "newapp";
		papp_setTabFocus(pappObj.currentTab);
	}else{
		console.log("Error: #detail: pageData is EMPTY.");
		loading_hide();
	}
});

$(document).on( "click", '.papp-tab-btn', function(e){
	///This will prevent event triggering more then once
	/*if(e.handled !== true) {
        e.handled = true;
        return true;
    }*/
	console.log(".papp-tab-btn: click ");
	papp_setTabFocus( $(this).attr( "data-tab" ) );
	
});
$(document).on( "click", '.papp-deleteuser-btn', function(e){
	console.log("papp-deleteuser-btn click");
	var userId = $(this).attr( "data-user" );
	
	/// Show popup confirm
	var popUp = '<div data-role="popup" id="papp-deleteuser-popup" data-theme="d" style="height:20;padding:0;"><div data-role="controlgroup" data-type="horizontal"><a id="pdp-yes-btn"  data-rel="back" data-role="button" data-icon="check">Yes</a><a data-rel="back" data-role="button" data-icon="delete">No</a></div></div>';
	
	$.mobile.activePage.append( popUp ).trigger( "pagecreate" );
	
	/// Release after close popup
	$( "#papp-deleteuser-popup" ).bind({
		popupafterclose: function(event, ui) {	$( this ).remove();}
	});
	
	/// Bind yes button is clicked
	$( "#pdp-yes-btn" ).bind( "click", function( e, ui ) {
		//alert("Yess is click!: userId = " + userId );
		//var userId = $(this).attr( "data-user" ),
		serviceId = pappObj.id;
		if( userId && serviceId ){
			var ws_removeUserFunc = ws_removeUserOfService;
			if ( pappObj.currentTab == "waitlist" ){
				ws_removeUserFunc = ws_removeCandidateOfService;
			}
			ws_removeUserFunc( serviceId, userId, function( msg ) {
				if ( msg.result === ResponseCodes.DATA_SUCCESS ) {
					//alert("Remove user success");
					gl_providerChangeUser(serviceId);
					if( pappObj.currentTab == "users"){
						papp_reloadUsersList();
					} else {
						papp_reloadWaitList();
					}
				} else {
					if( msg.result === ResponseCodes.DATA_SELECT_EMPTY ){
						if( pappObj.currentTab == "users"){
							papp_reloadUsersList();
						} else {
							papp_reloadWaitList();
						}
					} else {
						alert("Remove user fail");
					}
				}
			});
		}
	});
	
	/// Show popup
	var rowid = "#" + $(this).attr( "id" );
	//alert("rowid = " + rowid);	
	$ ( "#papp-deleteuser-popup" ).popup("open", { positionTo: rowid });
});

$(document).on( "click", '.papp-acceptuser-btn', function(e){
	
	var userId = $(this).attr( "data-user" ),
	serviceId = pappObj.id;
	if( userId && serviceId ){
		ws_addUser( serviceId, userId, function( msg ) {
			if ( msg.result === ResponseCodes.DATA_SUCCESS ) {
				alert("Add user success");
				gl_providerChangeUser(serviceId);
				papp_reloadUsersList();
				papp_reloadWaitList();
			} else {
				if ( msg.result === ResponseCodes.DATA_IS_EXISTED || papp_checkEmailExisted( userId ) ){
					papp_reloadUsersList();
					papp_reloadWaitList();
				} else {
					alert("Add user fail");
				}
			}
		});
	}
});


$(document).on('submit', '#provider-editapp-form', function( e ){
	var params = pappObj.service.params;
	/// Check parameter empty
	var noparam = Object.size(params);
	
	/// Prevent default submit  
	e.preventDefault();
	console.log("Bp: provider-editapp-form: Submit job occur !");
	
	/// Parse parameter submit
	dataString = $(this).serialize();
	console.log( "Bp: provider-editapp-form: onSubmit: " + dataString );
	//alert('dataString = ' + dataString);
	var serArr = $(this).serializeArray();
	
	var updates = [];
	var msg = "", tagId = "";
	for ( var i in serArr ){
		var value = serArr[ i ].value;
		msg = "", tagId = "";
		//if( ! value ) {
		//continue;
		//}
		console.log("Bp: name = " + serArr[ i ].name + ", value = " + value);
		
		switch ( serArr[ i ].name ){
		
		case "provider-newapp-id":
			pappObj.service.id = value;
			break;
		case "provider-newapp-name":
			if( pappObj.service.name !== value ){
				pappObj.service.name = value;
				updates.push("name");
			} 
			break;
		case "provider-newapp-image":
			//alert("value" + value);
			if( pappObj.service.image !== value && (!value)){
				pappObj.service.image = value;
				updates.push("image");
			}
			break;
		case "provider-newapp-path":
			if( pappObj.service.path !== value && (!value)){
				pappObj.service.path = value;
				updates.push("path");
			}
			break;

		case "provider-newapp-shpath":
			if( pappObj.service.shpath !== value){
				pappObj.service.shpath = value;
				updates.push("shpath");
			}
			break;

                case "provider-newapp-preshpath":
                        if( pappObj.service.preshpath !== value ){
                                pappObj.service.preshpath = value;
                                updates.push("preshpath");
                        }
                        break;

                case "provider-newapp-postshpath":
                        alert("Bp: "  + pappObj.service.postshpath + " vs " + value);

                        if( pappObj.service.postshpath !== value ){
                                pappObj.service.postshpath = value;
                                updates.push("postshpath");
                        }
                        break;

		case "provider-newapp-stgin":
			if(pappObj.service.stageinDirs && pappObj.service.stageinDirs [0]) {
                           if(pappObj.service.stageinDirs [0].path !== value) {
				pappObj.service.stageinDirs = [];
				pappObj.service.stageinDirs.push({"path":value});
				updates.push("stageinDirs");
                           } else {
                           	console.log("Stage-in: No update");  
                           }
			} 
                        else {
				pappObj.service.stageinDirs = [];
				pappObj.service.stageinDirs.push({"path":value});
				updates.push("stageinDirs");
			}
			break;
			
		case "provider-newapp-stgout":
                        if(pappObj.service.stageoutDirs && pappObj.service.stageoutDirs [0] ) {
                            if(pappObj.service.stageoutDirs [0].path !== value) {
                                pappObj.service.stageoutDirs = [];
                                pappObj.service.stageoutDirs.push({"path":value});
                                updates.push("stageoutDirs");
                            } else {
                                console.log("Stage-out: No update");
                            }
                        } 
                        else {
                                pappObj.service.stageoutDirs = [];
                                pappObj.service.stageoutDirs.push({"path":value});
                                updates.push("stageoutDirs");
                        }
			break;
			
		case "provider-newapp-elapsed":
			tagId = "provider-editapp-elapsed";
			try{
				var val = parseInt(value);
				if( val > 0 && val <= 65535){
					if( pappObj.service.maxElapsedTime !== val ){
						pappObj.service.maxElapsedTime = val;
						updates.push("maxElapsedTime");
					} else {
						console.log("maxElapsedTime: No update");
					}
				} else {
					msg = "Number value from 0 to 65,535 nodes";
					
				}
			}catch(e){
				msg = "Number is invalid";
			}
			break;
		case "provider-newapp-nodes":
			tagId = "provider-editapp-nodes";
			try{
				var val = parseInt(value);
				if( val > 0 && val <= 255){
					if( pappObj.service.noNodes !== val ){
						pappObj.service.noNodes = val;
						updates.push("noNodes");
					} else {
						console.log("Number of nodes: No update");
					}
				} else {
					msg = "Number value from 0 to 255 nodes";
				}
			}catch(e){
				msg = "Number is invalid";
			}
			break;
			
		case "provider-newapp-descript":
			if( pappObj.service.descript !== value ){
				pappObj.service.descript = value;
				updates.push("descript");
			}
			break;
		}

		if( msg != "" && tagId != ""){
			
			/*
			var idTag = serArr[ i ].name;
			//alert("input#" + idTag);
			//$.mobile.activePage.find("input#" + idTag).setCustomValidity( msg );
			document.getElementById(idTag).setCustomValidity(msg);
			$.mobile.activePage.find( "#" + idTag ).focus();
			*/
			uValid_setCustomValidity(tagId, msg);
			return true; /// Break submit
		}
	}
	
	/// Icon image
	var files = $('input#provider-editapp-icon').prop("files")
	if ( files.length > 0 ){
		if( files[0].size > 104857600 ) {
			uValid_setCustomValidity("provider-editapp-icon", "File size maximum is 100M.");
			return true;
		} else {
			pappObj.service.iconfile = files[0];
			pappObj.service.icontag = "provider-editapp-icon";
			pappObj.service.icon = pappObj.service.iconfile.name;
			updates.push("icon");
		}
	} else {
		console.log( "#provider-editapp-icon: No select file ");
	}
	pappObj.service.updates = updates;
	
	/// Change to detail page to confirm public service
	/// Pass data
	//padetailObj.service = pappObj.service;
	//padetailObj.fromTab = "newapp"
	
	if ( updates["path"] && pappObj.service.path !== undefined && pappObj.service.path !== "" ){
		/// Check path before change page 
		ws_checkPathOfService( providerObj.service.path, function(res){
			if( res.result == ResponseCodes.DATA_SUCCESS ){
				$("#papp-submit-msg").html("");
				$.mobile.changePage( '#provider-app-detail?id=' + pappObj.service.id + "&from=editapp", { transition: "slideup" , reverse: false});//, changeHash: false });
				return true;
			} else {
				document.getElementById("provider-newapp-path").setCustomValidity("Application path is already registered");
				$("#papp-submit-msg").html("Application path is already registered");
				return true;
			}
		});
	} else {
		$.mobile.changePage( '#provider-app-detail?id=' + pappObj.service.id + "&from=editapp", { transition: "slideup", reverse: false});//, changeHash: false  });
	}
	return true;
});

/*
 * Handler add user event of Users tab
 */
$(document).on("change", "input[type=email]#papp-adduser-email", function(e) {
	if ( $(this).val().length !== 0 ){
		$( "#papp-adduser-btn" ).button('enable');
	} else {
		$( "#papp-adduser-btn" ).button('disable');
	}
});

$(document).on("change", "input[type=text]#provider-editapp-path", function(e) {
	papp_refreshCommandPreview();
	$("#papp-submit-msg").html("");
});

$(document).on("change", "input[type=file]#provider-editapp-icon", function(e) {
	var files = e.target.files; //FileList object 
	if ( files.length > 0 ) {
		ufile_displayAsImage( files[0], "provider-editapp-icon-view" );
	} else {
		$("#provider-editapp-icon-view").attr("src", uIcon_genIconPath( pappObj.id ));
		//console.log("No such file");
	}
});

$(document).on('submit', '#papp-adduser-form', function( e ){
	$.mobile.loading("show");
	e.preventDefault();
	/// Parse parameter submit
	dataString = $(this).serialize();
	console.log( "Bp: papp-adduser-form: onSubmit: " + dataString );
	//alert('dataString = ' + dataString);
	var serArr = $(this).serializeArray(),
	email;
	for ( var i in serArr ){
		var value = serArr[ i ].value;
		if( ! value ) {
			continue;
		}
		if ( serArr[ i ].name == "papp-adduser-email"){
			email = value;
			break;
		}
	}
	if( email ){
		ws_addUser( pappObj.id, email, function( res ) {
			if ( res.result === ResponseCodes.DATA_SUCCESS ) {
				//alert("Add user success");
				papp_reloadUsersList();
				papp_reloadWaitList();
				$("#papp-adduser-msg").html('');
			} else {
				if ( res.result === ResponseCodes.DATA_IS_EXISTED || papp_checkEmailExisted( email ) ){
					$("#papp-adduser-msg").html( "Email address is already registered !" ); /// Load title
					papp_reloadUsersList();
				} else {
					alert("Add user fail");
				}
			}
			$.mobile.loading("hide");
		});
		
		/*if ( papp_checkEmailExisted( email ) ){
			$("#papp-adduser-msg").html( "Email address is already registered !" ); /// Load title
		} else {
			$("#papp-adduser-msg").html( "" );
			ws_addUser( pappObj.id, email, function( res ) {
				if ( res.result === ResponseCodes.DATA_SUCCESS ) {
					alert("Add user success");
					papp_reloadUsersList();
					papp_reloadWaitList();
				} else {
					alert("Add user fail");
				}
			});
		}*/
	} else {
		alert("Email is empty");
		$.mobile.loading("hide");
	}
	return false;
});

$(document).on( "click", '#provider-editapp-undo-btn', function(e){
	document.getElementById("provider-editapp-form").reset();
	papp_loadDataDetails();
	$( "#provider-editapp-submit-btn" ).button("disable");
	$( "#provider-editapp-undo-btn" ).addClass("ui-disabled");
});

$(document).on( "click", '#papp-back-btn', function(e){
	/*switch ( pappObj.from ){
    case "waitlist":
    	break;
    case "apps":
    	break;
    case "editapp":
    	break;
    default:
    	console.error('Unknow pappObj.from');
    	break;
    }*/
	var urlParam = "";
	if ( pappObj.id && pappObj.from ){
		urlParam = '?id=' + pappObj.id + '&from=' + pappObj.from;
	} 
	$.mobile.changePage( '#provider' + urlParam, { transition: 'slide', reverse: false  });
});

$(document).on( "change", '.provider-editapp-input', function(e){
	$( "#provider-editapp-submit-btn" ).button("enable");
	$( "#provider-editapp-undo-btn" ).removeClass("ui-disabled");
});

/*
 ********************************************************************************************
 * 						API function
 ********************************************************************************************
 * */

function papp_checkEmailExisted( email ){
	//return true;
	var resUsers = pappObj.resUsers[pappObj.id],
		isExisted = false;
	if( resUsers && resUsers.users && resUsers.users.items ){
		$.each( resUsers.users.items, function (index){
			if ( ! ( this.user ) ){
				return; // ~ continue
			}
			if ( this.user == email ) {
				isExisted = true;
				return false;
			}
		});
	}
	return isExisted;
}
/*
 * Handler when click delete button 
 */
function papp_deleteParamCallback( paramId ){
	switch ( pappObj.service.params[ paramId ].action ){
	case undefined:
	case "editparam":
		pappObj.service.params[ paramId ].action = "deleteparam";
		break;
	case "newparam":
		delete pappObj.service.params[ paramId ];
		break;
	default:
		console.error("Unknow pappObj.service.params[ paramId ].action = " + pappObj.service.params[ paramId ].action );
		break;
	}
	papp_refreshParamTable(pappObj.service.params);
	papp_refreshCommandPreview();
	$( "#provider-editapp-submit-btn" ).button("enable");
	$( "#provider-editapp-undo-btn" ).removeClass("ui-disabled");
};
/*
 * Handler when click edit button 
 */
function papp_addEditParamCallback ( param ){
	switch ( param.action ){
	case "newparam":
		$.mobile.activePage.find( "#provider-newparam-popup" ).popup().popup("close");
		/// Generate param
		param.id = pu_genParamId( pappObj.service.params );
		pappObj.service.params[param.id] = param;
		papp_refreshParamTable(pappObj.service.params);
		papp_refreshCommandPreview();
		break;
	case "editparam":
		oldParam = pappObj.service.params[ param.id ];
		if( ! oldParam ){
			console.error( "Unknow, paramid = " + param.id );
			return;
		}
		if ( oldParam.action ){
			param.action = oldParam.action;
		}
		pappObj.service.params[param.id] = param;
		
		$.mobile.activePage.find( "#provider-editparam-popup" ).popup().popup("close");
		papp_refreshParamTable(pappObj.service.params);
		papp_refreshCommandPreview();
		break;	
	default:
		console.error("Unkow action !");
		break;
	}
	$( "#provider-editapp-submit-btn" ).button("enable");
	$( "#provider-editapp-undo-btn" ).removeClass("ui-disabled");
};

/*
 * Query get all data for page
 */
function papp_retrieveData( callback ){
	/// Get list public images for new app
	ws_findImages(0, 1, 1, "", function(resImages){
		pappObj.resImages = resImages;
		ws_getParameters( pappObj.id, function( resParams ){
			pappObj.resParams[pappObj.id] = resParams;
			/// Get list users
			ws_getUsersOfService( pappObj.id, function( resUsers ){
				pappObj.resUsers[ pappObj.id ] = resUsers;
				ws_getCandidatesOfService( pappObj.id,function( resWaitUsers ){
					pappObj.resWaitUsers[ pappObj.id ] = resWaitUsers;
					if( callback ){
						pu_backupData();
						callback( RESULTS.SUCCESS );
					}
				});
			});
		});
	});
	return;
	/*
	/// Get service info
	ws_getService( pappObj.id, function( resService ){
		pappObj.resService[pappObj.id] = resService;
		/// Get list users
		ws_getUsersOfService( pappObj.id, function( resUsers ){
			pappObj.resUsers[pappObj.id] = resUsers;
			ws_getCandidatesOfService( pappObj.id, function( resWaitUsers ){
				pappObj.resWaitUsers[ pappObj.id ]  = resWaitUsers;
				if( callback ){
					callback( RESULTS.SUCCESS );
				}
		    });
	    });
	});*/
}

function papp_reloadUsersList(){
	ws_getUsersOfService( pappObj.id, function( resUsers ){
		pappObj.resUsers[pappObj.id] = resUsers;
		papp_loadDataUsersList();
	});
}

function papp_reloadWaitList(){
	ws_getCandidatesOfService( pappObj.id, function( resWaitUsers ){
		pappObj.resWaitUsers[ pappObj.id ]  = resWaitUsers;
		papp_loadDataWaitList();
	});
}
/*
 * Fill data to form
 */
function papp_loadData(){
	pappObj.loaded = true;
	/// Clear form;
	document.getElementById("provider-editapp-form").reset();
	papp_loadDataUsersList();
	papp_loadDataWaitList();
	papp_loadDataDetails();
}

function papp_loadDataWaitList(){
	/// Load users list
	if( ! $( "#papp-wait-list" ) ){
		log.error("$( #papp-wait-list ) not found");
		return;
	}
	
	if( pappObj.id && pappObj.resWaitUsers[ pappObj.id ] ){
		var resCandidate = pappObj.resWaitUsers[ pappObj.id ]; 
		var html = "";
		$( "#papp-wait-list" ).empty();
		$( "#papp-wait-list" ).listview();
		if( resCandidate.candidates && resCandidate.candidates.items && resCandidate.candidates.items.length > 0 ){
			$.each( resCandidate.candidates.items, function (index){
				if ( ! ( this.user ) ){
					return true; // ~ continue
				}
				html += '\
					<li>\
						<a href="#">' + this.user + '</a>\
						<a id="papp-deletewaituser-btn-' + index + '" class="papp-deleteuser-btn split-button-custom" data-user="' + this.user + '" data-role="button" data-icon="delete" data-theme= "e" data-iconpos="notext">Delete</a>\
						<a class="papp-acceptuser-btn split-button-custom" data-user="' + this.user + '" data-role="button" data-icon="check" data-iconpos="notext">Accept</a>\
						<a href="#" style="display: none;"></a>\
					</li>';
			});
			//$( "#papp-wait-list" ).append(html).listview('refresh');
			$( "#papp-wait-list" ).append( html ).listview('refresh').trigger("create");
		} else {
			$( "#papp-wait-list" ).html('<center>No users</center>').listview( "refresh" ); 
		}
	} else {
		$( "#papp-wait-list" ).html('<center>No users</center>').listview( "refresh" );
		console.error("Error: papp_loadDataWaitList can not load !");
	}
}

function papp_loadDataUsersList(){
	/// Load users list
	if( ! $( "#papp-users-list" ) ){
		log.error("$( #papp-users-list ) not found");
		return;
	}
	console.log( "pappObj.id = " + pappObj.id + ", pappObj.resUsers[ pappObj.id ] = " );
	console.log(pappObj.resUsers[ pappObj.id ]);
	
	if( pappObj.id && pappObj.resUsers[ pappObj.id ] ){
		var resUsers = pappObj.resUsers[ pappObj.id ]; 
		var html = "";
		$( "#papp-users-list" ).empty();
		$( "#papp-users-list" ).listview();
		if( resUsers.users && resUsers.users.items && resUsers.users.items.length > 0 ){
			$.each( resUsers.users.items, function (index){
				if ( this.user == "" || this.user == undefined ){
					return true; // ~ continue
				}
				html += '\
					<li>\
						<a>' + this.user + '</a>\
						<a id="papp-deleteuser-btn-' + index + '" data-role="button" class="papp-deleteuser-btn" data-user="' + this.user + '" href="#" >Delete</a>\
					</li>';
			});
			//$( "#papp-wait-list" ).append(htmltest).listview('refresh').trigger("create"); ///just test
			$( "#papp-users-list" ).append(html).listview('refresh').trigger( "create" );
		} else {
			$( "#papp-users-list" ).html('<center>No users</center>').listview( "refresh" );
		}
	} else {
		$( "#papp-users-list" ).html('<center>No users</center>').listview( "refresh" );
	}
}

function papp_loadDataDetails(){
	
	/// Get service from provider page
	pappObj.service = providerObj.services[pappObj.id];
	/*
	 * Parse parameters
	 */
	console.log( "pappObj.resParams[ pappObj.id ]");
	console.log( pappObj.resParams[ pappObj.id ] );
	if ( pappObj.id 
			&& pappObj.resParams[ pappObj.id ]
			&& pappObj.resParams[ pappObj.id ].params
			&& pappObj.resParams[ pappObj.id ].params.items ){
		pappObj.service.params = {};
		$.each( pappObj.resParams[ pappObj.id ].params.items, function (index){
			if ( ! ( this.id ) ){
				return true; // ~ continue
			}
			var param = {};
			param.id = this.id;
			param.name = this.name;
			param.type = this.type;
			param.defaultValue = this.defaultValue;
			param.descript = this.description;
			param.isRequired = this.isRequired;
			param.isOutput = this.isOutput;
			param.prefix = this.prefix;
			pappObj.service.params[ param.id ] = param;
		});
	} else {
		console.error("Message invalid");
	}
	console.log("pappObj.service.params = ");
	console.log(pappObj.service.params);
	/*
	 * Fill to form
	 */
	console.log("Fill data to form: pappObj.service = ");
	console.log( pappObj.service );
	
	/// Id
	$("#provider-editapp-id").val( pappObj.service.id );
	
	/// Name
	$("#provider-editapp-name").val( pappObj.service.name );
	$("#papp-title").html( pappObj.service.name ? pappObj.service.name : "(No name)"); /// Load title
	
	/// Path
	$("#provider-editapp-path").val( pappObj.service.path );
	
	/// Job script file path
	$("#provider-editapp-shpath").val( pappObj.service.shpath );

        /// Post-processing sh file path
        $("#provider-editapp-postshpath").val( pappObj.service.postshpath );

        /// Pre-processing sh file path
        $("#provider-editapp-preshpath").val( pappObj.service.preshpath );

	/// Stage-in
	console.log( pappObj.service.stageinDirs);
	if(pappObj.service.stageinDirs != undefined && pappObj.service.stageinDirs.length > 0){
		$("#provider-editapp-stgin").val( pappObj.service.stageinDirs[0].path);
	} else	 {
		$("#provider-editapp-stgin").val("");
	}
	/// Stageout
	
	
	if(pappObj.service.stageoutDirs != undefined && pappObj.service.stageoutDirs.length > 0){
		$("#provider-editapp-stgout").val( pappObj.service.stageoutDirs[0].path);
	}
	else	
		$("#provider-editapp-stgout").val("");
	
	/// Image 
	$("#provider-editapp-image").empty();
	if(pappObj.resImages.images && pappObj.resImages.images.length){
		$.each(pappObj.resImages.images, function(index, imageObj) {
			//alert("pappObj.service.image = " + pappObj.service.image + ", imageObj.image = " + imageObj.image);
			var optTemp = '<option value="' + imageObj.image + '" ' 
							+ (imageObj.image == pappObj.service.image ? "selected" : "") + '>' 
							+ imageObj.name 
						+ '</option>';
			$("#provider-editapp-image").append(optTemp);
		});
	} else {
		var optTemp = '<option value="' + pappObj.service.image + '">' + pappObj.service.image + '</option>';
		$("#provider-editapp-image").append(optTemp);
	}
	//alert("pappObj.service.image = " + pappObj.service.image);// + ", imageObj.image = " + imageObj.image);
	$("#provider-editapp-image").selectmenu();
	$("#provider-editapp-image").selectmenu('refresh', true);
	
	/// Icon
	$("#provider-editapp-icon-view").attr("src", uIcon_genIconPath(pappObj.service.id));
	
	$("#provider-editapp-elapsed").val( pappObj.service.maxElapsedTime );
	
	$("#provider-editapp-nodes").val( pappObj.service.noNodes );
	
	/// Description
	$("#provider-editapp-descript").val( pappObj.service.descript );
	
	papp_refreshParamTable();
	papp_refreshCommandPreview();
	$( "#provider-editapp-submit-btn" ).button("disable");
	$( "#provider-editapp-undo-btn" ).addClass("ui-disabled");
}

/*
 * Refresh parameter table
 */
function papp_refreshParamTable(){
	pu_refreshParamTable( 'editapp', 'provider-editapp-params-table', pappObj.service.params );
}

/*
 * Refresh preview textbox
 */
function papp_refreshCommandPreview(){
	pu_refreshCommandPreview( 'provider-editapp-preview', 'provider-editapp-path', pappObj.service );
}

/*
function papp_pageBeforeShow(){

	if ($.mobile.pageData && $.mobile.pageData.id){
		//app_clearData();
	    console.log("Parameter id=" + $.mobile.pageData.id);
	    
	    /// Check other service selected -> need fill new data
	    if( pappObj.serviceId != $.mobile.pageData.id ){
	    	pappObj.isOtherServices [ "#papp" ] = true;
	    	pappObj.isOtherServices [ "#papp-submit" ] = true;
	    	pappObj.isOtherServices [ "#papp-detail" ] = true;
	    }
	    
	    /// Save service id
	    pappObj.serviceId = $.mobile.pageData.id;
	    
	    /// Save pageData for refresh page
	    sessionStorage.setItem("papp.pageData.id", $.mobile.pageData.id);
	}else{
		if( sessionStorage.getItem("papp.pageData.id") != undefined ){
			if ( $.mobile.pageData == undefined ) {
				$.mobile.pageData = new Object();
			}
			if( $.mobile.pageData.id == undefined ) {
				$.mobile.pageData.id = sessionStorage.getItem("papp.pageData.id");
				/// Save service id
			    pappObj.serviceId = $.mobile.pageData.id;
				
			    pappObj.f5 = true;
				console.log("Bp: #papp: pagebeforeshow F5");
			}
		}else{
			//console.error("Page load: Can not load this page, please back to home and try again.");
			console.error("Page load: app.pageData.id not exist.");
		}
	}
	return pappObj;
}	
*/
function papp_setTabFocus( tabname ){
	$( '.papp-tab-btn').removeClass("ui-btn-active ui-state-persist");
	$( '#papp-tab-btn-' + tabname ).addClass("ui-btn-active ui-state-persist");
	switch ( tabname ){
	case "users":
		$( '#papp-tab-users' ).show();
		$( '#papp-tab-waitlist' ).hide();
		$( '#papp-tab-details' ).hide();
		break;
	case "waitlist":
		$( '#papp-tab-users' ).hide();
		$( '#papp-tab-waitlist' ).show();
		$( '#papp-tab-details' ).hide();
		break;
	case "details":
		$( '#papp-tab-users' ).hide();
		$( '#papp-tab-waitlist' ).hide();
		$( '#papp-tab-details' ).show();
		break;
	default:
		console.log("Error: Unknow ");
		return;
		//break;
	}
	/// Save to sessionStorage
	sessionStorage.setItem("provider-app.pappObj.currentTab", pappObj.currentTab);
	
	/// Save current tab
	pappObj.currentTab = tabname;
}

