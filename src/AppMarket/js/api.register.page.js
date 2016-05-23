
/*
* ******************************
* Define Global Variables
* ****************************** 
*/


/// Object to save submit job info
var registerObj = new Object();

/*
* ******************************
* Event handler
* ****************************** 
*/

$(document).on("pagebeforeshow", "#register", function(event){
	console.log('#register: pagebeforeshow.');
	/// Load email register
	$("#rf-email").val(localStorage.email);
});

$( document ).on( "pageshow", "#register", function(event) {
	console.log("#register: pageshow: pageData:");
	console.log($.mobile.pageData);
});

/*
$(document).ready(function(){
	/*
	*	Handle the reset form event.
	* /
	$( 'form#submit-form' ).bind( 'reset',function() {
		//alert("Bp: form reset.");
		if( submitInfo.params ){
			$.each( submitInfo.params, function( key, value ){
				//console.log ("Bp: reset form: key = " + key + ", value = ");
				//console.log ( value );
				if( value != undefined && value.file){
					submitInfo.params[ key ].file = undefined;
				}
			});
		}
	});
});
*/

$(document).on('submit', '#register-form', function( e ){
	document.getElementById("rf-submit").disabled = true;
	loading_show(60000, function(){
		document.getElementById("rf-submit").disabled = false;
		alert("Request timeout !");
		//register_fail(ERROR); /// Timeout
		return false;
	});
	
	/// Prevent default submit  
	e.preventDefault();
	
	/// Parse parameter submit
	dataString = $("#register-form").serialize();
	
	console.log($(this).serializeArray());
	var serArr = $(this).serializeArray();
	
	var valid = true;
	var regObj = new Object();
	$.each( serArr, function( index, value ) {
		//alert( index + ": " + value );
		var tagId = value.name;
		if( tagId != "" ){
			var msg, realVal;
			switch( tagId ){
			case "rf-email":
				regObj.email = value.value;
				break;
			case "rf-address":
				regObj.address = value.value;
				break;
			case "rf-descript":
				regObj.description = value.value;
				break;
			case "rf-phone":
				regObj.phone = value.value;
				break;
			case "rf-kdeskacc":
				regObj.kdeskacc = value.value;
				break;
			}
		}
	});
	
	/// Submit register
	ws_registerProvider( regObj, function (res) {
		document.getElementById("rf-submit").disabled = false;
		switch( res.result ){
		case ResponseCodes.DATA_SUCCESS:
			loading_hide();
			//register_success( res );
			alert("Register success !\nWhen admin approve this register, a mail will be to your email.");
			window.history.back();
			break;
		default:
			loading_hide();
			//register_fail( res );
			alert("Register is existed, please wait admin approve !");
			window.history.back();
			break;
		}
		return false;
	});

	/// Return false to prevent default submit  
	return false;
});

/*
  * ******************************
  * API function
  * ****************************** 
 */

function register_success( res ) {
	register_popupShow( "Register sent.", "check" );
}

function register_fail( error, msgRes ) {
	var msg = "Has been problem, register fail.";
	switch (error){
	case ERROR:
		msg = "Has been problem, register fail.";
		break;
	default:
		break;
	}
	loading_hide();
	register_popupShow( msg, "alert" );
}

/*
 **************************************
 * Popup API
 ************************************** 
 * */
function register_popupShow( msg, icon){
	if( ! icon ){
		icon = "check";
	}
	msg += '<img src="images/icons/'+ icon +'.png" height="32" width="32">';
	$( "#register-popup").popup({ positionTo: "window" });
	$( "#register-popup").html(msg).popup("option", "positionTo", "window");
	setTimeout(
		function() 
		{
			$( "#register-popup" ).popup( "open" );
		}, 1000);
}
 