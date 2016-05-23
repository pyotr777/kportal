/*
*  ******************************************************************************************
* 								Define Global Variables
*  ****************************************************************************************** 
*/

var jobsObj = new Object();
jobsObj.interval;
jobsObj.jobId;
jobsObj.serviceId;
jobsObj.hasRunningJob = false;
jobsObj.loaded = { "#jobs" : false, "#jobs-finished" : false };

/*
* ******************************************************************************************
* 									Event handler
* ******************************************************************************************
*/

$(document).on("pageshow", "#jobs", function(e, data){
	console.log("#jobs: pageshow");
	
	/// Reload active button
	$( '.bar-home-jobs' ).removeClass("ui-btn-active ui-state-persist");
	$( "#jobs-bar-btn-jobs" ).addClass("ui-btn-active ui-state-persist");
	
	/// Reload active button navbar
	$( '.jobs-tab-btn' ).removeClass("ui-btn-active ui-state-persist");
	$( "#jobs-tab-btn-running" ).addClass("ui-btn-active ui-state-persist");
	
	
	
	/// Reset flag;
	jobsObj.hasRunningJob = false;
	
	
	loading_show();
	
	if( jobsObj.resFindServices && ( ! gl_isNeedReloadData() ) ){
		if(  jobsObj.loaded ["#jobs"] 
				&& ( gl_isBackEvent() || gl_isChangeTabEvent() ) ){
			console.log("#jobs: pageshow: Is back page event do not need reload data.");
			loading_hide();
			return;
		}
		jobs_tabMenu( "#jobs" );
		
		console.log("Bp: pageshow, #jobs dont need reload data.");
		jobs_loadRunningJobs();
		loading_hide();
		
		/// Set flag to ignore reload data
		jobsObj.loaded ["#jobs"] = true;
		
		/// Set interval auto refresh
		jobs_setIntervalRefreshStatus();
	}else{
		
		jobs_retrieveData( function () {
			jobs_tabMenu( "#jobs" );
			
			jobs_loadRunningJobs();
			loading_hide();

			/// Set flag to ignore reload data
			jobsObj.loaded ["#jobs"] = true;
			
			/// Set interval auto refresh
			jobs_setIntervalRefreshStatus();
		});
	}
});


$(document).on("pageshow", "#jobs-finished", function(e, data){
	console.log("#jobs-finished: pageshow");
	
	/// Reload active button
	$( '.bar-home-jobs' ).removeClass("ui-btn-active ui-state-persist");
	
	/// Reload active button navbar
	$( '.jobs-tab-btn' ).removeClass("ui-btn-active ui-state-persist");
	$( "#jobs-tab-btn-finished" ).addClass("ui-btn-active ui-state-persist");
	
	
	loading_show();
	
	if( jobsObj.resFindServices && ( ! gl_isNeedReloadData() ) ){
		if( jobsObj.loaded ["#jobs-finished"] 
				&& ( gl_isBackEvent() || gl_isChangeTabEvent() ) ){
			loading_hide();
			console.log("#jobs-finished: pageshow: Is back page event do not need reload data.");
			return;
		}

		jobs_tabMenu( "#jobs-finished" );
		
		console.log("Bp: pageshow, #jobs-finished dont need reload data.");
		jobs_loadFinishedJobs();

		/// Set flag to ignore reload data
		jobsObj.loaded ["#jobs-finished"] = true;

		loading_hide();
	}else{

		jobs_tabMenu( "#jobs-finished" );
		
		jobs_retrieveData( function () {
			jobs_loadFinishedJobs();
			
			/// Set flag to ignore reload data
			jobsObj.loaded ["#jobs-finished"] = true;

			loading_hide();
			
		});
	}
});

$( document ).on( "pagehide", "#jobs", function(event) {
	console.log("pagehide");
	jobs_clearInterval();
});

//$(document).on( "expand", '.jobs-collapsible-job', function(){
//	  //alert('Expanded: id = ' + $(this).attr("id"));
//	  service = $(this).attr("id").replace("jobs-", "");
//	  alert('Expanded: idFull = ' + $(this).attr("id") + ", id = " + service);
//	  if( jobsObj 
//			&& jobsObj.resGetJobsByService  
//			  && jobsObj.resGetJobsByService[ service ]	
//	  			&& ( ! gl_isNeedReloadData( service ) ) ){
//		  jobs_loadJobsOfService( service, jobsObj.resGetJobsByService[ service ]);
//	  }else{
//		  ws_getJobsByService(service, function( resJobs ) {
//			  jobsObj.resGetJobsByService[ service ] = resJobs;
//			  jobs_loadJobsOfService( service, resJobs);
//		  });
//	  }
//});

$(document).on( "collapse", '.jobs-collapsible-job', function(){
	  //alert('Collapsed');
});

$(document).on( "click", '.jobs-btn-refresh', function(){
	loading_show();
	
	var tab = $(this).attr("data-tab");
	
	jobs_retrieveData( function () {
		if(tab == "jobs"){
			jobs_loadRunningJobs();
		}else{
			jobs_loadFinishedJobs();
		}
		loading_hide();
	});
});

/*
 * ******************************************************************************************
 * 									API function
 * ******************************************************************************************
*/

function jobs_clearData(){
	$( '#jobs-services-list' ).html('');
	$( '#jobs-message' ).html('');
	//$( '#jobs' ).trigger("create");
	$( '#jobs' ).trigger("pagecreate");
	
}

function jobs_loadFormJobs( onlyRunningJob ){
	console.log("Bp:  jobs_loadRunningJobs: jobsObj = ");
	
	console.log(jobsObj);
	
	var resFindServices = jobsObj.resFindServices ;
	var page = '#jobs';
	if( ! onlyRunningJob ){
		page = "#jobs-finished";
	}
	var resGetJobsByService = jobsObj.resGetJobsByService;
	$( page + '-message' ).html('');
	var pageId = page.replace("#", "");
	if(resFindServices && resFindServices.services && resFindServices.services.items && resFindServices.services.items.length > 0 ){
		
		$( page + '-services-list' ).html('');
		
		$( page + '-services-list' ).collapsibleset();
		if( resFindServices.services.items.length == 0 ){
			$( page + '-message' ).html('No such services.');
		}
		
		$.each( resFindServices.services.items, function(index) {
			var service = this.service;
			
			html = '';
			html += '<div id="' + pageId + '-' + service + '" class="' + pageId + '-collapsible-job" data-role="collapsible">';
			html += '	<h2 id="' + pageId + '-service-title-' + service + '"> ' + this.name + '</h2>';
			html += '	<ul id="' + pageId + '-job-list-' + service + '" data-role="listview">';
			html += '	</ul>';
			html += '</div>';
			
			$( page + '-services-list' ).append(html).collapsibleset( "refresh" );
			var listTagId = page + '-job-list-' + service;
			//alert( 'id=' + '"' + page + '-job-list-' + service + '" ');
			
			var count = jobs_loadJobsOfService( page, listTagId, service, resGetJobsByService[ service ] );
			//alert("list insert count:" + page + '-service-title-' + service );
			$( page + '-service-title-' + service + ' .ui-btn-text').append( '<i> (' + count + ') </i>').trigger("refresh");
			//$.mobile.activePage.find( listTagId + ' .ui-btn-text').append( '<i> (' + count + ') </i>').trigger("refresh");
			//$( listTagId + ' .ui-btn-text').append( '<i> (' + count + ') </i>').trigger("refresh");
			$( page + '-services-list' ).collapsibleset();
			
		});
		$( page ).trigger('pagecreate');
	}else{
		$( page + '-message' ).html('No such service.');
		$( page ).trigger("pagecreate");
		console.log("Error: jobs_loadServices(resFindServices) resFindServices is UNDEFINED");
		console.log(resFindServices);
	}
}


function jobs_loadFinishedJobs(){
	jobs_loadFormJobs( false );
}

function jobs_loadRunningJobs(){
	jobs_loadFormJobs( true );
//	console.log("Bp:  jobs_loadRunningJobs: jobsObj = ");
//	
//	console.log(jobsObj);
//	
//	var resFindServices = jobsObj.resFindServices ;
//	var resGetJobsByService = jobsObj.resGetJobsByService
//	$( '#jobs-message' ).html('');
//	if(resFindServices && resFindServices.services && resFindServices.services.items && resFindServices.services.items.length > 0 ){
//		$( '#jobs-services-list' ).html('');
//		$( '#jobs-services-list' ).collapsibleset();
//		if( resFindServices.services.items.length == 0 ){
//			$( '#jobs-message' ).html('No such services.');
//		}
//		$.each( resFindServices.services.items, function(index) {
//			var service = this.service;
//			html = '';
//			html += '<div id="jobs-' + service + '" class="jobs-collapsible-job" data-role="collapsible">';
//			html += '	<h2 id="jobs-service-title-' + service + '"> ' + service + '</h2>';
//			html += '	<ul id="jobs-job-list-' + service + '" data-role="listview">';
//			html += '	</ul>';
//			html += '</div>';
//			$( '#jobs-services-list' ).append(html).collapsibleset( "refresh" );
//			$( '#jobs' ).trigger("create");
//			var count = jobs_loadJobsOfService( service, resGetJobsByService[service] );
//			$( '#jobs-service-title-' + service + ' .ui-btn-text').append( '<i> (' + count + ') </i>').trigger("refresh");
//			$( '#jobs-services-list' ).collapsibleset();
//		});
//		$( '#jobs' ).trigger("create");
//	}else{
//		$( '#jobs-message' ).html('No such service.');
//		$( '#jobs' ).trigger("create");
//		console.log("Error: jobs_loadServices(resFindServices) resFindServices is UNDEFINED");
//		console.log(resFindServices);
//	}
}

function jobs_retrieveData( callback ){
	ws_findServices( function ( items ) {
		if( ! jobsObj.resFindServices ){
			jobsObj.resFindServices = new Object();
		}
		if( ! jobsObj.resFindServices.services ){
			jobsObj.resFindServices.services = new Object();
		}
		jobsObj.resFindServices.services.items = items;
		
		if( jobsObj.resFindServices.services.items.length > 0){
			jobs_retrieveDataRecursion( jobsObj.resFindServices.services.items, 0, callback );
		}else{
			if(callback){
				callback();
			}
		}
	});
}

function jobs_retrieveDataRecursion( services, index, callback ){
	if (index >=  jobsObj.resFindServices.services.items.length ){
		if( callback ) {
			callback();
		}
		return;
	}
	var service = services[ index ].service;
	ws_getJobsByService( service, function( resJobs ){
		if( resJobs.jobs && resJobs.jobs.items ){
			/// Save response message
			if( ! jobsObj.resGetJobsByService ){
				jobsObj.resGetJobsByService = {};
			}
			jobsObj.resGetJobsByService[ service ] = resJobs;
			
			jobs_retrieveDataRecursion( jobsObj.resFindServices.services.items, index + 1, callback );
		}else{
			console.error("Error: jobs_retrieveDataRecursion() response  resJobs.jobs.items tag is missing.");
		}
	});
}

//function jobs_loadServices(resFindServices){
//	console.log("Bp:  jobs_loadServices(resFindServices)");
//	console.log(resFindServices);
//	$( '#jobs-message' ).html('');
//	if(resFindServices && resFindServices.services && resFindServices.services.items && resFindServices.services.items.length > 0 ){
//		$( '#jobs-services-list' ).html('');
//		$( '#jobs-services-list' ).collapsibleset();
//		if( resFindServices.services.items.length == 0 ){
//			$( '#jobs-message' ).html('No such services.');
//		}
//		$.each( resFindServices.services.items, function(index) {
//			var service = this.service;
//			html = '';
//			html += '<div id="jobs-' + service + '" class="jobs-collapsible-job" data-role="collapsible">';
//			html += '	<h2 id="jobs-service-title-' + service + '"> ' + service + '</h2>';
//			html += '	<ul id="jobs-job-list-' + service + '" data-role="listview">';
//			html += '	</ul>';
//			html += '</div>';
//			$( '#jobs-services-list' ).append(html).collapsibleset( "refresh" );
//		});
//		$( '#jobs' ).trigger("create");
//	}else{
//		$( '#jobs-message' ).html('No such service.');
//		$( '#jobs' ).trigger("create");
//		console.log("Error: jobs_loadServices(resFindServices) resFindServices is UNDEFINED");
//		console.log(resFindServices);
//	}
//}

function jobs_loadJobsOfService( page, listTagId, serviceId, resJobs ){
	var count = 0, runningCount = 0;
	var listTag  = $.mobile.activePage.find( listTagId );
	if( ! listTag ){
		alert("listTagID = " + listTagId + " not found");
		return 0;
	}
	listTag.listview();
	//console.log("Bp: jobs_loadJobsOfService( page, serviceId, resJobs ): " + page + " , " + serviceId + ", resJobs = " );
	//console.log( resJobs );
	if( resJobs && resJobs.jobs && resJobs.jobs.items && resJobs.jobs.items.length > 0 ){
		//$( page + '-job-list-' + serviceId  ).listview();
		
		var html;
		/*if( resJobs.jobs.items.length == 0 ){
			$( page + '-job-list-' + serviceId  ).html('<center>No such jobs </center>');
			return;
		}*/
		
		$.each( resJobs.jobs.items, function (index){
			id = this.job;
			name = this.jobName;
			name = (name != undefined && name != "") ? name : "(No name)";
			status = '';
			if( this.status ){
				status = gl_jobStatusToString(this.status);
				
				if( status.toLowerCase() == "running" ||
						status.toLowerCase() == "queue" ) {
					/// Save notify for interval
					jobsObj.hasRunningJob = true;
					jobsObj.jobId = id;
					jobsObj.serviceId = serviceId;
					runningCount ++;
					if( page == "#jobs-finished" ){
						if( count == 0 && index == resJobs.jobs.items.length - 1 ){
							//$( page + '-job-list-' + serviceId  ).html('<center>No such jobs </center>');
							listTag.html('<center>No such jobs </center>');
							return false;
						} else {
							return;
						}
					}
				}else{
					count ++;
					if( page == "#jobs" ){
						if( runningCount == 0 && index == resJobs.jobs.items.length - 1 ){
							//$( page + '-job-list-' + serviceId  ).html('<center>No such jobs </center>');
							listTag.html('<center>No such jobs </center>');
							return false;
						} else {
							return;
						}
					}
				}
			} else {
				console.error("jobs_loadJobsOfService: Unknow status !!");
				return;
			}
			
			start = this.start;
			end = this.end;
			duration = this.duration;
			html = '';
			html += '<li>';
			html += '	<a href="#job-detail?id=' + id + '&service=' + serviceId + '">';
			html += '    	<h2>' + name + '</h2>';
			html += '    	<p>Start: &nbsp;&nbsp; <strong>' + start + '</strong></p>';
			html += '    	<p class="ui-li-aside">Duration&nbsp;&nbsp;<strong>' + duration + '</strong></p>';
			html += '    </a>';
			html += '    <span class="ui-li-count">' + status + '</span>';
			html += '</li>';
			//console.log(" count = " + count );
			//alert( "append to : " + page + '-job-list-' + serviceId );
			//$( page + '-job-list-' + serviceId ).append( html ).listview( "refresh" );
			listTag.append( html ).listview( "refresh" );
		});

		//$( page ).trigger("pagecreate");

	} else {
		//$( page + '-job-list-' + serviceId  ).empty();
		//$( page + '-job-list-' + serviceId  ).html('<center>No such jobs </center>');
		listTag.empty();
		listTag.html('<center>No such jobs </center>');
		//$( page ).trigger("pagecreate");
	}
	//alert("Bp: jobs_loadJobsOfService 01");
	if( page == "#jobs" ){
		return runningCount;		
	}
	return count;
}

function jobs_tabMenu( page ){
	
	var runActive = '',	finishActive = '', detailsActive = '', runhref = '', finishhref = '', runid = '', finishid = '';
	switch ( page ){
	case "#jobs":
		runid = ' id="jobs-tab-btn-running" ';
		
		finishhref = ' href="#jobs-finished" ';
		
		runActive = ' class="ui-btn-active ui-state-persist" ';
		break;
	case "#jobs-finished":
		finishid = ' id="jobs-tab-btn-finished" ';
		
		runhref = ' href="#jobs" ';
		
		finishActive = 'class="ui-btn-active ui-state-persist"';
		break;
	default:
		console.log("Error: " + page + " page not exist.");
		break;
	}
	
	var html = '';
	html += '<div data-role="navbar">';
	html += '<ul>';
	html += '   <li><a ' + runid + runhref + runActive + ' >Running</a></li>';
	html += '   <li><a ' + finishid + finishhref + finishActive + '>Finished</a></li>';
	html += '</ul>';
	html += '</div>';

	header = page + ' [data-role="header"]';
	navbar = page + ' [data-role="navbar"]';

	
	if( $( header ) && $( header ).html() ){
		if( $( navbar ) && $( navbar ).html() ){
			$( navbar ).replaceWith( html );
			console.log("Bp: jobs_tabMenu( id , page ) REPLACE");
		}else{
			console.log("Bp: jobs_tabMenu( id , page ) APPEND");
			$( header ).append( html );
		}
		$( '[data-role="navbar"]' ).navbar();
	}
	else{
		console.log("jobs_tabMenu: Not found." + header);
	}
	$( page ).trigger("pagecreate");
}

function jobs_setIntervalRefreshStatus ( ){
	if( ! jobsObj.hasRunningJob ){
		return;
	}
	jobsObj.interval = setInterval(function(){
		jobs_retrieveData( function () {
			jobsObj.hasRunningJob = false;

			jobs_loadRunningJobs();
			
			if( ! jobsObj.hasRunningJob ){
				jobs_clearInterval();
				
				gl_jobChange( jobsObj.jobId, jobsObj.serviceId );
			}
		});
	}, 10000);
};

function jobs_clearInterval(){
	clearInterval(jobsObj.interval);
};
