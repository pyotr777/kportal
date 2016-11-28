var loading_timer ;
function loading_show( to, callback ){
	
	$.mobile.loading("show");
	
	var timeout = 10000;
	
	if( to ){
		timeout = to;
	}
	
	loading_timer = setTimeout(function() {
		loading_hide();
		clearTimeout(loading_timer);
		if( callback ){
			callback ();
		}
	}, timeout);
}

function loading_hide(){
	if(loading_timer != undefined){}{
		clearTimeout(loading_timer);
	}
	$.mobile.loading("hide");
}
