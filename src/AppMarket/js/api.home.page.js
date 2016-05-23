var homeObj = new Object();
homeObj.numOfApp = 1;
homeObj.grid = 3;
homeObj.services;
homeObj.currentPage;


/*
* ******************************
* Event handler
* ****************************** 
*/

//$(document).on("pagebeforeshow", "#home", function(e, data){
//	console.log("#home: pagebeforeshow");
//	
//	if( ws && ws.readyState === 1){
//		home_retrieveServices();
//	}
//	else{
//		if( ! supports_html5_storage () ){
//	    	error_showPage("HTML5 Storage", "This browser don\'t support for HTML5 storage :\(");
//	    	return;
//		}
//		$.mobile.loading( "show" );
//
//		homeObj.currentPage = "#home";
//		ws_connect( home_connectCallback );
//	}
//});

$(document).on("pageshow", "#home", function(e, data){
	console.log("#home: pageshow");
	
	/// Reload active button
	$( '.bar-home-jobs' ).removeClass("ui-btn-active ui-state-persist");
	$( "#home-bar-btn-home" ).addClass("ui-btn-active ui-state-persist");
	
	if( ws && ws.readyState === 1){
		//home_retrieveServices();
		home_loadData();
	}
	else{
		if( ! supports_html5_storage () ){
	    	error_showPage("HTML5 Storage", "This browser don\'t support for HTML5 storage :\(");
	    	return;
		}
		$.mobile.loading( "show" );

		homeObj.currentPage = "#home";
		ws_connect(function(valid){
			if(valid == 1){
				home_connectCallback();
			} else {
				alert("Logout");
				home_logoutCallback("Token expied");
			}
		});
	}
});

//function home_retrieveServices(){
function home_loadData(){
	if( gl_isLogin() ){
		$("#home-menu-btn").removeClass('ui-btn-hidden');
		/// Redirect to provider page if is provider
		if( localStorage.userType == "admin" ){
			$.mobile.changePage("#admin");
			return;
		}
		if( localStorage.userType == "provider" ){
			$( '.hmp-provider' ).show();
			$( '.hmp-user' ).hide();
		} else {
			$( '.hmp-provider' ).hide();
			$( '.hmp-user' ).show();
			//$( '#home-list-menu-provider' ).hide();
			//$( '#home-list-menu-user' ).show();
		}
		
		home_loginSucess();
		if( homeObj.services && ( ! gl_isNeedReloadData() ) ){
			home_loadServices(homeObj.services);
		} else {
			ws_findServices( function( res ){
				home_loadServices(res);	
			});
		}
	}else{
		$.mobile.activePage.find("#login-btn").focus();
		console.log("Do not login.");
		//alert("Do not login");
		uLogin_resetBtn();
	}
}

/*
 * Load list menu
 */
function home_loadProviderMenuList(){
	$( '#home-list-menu-provider' ).show();
	$( '#home-list-menu-user' ).hide();
}

function home_loadUserMenuList(){
}

function home_connectCallback () {
	if( localStorage.loginId && localStorage.email && localStorage.userType) {
		home_login( home_loginCallback );
	}else{
		console.log("Bp: home_connectCallback.");
		cleanUserInfo();
		loading_hide();
	}
}

/*
 * *******************************************
 * Login button event
 * *******************************************
 */

$(document).on('click', '#home-btn-login', function(e){
	
	if($( "#signinText" ).text() === "Login"){
		cleanUserInfo();
		console.log("Bp: login now !");
		homeObj.currentPage = "#home";
		home_login( home_oa2LoginCallback );
	}else{
		console.log("Bp: logout now !");
		home_logout();
	}
});


/*
 * Callback function when have response from Oauth 2.0. 
 * */

function home_oa2LoginCallback( user ){
	if(user && user.email && user.access_token){
		ws_login1(user.email, user.access_token, home_loginCallback);	
	}else{
		home_loginError("Login fail.");
	}
}

function home_login( callback ){
	$.mobile.loading( "show" );
	//homeObj.loginSuccessCallback = callback;
	console.log("Bp: home_login(callback): callback = ");
	console.log(callback);
	if( localStorage.loginId && localStorage.email && localStorage.userType ){
		ws_login2(localStorage.loginId, callback);
	}else{
		cleanUserInfo();
		oa2_login( callback );
	}
}


/*
 * Callback function when have response from server. 
 * */

function home_loginCallback( msg ){
	//msg.valid = 0; /// just test
	//console.log("Bp: home_loginCallback( msg )" + JSON.stringify(msg));
	if( msg.valid != undefined ){
		if(msg.valid == 0) {
			home_loginError("Login fail !");
			return;
		}
		
		if( requests[ msg.requestId ].request.email ){
			localStorage.email = requests[ msg.requestId ].request.email;
		}
		
		if(msg.loginId){
			localStorage.loginId = msg.loginId;
		}
		
		localStorage.userType = msg.userType;
		
		/// Redirect to provider page if is provider
		if( localStorage.userType == "provider" ){
			$.mobile.changePage("#provider");
			return;
		}
		
		//ws_findServices( home_loadServices );
		//console.log("Bp: home_loginCallback: current page:" + homeObj.currentPage);
		
		//if(homeObj.currentPage && homeObj.currentPage != "#home"){
		//localStorage.setItem( "appmarket.login.redirect", "#market" );
		if( localStorage.getItem("appmarket.login.redirect") === "#market" ){
			localStorage.removeItem("appmarket.login.redirect");
			$.mobile.changePage("#market");
		}else{
			//home_retrieveServices();
			home_loadData();
		}
	}else{
		console.log("home_loginCallback( msg ): msg is unknow!");
		console.log( msg );
	}
}

function home_loginError(message){
	//popupShow( "Login", message );
	$.mobile.loading( "hide" );
	home_popupShow( message );
	cleanUserInfo();
	$.mobile.changePage( "#home", { reloadPage: true, transition: "none"} );
}

function home_loginSucess(){
	/*$("#signinText").text(localStorage.email);
	$("#home-btn-login").buttonMarkup({ icon: "delete" });*/
	uLogin_success();
}

/*
 * *******************************************
 * Logout button event
 * *******************************************
 */

function home_logout(){
	ws_logout( localStorage.loginId, home_logoutCallback );
}

function cleanUserInfo(){
	
	/// Remove from the localStorage
	localStorage.removeItem("userType");
	localStorage.removeItem("loginId");
	localStorage.removeItem("email");
	
	/// Remove everything from the sessionStorage object.
	sessionStorage.clear();
	
}

function home_cleanPageData(){
	homeObj.services = undefined;
}

function home_logoutCallback(msg){
	console.log("Bp: home_logoutCallback: msg: " + msg);
	$.mobile.loading( "show" );
	if( msg.valid == 1 ){
		/*$( "#signinText" ).text("Login");
		$( "#home-btn-login" ).buttonMarkup({ icon: "check" });*/
		
		oa2_logout(function(){
			console.log( "Bp: home_logoutCallback(msg): oa2_logout callback.");
			/// Clean temp data
			cleanUserInfo();
			
			home_cleanPageData();
			
			$.mobile.loading( "hide" );
			
			/// Reload page
			$.mobile.changePage( "#home", { reloadPage: true, transition: "none"} );

		});
	}
	else{
		console.error("Logout fail.");
		$.mobile.loading( "hide" );
		cleanUserInfo();
		home_cleanPageData();
		$.mobile.changePage( "#home", { reloadPage: true, transition: "none"} );
	}
}

/*
 * *******************************************
 * Market button event
 * *******************************************
 */
$(document).on('click', '#home-btn-market', function(e){
	home_marketOnclick();
});

function home_marketOnclick(){
	if( ! gl_isLogin() ){
		//homeObj.currentPage = "#market";
		localStorage.setItem( "appmarket.login.redirect", "#market" );
		home_login(home_oa2LoginCallback);
		return;
	}
	
	/// Save for redirect after aoth2.0 login
	sessionStorage.setItem("appmarket.page.prev", "#home");
	
	$.mobile.changePage("#market");
}

function home_marketOnclickCallback(){
	console.log("Bp: home_marketOnclickCallback().");
	$.mobile.changePage("#market");
}

$(document).on('click', '#menu-btn', function(e){
	console.log("#menu-btn: clicked");
	// Append #bar
	$.mobile.navigate( "#bar", {
	info: "info about the #bar hash"
	});

	// Replace #bar with #baz
	$.mobile.navigate( "#baz" );

	// Log the results of the navigate event
	$( window ).on( "navigate", function( event, data ){
	console.log( data.state.info );
	console.log( data.state.direction );
	console.log( data.state.url );
	console.log( data.state.hash );
	});

	// Go back to pop the state for #bar and log it
	window.history.back();
});

// Bind to the click of the example link
$( "#menu-btn" ).click(function( event ) {
  // Append #bar
  $.mobile.navigate( "#bar", {
    info: "info about the #bar hash"
  });

  // Replace #bar with #baz
  $.mobile.navigate( "#baz" );

  // Log the results of the navigate event
  $( window ).on( "navigate", function( event, data ){
    console.log( data.state.info );
    console.log( data.state.direction );
    console.log( data.state.url );
    console.log( data.state.hash );
  });

  // Go back to pop the state for #bar and log it
  window.history.back();
});

/*
 * *******************************************
 * Other api
 * *******************************************
 */

function home_loadServices( services ){
	
	console.log("Bp: home_loadServices( services ): services = ");
	console.log(services);
	homeObj.services = services;
	
	/// Save to storage
	sessionStorage.services = JSON.stringify(services);
	
	$("#grid").empty();
	
	var cell = '';
	cell += '<div class="ui-block-a">';
	cell += '	<div data-role="controlgroup"  align="center">';
	cell += '		<a id="home-btn-market" href="#" rel="external" style><img class="app-icon" src="images/market.png" alt="market" /></a>Market';
	cell += '	</div>';
	cell += '</div>';
	
	$("#grid").append(cell);
	homeObj.numOfApp = 1;
	
	//for(i = 0 ; i < services.length ; i ++ ){
	for (var i in services) {
		appendApp(services[i]);		
	}
	$.mobile.loading("hide");
}

function appendApp(service) {
	homeObj.numOfApp ++;
	//console.log(service);
	nameApp = service.name;
	/// Calculate cell position
	var pos = homeObj.numOfApp % homeObj.grid;
	var ui_block_x = 'a';
	
	switch(pos)
	{
	case 0:
		if(homeObj.grid == 2)
			ui_block_x = 'b';
		else if(homeObj.grid == 3)
			ui_block_x = 'c';
		else if(homeObj.grid == 4)
			ui_block_x = 'd';
		else if(homeObj.grid == 5)
			ui_block_x = 'e';
		
		break;
	case 1:
		ui_block_x = 'a';
		break;
	case 2:
		ui_block_x = 'b';
		break;
	case 3:
		ui_block_x = 'c';
		break;
	case 4:
		ui_block_x = 'd';
		break;
	case 5:
		ui_block_x = 'e';
		break;
	}
	
	
	var cell =  '';
	cell += '<div class="ui-block-' + ui_block_x + '">';
	cell += '	<div data-role="controlgroup" align="center">';
	cell += '		<a href="#app?id=' + service.service + '">';
	cell += '			<img class="app-icon" src="' + uIcon_genIconPath( service.service ) + '" height="60px" width="60px" onerror="this.src=\'images/no-image.png\'" alt="' + service.name + '"/>';
	cell += '			<span class="icon-label">' + nameApp + '</span>';			
	cell += '		</a>';
	//cell += nameApp ;
	cell += '	</div>';
	cell += '</div>';
	
//	cell += '	<div class="ui-block-' + ui_block_x + '">';
//	cell += '		<div data-role="controlgroup" align="center">';
//	cell += '			<a href="#app?id=' + nameApp + '" ><img class="app-icon" src="images/' + nameApp + '.png" alt="' + nameApp + '" onerror="this.src=\'images/service_default.png\'"/> </a>' + nameApp;
//	cell += '		</div>';
//	cell += '	</div>';
	$("#grid").append(cell);
	$('#home').trigger('create');
}

function home_btnJobsClickRollback(){
	home_popupShow("Require login before seeing Jobs page.", alert, function(){
		$( '#home-bar-btn-jobs' ).removeClass("ui-btn-active");
	});
}

/*
 * **********************************************
 * 
 * Swipe home page to jobs * 
 * 
 * **********************************************
 * */

$( document ).on( "pageinit", "[data-role='page'].swipe-page", function() {
	if( ! gl_isLogin())
		return;
	var page = "#" + $( this ).attr( "id" ),
	    // Get the filename of the next page that we stored in the data-next attribute
	    next = $( this ).jqmData( "next" ),
	    // Get the filename of the previous page that we stored in the data-prev attribute
	    prev = $( this ).jqmData( "prev" );
	// Check if we did set the data-next attribute
		//console.log("Next:" + next + ", prev:" + prev);
	if ( next ) {
	    // Prefetch the next page
	    //$.mobile.loadPage( next + ".html" );
	    // Navigate to next page on swipe left
	    $( document ).on( "swipeleft", page, function() {
	    	$.mobile.changePage( "#" + next, { transition: "slide" , reverse: true });
	    });
	    // Navigate to next page when the "next" button is clicked
	    $( ".control .next", page ).on( "click", function() {
	    	$.mobile.changePage( "#" + next, { transition: "slide" , reverse: true });
	    });
	}
	// Disable the "next" button if there is no next page
	else {
	    //$( ".control .next", page ).addClass( "ui-disabled" );
	}
	// The same for the previous page (we set data-dom-cache="true" so there is no need to prefetch)
	if ( prev ) {
	    $( document ).on( "swiperight", page, function() {
	        //$.mobile.changePage( prev + ".html", { transition: "slide", reverse: true } );
	        //console.log("#" + prev);
	        $.mobile.changePage( "#" + prev, { transition: "slide", reverse: true });
	        
	    });
	    $( ".control .prev", page ).on( "click", function() {
	        $.mobile.changePage( "#" + prev, { transition: "slide", reverse: true } );
	    });
	}
	else {
	    //$( ".control .prev", page ).addClass( "ui-disabled" );
	}
});


/*
 ************************************************************
 * Event access page quick use for #home, #jobs
 ************************************************************
 * */

//$(document).on('click', '.bar-home-jobs', function(e){
//	$( '.bar-home-jobs' ).removeClass("ui-btn-active ui-state-persist");
//	$(this).addClass("ui-btn-active ui-state-persist");
//});

/*
 **************************************
 * Popup API
 ************************************** 
 * */
function home_popupShow(msg, icon, callback){
	$.mobile.loading("hide");
	msg += '<img src="images/icons/warning.png">';
	$( "#home-popup" ).popup({ 
		history: false,
		afterclose: function( event, ui ) {
			if(callback) {
				callback();
			}
		}
		});
	$( "#home-popup-message" ).html(msg);
	$( "#home-popup" ).popup( "open" );
	
}

function home_popupHide(){
	$( "#home-pupup" ).popup("close");
}

