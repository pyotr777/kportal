var adminObj = new Object();
adminObj.isNeedReloadData = false;
adminObj.isLoaded = false;
adminObj.ignoreInitData = false;
adminObj.currentTab = "wait";
adminObj.registers;
adminObj.providers;
adminObj.selectedRegisterIndex = undefined;
adminObj.selectedProviderIndex = undefined;

/*
* ******************************************************************************************
* 									Event handler
* ****************************** ***********************************************************
*/
$(document).on("pagebeforeshow", "#admin", function(e, data){
	console.log("#admin pagebeforeshow!");
	//adminObj.isNeedReloadData = gl_isNeedReloadData();
	
    /// Load login name
    uLogin_success();
});

$(document).on("pageshow", "#admin", function(e, data){
	console.log("#admin: pageshow");
	
	//loading_show( 1500 );
		
    //if( db_services  
    //		&& ( adminObj.isNeedReloadData || ( ! adminObj.isLoaded ) ))
    //{
    	
		//admin_loadData( db_services );
		//adminObj.isLoaded = true;
		//loading_hide();
		admin_setSelectedTab( adminObj.currentTab );
		
//    	$( '#admin-tab-installed' ).show();
//    	$( '#admin-tab-applied' ).hide();
//    	$( '#admin-tab-noninstall' ).hide();
    //}
    //else{
    	//search_popupShow("Service not found.");
    	//console.log('Bp: "#admin: pageshow: Do not need reload data.');
    	//loading_hide();
    //}
	
});

$(document).on( "click", '.admin-tab-btn', function(){
	admin_setSelectedTab( $(this).attr("data-tab") );
});

/*
 * ************************************************************************
 * 							API function
 * ****************************** ******************************************
*/

function admin_setSelectedTab( tabname ){
	$( '.admin-tab-btn').removeClass("ui-btn-active ui-state-persist");
	$( '#admin-tab-btn-' + tabname ).addClass("ui-btn-active ui-state-persist");
	switch ( tabname ){
	case "provider":
		$( '#admin-tab-provider' ).show();
		$( '#admin-tab-wait' ).hide();
		/// Load tab data
		admin_loadProviderTab();
		break;
	case "wait":
		$( '#admin-tab-provider' ).hide();
		$( '#admin-tab-wait' ).show();
		/// Load tab data
		admin_loadWaitTab();
		break;
	}

	/// Save current tab
	adminObj.currentTab = tabname;
}

/* 
 * Load data tab providers list
 *"providers" : [
 *       { "user" : userEmail01, "phone": "phoneNumber", "address":"", "description" : providerDescription },
 *      { "user" : userEmail01, "phone": "phoneNumber", "address":"", "description" : providerDescription },
 *      ...
 *   ]
 */
function admin_loadProviderTab(){
	var listID = "#admin-card-list-provider";
	$(listID).empty();
	ws_getProviders(function(res){
		var isHasInvalid = false;
		$(listID).empty();
		if(res.providers && res.providers.length > 0){
			adminObj.providers = res.providers;
			$.each( res.providers, function ( index ) {
				admin_appendProviderItem( listID, this, index );
			});
		} else {
			$(listID).append("<br/><center>No such provider.</center>");//.trigger('create');
		}
		$(listID).listview().listview('refresh');
		$(listID).trigger("create");
	});
}

function admin_appendProviderItem(listID, register, arrayIndex){
	var html = '';
	html += '<li>'
			+ '<a href="#" onclick="admin_providerSelectedChange(' + arrayIndex + ');">' + register.email + '</a>'
            + '<a onclick="addmin_rejectProvider(\''+ register.email + '\');\"' 
            + ' data-rel="popup" data-position-to="window" data-transition="pop" data-icon="delete" data-theme="c">Delete</a>'
		    + '</li>';
	$(listID).append(html);//.trigger('create');
}

/*
 * 
 */
function admin_providerSelectedChange(arrayIndex){
	if(arrayIndex >= 0 && arrayIndex < adminObj.providers.length){
		adminObj.selectedProviderIndex = arrayIndex;
		var register = adminObj.providers[arrayIndex];
		/// Load data to popup
		$("#arp-email").val(register.email);
		$("#arp-phone").val(register.phone);
		$("#arp-address").val(register.address);
		$("#arp-descript").val(register.description);
		$("#arp-kacc").val(register.kdeskacc);
		
		$("#arp-publickey-lbl").show();
		$("#arp-publickey").show();
		$("#arp-publickey").val(register.publicKey);
		
		$("#arp-timeusable-lbl").show();
		$("#arp-timeusable").show();
		$("#arp-timeusable").val(register.timeUsable);
		$("#arp-provider-reject-btn").show();
		$("#arp-provider-apply-btn").show();
		
		$("#arp-register-accept-btn").hide();
		$("#arp-register-reject-btn").hide();
	
		/// Show popup
		$( "#admin-register-popup" ).popup( "open", { positionTo: "window" } );
	}
}

/*
 * 
 */
function addmin_rejectProvider(userId) {
	$.mobile.loading("show");
    ws_rejectProvider(userId, function( msg ) {
		$.mobile.loading("hide");
       if (msg.result === ResponseCodes.DATA_SUCCESS) {
           alert("remove provider success");
       } else {
           alert("remove provider fail");
       }
	   admin_loadProviderTab();
       //load_CandidateOfService();
   });
}

/*
 * 
 */
function admin_rejectProviderBtnClick(){
	var register = adminObj.providers[adminObj.selectedProviderIndex];
	addmin_rejectProvider(register.email);
}

function admin_applyroviderBtnClick(){
	var register = adminObj.providers[adminObj.selectedProviderIndex];	
	var timeUsable = $("#arp-timeusable").val();
	var userId = register.email;
	if(timeUsable != register.timeUsable){
		$.mobile.loading("show");
		ws_updateTimeUsable(userId, timeUsable, function( msg ) {
			$.mobile.loading("hide");
		   if (msg.result === ResponseCodes.DATA_SUCCESS) {
				alert("Apply time usable success");
		   } else {
				alert("Apply time usable  fail");
		   }
		   admin_loadProviderTab();
		});
	}
}

/* 
 * Load data tab wait list
 *"registers" : [
 *       { "user" : userEmail01, "phone": "phoneNumber", "address":"", "description" : providerDescription },
 *      { "user" : userEmail01, "phone": "phoneNumber", "address":"", "description" : providerDescription },
 *      ...
 *   ]
 */
function admin_loadWaitTab(){
	var listID = "#admin-card-list-wait";
	$(listID).empty();
	ws_getProviderRegisters(function(res){
		var isHasInvalid = false;
		$(listID).empty();
		if(res.registers && res.registers.length > 0){
			adminObj.registers = res.registers;
			$.each( res.registers, function ( index ) {
				admin_appendRegisterItem( listID, this, index);
			});
		} else {
			res.registers = [];
			$(listID).append("<br/><center>No such register.</center>");//.trigger('create');
		}
		$(listID).listview().listview('refresh');
		$(listID).trigger("create");
	});
}

function admin_appendRegisterItem(listID, register, arrayIndex){
	var html = '';
	html += '<li>';
	html +='<a href="#" onclick="admin_registerSelectedChange(' + arrayIndex + ');">' + register.email + '</a>';
	html +='<a onclick="addmin_rejectRegister(\''+ register.email
			+'\');" class="split-button-custom" data-role="button" data-icon="delete" data-theme= "e" data-iconpos="notext">Reject</a>';
	html +='<a onclick="addmin_acceptRegister(\''+ register.email
			+'\');" class="split-button-custom" data-role="button" data-icon="check" data-iconpos="notext">Accept</a>';
	html +='<a href="#" style="display: none;">';
	html +='</li>';
	$(listID).append( html ).listview('refresh').trigger("create");//.trigger('create');
	//$( "#papp-wait-list" ).append( html ).listview('refresh').trigger("create");
}

/*
 * 
 */
function addmin_rejectRegister(userId) {
    console.log("ws_rejectProviderRegister");
	$.mobile.loading("show");
    ws_rejectProviderRegister(userId, function( msg ) {
		$.mobile.loading("hide");
       if (msg.result === ResponseCodes.DATA_SUCCESS) {
           alert("remove candidate success");
       } else {
            alert("remove candidate fail");
       }
       //load_CandidateOfService();
	   admin_loadWaitTab();
   });
}

/*
 * 
 */
function addmin_acceptRegister(userId) {
    console.log("acceptRegister");
	$.mobile.loading("show");
    ws_acceptProviderRegister(userId, function( msg ) {
       if (msg.result === ResponseCodes.DATA_SUCCESS) {
           alert("Add user success");
       } else {
            alert("Add user fail");
	   }
	   //load_CandidateOfService();
	   admin_loadWaitTab();
	   $.mobile.loading("hide");
    });
}

/*
 * 
 */
function admin_acceptRegisterBtnClick(){
	var register = adminObj.registers[adminObj.selectedRegisterIndex];
	addmin_acceptRegister(register.email);
}
/*
 * 
 */
function admin_rejectRegisterBtnClick(){
	var register = adminObj.registers[adminObj.selectedRegisterIndex];
	addmin_rejectRegister(register.email);
}

/*
 * 
 */
function admin_registerSelectedChange(arrayIndex){
	if(arrayIndex >= 0 && arrayIndex < adminObj.registers.length){
		adminObj.selectedRegisterIndex = arrayIndex;
		var register = adminObj.registers[arrayIndex];
		/// Load data to popup
		$("#arp-email").val(register.email);
		$("#arp-phone").val(register.phone);
		$("#arp-address").val(register.address);
		$("#arp-descript").val(register.description);
		$("#arp-kacc").val(register.kdeskacc);
		$("#arp-publickey").hide();
		$("#arp-timeusable").hide()
		$("#arp-publickey-lbl").hide();
		$("#arp-timeusable-lbl").hide();
		$("#arp-provider-reject-btn").hide();
		$("#arp-provider-apply-btn").hide();
		$("#arp-register-accept-btn").show();
		$("#arp-register-reject-btn").show();
	
		/// Show popup
		$( "#admin-register-popup" ).popup( "open", { positionTo: "window" } );
	}
}


