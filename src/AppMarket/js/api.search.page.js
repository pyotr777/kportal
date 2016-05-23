
/*
 * ******************************
 * Event handler
 * ****************************** 
 */

$(document).on("pagebeforeshow", "#market-search", function(event){
    
	console.log('#market-search: pagebeforeshow.');
    
    $.mobile.loading("show");
    
    if( db_services == undefined ){
    	//market_initData2();
    	market_refreshServices(function (){
    		search_loadServices(db_services);
    		$.mobile.loading("hide");
    	});
    }else{
    	if( db_services.length == 0 ){
        	search_popupShow("Service not found.");
        	$("#search-list").empty();
    	}else{
        	search_loadServices(db_services);    		
    	}
    	$.mobile.loading("hide");
    }
	
});


$(document).on("pageshow", "#market-search", function(event){
	$("#search-list").trigger( 'create' );
});
/*
 * ******************************
 * API function
 * ****************************** 
 */

function search_loadServices(services){
	console.log(services);
	if(services == undefined)
		return;
	/// Clean list app
	//$("#search-list").empty();
	/// Append new
	var html = "";
	$("#ms-content").html('');
	$('<ul/>',{'data-role':'listview','data-inset':true,'data-filter-reveal':true,'data-filter':true,'data-filter-placeholder':'Search service...'}).appendTo( '#ms-content' );
	for (var i in services) {
		html = search_appendToList(services[i]);
		$('<li/>').html( html ).appendTo( '#ms-content ul' );
	}
	$('#ms-content').trigger( 'create' );
	$.mobile.loading("hide");
}

function search_appendToList(service){
	console.log("Bp: search_appendToList(service):");
	console.log(service);
	var name = service.name;
	var id = service.id;
	var href = '#detail' + '?id=' + id;
	var provider = service.provider;
	var status = service.status;
	
//    var name = service.service,
//    	id = service.service,
//    	status = "Uninstall",
//    	href = '#detail' + '?id=' + name;
	
	var li = '';
	//li += '<li>';
	li += '		<a href="' + href + '">';
	li += '    		<img src="' + uIcon_genIconPath( id ) + '" height="77px" width="77px" onerror="this.src=\'images/no-image.png\'">';
	li += '   		<h2> ' + name + '</h2>';
	li += '    		<p>' + provider + '</p>';
	li += '		</a>';
	li += '    <span class="ui-li-count price">' + status + '</span>';
	//li += '</li>';
	//$("#search-list").append(li).listview("refresh");
	return li;
}

/*
 **************************************
 * Popup API
 ************************************** 
 * */
function search_popupShow(msg, icon){
	msg = '<img src="images/icons/warning.png">' + " " + msg;
	$( "#search-popup" ).popup();
	$( "#search-popup-message" ).html(msg);
	$( "#search-popup" ).popup( "open" );
	
}
