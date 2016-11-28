function popupShow(title, message){
	$( "#popupBasic" ).popup();
	$( "#popupBasic-title" ).text( title );
	$( "#popupBasic-message" ).text( message );
	$( "#popupBasic" ).popup( "open" );
}

function popupHide(){
	$( "#popupBasic-title" ).text( "" );
	$( "#popupBasic-message" ).text( "" );
	$( "#popupBasic" ).popup( "close" );
}
