//HISTORY
//2013/09/06    EH0001  longnm  Add api for client

var providerAppWaitListObj = new Object();
var service;

$(document).on('pageinit', '#provider-app-wait', function(){
    console.log("#provider-app-wait pageinit");

    //$("#provider-app-wait-list").listview().listview('refresh');
});

$(document).on("pageshow", "#provider-app-wait",function(event){
    console.log('#provider-app-wait: pagebeforeshow.');
    $.mobile.loading("show");
    load_CandidateOfService();
});

function load_CandidateOfService(){
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
           } else{
               console.log("service UNDEFINED");
               $.mobile.changePage("#provider-wait");
               $.mobile.loading("hide");
               return;
           }
        } else {
            $.mobile.changePage("#provider-wait");
            $.mobile.loading("hide");
            return;
        }
        
    }
    service = "";
    service = $.mobile.pageData.serviceid;
    ws_getCandidatesOfService(service,function( msg ){
    	providerAppWaitListObj.msg = msg;
    	load_appendCandidate();
    	$.mobile.loading("hide");
    });
}

function load_appendCandidate(){
    console.log("load_appendService");
    $("#provider-app-wait-signinText").text(localStorage.email);
    //add title
    var title ='';
    title = service;
    $("#title-provider-app-wait-list").html(title);

    /*
    //add tabs
    var users = '<a href="#provider-app-users?serviceid=' + service + '" class="ui-btn-active ui-state-persist">Users</a>';
    $("#provider-app-wait-link-tab-users").html(users);

    var wailist = '<a href="#provider-app-wait?serviceid=' + service + '">Wait List</a>';
    $("#provider-app-wait-link-tab-waitlist").html(wailist);
    */
    //add content
    $("#provider-app-wait-list").empty();
    var html ='';
    for( var i in providerAppWaitListObj.msg.candidates.items )
    {
    	html += '<li>';
        html +='<a href="#">' + providerAppWaitListObj.msg.candidates.items[i].user + '</a>';
        html +='<a onclick="removeCandidate(\''+ providerAppWaitListObj.msg.candidates.items[i].user
            	+ '\',\'' + service
	            +'\');" class="split-button-custom" data-role="button" data-icon="delete" data-theme= "e" data-iconpos="notext">1st link</a>';
	    html +='<a onclick="addUser(\''+ providerAppWaitListObj.msg.candidates.items[i].user
	            + '\',\'' + service
	            +'\');" class="split-button-custom" data-role="button" data-icon="check" data-iconpos="notext">2nd link</a>';
        html +='<a href="#" style="display: none;">Dummy</a>';
        html +='</li>';
    }
    console.log("Bp: html = " + html);
    $("#provider-app-wait-list").append(html);
    $("#provider-app-wait-list").listview('refresh');
    $("#provider-app-wait").trigger("create");
    
}

function removeCandidate(userId, serviceId) {
    console.log("removeCandidate");
    ws_removeCandidateOfService(serviceId,userId,function( msg ) {
       if (msg.valid === 1) {
           alert("remove candidate success");
       } else {
            alert("remove candidate fail");
       }
       load_CandidateOfService();
   });
   
}

function addUser(userId, serviceId) {
    console.log("addUser");
    ws_addUser(serviceId,userId,function( msg ) {
       if (msg.valid == 1) {
           alert("Add user success");
       } else {
            alert("Add user fail");
	   }
	   load_CandidateOfService();
    });
    
}

$(document).on('click', '#provider-app-wait-btn-login', function(e){

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
$(document).on('click', '#provider-app-wait-link-tab-users', function(e){
    console.log("click provider-app-wait-link-tab-waitlist");
    var url = document.URL;
    url.replace("provider-app-wait","provider-app-users");
    url += "?serviceid=" + service;
    window.location.replace(url);
});
*/
function click_provider_app_user_link() {
    console.log("click provider-app-wait-link-tab-waitlist");
    $.mobile.changePage("#provider-app-users?serviceid=" + service);
}
