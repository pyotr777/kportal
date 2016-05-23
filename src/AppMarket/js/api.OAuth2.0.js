var OAUTHURL    =   'https://accounts.google.com/o/oauth2/auth?';
var VALIDURL    =   'https://www.googleapis.com/oauth2/v1/tokeninfo?access_token=';
var SCOPE       =   'https://www.googleapis.com/auth/userinfo.profile https://www.googleapis.com/auth/userinfo.email';

/*
 * Test AppMarket_test
 * */
//
//var CLIENTID    =   '4624769292.apps.googleusercontent.com';
//
///// Secure redirect for LAN local
////var REDIRECT    =   'https://localhost/AppMarket_test/';
//
///// Secure redirect for LAN local
////var REDIRECT    =   'https://localhost/AppMarket_test/';
////var REDIRECT    =   'http://localhost:9006/AppMarket/';
//
///// Secure redirect for internet
//var REDIRECT    =   'https://bounceme.anlab.info:9005/AppMarket_test/';
////var REDIRECT    =   "https://appmarket.anlab.info/AppMarket_test/";
//
///// redirect for internet
////var REDIRECT    =   'http://bounceme.anlab.info:9006/AppMarket_test/';
//

/*
 * AppMarket
 * */
/*
var CLIENTID    =   '614195582225';
/// Secure redirect for LAN local
//var REDIRECT    =   'https://localhost/AppMarket/';
//var REDIRECT    =   'http://localhost:9006/AppMarket/';

/// Secure redirect for internet
var REDIRECT    =   'https://bounceme.anlab.info:9005/AppMarket/';
//var REDIRECT    =   "https://appmarket.anlab.info:9005/AppMarket/";

/// redirect for internet
//var REDIRECT    =   'http://bounceme.anlab.info:9006/AppMarket/';
*/

/*
 * Read from config file 
 * */
CLIENTID = config.oauth20_clientid;
REDIRECT = config.oauth20_redirect;

/*
 * Public
 * */

var LOGOUT      =   'https://accounts.google.com/Logout';
var TYPE        =   'token';
var _url        =   OAUTHURL + 
					'scope=' + SCOPE + 
					'&client_id=' + CLIENTID + 
					'&redirect_uri=' + REDIRECT + 
				    '&response_type=' + TYPE;
var acToken;
var tokenType;
var expiresIn;
var user;
var oa2_loginCallback;
var flag = 0;

function oa2_logout(callback){
	//var logoutWin         =   window.open(LOGOUT, '_blank');
	//var logoutWin         =   window.open(LOGOUT, '_newtab');
//	
//	var logoutPollTimer   =   window.setInterval(function() {
////	    if( logoutWin.closed ){
////    		//alert("Window closed.");
////    		window.clearInterval( logoutPollTimer );
////    		//window.location = '';
////    		return;
////    	}
//	    
//		window.clearInterval(logoutPollTimer);
//		logoutWin.close();
//		
		callback();
//	}, 1500);
//	console.log( "Bp: oa2_logout(callback): '$( #home_iframe )");
//	console.log($( "#home_iframe" ));
//	$( "iframe#home_iframe" ).location = 'https://www.google.com/accounts/Logout';
//	console.log($( "iframe#home_iframe" ).location);
//	callback();
//	return false;
//	
//	$.ajax({
//		url: LOGOUT,
//		data: null,
//		success: function(resp) {
//			console.log("Bp: oa2_logout(callback): response ");
//			console.log(resp);
//			callback( resp );
//		},
//		dataType: "jsonp"
//	});
}

//
//function oa2_logout( callback ){
//	//$( "#home_iframe" ).location = 'https://www.google.com/accounts/Logout'; 
//	//$( "#home_iframe" ).attr('src', LOGOUT);
//	document.getElementById('home_iframe').src='https://www.google.com/accounts/Logout';
//	if( callback ){
//		callback();	
//	}else{
//		alert("Bp:  oa2_logout( callback ) is NULL.");
//	}
//	return false;
//}

function oa2_loginContinue(url){
	acToken =   gup(url, 'access_token');
    tokenType = gup(url, 'token_type');
    expiresIn = gup(url, 'expires_in');
    console.log("Bp: oa2_loginCallback(url): " + acToken + ", " + tokenType + ", " + expiresIn);
    validateToken(acToken, function (user){
    	var callbackName = localStorage.getItem( "appmarket.oa2_login.callback" );
    	localStorage.removeItem( "appmarket.oa2_login.callback" );
    	console.log("Bp: oa2_loginCallback(url): callbackName = " + callbackName);
    	if( callbackName ){
    		window[callbackName](user);
    	}else{
			console.log("Error: oa2_loginContinue(url) callbackName is not exist.");
		}
    });
}

function oa2_login(callback) {
	
	localStorage.setItem( "appmarket.oa2_login.callback", callback.name );
	window.document.location = _url;
}

    
function validateToken(token, callback) {
	
    $.ajax({
        url: VALIDURL + token,
        data: null,
        success: function(responseText){  
             getUserInfo(callback);
        },
        dataType: "jsonp"
    });
}

function getUserInfo( callback ) {
	$.ajax({
		url: 'https://www.googleapis.com/oauth2/v1/userinfo?access_token=' + acToken,
		data: null,
		success: function(resp) {
			user	=	resp;
			console.log("Bp: getUserInfo(): user = ")
			console.log(user);
			/*$('#uName').append(user.name);
			$('#imgHolder').attr('src', user.picture);*/
			//return user;
			//alert("Bp: getUserInfo: acToken=" + acToken);
			
			user.access_token = acToken;
			callback( user );
			},
			dataType: "jsonp"
	});
}

function tokenExpire(){
	$.ajax({
		url: 'https://www.googleapis.com/oauth2/v1/tokeninfo?access_token=' + acToken,
		data: null,
		success: function(resp) {
			user	=	resp;
			console.log(user);
			expiresIn = user.expires_in;
			$('#expire').empty();
			$('#expire').append(user.expires_in);
		},
		dataType: "jsonp"
	});
}

function gup(url, name) {
    name = name.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
    var regexS = "[\\#&]"+name+"=([^&#]*)";
    var regex = new RegExp( regexS );
    var results = regex.exec( url );
    if( results == null )
    	return "";
    else
        return results[1];
}