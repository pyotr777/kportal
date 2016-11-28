/*
*  ******************************************************************************************
* 								Define Global Variables
*  ****************************************************************************************** 
*/

var jobDetailObj = new Object();
jobDetailObj.resService = {};
jobDetailObj.resParams = {};
jobDetailObj.resGetJobInfos = {};
jobDetailObj.resGetOutputFile = {};
jobDetailObj.shortcutFiles = {};
jobDetailObj.interval;
jobDetailObj.currentStatus;
jobDetailObj.previousStatus;
/*
* ******************************************************************************************
* 									Event handler
* ******************************************************************************************
*/

$(document).on("pagebeforeshow", "#job-detail", function(e, data){
	if( sessionStorage.getItem("job-detail.pageData.id") != undefined
			&& sessionStorage.getItem("job-detail.pageData.service") != undefined ){
		if ( $.mobile.pageData == undefined ) {
			$.mobile.pageData = new Object();
		}
		if( $.mobile.pageData.service == undefined ){
			$.mobile.pageData.service = sessionStorage.getItem("job-detail.pageData.service");			
		} 
		if( $.mobile.pageData.id == undefined ) {
			$.mobile.pageData.id = sessionStorage.getItem("job-detail.pageData.id");
			detailObj.f5 = true;
		}
	}else{
		console.log("#job-detail: pagebeforeshow: job-detail.pageData.id & job-detail.pageData.service is UNDEFINED.")
	}
});

$(document).on("pageshow", "#job-detail", function(e, data){
	console.log("#job-detail: pageshow");
	
	/// Reset flag
	jobDetailObj.isRunningJob = false;
	jobDetailObj.statusChangeToFinished = false;
	
	loading_show();
	
	if( gl_isBackEvent() ){
		console.log("#job-detail: pageshow is BACK EVENT.");
		return;
	}
	if ( $.mobile.pageData && $.mobile.pageData.id &&  $.mobile.pageData.service){
		//detail_clearData();
	    console.log("Parameter id=" + $.mobile.pageData.id);
	    
	    /// Save job id
	    jobDetailObj.jobId = $.mobile.pageData.id;
	    jobDetailObj.serviceId = $.mobile.pageData.service;
	    
	    /// Save pageData for refresh page
	    sessionStorage.setItem("job-detail.pageData.id", $.mobile.pageData.id);
	    sessionStorage.setItem("job-detail.pageData.service", $.mobile.pageData.service);

	    if( jobDetailObj.resGetEntriesOfJob
	    	&& jobDetailObj.resGetEntriesOfJob[ jobDetailObj.jobId ] 
	    	&& jobDetailObj.resGetJobInfos[ jobDetailObj.jobId ]
	    	&& ( ! gl_isNeedReloadData( { "job" : jobDetailObj.jobId }) )
	    ) {
	    	jdetail_loadData();
	    	loading_hide();
	    	
	    	/// Set auto refresh status
	    	jdetail_setIntervalRefreshStatus ( )
	    }else{
	    	jdetail_retrieveData( function( result ) {
				//alert("af ter retrieve data " + result);
	    		if( result == 0 ){
		    		console.log("Bp: pageshow, #job-detail 03");
		    		jdetail_loadData();
		    		loading_hide();
		    		
			    	/// Set auto refresh status
			    	jdetail_setIntervalRefreshStatus ( )
	    		}else{
	    			jdetail_popupShow("Get <strong>" + jobDetailObj.jobId + "</strong> job from server has been problem.", "alert");
	    		}
	    	});
	    }
	}else{
		console.error("#detail: pageData is EMPTY.");
	}
	
});

$( document ).on( "pagehide", "#job-detail", function(event) {
	console.log("#job-detai: pagehide");
	jdetail_clearInterval();
});

$(document).on( "click", '.job-detail-btn', function(){
	console.log("Bp: .job-detail-btn: " + this.id + " is clicked. ");
	var action = this.id.replace("job-detail-btn-", "").toLowerCase();
	
	$.mobile.loading("show");
	
	switch( action ){
	case "cancel":
		console.log("Bp: .job-detail-btn: cancel job!");
		ws_cancelJob( jobId , function ( res ){
			status = gl_jobStatusToString ( res.status );
			jdetail_refreshJobInfo( function(){
				$.mobile.loading("hide");
				gl_jobChange(jobDetailObj.serviceId, jobId);
			});
		});
		break;
	case "re-submit":
		console.log("Bp: .job-detail-btn: re-submit job!");
		ws_restartJob( jobId , function ( res ){
			$.mobile.loading("hide");
			//if( res.job != 0 && res.valid == 1 ){
			if( res.job && res.result == ResponseCodes.DATA_SUCCESS ){
				jdetail_popupShow( "JobId is <strong><b>" + res.job + "</b></strong>, re-submit job success.", "check" );
				gl_jobNew( jobDetailObj.serviceId, res.job );
			} else {
				jdetail_popupShow( "Re-submit job fail.", "alert" );
			}
			/*status = gl_jobStatusToString ( res.status );
			jdetail_refreshJobInfo( function(){
				$.mobile.loading("hide");
				gl_jobChange(jobDetailObj.serviceId, jobId);

		    	/// Set auto refresh status
		    	jdetail_setIntervalRefreshStatus ( );
			});*/
		});
		break;
	case "delete":
		console.log("Bp: .job-detail-btn: cancel job!");
		ws_deleteJob( jobId , function ( res ){
			$.mobile.loading("hide");
			status = gl_jobStatusToString ( res.status );
			gl_jobChange(jobDetailObj.serviceId, jobId);
			history.back();
		});
		break;
	default:
		$.mobile.loading("hide");
		console.log("Error: .job-detail-btn onclick: \"" + action + "\" action UNKNOW.")
		break;
	}
});

$(document).on( "click", '.job-detail-explore', function(){
	console.log("Explore: entry = " + $(this).attr("data-entry"));
	
	var entry = $(this).attr("data-entry");
	var jobId = jobDetailObj.jobId;
	ws_getEntriesOfJob( jobId, entry, function( resEntries ){
		if( ! jobDetailObj.resGetEntriesOfJob ){
			jobDetailObj.resGetEntriesOfJob = {};
		}
		
		jdetail_loadDataEntriesInfo ( resEntries, entry );
		//jobDetailObj.resGetEntriesOfJob[ jobId ] = resEntries;
		$('#jdc-info').trigger('collapse');
		//$('#blockSecond').trigger('collapse');
		$("#job-detail").trigger('create');
		//$('#jdc-browse').collapsible('expand');
	});
});

$(document).on( "click", '.job-detail-download', function(){
	console.log("Download: entry = " + $(this).attr("data-entry"));
	$.mobile.loading("show");
	var entry = $(this).attr("data-entry");
	var jobId = jobDetailObj.jobId;
	if( entry && jobId ){
		if( jobDetailObj.resGetOutputFile[ jobId + "-" + entry ] ){
			console.log("Bp: .job-detail-download: \"" +  jobId + "-" + entry  + "\" is existed. Don't need load.");
			ufile_saveDownloadFile(entry, jobDetailObj.resGetOutputFile[ jobId + "-" + entry ]);
			loading_hide();
		}else{
			ws_getOutputFile( jobId, entry, function( res ){
				if( res.result == ResponseCodes.FILE_ACTION_SUCCESS ){
					//jobDetailObj.resGetOutputFile[ jobId + "-" + entry ] = filecontent; /// Save for other download
					ufile_saveDownloadFile(entry, res.filedata);
					res = {};
					loading_hide();
				}else{
					$.mobile.loading("hide");
					alert("Download file fail");
					console.error("Error: .job-detail-download :ws_getOutputFile(): \"" +  jobId + "-" + entry  + "\"  FAIL.");
				}
			});
		}

	}else{
		console.log("Error: .job-detail-download: entry || jobId is UNDEFINED.");
	}
});

/// Download shortcut with dependent files
/*
$(document).on( "click", '.job-detail-download-shortcut', function(){
	
	console.log("Download: entry shortcut = " + $(this).attr("data-entry"));
	
	//loading_show();
	
	var jobId = jobDetailObj.jobId;
	var mainEntry =  $(this).attr("data-entry");
	var popupId =  $(this).attr("data-popup");
	
	var resEntries = jobDetailObj.resGetEntriesOfJob[ jobId ];
	
	var gen_entriesHtml = function ( mainEntry, resEntries ){
		var entriesHtml;
		if( resEntries 
				&& resEntries.entries 
				&& resEntries.entries.items 
				&& resEntries.entries.items.length > 0 ){
			entriesHtml = '';
			$.each( resEntries.entries.items , function ( index, element ){
				if( mainEntry == element.entry || element.isDir){
					return;
				}
				var idname = 'jddf-entry-' + index;
				entriesHtml += '\
<input type="checkbox" name="' + idname + '" id="' + idname + '" value="' + element.entry + '">\
<label for="' + idname + '">' + element.entry + '</label>';
			});
		}
		return entriesHtml;
	};
	var popupHtml = '\
<div data-role="popup" id="jd-dependent-popup" data-overlay-theme="a" data-theme="c" data-dismissible="false" style="max-width:400px;" class="ui-corner-all">\
    <div data-role="header" data-theme="a" class="ui-corner-top">\
        <h2>Dependent files ?</h2>\
    </div>\
    <div data-role="content" data-theme="d" class="ui-corner-bottom ui-content">\
     <form class="jd-dependent-form" data-ajax="false" enctype="multipart/form-data">\
		<input type="hidden" name="jddf-job" value="' + jobId + '">\
     	<input type="hidden" name="jddf-main-entry" value="' + mainEntry + '">\
        <fieldset data-role="controlgroup" data-iconpos="right">' 
        + gen_entriesHtml(mainEntry, resEntries) +
        '</fieldset>\
	    <button type="submit" data-inline="true" data-theme="b" data-transition="flow">Done</button>\
        <a href="#" data-role="button" data-inline="true" data-rel="back" data-theme="c">Cancel</a>\
	</form>\
    </div>\
</div>\
	';
	//loading_hide();
	$.mobile.activePage.append( popupHtml ).trigger( "pagecreate" );
	
	/// Release after close popup
	$( "#jd-dependent-popup" ).bind({   
		popupafterclose: function(event, ui) {	$( this ).remove(); }
	});
	
	/// Show popup
	//$ ( "#jd-dependent-popup" ).popup("open");
	
	//$( "#" + popupId ).popup("close"); //TODO: fixbug firefox not show popup dependent
	$( "#" + popupId ).bind({   
		popupafterclose: function(event, ui) {	
			//$( this ).remove();
			$ ( "#jd-dependent-popup" ).popup("open");
			}
	});
	
});

$(document).on("submit", ".jd-dependent-form", function(e, data){
	console.log("Bp: jd-dependent-form Submit occur !");
	/// Prevent default submit  
	e.preventDefault();
	var serArr = $(this).serializeArray();
	var content = {};
	console.log(serArr);
	$.each( serArr, function( index, element ) {
		var tagId = element.name;
		if( tagId == "" || element.value == "" ){
			return; ///continue;
		}
		switch(tagId){
		case "jddf-job":
			content.job = element.value;
			return; ///continue;
		case "jddf-main-entry":
			content.path = element.value;
			return; ///continue;
		}
		
		if( tagId.indexOf("jddf-entry-") == 0 ){
			 if( ! content.dependents ){
				 content.dependents = [];
			 }
			 content.dependents.push(element.value);
		}
	});
	//console.log(content);
	$ ( "#jd-dependent-popup" ).popup("close");
	ufile_saveDownloadFile(content.path + ".shortcut", JSON.stringify ( content ));
	return true;
});
*/
$(document).on( "click", '.job-detail-download-shortcut', function(){
	console.log("Download: entry = " + $(this).attr("data-entry"));
	
	loading_show();
	
	var entry = $(this).attr("data-entry");
	var jobId = jobDetailObj.jobId;
	if( entry && jobId ){
		if( jobDetailObj.shortcutFiles[ jobId + "-" + entry ] ){
			
			console.log("Bp: .job-detail-download-shortcut: \"" +  jobId + "-" + entry  + "\" is existed. Don't need load.");
			
			ufile_saveDownloadFile(entry + ".shortcut", jobDetailObj.shortcutFiles[ jobId + "-" + entry ]);
		}else{
			var filecontent = jdetail_genShortcutFileContent( jobId, entry );
			
			console.log(filecontent);

			jobDetailObj.shortcutFiles[ jobId + "-" + entry ] = filecontent;
			
			ufile_saveDownloadFile(entry + ".shortcut", jobDetailObj.shortcutFiles[ jobId + "-" + entry ]);
			
		}
		loading_hide();
	}else{
		console.log("Error: .job-detail-download-shortcut: entry || jobId is UNDEFINED.");
	}
});

function jdetail_genShortcutFileContent( jobId, entry ){
	var filecontent = { "job" : jobId, "path" : entry };
	return JSON.stringify ( filecontent );
}

$(document).on('click', '#job-detail-refresh', function(e){
	loading_show();
	if ( jobDetailObj.jobId && jobDetailObj.serviceId ) {
		jdetail_retrieveDataJobInfo( jobId, jobDetailObj.serviceId, function(){
			jdetail_loadData();
			loading_hide();
		} );
	}
	else{
		console.error("Error: #job-detail-refresh: click: jobDetailObj.jobId || jobDetailObj.serviceId is UNDEFINED.");
	}
});

/*
 * *****************************************************************************************
 * 									API function
 * ****************************** **********************************************************
*/

function jdetail_refreshJobInfo( callback ){
	ws_getJobInfos( jobDetailObj.jobId, jobDetailObj.serviceId, function ( resJobInfo ){
		var jobId = jobDetailObj.jobId;
		
		jobDetailObj.resGetJobInfos[ jobId ] = resJobInfo;
		
		var status = '';
		status = gl_jobStatusToString(resJobInfo.status);
		statusDetail = jdetail_jobStatusToStringDetail( resJobInfo.status );
		
		if( resJobInfo.status ){
			status = gl_jobStatusToString(resJobInfo.status);
			
			console.log("Status = " + status);
			/// Save new & previous status
			jobDetailObj.previousStatus = jobDetailObj.currentStatus;
			jobDetailObj.currentStatus = status.toLowerCase();
		}
		
		var start = resJobInfo.start,
			end = resJobInfo.end,
			duration = resJobInfo.duration,
			runMethod = resJobInfo.runMethod;
		
		//$( '#job-detail-run-method' ).val( runMethod);
		$( '#job-detail-id' ).val( jobId );
		$( '#job-detail-status' ).val( statusDetail );
		$( '#job-detail-start' ).val( start );
		$( '#job-detail-duration' ).val( duration );
		
		var html = '';
		//alert("status " + status);
		if( status.toLowerCase() == "running" || status.toLowerCase() == "queue" ){
			/// Notify change status
			jobDetailObj.isRunningJob = true;
			
			/// Delete end date if exist
			$( '#job-detail-fieldcontain-end' ).remove();
			
			/// Suppend, Cancel button
			html += '<fieldset class="ui-grid-a">';
			//html += '	<div class="ui-block-a"><a data-role="button" class="job-detail-btn" id="job-detail-btn-suspend" data-icon="minus">Suppend</a></div>';
			html += '	<div class="ui-block-b"><a data-role="button" class="job-detail-btn" id="job-detail-btn-cancel" data-icon="delete">Cancel</a></div>';
			html += '</fieldset>';
			//html += '</li>';
			
		}else if( status.toLowerCase() == "initializing" ){
			html += '<fieldset class="ui-grid-a">';
			html += '	<div class="ui-block-b"><a data-role="button" class="job-detail-btn" id="job-detail-btn-delete" data-icon="delete">Delete</a></div>';
			html += '</fieldset>';
		} else {
			if( status.toLowerCase() == "cancelled" ){
				$( '#job-detail-fieldcontain-end' ).remove();
			}else{
				$( '#job-detail-end' ).val( end );
			}
			
			/// Delete / re-submit button
			html += '<fieldset class="ui-grid-a">';
			html += '	<div class="ui-block-a"><a data-role="button" class="job-detail-btn" id="job-detail-btn-re-submit"data-icon="refresh">Re-submit</a></div>';
			html += '	<div class="ui-block-b"><a data-role="button" class="job-detail-btn" id="job-detail-btn-delete" data-icon="delete">Delete</a></div>';
			html += '</fieldset>';
		}
		$("#job-detail-btn-container").html(html).trigger('create');
		$("#job-detail-list-info").listview("refresh");
		//$("#job-detail").trigger('create');
		
		if( callback ){
			callback();
		}
	});
}
function jdetail_retrieveData ( callback ){
	//console.log("Bp: jdetail_retrieveData ( callback ): 01");
	var jobId = jobDetailObj.jobId ;
	var serviceId = jobDetailObj.serviceId;
	
	if ( jobDetailObj.jobId && jobDetailObj.serviceId) {
		//console.log("Bp: jdetail_retrieveData ( callback ): 02");
		ws_getService(serviceId, function ( resService ){
			if( resService.result ){
				console.error("Error: ws_getRunMethodFile not finish.");
				return;
			}
			jobDetailObj.resService[ serviceId ] = resService;
			ws_getParameters( serviceId, function( resParams ){
				jobDetailObj.resParams[ serviceId ] = resParams;
				//jobDetailObj.resGetRunMethodFile[ serviceId ] = methodFile;
				jdetail_retrieveDataJobInfo( jobId, serviceId, callback );
			});
		});
		
	}else{
		console.error("Error: jdetail_retrieveData(): jobId is EMPTY.");
	}
}
/*
function jdetail_retrieveData ( callback ){
	console.log("Bp: jdetail_retrieveData ( callback ): 01");
	var jobId = jobDetailObj.jobId ;
	var serviceId = jobDetailObj.serviceId;
	
	if ( jobDetailObj.jobId && jobDetailObj.serviceId) {
		console.log("Bp: jdetail_retrieveData ( callback ): 02");
		ws_getRunMethodFile(serviceId, function ( methodFile, isSuccess ){
			if( ! isSuccess ){
				console.error("Error: ws_getRunMethodFile not finish.");
				return;
			}
			if( ! jobDetailObj.resGetRunMethodFile ){
				jobDetailObj.resGetRunMethodFile = {};
			}
			jobDetailObj.resGetRunMethodFile[ serviceId ] = methodFile;
			jdetail_retrieveDataJobInfo( jobId, serviceId, callback );
			ws_getJobInfos(jobId, serviceId, function ( resJobInfo ){
				if( ! resJobInfo.job || resJobInfo.job != jobId ){
					callback (-1);
					return; 
				}
				if( ! jobDetailObj.resGetJobInfos ){
					jobDetailObj.resGetJobInfos = {};
				}
				console.log("Bp: jdetail_retrieveData ( callback ): 03");
				jobDetailObj.resGetJobInfos[ jobId ] = resJobInfo;
				
				ws_getEntriesOfJob( jobId, function( resEntries ){
					console.log("Bp: jdetail_retrieveData ( callback ): 04");
					if( ! jobDetailObj.resGetEntriesOfJob ){
						jobDetailObj.resGetEntriesOfJob = {};
					}
					jobDetailObj.resGetEntriesOfJob[ jobId ] = resEntries;
					if( callback ){
						console.log("Bp: jdetail_retrieveData ( callback ): 05");
						callback (0);
					}
				});
				
	    	});
		});
		
	}else{
		console.error("Error: jdetail_retrieveData(): jobId is EMPTY.");
	}
}*/

function jdetail_retrieveDataJobInfo( jobId, serviceId, callback ){
	ws_getJobInfos(jobId, serviceId, function ( resJobInfo ){
		if( ! resJobInfo.job || resJobInfo.job != jobId ){
			callback (-1);
			return; 
		}
		if( ! jobDetailObj.resGetJobInfos ){
			jobDetailObj.resGetJobInfos = {};
		}
		jobDetailObj.resGetJobInfos[ jobId ] = resJobInfo;
		
		ws_getEntriesOfJob( jobId, "", function( resEntries ){
			if( ! jobDetailObj.resGetEntriesOfJob ){
				jobDetailObj.resGetEntriesOfJob = {};
			}
			jobDetailObj.resGetEntriesOfJob[ jobId ] = resEntries;
			if( callback ){
				callback (0);
			}
		});
		
	});
}


function jdetail_loadData(){
	
	jobId = jobDetailObj.jobId;
	
	var resJobInfo = jobDetailObj.resGetJobInfos[ jobId ];
	var resEntries = jobDetailObj.resGetEntriesOfJob[ jobId ];
	
	name = resJobInfo.jobName;
	name = (name != undefined && name != "") ? name : "(No name)";
	$("#job-detail-title").text( name );
	document.title = name;
	
	$("#job-detail-list-info").listview().empty();
		
	jdetail_loadDataProcessInfo ( resJobInfo );
	
	//jdetail_loadDataParamsInfo ( resJobInfo, jobDetailObj.resGetRunMethodFile[ jobDetailObj.serviceId ] );

	jdetail_loadDataParamsInfo ( resJobInfo,  jobDetailObj.resService[ jobDetailObj.serviceId ], jobDetailObj.resParams[ jobDetailObj.serviceId ] );
	
	$("#job-detail-list-info").listview("refresh");
	
	$("#job-detail-list-browse").listview().empty();
	
	$('.job-detail-download-popup').remove();
	
	jdetail_loadDataEntriesInfo ( resEntries );
	
	$("#job-detail-list-browse").listview("refresh");
	
	$("#job-detail").trigger('create');
}

function jdetail_jobStatusToStringDetail ( status ){
	var statusDetail = '';
	statusStr = gl_jobStatusToString(status);
	switch( status ){
	case 5:/// Can not start job
		statusDetail = "ERROR ( Can not start job )"
		break;
	case 6:/// Job info on shortcut file invalid
		statusDetail = "ERROR ( Job not found or no such job output file )";
		break;
	default:
		statusDetail = statusStr.toUpperCase();
		break;
	}
	return statusDetail;
}

function jdetail_loadDataProcessInfo( resJobInfo ){
	var jobId = resJobInfo.job;
	name = resJobInfo.jobName;
	status = gl_jobStatusToString(resJobInfo.status);
	statusDetail = jdetail_jobStatusToStringDetail( resJobInfo.status );
	maxElapsedTime = resJobInfo.maxElapsedTime;
	numberOfNodes = resJobInfo.numberOfNodes;
	start = resJobInfo.start;
	end = resJobInfo.end;
	duration = resJobInfo.duration;
	//runMethod = resJobInfo.runMethod;

	/// Save new & previous status
	jobDetailObj.previousStatus = jobDetailObj.currentStatus;
	jobDetailObj.currentStatus = status.toLowerCase();

	
	html = '';
	/// Divider process info
	html += '<li data-role="list-divider">Statistic</li>';
	html += '<li id="job-detail-process-info">';
	
	/// JobId
	html += '	<div data-role="fieldcontain">';
	html += '		<label for="job-detail-id">Id</label>';
	html += '		<input readonly type="text" name="job-detail-id" id="job-detail-id" value=" ' + jobId + ' ">';
	html += '	</div>';
/*
	/// JobName
	html += '	<div data-role="fieldcontain">';
	html += '		<label for="job-detail-name">Job name</label>';
	html += '		<input readonly type="text" name="job-detail-id" id="job-detail-name" value=" ' + ( name != "" ? name : "(No name)" ) + ' ">';
	html += '	</div>';*/
/*
	/// Max elapsed time
	html += '	<div data-role="fieldcontain">';
	html += '		<label for="job-detail-max-elapsed-time">Max elapsed time (s)</label>';
	html += '		<input readonly type="text" name="job-detail-max-elapsed-time" id="job-detail-max-elapsed-time" value=" ' + maxElapsedTime + ' ">';
	html += '	</div>';

	/// Number of nodes
	html += '	<div data-role="fieldcontain">';
	html += '		<label for="job-detail-number-of-nodes">Number of nodes</label>';
	html += '		<input readonly type="text" name="job-detail-number-of-nodes" id="job-detail-number-of-nodes" value=" ' + numberOfNodes + ' ">';
	html += '	</div>';
*/
	/*
	/// RunMethodId
	html += '	<div data-role="fieldcontain">';
	html += '		<label for="job-detail-run-method">Run method</label>';
	html += '		<input readonly type="text" name="job-detail-run-method" id="job-detail-run-method" value=" ' + runMethod + ' ">';
	html += '	</div>';
	*/
	/// Status
	html += '	<div data-role="fieldcontain">';
	html += '		<label for="job-detail-status">Status</label>';
	html += '		<input readonly type="text" name="job-detail-status" id="job-detail-status" value=" ' + statusDetail + ' ">';
	html += '	</div>';
	
	/// Duration
	html += '	<div data-role="fieldcontain">';
	html += '		<label for="job-detail-duration">Duration (s)</label>';
	html += '		<input readonly type="text" data-clear-btn="false" name="job-detail-duration" id="job-detail-duration" value="' + duration + '">';
	html += '	</div>';
	
	/// Start
	html += '	<div data-role="fieldcontain">';
	html += '		<label for="job-detail-start">Start</label>';
	html += '		<input readonly type="text" data-clear-btn="false" name="job-detail-start" id="job-detail-start" value="' + start + '">';
	html += '	</div>';
	
	if( status.toLowerCase() == "running" || status.toLowerCase() == "queue" ){
		
		html += '</li>'; // <!--/job-detail-process-info -->
		
		/// Suppend, Cancel button
		html += '<li id="job-detail-btn-container">';
		html += '<fieldset class="ui-grid-a">';
		//html += '	<div class="ui-block-a"><a data-role="button" class="job-detail-btn" id="job-detail-btn-suspend" data-icon="minus">Suppend</a></div>';
		html += '	<div class="ui-block-b"><a data-role="button" class="job-detail-btn" id="job-detail-btn-cancel" data-icon="delete">Cancel</a></div>';
		html += '</fieldset>';
		html += '</li>';
		
	} else if(status.toLowerCase() == "initializing") { 
		/// Delete / re-submit button
		html += '<li id="job-detail-btn-container">';
		html += '<fieldset class="ui-grid-a">';
		html += '	<div class="ui-block-b"><a data-role="button" class="job-detail-btn" id="job-detail-btn-delete" data-icon="delete">Delete</a></div>';
		html += '</fieldset>';
		html += '</li>';
	} else {
		if( status.toLowerCase() == "cancelled" || status.toLowerCase() == "error" ){
			html += '</li>'; // <!--/job-detail-process-info -->
		}else{
			/// End
			html += '	<div data-role="fieldcontain" id="job-detail-fieldcontain-end">';
			html += '		<label for="job-detail-end">End</label>';
			html += '		<input readonly type="text" data-clear-btn="false" name="job-detail-end" id="job-detail-end" value="' + end + '">';
			html += '	</div>';
	
			html += '</li>'; // <!--/job-detail-process-info -->
		}

		/// Check if job error deny re-submit
		var disabled = '';
//		if( status.toLowerCase() == "error" ){
//			disabled = ' disabled="disabled" ';
//		}
		
		/// Delete / re-submit button
		html += '<li id="job-detail-btn-container">';
		html += '<fieldset class="ui-grid-a">';
		html += '	<div class="ui-block-a"><a data-role="button" class="job-detail-btn" id="job-detail-btn-re-submit"data-icon="refresh" ' + disabled + '>Re-submit</a></div>';
		html += '	<div class="ui-block-b"><a data-role="button" class="job-detail-btn" id="job-detail-btn-delete" data-icon="delete">Delete</a></div>';
		html += '</fieldset>';
		html += '</li>';
	}
	
	$( "#job-detail-list-info" ).append( html ).listview( "refresh" );
}

function jdetail_loadDataParamsInfo ( resJobInfo, resService, resParams ){
	var serviceId = jobDetailObj.serviceId;
	var gen_dependentFilesStr = function ( paramId, dependences ){
		var strs = "", countMatch = 0;
		if( dependences ){
			
			$.each( dependences, function ( index, dependence) {
				if( dependence.paramId == paramId ){
					if( countMatch > 0 ){
						strs += ", ";
					}
					if ( dependence.job ){
						strs += dependence.job + "/" ;
					}
					strs += dependence.path;
					countMatch ++;
				}
			});
		} else {
			
		}
		return strs;
	};
	
	if( !( resService && resParams ) ){
		console.error("Error: resService || resParams is undefined");
		return;
	}
	var html = '';
	
	/// Divider Parameters info
	html += '<li data-role="list-divider">Parameters </li>';
	//$( "#job-detail-list-info" ).append( html );
	
	/// Parameters
	//$( "#job-detail-list-info" ).append ('<li>');
	html += '<li data-role="fieldcontain">';
	$.each( resParams.params.items, function (index){
		/// Label
		var label = this.name ? this.name : "(No name)";
		label += ' (' + this.type + ')';
		
		/// Required attribute
		var requiredAttr = '';
		if( this.isRequired ){
			label += ' *';
			requiredAttr = " required "; 
		}
		
		var inputTag = '', dependentTag = '';
		
		if( this.id == undefined ){
			console.error("Id is undefined");
			return;
		}
		var id = this.id;
		var labelTag = '';
		labelTag += '<label class="long-name" for="job-detail-param-' + id +'">' + label ;
		labelTag += '	<a href="#job-detail-' + id + '" data-rel="popup" data-role="button" class="ui-icon-alt" data-inline="true" data-transition="pop" data-icon="info" data-theme="c" data-iconshadow="false" data-iconpos="notext">Learn more</a>';
		labelTag += '</label>';
		
		var infoTag = '';
		infoTag += '<div data-role="popup" id="job-detail-'+ id +'" class="ui-content" data-theme="e" style="max-width:350px;">';
		infoTag += '	<p>' + ( this.description ? uString_escapeHtml( this.description ) : "(No description)" )+ '</p>';
		infoTag += '</div>';
		
		inputTag = jdetail_genInputTag( this.type, this.isOutput, id, resJobInfo.params );
		if(inputTag != ""){
			//inputTag = '<li><div data-role="fieldcontain">' + labelTag + inputTag + infoTag + '</div></li>';
			inputTag = '<div data-role="fieldcontain">' + labelTag + inputTag + infoTag + '</div>';
			//inputTag = '<div>' + labelTag + inputTag + infoTag + '</div>';
			//console.log("Bp: jdetail_loadDataParamsInfo ( resJobInfo ):inputTag = " + inputTag);
			//$( "#job-detail-list-info" ).append( inputTag );
			
			/// Dependent files
			if( resJobInfo.params.dependence ){
				var dependStrs = gen_dependentFilesStr( id, resJobInfo.params.dependence );
				if ( dependStrs.length > 0 ) {
					dependentTag = '\
<div data-role="fieldcontain">\
	<label for="job-detail-dependent-param-' + id + '">&nbspdependent files</label>\
	<input type="text" id="job-detail-dependent-param-' + id + '" value="' + dependStrs + '" readonly>\
</div>';
					inputTag += dependentTag;
				}
			}

		}else{
			console.log("jdetail_loadDataParamsInfo ( resJobInfo ): inputTag is EMPTY.");
		}
		html += inputTag;
	});
	html += '</li>';
	//$( "#job-detail-list-info" ).append ('</li>');
	$( "#job-detail-list-info" ).append( html );
	$( "#job-detail-list-info" ).trigger('create');

}



/*
function jdetail_loadDataParamsInfo2 ( resJobInfo ){
	
	serviceId = jobDetailObj.serviceId;
	runMethodId = resJobInfo.runMethod;
	console.log("Bp: jdetail_loadDataParamsInfo ( resJobInfo ): serviceId = " + serviceId + ", runMethodId = " + runMethodId );
	var html = '';
	
	/// Divider Parameters info
	html += '<li data-role="list-divider">Parameters </li>';
	$( "#job-detail-list-info" ).append( html );
	
	var methodFile = jobDetailObj.resGetRunMethodFile[ jobDetailObj.serviceId ];
	console.log("Bp: jdetail_loadDataParamsInfo ( resJobInfo ): xml = \n" + methodFile);
	/// Parse xml text
	var xmlDoc;
	try {
		xmlDoc = $.parseXML( methodFile );
	}catch ( err ){
		console.error( "Error: runMethodsFile parse have error. " + err.message);
		console.log(err);
		return;
	}
	
	var xml = $( xmlDoc );
	
	if( ! xml.children('Service') ) {
		console.error( "Error: jdetail_loadDataParamsInfo: xml : Service tag is missing." );
		return;
	}
	
	var matched = false;
	var $service = xml.children('Service'); 
	if( ! $service.children("RunMethods") ){
		console.error( "Error: jdetail_loadDataParamsInfo: xml : RunMethods tag is missing." );
		return;
	}
	var $runmethods = $ ( $service.children("RunMethods") );
	$runmethods.find( 'RunMethod' ).each( function(){
		var name;
		var $runmethod = $(this);

		var id = $runmethod.children( 'Id' ).text();
		
		if( id == runMethodId && $runmethod.children( 'Parameters' ) ){
			var $parameters = $runmethod.children( 'Parameters' );
			$parameters.find( 'Param' ).each( function(){
				var $param = $(this);
				
				if( $param.attr('paramName') != undefined || $param.attr('optionName') != undefined ){
					name = ( $param.attr('paramName') != undefined ) ? $param.attr('paramName') : $param.attr('optionName'); 
				}
				var isRequired = ( $param.attr('isRequired') != undefined ) && ( $param.attr('isRequired') === "true" );
				var isOutput = ( $param.attr('isOutput') != undefined ) && ( $param.attr('isOutput') === "true" )
				var id = $param.children('Id').text();
				var des = $param.children('Description').text();
				var type = $param.children('Type').text();
				
				console.log( "Bp: id = " + id + ", des = " + des + ", type = " + type + ", isOutput = "+ isOutput + ", isRequired = " + isRequired);
				
				var label = name;
				if( ! label ){
					label = des;
				}
				
				label = label ? label : "";  
				
				label += ' (' + type + ')';
				
				if( isRequired ){
					label += ' *';
				}
				
				var labelTag = '';
				labelTag += '<label for="job-detail-param-' + id +'">' + label ;
				labelTag += '	<a href="#job-detail-' + id + '" data-rel="popup" data-role="button" class="ui-icon-alt" data-inline="true" data-transition="pop" data-icon="info" data-theme="c" data-iconshadow="false" data-iconpos="notext">Learn more</a>';
				labelTag += '</label>';
				
				var infoTag = '';
				infoTag += '<div data-role="popup" id="job-detail-'+ id +'" class="ui-content" data-theme="e" style="max-width:350px;">';
				infoTag += '	<p>' + des + '</p>';
				infoTag += '</div>';
				
				inputTag = jdetail_genInputTag( type, isOutput, id, resJobInfo.params );
				
				if(inputTag != ""){
					inputTag = '<li><div data-role="fieldcontain">' + labelTag + inputTag + infoTag + '</div></li>';
					//console.log("Bp: jdetail_loadDataParamsInfo ( resJobInfo ):inputTag = " + inputTag);
					$( "#job-detail-list-info" ).append( inputTag );
				}else{
					console.log("jdetail_loadDataParamsInfo ( resJobInfo ): inputTag is EMPTY.");
				}
			});

			$("#job-detail-list-info").trigger('create');
			
			/// Break loop
			matched = true;
			return false;
		} else { 
			console.error( "Error: jdetail_loadDataParamsInfo: xml: Parameters tag is missing." );	
		}
	});
	
	if( ! matched ) { 
		console.error( "Error: jdetail_loadDataParamsInfo: xml: RunMethod " + runMethodId + "not found." );	
	}
}*/

function jdetail_genInputTag( type, isOutput, paramId, params ){
	var inputTag = '', id = paramId;
	var value, fromOtherjob = false;
	//console.log("jdetail_genInputTag(type, isOutput, paramId, params): " + type + ", " + isOutput + ", " + paramId + ", params");
	//console.log(params);
	
	if( params && params [ paramId ] ){
		value = params [ paramId ]
	}else{
		console.log("jdetail_genInputTag():params");
		console.log(params);
		console.log( "Error: jdetail_genInputTag( type, paramId, params) params is UNDEFINED." );
		return "";
	}
	switch( type ){
	
	case "bool":
		trueSelected = '';
		falseSelected = '';
		if( value == true ){
			trueSelected = "selected";
		}else{
			falseSelected = "selected";
		}
		inputTag += '<select readonly id="' + id +'" name="' + id +'" data-role="slider" data-theme="c" readonly>';
		inputTag += '	<option ' + trueSelected + ' value="true">True</option>';
		inputTag += '	<option ' + falseSelected + ' value="false">False</option>';
		inputTag += '</select>';
		break;
	case "byte":
		value = value ? value : '';
		inputTag += '<input readonly type="number" name="job-detail-param-' + id +'" id="job-detail-param-' + id +'" class="byte" value="' + value + '">';
		break;
	case "i16":
		inputTag += '<input readonly type="number" name="job-detail-param-' + id +'" id="job-detail-param-' + id +'" class="i16" value="' + value + '">';
		break;
	case "i32":
		inputTag += '<input readonly type="number" name="job-detail-param-' + id +'" id="job-detail-param-' + id +'" class="i32" value="' + value + '">';
		break;
	case "i64":
		inputTag += '<input readonly type="number" name="job-detail-param-' + id +'" id="job-detail-param-' + id +'" class="i64" value="' + value + '">';
		break;
	case "double":
		inputTag += '<input readonly type="number" name="job-detail-param-' + id +'" id="job-detail-param-' + id +'" class="d" value="' + value + '" >';
		break;
	case "string":
		inputTag += '<input readonly type="text" name="job-detail-param-' + id +'" id="job-detail-param-' + id +'" value="' + value + '">';
		break;
	case "file":
		var filename = "";
		if( isOutput ){
			filename = value;
		} else if( value.type == "local" ){
			filename = value.path;
		} else if( value.job ){
			filename = value.job + "/" + value.path;
		} else {
			console.error("Invalid parameter tag");
			break;
		}
		inputTag += '<input readonly type="text" name="job-detail-param-' + id +'" id="job-detail-param-' + id +'" value="' + filename + '">';
		/*filename = '';
		if( ! fromOtherjob ){
			if( isOutput ){
				filename = value;
			}else{
				filename = value.path;
			}
		}else{
			filename = value.path;
			if( value.job ){
				inputTag += '<input readonly type="text" name="job-detail-param-' + id +'" id="job-detail-param-' + id +'" value="' + value.job + '">';
			}
		}
		inputTag += '<input readonly type="text" name="job-detail-param-' + id +'" id="job-detail-param-' + id +'" value="' + filename + '">';*/
		break;
	default:
		console.log("Type " + type + " is unknow!");
		break;
	}
	return inputTag;
}

function jdetail_loadDataEntriesInfo ( resEntries, parentEntry ){
	if( (resEntries 
			&& resEntries.entries 
			&& resEntries.entries.items 
			&& resEntries.entries.items.length > 0)
			|| (resEntries.entries.items.length == 0 && parentEntry != undefined && parentEntry != "")){
		var html = '';
		var htmlmenu = '';
		
		$("#job-detail-list-browse").listview().empty();
		var level = 0;
		var grandEntry = "";
		if(parentEntry != undefined && parentEntry != ""){
			var subs = parentEntry.split("/"); 
			
			level = subs.length;
			if(subs.length >= 2){
				for(var i = 0; i < subs.length-1; i++){
					grandEntry = grandEntry + subs[i] + (i < subs.length - 2 ? "/" : "") ;
				}
			}
		}
		if(parentEntry != undefined  && parentEntry != ""){
			//alert("grandEntry " + grandEntry);
			var menuId = 'job-detail-download-'+ level;
			//html += '<li><a href="#" class="job-detail-download" data-entry="' + entry + '">';
			html += '<li><a href="#' + menuId +'" data-rel="popup" data-position-to="origin" data-transition="pop">';
			html += '	<img src="images/icons/folder.png">';
			html += '	<h2>..</h2>';
			html += '    <p><strong><i>Path: </i></strong>' + grandEntry + '</p></a>';
			//html += '    <a href="#" class="job-detail-download-shortcut" data-entry="' + entry + '">Download folder shortcut</a>';
			html += '</li>';
			if( ! $(menuId).length > 0 ){
				htmlmenu += jdetail_downloadMenu(menuId, grandEntry, true);
			}
		}
		$.each( resEntries.entries.items , function ( index ){
			var entry = this.entry,
				isDir = this.isDir;
			var menuId = 'job-detail-download-'+ level + '-' + index;
			
			//path  = (parentEntry != undefined ? (parentEntry + '/') : "") + entry;
			path = entry.replace("//", "\\");
			if( isDir ) {
				//html += '<li><a href="#" class="job-detail-download" data-entry="' + entry + '">';
				//html += '<li><a href="#job-detail-download-' + index + '" data-rel="popup" data-position-to="origin" data-transition="pop">';
				html += '<li><a href="#' + menuId + '" data-rel="popup" data-position-to="origin" data-transition="pop">';
				html += '	<img src="images/icons/folder.png">';
				html += '	<h2>' + entry + '</h2>';
				html += '    <p><strong><i>Path: </i></strong>' + path + '</p></a>';
				//html += '    <a href="#" class="job-detail-download-shortcut" data-entry="' + entry + '">Download folder shortcut</a>';
				html += '</li>';
			} else {
				//html += '<li><a href="#" class="job-detail-download" data-entry="' + entry + '">';
				//html += '<li><a href="#job-detail-download-' + index + '" data-rel="popup" data-position-to="origin" data-transition="pop">';
				html += '<li><a href="#' + menuId + '" data-rel="popup" data-position-to="origin" data-transition="pop">';
				icon = jdetail_fileIconByExtension(entry);
				filename = ufile_getFileName(entry); 
				html += '	<img src="images/icons/' + icon + '.png">';
				html += '	<h2>' + filename + '</h2>';
				html += '    <p><strong><i>Path: </i></strong>' + entry + '</p></a>';
				//html += '    <a href="#" class="job-detail-download-shortcut" data-entry="' + entry + '">Download file shortcut</a>';
				html += '</li>';
			}
			if( ! $(menuId).length > 0 ){
				htmlmenu += jdetail_downloadMenu(menuId, path, isDir);
			}
		});
		$( "#job-detail-list-browse" ).append( html ).listview( "refresh" );
		$( '#job-detail' ).append( htmlmenu );
	} else {
		$( "#job-detail-list-browse" ).append( '<center> No such file or directory.</center>' );
	}
}

function jdetail_downloadMenu( menuId, entry, isDir ){
	
	var html = '';
	html += '<div data-role="popup" id="' + menuId + '" class="job-detail-download-popup" data-theme="d">';
	html += '    <ul data-role="listview" data-inset="true" style="min-width:50px;" data-theme="d">';
	html += '        <li data-role="divider" data-theme="c">Download</li>';
	html += '        <li data-icon="false">';
	html += '        	<div data-role="controlgroup" data-type="horizontal">';
	html += '	            <a href="#" class="job-detail-' + (isDir == true ? 'explore' : 'download') +'" data-entry="' + entry + '" data-role="button" data-rel="back" >' + (isDir == true ? 'Explore' : 'File') + '</a>';
	html += '	            <a href="#" class="job-detail-download-shortcut" data-entry="' + entry + '" data-popup="'+ menuId +'" data-role="button" data-rel="back">Shortcut</a>';
	//html += '	            <a href="#" class="job-detail-download-shortcut" data-entry="' + entry + '" data-popup="job-detail-download-'+ index + '" data-role="button" data-rel="back">Shortcut</a>';
	html += '			</div>';
	html += '        </li>';
	html += '    </ul>';
	html += '</div>';
	return html;
}

function jdetail_fileIconByExtension ( entry ){
	//console.log("Bp: jdetail_fileIconByExtension ( entry ):" + entry);
	if ( ufile_isImageExtension ( entry )){
		return "image";
	}
	if ( ufile_isBinaryExtension ( entry )){
		return "binary";
	}
	if ( ufile_isXmlExtension ( entry )){
		return "xml";
	}
	if ( ufile_isTextExtension ( entry )){
		return "text";
	}
	
	/// Default icon
	return "file";
}

function jdetail_refreshJobInfoInterval(){
	console.log("Bp: jdetail_refreshJobInfoInterval: Auto refresh status !");
	jdetail_refreshJobInfo( function(){
		
		switch( jobDetailObj.currentStatus ){
		case "finished":
			jdetail_retrieveDataJobInfo( jobId, jobDetailObj.serviceId, function(){
				jdetail_loadData();
				
				/// Set notify
				gl_jobChange(jobDetailObj.serviceId, jobId);
				console.log("jdetail_refreshJobInfoInterval: jdetail_clearInterval finished ");
				///
				jdetail_clearInterval();
			});
			break;
		case "running":
			console.log("Continue: RUNNING");
			break;
		default:
			/// Set notify: status is change
			gl_jobChange(jobDetailObj.serviceId, jobId);
			console.log("jdetail_refreshJobInfoInterval: jdetail_clearInterval ");
			///
			jdetail_clearInterval();
			break;	
		}
	});
}

function jdetail_setIntervalRefreshStatus (){
	//console.log("Bp: jdetail_setIntervalRefreshStatus ( ): jobDetailObj.currentStatus = " + jobDetailObj.currentStatus  + " && jobDetailObj.previousStatus = " + jobDetailObj.previousStatus);
	if( jobDetailObj.currentStatus != "running" ){
		return;
	}
	if ( ! ( jobDetailObj.jobId && jobDetailObj.serviceId ) ){
		console.error("Error: #job-detail-refresh: click: jobDetailObj.jobId || jobDetailObj.serviceId is UNDEFINED.");
		return;
	}
	
	jobDetailObj.interval = setInterval(function(){
		jdetail_refreshJobInfoInterval();
	}, 3000);
};

function jdetail_clearInterval(){
	clearInterval(jobDetailObj.interval);
};


/*
 **************************************
 * Popup API
 ************************************** 
 * */
function jdetail_popupShow(msg, icon){
	if( ! icon ) {
		icon = "check";
	}
//	msg += '<a href="#" data-rel="popup" data-role="button" class="ui-icon-alt" data-inline="true" data-transition="pop" data-icon="' + icon + '" data-theme="e" data-iconpos="notext"></a>';
	/// Error: 
	msg += '<img src="images/icons/'+ icon +'.png" height="32" width="32">';
	$( "#jdetail-popup" ).popup();
	$( "#jdetail-popup" ).html(msg).popup();
	$( "#jdetail-popup" ).popup("open");
	
}
