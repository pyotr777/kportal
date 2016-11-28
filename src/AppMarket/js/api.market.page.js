var marketObj = new Object();
marketObj.isNeedReloadData = false;
marketObj.isLoaded = false;
marketObj.numOfApp = { "installed" : 0, "applied" : 0, "noninstall" : 0 };
marketObj.ignoreInitData = false;
marketObj.currentTab = "installed";

var db_services;

/*
* ******************************************************************************************
* 									Event handler
* ****************************** ***********************************************************
*/
$(document).on("pagebeforeshow", "#market", function(event){
	
});

/*
$(document).on("pagebeforeshow", "#market", function(e, data){
	
	console.log("#market pagebeforeshow!");
	marketObj.isNeedReloadData = gl_isNeedReloadData();
});*/

$(document).on("pageshow", "#market", function(e, data){
	console.log("#market: pageshow");
	loading_show( 3500 );
//	sessionStorage.services = JSON.stringify( items );
//	marketObj.registerServices = items;
	market_refreshServices(function(services){
		market_loadData(services);
//		marketObj.isLoaded = true;
		loading_hide();
		market_setTabFocus( marketObj.currentTab );
	});
	/*
    if( db_services  
    		&& ( marketObj.isNeedReloadData || ( ! marketObj.isLoaded ) ))
    {
    	
		market_loadData( db_services );
		marketObj.isLoaded = true;
		loading_hide();
		market_setTabFocus( marketObj.currentTab );
    }
    else{
    	//search_popupShow("Service not found.");
    	console.log('Bp: "#market: pageshow: Do not need reload data.');
    	loading_hide();
    }*/
	
});

$(document).on( "click", '.market-tab-btn', function(){
	market_setTabFocus( $(this).attr( "data-tab" ) );
});

/*
 * ************************************************************************
 * 							API function
 * ****************************** ******************************************
*/

function market_setTabFocus( tabname ){
	$( '.market-tab-btn').removeClass("ui-btn-active ui-state-persist");
	$( '#market-tab-btn-' + tabname ).addClass("ui-btn-active ui-state-persist");
	switch ( tabname ){
	case "installed":
		$( '#market-tab-installed' ).show();
		$( '#market-tab-applied' ).hide();
		$( '#market-tab-noninstall' ).hide();
		break;
	case "applied":
		$( '#market-tab-installed' ).hide();
		$( '#market-tab-applied' ).show();
		$( '#market-tab-noninstall' ).hide();
		break;
	case "noninstall":
		$( '#market-tab-installed' ).hide();
		$( '#market-tab-applied' ).hide();
		$( '#market-tab-noninstall' ).show();
		break;
	}

	/// Save current tab
	marketObj.currentTab = tabname;
}

function market_refreshServices( callback ){
	
	ws_getAllServices(function ( resAll ) {
		if( resAll.services && resAll.services.items ){
			ws_findServices( function ( resItems ) {
				ws_getWaitingServices( function ( resWait ) {
					if( resWait.services && resWait.services.items ){
						marketObj.registerServices = resItems;
						marketObj.waitServices = resWait.services.items;
						marketObj.allServices = resAll.services.items;
						market_mergeServiceInfo();
						/// Backup data
						uStorage_backupData();
						if( callback ){
							callback(db_services);
						}
					}else{
						console.error("Bp: market_refreshServices: Error: resWait.services.items is missing.");
					}
				});
			});			
		}else{
			console.error("Bp: market_refreshServices: Error: resAll.services.items is missing.");
		}
	});
}

function market_loadData( services ) {
	
	if( services == undefined ){
		console.log("Error: market_loadData( services ) : services is UNDEFINED.");
		$.mobile.loading("hide");
		return;
	}
	
	/// Clear card list
	$("#card-list-noninstall").empty();
	$("#card-list-applied").empty();
	$("#card-list-installed").empty();
	
	$.each( services, function ( key, value ) {
		market_appendApp( value );
	});
	
	/// Check is empty tab
	$.each(marketObj.numOfApp, function (key, value){
		if( value == 0 ){
			var gridName = "#card-list-" + key;
			$(gridName).append("<center>No such service.</center>").trigger('create');
		}
	});
	
	/// Refresh css
	$('#market').trigger('create');
	//$.mobile.loading("hide");
}
/*
function market_initDataAllService(){

	console.log( "Bp: market_initData( from, to, params, event ): sessionStorage.services");
	console.log( sessionStorage.services );
	
	if( marketObj.allServices ){
		console.log( "Bp: market_initData( from, to, params, event ): marketObj.allServices");
		console.log( marketObj.allServices );
		if( marketObj.waitServices ){
			market_mergeServiceInfo();
			
			/// Change page to continue load data
			marketObj.ignoreInitData = true;
			$.mobile.changePage ( "#market" );
		}else{
			ws_getWaitingServices( function ( resWait ) {
				console.log( "Bp: market_initData: resWait:" );
				console.log( resWait );
				if( ! resWait.services ){
					console.log( "Error: services tag is missing.");
					return;
				}
				marketObj.waitServices = resWait.services.items;
				market_mergeServiceInfo();
				
				/// Change page to continue load data
				marketObj.ignoreInitData = true;
				$.mobile.changePage ( "#market" );
			});
		}
	}else{
		ws_getAllServices( function ( res ) {
			console.log("Bp: market_initData: res:");
			console.log( res );
			if( ! res.services ){
				console.log( "Error: services tag is missing.");
				return;
			}
			
			marketObj.allServices = res.services.items;
			
			if( marketObj.waitServices ){
				market_mergeServiceInfo();
				
				/// Change page to continue load data
				marketObj.ignoreInitData = true;
				$.mobile.changePage ( "#market" );
			}else{
				ws_getWaitingServices( function ( resWait ) {
					console.log( "Bp: market_initData: resWait:" );
					console.log( resWait );
					if( ! resWait.services ){
						console.log( "Error: services tag is missing.");
						return;
					}
					marketObj.waitServices = resWait.services.items;
					market_mergeServiceInfo();
					
					/// Change page to continue load data
					marketObj.ignoreInitData = true;
					$.mobile.changePage ( "#market" );
				});
			}
		});
	}
}
*/
function market_mergeServiceInfo (){
	console.log( "Bp: market_mergeServiceInfo: marketObj.allServices=");
	console.log( marketObj.allServices );
	console.log( "marketObj.waitServices ="); 
	console.log( marketObj.waitServices );
	console.log( "marketObj.registerServices ="); 
	console.log( marketObj.registerServices );

	if( ! db_services ){
		db_services = {};
	}
	
	for( var i in marketObj.allServices ) {
		var id = marketObj.allServices[i].service;
		if( id == undefined ){
			continue;
		}
		var service = marketObj.allServices[i];
		service.id = id;
		service.status = "Install";
		//service.provider = marketObj.allServices[i].provider;

		var isCandidate = false, isRegister = false;
		
		/// Check into candidate list
		for( var j in marketObj.waitServices ){
			if( id == marketObj.waitServices[ j ].service ){
				isCandidate = true;
				break;
			}
		}
		
		if( isCandidate ){
			service.status = "Cancel";
		}else{
			
			/// Check into register list
			for( var k in marketObj.registerServices ){
				if(  id == marketObj.registerServices[ k ].service ){
					isRegister = true;
					break;
				}
			}
		}
		if( isRegister ){
			service.status = "Uninstall";
		}
		db_services [ id ] = service;
	}
}

function market_appendApp( service ){
	
	var name = service.name; //service.service;
	var id = service.id;
	var href = '#detail' + '?id=' + id;
	var provider = service.provider;
	var status = service.status;
	var tabid = "noninstall";
	
	switch(status.toLowerCase()){
	case "install":
		tabid = "noninstall";
		break;
	case "cancel":
		tabid = "applied";
		break;
	case "uninstall":
		tabid = "installed";
		break;
	default:
		console.error("Unknow status = " + status);
		break;
	}

	marketObj.numOfApp [ tabid ] ++;
	var gridName = "#card-list-" + tabid;
	
	var html = '';
	html += '<div class="card" id=' + id + '>';
		html += '<div class="card-content">';
			
			html += '<div class="card-cover">';
				html += '<div class="cover-image-container">';
					html += '<div class="cover-outer-align">';
						html += '<div class="cover-inner-align">';
							html += '<a class="card-click-target" href="' + href + '">';
							html += '<img class="card-cover-image"  src="' + uIcon_genIconPath( id ) + '" onerror="this.src=\'images/no-image.png\'"/>';
							html += '</a>';
						html += '</div>';
					html += '</div>';
				html += '</div>';
			html += '</div>';
			html += '<div class="details">';
				html += '<a class="title" href="' + href + '" title="'+ name + '" >';
					html += '<span style="text-overflow: ellipsis;overflow: hidden;white-space: nowrap;">';
							html += name;
							//html += '<span class="paragraph-end"></span>';
					html += '</span>';
				html += '</a>';
				html += '<div class="subtitle-container">';
					html += '<span class="price-container">';
						html += '<span class="price">' + status + '</span>';
					html += '</span>';
				html += '</div>';
			html += '</div>';
		html += '</div>';
	html += '</div>';

	$(gridName).append(html).trigger('create');
}
