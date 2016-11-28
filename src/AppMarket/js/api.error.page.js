var errorObj = new Object();
errorObj.error = false;

$(document).on("pageshow", "#error", function(e, data){
	/*if( ! errorObj.error ){
		$.changePage( "#home" );
	}*/
	if( errorObj.title || errorObj.message ){
		sessionStorage.setItem("errorObj", JSON.stringify(errorObj));
	}else{
		errorObj = JSON.parse( sessionStorage.getItem("errorObj") );
		if( errorObj ){
			document.getElementById("error_title").innerHTML = errorObj.title;
			document.getElementById("error_messages").innerHTML = errorObj.message+ "<br />";
			$("#error").trigger("createpage");
		} else {
			
		}
	}
});

$(document).on("click", "#error-refresh-btn", function(e, data){
	$.mobile.loading("show");
	ws_connect( error_connectCallback );
});

function error_connectCallback(){
	$.mobile.changePage("#home",{
		reloadPage:true
	});
}

function error_showPage(title, message) {
	//alert("error_showPage");
	$.mobile.loading("hide");
	errorObj.title = title;
	errorObj.message = message;
	document.getElementById("error_title").innerHTML = title;
	document.getElementById("error_messages").innerHTML = message+ "<br />";
	errorObj.error = true;
	$.mobile.changePage("#error");
}
