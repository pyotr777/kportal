/*
*  ******************************************************************************************
* 								Define Global Variables
*  ****************************************************************************************** 
*/
/*
 * 
 * Code for send parameter url
 *
 * */
$( window ).resize(function() {
    ///$( "#dialog" ).dialog( "option", "position", $( "#dialog" ).dialog( "option", "position" ) );
});

(function( $, window, undefined ) {
	
	/*
	 * Function parse string to Object
	 * */
	function queryStringToObject( qstr )
	{
		var result = {},
			nvPairs = ( ( qstr || "" ).replace( /^\?/, "" ).split( /&/ ) ),
			i, pair, n, v;

		for ( i = 0; i < nvPairs.length; i++ ) {
			var pstr = nvPairs[ i ];
			if ( pstr ) {
				pair = pstr.split( /=/ );
				n = pair[ 0 ];
				v = pair[ 1 ];
				if ( result[ n ] === undefined ) {
					result[ n ] = v;
				} else {
					if ( typeof result[ n ] !== "object" ) {
						result[ n ] = [ result[ n ] ];
					}
					result[ n ].push( v );
				}
			}
		}

		return result;
	}
	
	/// Prevent load list when filter
	/*$( document ).bind( "pageinit", function(e){
		$('#app-list-jobs').listview('option', 'filterCallback', function(){
			
		});
		//$( "#app-list-jobs" ).listview();
	});*/
	
	$( document ).bind( "pagebeforechange", function( e, data ) {
//		console.log("Bp: pagebeforechange: data = ");
		//console.log("data.toPage: ");
		//console.log(data.toPage);
		//var obj = $.mobile.path.parseUrl(data.toPage);
		//console.log(obj);
		var to = data.toPage,
		
        from = data.options.fromPage;
		
		if ( typeof data.toPage === "string" ) {
			
			/// Parse parameters
			var u = $.mobile.path.parseUrl( data.toPage );
			if ( $.mobile.path.isEmbeddedPage( u ) ) {
				//alert('u.hash.replace( /^#/, "" ):' + u.hash.replace( /^#/, "" ));
				var u2 = $.mobile.path.parseUrl( u.hash.replace( /^#/, "" ) );
				if ( u2.search ) {
					if ( ! data.options.dataUrl ) {
						data.options.dataUrl = data.toPage;
					}
					data.options.pageData = queryStringToObject( u2.search );
					if( data.options.pageData ){
						data.toPage = u.hrefNoHash + "#" + u2.pathname;
					}else{
						data.toPage = "#home";
						alert("pagebeforechange: No page data !");
					}
					
				}
			}
			
			/// Save parameter to DOM
			$.mobile.pageData = (data && data.options && data.options.pageData)
	        ? data.options.pageData
	        : null;
			
			console.log("pagebeforechange: from = " + from + ", to = " + to);
	    	
			var u = $.mobile.path.parseUrl(to);
	    	
	    	console.log("ParseUrl:");
	    	console.log(u);
	    	
	    	//to = u.hash || '#' + u.pathname.substring(1);
	    	to = u.hash || '#home';
	    	
	        if (from){
	        	from = '#' + from.attr('id');
	        }
	      
        	if( ! checkPermission(from, getOnlyPageName( to )) ){
	        	console.log("Permission denied.");
	        	data.toPage = u.hrefNoHash;
	        	if( to == "#jobs" ){
	        		home_btnJobsClickRollback();
	        	}
	        	
	        	/// If user is provider back to home page
	        	if( gl_isLogin() ){
	        		data.toPage += "#home";
	        		
	        		// Replaces the current history entry
	        		history.replaceState(null, '', '#home');
	        		return;
	        	}
	        	e.preventDefault();
	        	return;
	        }

	        /// Check event back
	        checkBackEvent( from, getOnlyPageName( to ) );
	        
	        /// Check event back
	        checkChangeTabEvent( from, getOnlyPageName( to ) );
	        
	        /// Init page data before change page
	        //to = getOnlyPageName( to );
	        //initPageData(from, to, data.options.pageData, e);
	        
		}else{
			console.log("data.toPage is not string type.");
			console.log(data.toPage);
			if( to.context && to.context.documentURI && to.context.documentURI.indexOf("access_token") >= 0 ){
				oa2_loginContinue(to.context.documentURI);
			}
			//alert("continue");
			//return;
/*
			if( to.context && to.context.documentURI && to.context.documentURI ){
				/// Check permission
		        if( ! checkPermission(from, to) ){
		        	console.log("Permission denied.");
		        	data.toPage = u.hrefNoHash;

		        	if( to == "#jobs" ){
		        		home_btnJobsClickRollback();
		        	}
		        	
		        	/// If user is provider back to home page 
		        	if( gl_isLogin() ){
		        		data.toPage += "#home";
		        		
		        		// Replaces the current history entry
		        		history.replaceState(null, '', '#home');
		        		return;
		        	}

		        	e.preventDefault();
		        	return;
		        }
			}*/
		}
	});
	
})( jQuery, window );

//
//
//$(document).ready(function(){
//	alert("document.ready");
//	
//	if( appObj.loaded ){
//		setInterval(updateClock, 1000);
//	}
//	
//});


/*
 * ******************************
 * API function
 * ****************************** 
*/

var specchars = "?&";

function getOnlyPageName( fullname ){
	var pos = -1;
	for (var i = 0 ; i < specchars.length ; i ++){
		var tmp = fullname.indexOf( specchars[i] );
		//console.log("pos = " + pos + ", specchars[i] = " + specchars[i] + ", tmp = " + tmp );
		if ( ( pos < 0 ) || ( pos > 0 && tmp > 0 && tmp < pos ) ) {
			pos = tmp;
		}
	}
	if( pos < 0 ){
		//console.log("Bp: getOnlyPageName( fullname ): " + fullname);
		return fullname;
	}else{
		//console.log("Bp: getOnlyPageName( fullname ): " + fullname.substring( 0, pos ));
		return fullname.substring( 0, pos );
	}
}
/*
function  initPageData(from, to, params, event){
	console.log("Bp: initPageData: from=" + from + ", to=" + to);
	toShort = getOnlyPageName( to );
	
	switch( toShort ){
	case "#app":
		//app_initData(from, to, params, event);
		//$.mobile.changePage(toShort);
		break;
	case "#app-submit":
		//submit_initData(from, to, params, event);
		break;
	//case "#market":
		//market_initData( from, to, params, event );
		break;
	default:
		//console.log("Error: page not found.");
	
		break;
	}
}*/

/*
*	Check permission user
*/

function checkPermission(fromPage, toPage){
	console.log("Bp: checkPermission: from: " + fromPage + ", to: " + toPage + " page.");
	permit = false;
	if( gl_isLogin () ){
		if ( gl_isAdmin() ){
			switch( toPage ){
				case "#error":
				case "#home":
				case "#admin":
				permit = true;
				break;
			default:
				permit = false;
				break;
			}
		} else if ( gl_isProvider() ){
			switch( toPage ){
			case "#admin":
			/// Provider page
			/*
			case "#provider-app-users":
			case "#provider-app-wait":
			case "#provider":
			case "#provider-wait":
			case "#provider-paramedit-popup":
			case "#provider-paramnew-popup":
			/// Provider app page
			case "#provider-app":
			case "#provider-app-detail":
			
			/// Provider image page
			case "#provider-image":
			*/
				permit = false;
				break;
			default:
				permit = true;
				break;
			}
			
		}else{
			switch( toPage ){
			case "#home":
			case "#market":
			case "#market-search":
			case "#detail":
			case "#jobs":
			case "#jobs-finished":
			case "#job-detail":
			case "#app":
			case "#app-submit":
			case "#app-detail":
			case "#register":
			/*
			case "#provider-app-users":
			case "#provider-app-wait":
			case "#provider":
			case "#provider-wait":
			case "#admin":
			case "#provider-image":*/
				permit = true;
				break;
			default:		
				permit = false;
				break;
			}
		
		}
	} else {
		permit = false;
		
		switch( toPage ){
		case "#error":
		case "#market":
		case "#home":
			//alert("Permit = " + permit)
			permit = true;
			break;
		}	
	}
	//alert("Permit = " + permit)
	return permit;
}

/*
 * ******************************************************************************************
 * 									Global API function
 * ******************************************************************************************
*/

/*
 * Save current page id.
 * */
function gl_getCurrentPage(){
	var gl_currentPage = $.mobile.activePage[0].id;
	var currentPage;
	switch(gl_currentPage){
		case "":
			currentPage = "#home";
			break;
		default:
			currentPage = "#" + gl_currentPage;
			break;
	}
	return currentPage;
}

/*
*	Check change tab event 
*/
var isChangeTabEvent = {};

function checkChangeTabEvent (fromPage, toPage){
	/// App page: status, submit, detail tabs.
	if( fromPage && toPage && fromPage.indexOf("#app") == toPage.indexOf("#app") ){
		isChangeTabEvent[ toPage ] = true;
		return true;
	}
	
	if( fromPage && toPage && fromPage.indexOf("#jobs") == toPage.indexOf("#jobs") ){
		isChangeTabEvent[ toPage ] = true;
		return true;
	}
	
	/*/// Provider page include 3 tabs: provider-apps, provider-WaitList, provider-NewApp
	if( fromPage && toPage && fromPage.indexOf("#provider") == toPage.indexOf("#provider") ){
		isChangeTabEvent[ toPage ] = true;
		return true;
	}*/
	
	return false;
}

function gl_isChangeTabEvent(){
	var current = gl_getCurrentPage();
	console.log( "gl_isChangeTabEvent() current = " + current );
	console.log( "gl_isBackEvent() isBackEvents = " );
	console.log(isChangeTabEvent);
	if ( isChangeTabEvent[ current ] ){
		console.log('Bp: gl_isChangeTabEvent() TRUE');
		
		/// Only check once after clear status
		isChangeTabEvent[ current ] = false;
		return true;
	}else {
		return false;
	}
	 
}

/*
*	Check back page event 
*/

var isBackEvents = {};

function checkBackEvent (fromPage, toPage){
	switch ( fromPage ){
	case "#job-detail":
		if( toPage == "#jobs" || toPage == "#jobs-finished"){
			isBackEvents[ toPage ] = true;
		}else{
			isBackEvents[ toPage ] = false;
		}
		break;
	/*case "#app":
		if( toPage == "#jobs" || toPage == "#jobs-finished"){
			isBackEvents[ toPage ] = true;
		}else{
			isBackEvents[ toPage ] = false;
		}
		break;
		*/
	}
}

function gl_isBackEvent(){
	var current = gl_getCurrentPage();
	console.log( "gl_isBackEvent() current = " + current );
	console.log( "gl_isBackEvent() isBackEvents = " ); 
	console.log( isBackEvents );
	isBackEvents[ current ]
	if( isBackEvents[ current ] && isBackEvents[ current ] == true ){
		return true;
	}
	return false;
}

/*
 * **********************************
 * Manager refresh page flag
 * **********************************
 * */
var pagesNotification = {
		"#home": [
		          { "service" : "", "hasChange" : false }
		          ],
		"#market": [
		          { "service" : "", "hasChange" : false }
		          ],
		"#app": [
		          { "service" : "", "hasChange" : false }
		          ],
		"#jobs": [
		          { "service" : "", "hasChange" : false }
		          ],
  		"#jobs-finished": [
		          { "service" : "", "hasChange" : false }
		          ],
		"#job-detail": [
		                { "job" : "", "hasChange" : false } 
		                ],
		"#provider": [
		              { "service" : "", "hasChange" : false } 
		              ],
		"#provider-app": [
              { "service" : "", "hasChange" : false } 
              ]
};

function gl_jobChange( serviceId, jobId ){
	/// Set job notify change
	if( $.isArray(pagesNotification["#job-detail"]) ){
		var existed = false;
		$.each( pagesNotification["#job-detail"] , function ( index ) {
			if( this.job == jobId ){
				existed = true;
				this.hasChange = true;
				return false;
			}
		});
		if( ! existed ) {
			pagesNotification["#job-detail"].push({"job" : jobId, hasChange : true})
		}
	}
	
	/// Set service notify change
	setServiceChange( "#app", serviceId );
	setServiceChange( "#jobs", serviceId );
	setServiceChange( "#jobs-finished", serviceId );
}

function gl_jobNew( serviceId ){
	/// Set service notify change
	setServiceChange( "#app", serviceId );
	setServiceChange( "#jobs", serviceId );
	setServiceChange( "#jobs-finished", serviceId );
}

function setServiceChange( page, serviceId ){
	
	if( $.isArray(pagesNotification[page]) ){
		var existed = false;
		$.each( pagesNotification[page] , function ( index ) {
			if( this.service == serviceId ){
				this.hasChange = true;
				existed = true;
				return false;
			}
		});
		if( ! existed ) {
			pagesNotification[ page ].push({"service" : serviceId, hasChange : true})
		}
	}
}

/*
 * Set service change notify when app page has service change.
 * */
function gl_appChange( serviceId ){
	setServiceChange( "#market", serviceId );
	setServiceChange( "#app", serviceId );
	setServiceChange( "#jobs", serviceId );
	setServiceChange( "#jobs-finished", serviceId );
}

/*
 * Set service change notify when user remove app.
 * */
function gl_appRemove( serviceId ){
	setServiceChange( "#home", serviceId );
	setServiceChange( "#market", serviceId );
	setServiceChange( "#jobs", serviceId );
	setServiceChange( "#jobs-finished", serviceId );
}
/*
 * 
 */
function gl_providerChangeService( serviceId ){
	setServiceChange( "#provider", serviceId );
	setServiceChange( "#provider-app", serviceId );
}


function gl_providerChangeUser( serviceId ){
	setServiceChange( "#provider", serviceId );
	setServiceChange( "#provider-app", serviceId );
}

/*
 *	Function check status need reload	 
 */
function gl_isNeedReloadData( params ) {
	var currentPage = gl_getCurrentPage();
	var noti = pagesNotification[ currentPage ], 
		isNeed = true;
	console.log("Bp: isNeedReloadData( params ): noti ");
	console.log(params);
	console.log(noti);
	if( noti == undefined ){
		isNeed = true;
	} else if( params && params.service ){
		$.each( noti, function (index){
			if( this.service == params.service && this.hasChange == true){
				isNeed = true;
				this.hasChange = false;
				return false;
			}
		});	
	}else{
		$.each( noti, function (index){
			if( this.hasChange == true ){
				isNeed = true;
				this.hasChange = false;
				return false;
			}
		});
	}
	//alert("currentPage = " + currentPage + ", isNeed = " + isNeed);
	console.log("Bp: isNeedReloadData( params ): isNeed is " + isNeed);
	return isNeed;

	/*switch( currentPage ){
	case "#home":
	case "#market":
	case "#app":
		noti = pagesNotification[ currentPage ];
		if( params && params.service ){
			$.each( noti, function (index){
				if( this.service == params.service && this.hasChange == true){
					isNeed = true;
					this.hasChange = false;
					return false;
				}
			});
		}else{
			$.each( noti, function (index){
				if( this.hasChange == true ){
					isNeed = true;
					this.hasChange = false;
					return false;
				}
			});
		}
	break;

	case "#jobs":
	case "#jobs-finished":
		//pageId = currentPage
		noti = pagesNotification[ currentPage ];
		console.log("Bp:  gl_isNeedReloadData( params ); noti = ");
		console.log(noti);
		if( params && params.service ){
			$.each( noti, function (index){
				if( this.service == params.service && this.hasChange == true){
					isNeed = true;
					this.hasChange = false;
					return false;
				}
			});	
		}else{
			$.each( noti, function (index){
				if( this.hasChange == true ){
					isNeed = true;
					this.hasChange = false;
					return false;
				}
			});
		}
		break;
	case "#provider":
	case "#provider-app":
		noti = pagesNotification[ currentPage ];
		if( params && params.service ){
			$.each( noti, function (index){
				if( this.service == params.service && this.hasChange == true){
					isNeed = true;
					this.hasChange = false;
					return false;
				}
			});	
		}else{
			$.each( noti, function (index){
				if( this.hasChange == true ){
					isNeed = true;
					this.hasChange = false;
					return false;
				}
			});
		}
		break;
	default:
		noti = pagesNotification[ currentPage ];
		if( params && params.service ){
			$.each( noti, function (index){
				if( this.service == params.service && this.hasChange == true){
					isNeed = true;
					this.hasChange = false;
					return false;
				}
			});	
		}else{
			$.each( noti, function (index){
				if( this.hasChange == true ){
					isNeed = true;
					this.hasChange = false;
					return false;
				}
			});
		}
		break;
	}
	console.log("Bp: isNeedReloadData( params ): isNeed is " + isNeed);
	return isNeed;
	*/
}
/*
 * Check for HTML5 Storage
 * 
 * */
function supports_html5_storage() {
	try {
		return 'localStorage' in window && window['localStorage'] !== null;
	} catch (e) {
	    return false;
	}
}
/*
 * Check user login
 * */
function gl_isLogin(){
	return localStorage.loginId != undefined && localStorage.email != undefined && localStorage.userType!= undefined ;
}

/*
 * Check user is provider
 * */
function gl_isProvider(){
	return ( gl_isLogin ()  && localStorage.userType == "provider" );
}
/*
 * Check user is admin
 */
function gl_isAdmin(){
	return ( gl_isLogin ()  && localStorage.userType == "admin" );
}
/*
 * Job status to string
 * */
function gl_jobStatusToString( status ){
	var str = '';	
	switch(status){
	case 1: //JOB_FINISH,
		str = "Finished";
		break;
	case 2: //JOB_RUNNING,
		str = "Running"
		break; 
	case 3: //JOB_START,
		str = "Initializing";
		break;
	default:
		str = "Error"
		break;
	}
/*
	var str = '';	
	switch(status){
	case 0: //JOB_UNKNOW,
		break;
	case 1: //JOB_FINISH,
		str = "Finished";
		break;
	case 2: //JOB_RUNNING,
		str = "Running"
		break; 
	case 3: //JOB_START,
		str = "Started";
		break;
	case 4: //JOB_SUSPEND,
		//str = "suspend";
		str = "Cancelled";
		break;
	case 7: //JOB_QUEUE,
		str = "Queue";	
		break;
	
	default:
		str = "Error"
		break;
	}*/
	return str;
}
