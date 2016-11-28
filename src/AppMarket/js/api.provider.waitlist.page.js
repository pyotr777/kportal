//HISTORY
//2013/09/06    EH0001  longnm  Add api for client

var providerWaitListObj = new Object();

$(document).on('pageinit', '#provider-wait', function(){
    console.log("#provider-wait pageinit");
});

$(document).on("pageshow", "#provider-wait",function(event){
    console.log('#provider-wait: pagebeforeshow.');
    $("#provider-wait").trigger("create");
    $.mobile.loading("show");
    load_ServicesHaveCandidate();
    

});

function load_ServicesHaveCandidate(){
    console.log("get list service have candidate");
    ws_getServicesHaveCandidate(function( msg ){
        providerWaitListObj.msg = msg;
        $.mobile.loading("hide");
        $.mobile.loading("show");
        load_appendServicesHaveCandidate();
        $.mobile.loading("hide");
    });
}


function load_appendServicesHaveCandidate(service){
    console.log("load_appendService");
    $("#provider-wait-signinText").text(localStorage.email);
    var html ='';
    var i = 0;
    if (i < providerWaitListObj.msg.services.items.length)
        show_service_have_wait(i,html);
}

$(document).on('click', '#provider-wait-btn-login', function(e){
	
	if($( "#provider-signinText" ).text() === "Login"){
		console.log("Bp: login now !");
		homeObj.currentPage = "#home";
		home_login( home_oa2LoginCallback );
	}else{
		console.log("Bp: logout now !");
		home_logout();
	}
});

function show_service_have_wait(index,html){

    ws_getCandidatesOfService (providerWaitListObj.msg.services.items[index].service, function(msg) {
        var count = 0;
        for( var j in msg.candidates.items ) {
            count += 1;
        }
        console.log(providerWaitListObj.msg.services.items[index].service + ": " + count);
        html += '<li><a href="#provider-app-wait?serviceid='
                + providerWaitListObj.msg.services.items[index].service
                + '">'
                + providerWaitListObj.msg.services.items[index].service
                + '</a><span class="ui-li-count">' + count + '</span></li>';
        $("#provider-wait-list").html(html);
        $("#provider-wait-list").listview('refresh');
        if ((index+1) < providerWaitListObj.msg.services.items.length) {
            show_service_have_wait(index + 1, html);
        }
    });
}
