//HISTORY
//2013/09/06    EH0001  longnm  Add api for client

var providerAppUserObj = new Object();
var service,user;

$(document).on('pageinit', '#provider-app-users', function(){
    console.log("#provider-app-wait pageinit");
});

$(document).on("pageshow", "#provider-app-users",function(event){
    console.log('#provider-app-users: pagebeforeshow.');



    $.mobile.loading("show");
    load_UserOfService();
});

function load_UserOfService(){
    console.log("get list service have candidate");
    if (!($.mobile.pageData) || (!$.mobile.pageData.serviceid)) {
        if (localStorage.getItem('long_param')) {
           service = localStorage.getItem('long_param');
           localStorage.removeItem('long_param');
           if (service) {
               if (!($.mobile.pageData)) {
                   $.mobile.pageData = new Object();
               }
               $.mobile.pageData.serviceid = service;
           } else {
               console.log("service UNDEFINED");
               $.mobile.changePage("#provider");
           }
        } else {
            $.mobile.changePage("#provider");
        }
        $.mobile.loading("hide");
        return;
    }
    service = "";
    service = $.mobile.pageData.serviceid;

    $.mobile.loading("show");
    ws_getUsersOfService(service,function( msg ){
        providerAppUserObj.msg = msg;
        load_appendUsersOfService(service);
        $.mobile.loading("hide");
    });
}


function load_appendUsersOfService(service){
    console.log("load_appendUserOfService");
    $("#provider-app-users-signinText").text(localStorage.email);
    //add title
    var title ='';
    title = service;
    $("#title-provider-app-list-users").html(title);

    /*
    //add tabs
    var users = '<a href="#provider-app-users?serviceid=' + service + '" class="ui-btn-active ui-state-persist">Users</a>';
    $("#provider-app-user-link-tab-users").html(users);

    var wailist = '<a href="#provider-app-wait?serviceid=' + service + '">Wait List</a>';
    $("#provider-app-user-link-tab-waitlist").html(wailist);
    */
    //add content
    $("#provider-app-list-users").empty();
    var html ='';
    for( var i in providerAppUserObj.msg.users.items )
    {
            html += '<li><a href="#">'
                    + providerAppUserObj.msg.users.items[i].user
            + '</a><a onclick="removeUser(\''+ providerAppUserObj.msg.users.items[i].user
            + '\',\'' + service + '\');\"' 
            + 'data-rel="popup" data-position-to="window" data-transition="pop">Delete</a></li>';
    }
    console.log("Bp: html = " + html);
    $.mobile.loading("hide");
    $("#provider-app-list-users").append(html);
    $('#provider-app-list-users').listview('refresh');
    $("#provider-app-users").trigger("create");
    
}

function removeUser(userId, serviceId) {
    console.log("remove user");
    ws_removeUserOfService(serviceId,userId,function( msg ) {
       if (msg.valid === 1) {
           alert("Remove user success");
       } else {
            alert("Remove user fail");
       }
       load_UserOfService();
   });
}

$(document).on('click', '#provider-app-users-btn-login', function(e){
	
	if($( "#provider-signinText" ).text() === "Login"){
		console.log("Bp: login now !");
		homeObj.currentPage = "#home";
		home_login( home_oa2LoginCallback );
	}else{
		console.log("Bp: logout now !");
		home_logout();
	}
});

/*
$(document).on('click', '#provider-app-wait-link-tab-waitlist', function(e){
    console.log("click provider-app-wait-link-tab-waitlist");
    var url = document.URL;
    url.replace("provider-app-users","provider-app-wait");
    url += "?serviceid=" + service;
    window.location.replace(url);
});
*/
function click_provider_app_wait_link() {
    console.log("click provider-app-wait-link-tab-waitlist");
    $.mobile.changePage("#provider-app-wait?serviceid=" + service);
}

window.onbeforeunload = function (e) {
	if( service ){
		localStorage.long_param = service;
	}
};
