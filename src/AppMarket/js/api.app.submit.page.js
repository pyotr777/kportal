
/*
* ******************************
* Define Global Variables
* ****************************** 
*/


/// Object to save submit job info
var submitInfo = new Object();

/// List runmethods of service
var runMethods = {}; /// If use [] is array normal ( index, value ), {} is map array (key, value)


var asubmitObj = new Object();
asubmitObj.loaded = false;
/*
* ******************************
* Event handler
* ****************************** 
*/

$(document).on("pagebeforeshow", "#app-submit", function(event){
	
	console.log('#app-submit: pagebeforeshow.');
	console.log("pageData:" + $.mobile.pageData);
	//app_pageBeforeShow();
	appObj.isChangeTab = gl_isChangeTabEvent();
	appObj.isBackPage = gl_isBackEvent();
	appObj.isOtherService = app_isOtherService();
	if( appObj.isOtherService 
			|| ( ( ! appObj.isBackPage ) && ( ! appObj.isChangeTab ) )){
		submit_cleanData();
		submit_clearCacheData();
	}
	
	if( ! ($.mobile.pageData && $.mobile.pageData.id ) ){
		//alert("restore");
		uStorage_restoreData();
	}
	
	//if( $.mobile.pageData && $.mobile.pageData.id ){
//		$.mobile.loading("show");
//
//		appObj.serviceId = $.mobile.pageData.id;
//		
//	    console.log("Parameter id=" + $.mobile.pageData.id);
//		if( !  sessionStorage.getItem( $.mobile.pageData.id ) ){
//			app_retrieveData( appObj.serviceId, function (){
//				console.log("Bp: $.mobile.pageData.id = " + $.mobile.pageData.id);
//		    	submit_loadData( $.mobile.pageData.id, sessionStorage.getItem( $.mobile.pageData.id ) );	// origin 
//				submitInfo.serviceId = $.mobile.pageData.id;
//			} );
//	     	//console.log("Error: Service description file not found.");
//	     	
//	    }else{
//	    	console.log("Bp: $.mobile.pageData.id = " + $.mobile.pageData.id);
//	    	submit_loadData( $.mobile.pageData.id, sessionStorage.getItem( $.mobile.pageData.id ) );	// origin 
//			submitInfo.serviceId = $.mobile.pageData.id;
//	    }
//		$.mobile.loading("hide");
//	}else{
//		console.log("Error: pagebeforeshow: Id paramerter is EMPTY.");
//	}
});

//$(document).on("pagebeforeshow", "#app-submit", function(event){
/// Note: use beforepageshow event can't load app_tabMenu() => need use pageshow event 
$( document ).on( "pageshow", "#app-submit", function(event) {
	console.log("#app: pageshow: pageData:");
	console.log($.mobile.pageData);
	$.mobile.loading("show");
	
	if( appObj.serviceId 
			&& appObj.resGetService
			&& ( ! gl_isNeedReloadData() ) ){
		if( ( appObj.isBackPage || appObj.isChangeTab ) 
				&& asubmitObj.loaded 
				&& ( ! appObj.isOtherService ) ){
		//if( ( appObj.isBackPage || appObj.isChangeTab ) && asubmitObj.loaded ){
			console.log("#app-submit: pageshow: Is back page event do not need reload data.");
			document.title = $('#submit-title').text();
			appObj.changeTab = false;
			appObj.isChangeTab = false;
			$.mobile.loading("hide");
			return;
		}

		app_tabMenu($.mobile.pageData.id, "#app-submit" );
		
		console.log("Bp: pageshow, #app dont need reload data.");
		submit_loadData( appObj.serviceId , appObj.resGetService[ appObj.serviceId ] );
		asubmitObj.loaded = true;
		$.mobile.loading("hide");
	}else{

		app_tabMenu( appObj.serviceId, "#app-submit" );
		
		app_retrieveData( appObj.serviceId, function ( result ) {
			if( result == 0){
				/// Fill data to form
				submit_loadData( appObj.serviceId , appObj.resGetService[ appObj.serviceId ] );
				asubmitObj.loaded = true;
				$.mobile.loading("hide");
			}else{
				console.error("Error: #app pageshow: app_retrieveData has problem. result = " + result);;
				$.mobile.loading("hide");
			}
		});
	}
});
/*
$( document ).on( "pageshow", "#app-submit2", function(event) {
	console.log("#app: pageshow: pageData:");
	console.log($.mobile.pageData);
	$.mobile.loading("show");
	setTimeout(function() { 
		$.mobile.loading("hide");
	}, 15000);
	
	if( appObj.serviceId 
			&& appObj.resFindServices 
			&& appObj.resRunMethodFile[ appObj.serviceId  ]
			&& ( ! gl_isNeedReloadData() ) ){
		if( ( appObj.isBackPage || appObj.isChangeTab ) 
				&& asubmitObj.loaded 
				&& ( ! appObj.isOtherService ) ){
		//if( ( appObj.isBackPage || appObj.isChangeTab ) && asubmitObj.loaded ){
			console.log("#app-submit: pageshow: Is back page event do not need reload data.");
			document.title = $('#submit-title').text();
			appObj.changeTab = false;
			appObj.isChangeTab = false;
			loading_hide();
			return;
		}

		app_tabMenu($.mobile.pageData.id, "#app-submit" );
		
		console.log("Bp: pageshow, #app dont need reload data.");
		submit_loadData( appObj.serviceId , appObj.resRunMethodFile[ appObj.serviceId ] );
		asubmitObj.loaded = true;
		loading_hide();
	}else{

		app_tabMenu($.mobile.pageData.id, "#app-submit" );
		
		app_retrieveData( appObj.serviceId, function ( result ) {
			if( result == 0){
				/// Fill data to form
				submit_loadData( appObj.serviceId , appObj.resRunMethodFile[ appObj.serviceId ] );
				asubmitObj.loaded = true;
				loading_hide();
			}else{
				console.error("Error: #app pageshow: app_retrieveData has problem. result = " + result);;
			}
		});
	}	
});*/

$(document).ready(function(){
	/*
	*	Handle change file select.
	*/
//	$( 'input[type="file"]' ).bind( "change", function() { 
//	//$( 'input[type="file"]' ).change(function(){
//		alert("File change");
//		
//		/*/// Read file
//		var reader = new FileReader();
//		reader.onloadend = function() {
//			
//			/// Just for debug
//			blobBinaryString = reader.result;
//			console.log("Byte:");
//		    console.log(blobBinaryString);
//
//			/// Save data
//			var paramId = $(this).attr( "id" );
//			if( ! submitInfo.params )
//				submitInfo.params = new Object();
//			submitInfo.params[ paramId ] = reader.result;
//			console.log("submitInfo.params[ paramId ]" + submitInfo.params[ paramId ]);
//		};*/
//		
//		var paramId = $(this).attr( "id" );
//	    var fileObj = this.files[0];
//	    /// < 100M ~ 104857600 bit
//	    if( fileObj.size > 104857600 ){
//	    	alert('Error: the file size is more than 100 M.');
//	    	return;
//	    }
	
//	    if( ! submitInfo.params[ parmId ] ){
//	    	submitInfo.params[ parmId ] = new Object();
//	    }
//	    submitInfo.params[ parmId ].file = fileObj;
//	    console.log("File name: " + submitInfo.params[ parmId ].file.name + ", size: " + submitInfo.params[ parmId ].file.size);
//	    
//	    console.log("Bp: " + paramId + " = "  + $(this).val());
	    
	    //reader.readAsArrayBuffer(fileName);
	    //reader.readAsDataURL(fileName);
	    //reader.readAsBinaryString(fileObj);	     
//	});
	
	/*
	*	Handle the method select change event.
	*/
	$( "#submit-cmbx-method" ).bind( "change", function(event, ui) { 
		submit_genForm( runMethods[ $(this).val()]);	
	});
	
	/*
	*	Handle the reset form event.
	*/
	$( 'form#submit-form' ).bind( 'reset',function() {
		//alert("Bp: form reset.");
		if( submitInfo.params ){
			$.each( submitInfo.params, function( key, value ){
				console.log ("Bp: reset form: key = " + key + ", value = ");
				console.log ( value );
				if( value != undefined && value.file){
					submitInfo.params[ key ].file = undefined;
				}
			});
		}
	});
});

//
//function escapeHtml(text) {
//	  return text
//	      .replace(/&/g, "&amp;")
//	      .replace(/</g, "&lt;")
//	      .replace(/>/g, "&gt;")
//	      .replace(/"/g, "&quot;")
//	      .replace(/'/g, "&#039;");	
//}

//$(document).on( "change", 'input[ type="text" ]', function(){
//	alert("Text change");
//	console.log("Bp: text change: this.val() = ");
//	console.log(this.value);
//	
//	txt = this.value;
//	//txtspecial = escapeHtml(txt);
//	txtspecial = escape(txt);
//	
//	console.log("Bp; textchange: txt vs txtspceial " + txt + ", " + txtspecial);
//	this.value = txtspecial;
//} );

$(document).on('submit', '#submit-form', function( e ){
	$.mobile.loading("show");
	
	console.log("Bp: Submit job occur !");
	/// Prevent default submit  
	e.preventDefault();
	
	var job = {};
	job.params = {};
	job.serviceId = appObj.serviceId;
	
	submitInfo.serviceId = appObj.serviceId;
	
	/// Parse parameter submit
	dataString = $("#submit-form").serialize();
	console.log( "Bp: onSubmit: " + dataString );
	
	console.log($(this).serializeArray());
	var serArr = $(this).serializeArray();
	var resParams = appObj.resGetParameters[ appObj.serviceId ];
	
	var valid = true;
	$.each( serArr, function( index, value ) {
		//alert( index + ": " + value.name + " = " + value.value );
		var tagId = value.name;
		if( tagId != "" ){
			var msg, realVal;
			if( tagId.indexOf("sjf-param-") < 0 ){
				switch( tagId ){
				case "sjf-jobName":
					job.jobName = value.value;
					break;
				case "sjf-maxElapsedTime":
					realVal = parseInt( value.value );
					//job.maxElapsedTime = parseInt( value.value );
					if( realVal > 0 ){
						job.maxElapsedTime = realVal; 
					} else {
						msg = "Minimum greater than zer";
					}
					break;
				case "sjf-numberOfNodes":
					//job.numberOfNodes = parseInt( value.value );
					realVal = parseInt( value.value );
					if( realVal > 0 ){
						job.numberOfNodes = realVal; 
					} else {
						msg = "Minimum greater than zero";
					}
					break;
				}
				if( msg && msg.length > 0 ){
					document.getElementById(tagId).setCustomValidity(msg); 
					//$( "#" + tagId ).setCustomValidity( msg );
					$( "#" + tagId ).focus();
					valid = false;
					$.mobile.loading("hide");
					return false;
				}
				
			} else {
				var paramId = tagId.substring(10);
				console.log("aSubmit_getParamType( resParams, paramId );" + paramId);
				var paramType = aSubmit_getParamType( resParams, paramId ); 
				if( paramType == undefined || paramType == "") {
					$.mobile.loading("hide");
					return false;
				}
				//alert("paramId = " + paramId + ", paramType = " + paramType + ", value.value = " + value.value);
				var paramOutput = {};
				valid = uParam_checkValid( tagId, paramId, paramType, value.value, paramOutput);
				//uParam_checkValid( tagId, paramId, paramType, paramVal ){
				if( ! valid ){
					//alert("invalid");
					$.mobile.loading("hide");
					return false;
				} else {
					if( paramOutput.value && paramOutput.value != "" ){
						job.params [paramId] = paramOutput.value;
					} else {
						console.log("is empty string");
					}
				}
			}
		}
	});
	
	if( ! valid ){
		alert("Invalid parameter");
		return false;
	}
	
	/// Check file parameter
	$.each(resParams.params.items , function( index, value ){
		if( value.type === "file" && value.isOutput != true ){
			var fileTag = document.getElementById( "sjf-param-" + value.id );
			if( fileTag && fileTag.files.length > 0 ){
				job.params [ value.id ] = {};
				job.params [ value.id ].type = value.type;
				job.params [ value.id ].value = fileTag.files;
				
				/*
				/// Get dependent files
				if( dependTag && dependTag.files.length > 0 ){
					if( ! job.params.dependence ){
						job.params.dependence = {};
					}
					job.params.dependence[ value.id ] = dependTag.files;
				} else {
					console.log("Parameter no dependent files");
				}*/
			} else {
				console.log("Parameter file tag not found");
				//alert("Parameter file tag not found");
			}
			
		}
	});
	/// Get list file need send.
	var fileParams = new Array();
	$.each( job.params, function(key, value) {
		if( value.type == "file"/* && value.isOutput != true*/ ){
			//fileParams [ fileParams.length ] = value;
			
			fileParams.push( value.value[0] );
			/*
			if( job.params.dependence && job.params.dependence[ key ] &&  job.params.dependence[ key ].length > 0 ){
				///fileParams = fileParams.concat( job.params.dependence[ key ] );
				/// Filter duplicate main file
				$.each( job.params.dependence[ key ], function ( index, value ){
					var isExist = false;
					$.each( fileParams, function( index, file ){
						if( file.name == value.name ){
							isExist = true;
							return false;
						}
					});
					if( isExist ){
						return;
					} else {
						fileParams.push(value);
					}
				});
			}*/
		}
	});
	
	/// Submit job
	ws_submitJob( job, function (res) {
		aSubmit_processSubmitResponse(fileParams, res, function(res){
			switch( res.result ){
			case ResponseCodes.DATA_SUCCESS:
				$.mobile.loading("hide");
				submit_success( res );
				break;
			case ResponseCodes.ERROR_TIMEQUOTA_OVER:
				$.mobile.loading("hide");
				submit_fail( -1, res );
			default:
				$.mobile.loading("hide");
				submit_fail( -1, res );
				break;
		}
		return false;
		});
	});

	/// Return false to prevent default submit  
	return false;
});

function aSubmit_processSubmitResponse(files, prevRes, callback){
	if( prevRes.uploadFile != undefined){
		$.each(files, function(index){
			if(this.name == prevRes.uploadFile){
				ws_sendFileParamPart(prevRes.requestId, prevRes.param, this, 0, function(sent, total){
					/// Notify uploading
				}, function(res){
					aSubmit_processSubmitResponse(files, res, callback);
				});
			}
		});
	} else {
		callback(prevRes);
	}
}

function aSubmit_getParamType( resParams, paramId ){
	var type;
	$.each(resParams.params.items , function( index, value ){
		if( value.id === paramId ){
			type = value.type;
			return true;
		}
	});
	return type;
}

function uParam_checkValid( tagId, paramId, paramType, paramVal, paramOutput ){
	var realValue, msg;
	try{
		switch( paramType ){
		case "bool":
			if( paramVal == "true" ){
				realValue = true;
			}else{
				realValue = false;
			}
			break;
		case "byte":
			realValue = parseInt( paramVal );
			if( realValue < -128 || realValue > 127){
				//alert("Overload value of BYTE type.");
				msg = "Overload value of BYTE type.";
				realValue = undefined;
			}
			break;
		case "i16":
			realValue = parseInt( paramVal );
			if( realValue < -32768 || realValue > 32767){
				//alert("Overload value of I16 type.");
				msg = "Overload value of I16 type.";
				realValue = undefined;
			}
			break;
		case "i32":
			realValue = parseInt( paramVal );
			if( realValue < -2147483648 || realValue > 2147483647){
				//alert("Overload value of I32 type.");
				msg = "Overload value of I32 type.";
				realValue = undefined;
			}
			break;
		case "i64":
			realValue = parseInt( paramVal );
			if( realValue < -9223372036854775808 || realValue > 9223372036854775807){
				//alert("Overload value of I64 type.");
				msg = "Overload value of I16 type.";
				realValue = undefined;
			}
			break;
		case "double":
			realValue = parseFloat( paramVal );
			//realValue = parseDouble( paramVal );
			break;
		/// Note: Data from file select elements is not serialized. 
		/// so file have in serialized is ouput file.
		case "file":
		case "string":
			realValue = paramVal;
			break;
		default:
			console.log('Error: uParam_checkValid: ' + paramType +' is unknow param type.');
			break;
		}
		
	} catch( e ) {
		console.log("Exception:");
		console.log(e);
		//alert( "Exception: Parse value fail." );
		msg = "Invalid value.";
		return false;
	}
	if( msg ){
		//alert("invalid: #" + tagId + msg );
		document.getElementById( tagId ).setCustomValidity( msg );
		//$( "#" + tagId ).setCustomValidity( msg );
		$( "#" + tagId ).focus();
		return false;
	} else {
		paramOutput.type = paramType;
		paramOutput.value = realValue;
		return true;
	}
	//params[ id ].value = realValue;
	//params[ id ].type = type;
	
	//console.log("Bp: uParam_checkValid: submitInfo.params ");
	//console.log(submitInfo.params);
}
/*
$(document).on('submit', '#submit-form2', function( e ){
	/// Prevent default submit  
	e.preventDefault();
	
	//alert("BP: submit.");
	console.log("Bp: Submit job occur !");
	
	/// Set serviceId.
	submitInfo.serviceId = appObj.serviceId;
	
	/// Parse parameter submit
	dataString = $("#submit-form").serialize();
	console.log( "Bp: onSubmit: " + dataString );
	console.log($(this).serializeArray());
	
	var serArr = $(this).serializeArray();
	var runMethodId;
	for ( var i in serArr ){
		if( serArr[ i ].name ==  "runMethodId" ){
			runMethodId = serArr[ i ].value;
			break;
		}
	}
	console.log ( "Bp: onSubmit: runMethodId=" + runMethodId );
	
	/// RunMethodId info
	submitInfo.runMethodId = runMethodId;
	
	runMethod = runMethods[ runMethodId ];
	console.log("Bp: onSubmit: runMethod=");
	console.log(runMethod);
		
	/// Check valid file input
//	if( ! submit_checkValidFileInput( ) ){
//		
//	}
	
	//// Parse list of params
	//var params = {};
	if( ! submitInfo.params ) {
		submitInfo.params = {}; 
	}
	
	if( ! submit_checkValidParam( runMethod, serArr, submitInfo.params) ) {
		console.log("Error: onSubmit: Have invalid parameter.");
		//alert("submit_checkValidParam: submit false.");
		return false;
	}
	console.log("Bp: submit_checkValidParam: onSubmit: submitInfo.params = ");
	console.log(submitInfo.params);
	
	/// Submit job
	ws_submitJob( submitInfo, function (res) {
		
		if( res < 0){
			/// Submit job error
			submit_fail(res);
		}else if( res.job ) {
			if( res.job == 0 || res.valid != 1){
				submit_fail( res );
			}else{
				submit_success( res );
			}
		}
		else if( res.valid ){
			
			var fileParams = new Array();
			
			/// Get list file need send.
			$.each( submitInfo.params, function(key, value) {
				
				if( value.type == "file" && value.isOutput != true ){
					fileParams [ fileParams.length ] = value;
				}
			});
			
			/// Send file 
			var i = 0;
			submit_sendFileRecursive(res.requestId, fileParams, i, submit_sendFileRecursive);
			
		}else{
			submit_fail( res );
			//alert( "Error: onSubmit: response unknow." );
		}
		return false;
	});

	/// Return false to prevent default submit  
	return false;
});
*/
$(document).on('click', '#app-submit-btn-refresh', function(e){
	console.log( "Bp: refresh onclick." );
	if( appObj.serviceId ){
		$.mobile.loading("show");
		ws_getService( appObj.serviceId, function(methodFile, isSuccess){
			if( isSuccess ){
				appObj.resRunMethodFile [appObj.serviceId] = methodFile;
				submit_loadData( appObj.serviceId , appObj.resRunMethodFile[ appObj.serviceId ] );
				asubmitObj.loaded = true;
			}
			$.mobile.loading("hide");
		});
	}
});
/*
  * ******************************
  * API function
  * ****************************** 
 */

function submit_sendFileRecursive(requestId, fileParams, index, callback){
	if( fileParams.length == 0  ){
		console.log("Bp: submit_readFileCallback(fileParams, index, callback): fileParams is EMPTY.");
		loading_hide();
		return;
	}
	if( fileParams.length == index ){
		console.log("Bp: submit_sendFileRecursive: send finish.");
		ws_sendFileFinish( requestId, 1, function ( res ){
			loading_hide();
			switch( res.result ){
			case ResponseCodes.DATA_SUCCESS:
				submit_success( res );
				break;
			case ResponseCodes.ERROR_FX10_RETURN:
				submit_fail( ERR_SERVER_RESPONSE, 'Submit job error: ' + uResponseCode_toMessage(res.result) );
				break;
			default:
				submit_fail( res );
				break;
			}
			return;
		});
		return;
	}
	
	//if( fileParams[index] ){
		//if( fileParams[ index ].value[0] || fileParams[ index ].value[0].name ){
			//fileInfo = fileParams[ index ].value[0];
	/*console.log("fileParams[index]");
	console.log(fileParams[index])*/;
	if( fileParams[index] && fileParams[index].name){
		fileInfo = fileParams[index];
			ws_sendFileName(requestId, fileInfo.name);
			
			var reader = new FileReader();
			reader.onloadend = function() {
				if( reader.error ){
					gl_setError( ERR_READ_FILE, reader.error );
					submit_fail(ERR_READ_FILE);
					return;
				}
				binaryString = reader.result;
				/*console.log("Byte:");
			    console.log(binaryString);*/
			    
			    ws_sendFileData(requestId, binaryString);
			    callback(requestId, fileParams, index + 1, submit_sendFileRecursive)
			    
			};
			
			reader.readAsArrayBuffer(fileInfo);
			//reader.readAsDataURL(fileInfo);
			//reader.readAsBinaryString(fileInfo);
	} else {
		alert("Error: onSubmit: fileInfo is UNDEFINED.");
		submit_fail(ERROR);
		return;
	}
	//}
}

function submit_success( res ) {
	if( res.job && res.result == ResponseCodes.DATA_SUCCESS ){
		submit_popupShow( "Job <strong><b>" + res.job + "</b></strong>, is pushed to processing queue", "check" );
		gl_jobNew( appObj.serviceId, res.job );
	} else {
		console.error( "Error: submit jobId unknow.");
	}
}

function submit_fail( error, msgRes ) {
	var msg = "Has been problem, submit job fail.";
	
	switch (error){
	case ERROR:
		msg = "Has been problem, submit job fail.";
		break;
	case ERR_READ_FILE:
		msg = "File input not found or can not open, submit job fail.";
		break;
	case ERR_INVALID_SHORTCUT_FILE_FORMAT:
		msg = "Shortcut file format is invalid, submit job fail.";
		break;
	case ERR_SERVER_RESPONSE:
		msg = msgRes;
		break;
	default:
		switch(msgRes.result){
		case ResponseCodes.ERR_TIMEQUOTA_OVER:
			msg = "Max elapse time is greater than availabel time quota of the provider.\n";
			break;
		}
		break;	
	}
	loading_hide();
	submit_popupShow( msg, "alert" );
}

function submit_loadData( id, service ){
	
	//console.log( "BP: submit_loadData( id, service ): id = " + service.service + ", service = " + service );
	
	/// Clean data before load
	submit_clearCacheData();
	
	/// Title
	var title = service.name ? service.name : "(No name)";
	document.title = title;
	$( "#submit-title" ).text(title);
	//$( "#app-submit" ).trigger('create');
	
	//var id = service.service;
	if ( appObj.resGetParameters[ id ] ){
		submit_genForm( service, appObj.resGetParameters[ id ] );
	} else {
		console.error( "appObj.getParameters[id] not found.");
	}
}

//asubmitObj.previewParams = {};
function submit_genForm ( resService, resParams ){
	if( !( resService && resParams ) ){
		console.error("Error: resService || resParams is undefined");
		return;
	}
	/// Clear form
	$( "#submit-form" ).empty();
	
	/// Save path to preview command
	asubmitObj.previewPath = resService.path;
	
	/// Properties
	var inputTag = '\
<div data-role="fieldcontain">\
	<h3>Properties</h3>\
</div>';
	
	/// Job name
	inputTag += '\
<div data-role="fieldcontain">\
     <label for="sjf-jobName">Job name</label>\
     <input type="text" name="sjf-jobName" id="sjf-jobName" value="" pattern="[^><=\\\!\@\#\$\%\^\&\?|]*" title="without [><=\!@#$%^&?] character" data-clear-btn="true" >\
</div>';

	inputTag += '\
<div data-role="fieldcontain">\
	<label for="sjf-maxElapsedTime">Max elapsed time(s)</label>\
	<input type="range" name="sjf-maxElapsedTime" id="sjf-maxElapsedTime" min="0" step="1" max="' 
		+ ( resService.maxElapsedTime ? resService.maxElapsedTime : 36000 )
		+ '" value="1" data-highlight="true" pattern="^[1-9][0-9]{0,9}" title="Minimum greater than zero" required>\
</div>';
	inputTag += '\
<div data-role="fieldcontain">\
	<label for="sjf-numberOfNodes">Number of nodes</label>\
	<input type="range" name="sjf-numberOfNodes" id="sjf-numberOfNodes" min="0" step="1" max="' 
		+ ( resService.numberOfNodes ? resService.numberOfNodes : 100 ) 
		+ '" value="1" data-highlight="true" pattern="^[1-9][0-9]{0,9}" title="Minimum greater than zero" required>\
</div>';
	/// Legend
	inputTag += '\
<div data-role="fieldcontain">\
	<h3>Parameters:</h3>\
</div>';
	
	$( "#submit-form" ).append( inputTag ).trigger("create");
	
	/*
	 * var param = {};
			param.id = this.id;
			param.name = this.name;
			param.type = this.type;
			param.defaultValue = this.defaultValue;
			param.descript = this.description;
			param.isRequired = this.isRequired;
			param.isOutput = this.isOutput;
			param.prefix = this.prefix;
			pappObj.service.params[ param.id ] = param;
	 */
	/// Parameters
	var numOfParam = 0;
	$.each( resParams.params.items, function (index){
		numOfParam = index + 1;
		/// Label
		var label = this.name ? this.name : "(No name)";
		label += ' (' + this.type + ')';
		
		/// Required attribute
		var requiredAttr = '';
		if( this.isRequired ){
			label += ' *';
			requiredAttr = " required "; 
		}
		
		var inputTag = '',
		dependInput = '';
		
		
		/// Clear if exist popup same id
		popuptag = $("#submit-info-" + this.id);
		if( popuptag != undefined){
			popuptag.remove();
		}
		
		var labelTag = '';
		labelTag += '<label class="long-name" for="sjf-param-' + this.id +'">' + label ;
		labelTag += '	<a href="#submit-info-' + this.id + '" data-rel="popup" data-role="button" class="ui-icon-alt" data-inline="true" data-transition="pop" data-icon="info" data-theme="c" data-iconshadow="false" data-iconpos="notext">Learn more</a>';
		labelTag += '</label>';
		
		/// Info tag
		var infoTag = '';
		infoTag += '<div data-role="popup" id="submit-info-'+ this.id +'" class="ui-content" data-theme="e" style="max-width:350px;">';
		infoTag += '	<p>' + ( this.description ? this.description : "" ) + '</p>';
		infoTag += '</div>';
		
		/// Default value
		var defaultVal = this.defaultValue == undefined ? "" : this.defaultValue;
		
		/// ID
		if( this.id == undefined ){
			console.error("Id is undefined");
			return;
		}
		
		var id = this.id,
		nameid = 'name="sjf-param-' + this.id + '" id="sjf-param-' + this.id +'"';
		
		switch( this.type ){
		
		case "bool":
			inputTag += '<select ' + nameid + ' data-role="slider" data-theme="c">';
			inputTag += '	<option value="true">True</option>';
			inputTag += '	<option value="false">False</option>';
			inputTag += '</select>';
			break;
		case "byte":
			inputTag += '<input type="number" data-clear-btn="true" ' + nameid + ' class="sjf-param byte" value="' + defaultVal + '" pattern="^[0-9]{0,3}" ' + requiredAttr + '>';
			break;
		case "i16":
			inputTag += '<input type="number" data-clear-btn="true" ' + nameid + ' class="sjf-param i16" value="' + defaultVal + '" pattern="^[0-9]{0,5}"' + requiredAttr + '>';
			break;
		case "i32":
			inputTag += '<input type="number" data-clear-btn="true" ' + nameid + ' class="sjf-param i32" value="' + defaultVal + '" pattern="^[0-9]{0,10}"' + requiredAttr + '>';
			break;
		case "i64":
			inputTag += '<input type="number" data-clear-btn="true" ' + nameid + ' class="sjf-param i64" value="' + defaultVal + '"' + requiredAttr + '>';
			break;
		case "double":
			inputTag += '<input type="text" data-clear-btn="true" ' + nameid + ' class="sjf-param double" value="' + defaultVal + '" pattern="^\-?[0-9]+(\.[0-9]+)?$"' + requiredAttr + '>';
			break;
		case "string":
			inputTag += '<input type="text" data-clear-btn="true" ' + nameid + ' class="sjf-param" string" value="' + defaultVal + '"  pattern="[^><=\\\!\@\#\$\%\^\&\?|]*" title="without [><=\!@#$%^&?,] character" ' + requiredAttr + '>';
			break;
		case "file":
			if( this.isOutput ){
				inputTag += '<input type="text" data-clear-btn="true" ' + nameid + ' class="sjf-param" value="' + defaultVal + '"  pattern="[^><=\\\!\@\#\$\%\^\&\?|]*" title="without [><=\!@#$%^&?] character" ' + requiredAttr + '>';
			}else{
				inputTag += '<input type="file" data-clear-btn="true" ' + nameid + ' class="sjf-param" value="' + defaultVal + '"' + requiredAttr + '>';
				/*
				dependInput = '\
<div data-role="fieldcontain" data-type="horizontal">\
	<fieldset data-role="controlgroup">\
		<legend>&nbspdependent files</legend>\
		<input type="file" id="sjf-dependents-param-' + this.id + '" name="sjf-dependents-param-' + this.id + '" class="sjf-dependents-param" multiple>\
	</fieldset>\
</div>';*/
			}
			break;
		default:
			console.log("Type " + this.type + " is unknow!");
			break;
		}
		if(inputTag != ""){
			inputTag = '<div data-role="fieldcontain">' + labelTag + inputTag + infoTag + dependInput +'</div>';
			$( "#submit-form" ).append( inputTag );
		}
	});
	if( numOfParam == 0 ){
		$( "#submit-form" ).append("<p>(No parameter)</p>");
	}
	/// Preview textbox
	/*
	inputTag = '\
<div data-role="fieldcontain">\
<h3>Preview</h3>\
</div>\
<div data-role="fieldcontain">\
<label for="sjf-preview" class="ui-hidden-accessible">Description</label>\
<textarea readonly cols="40" rows="8" name="sjf-preview" id="sjf-preview" style="width:100%"></textarea>\
</div>';
	$("#submit-form").append( inputTag );
	*/
	/// Buttons
	$("#submit-form").append('<input type="submit" value="Submit Job" data-inline="true" data-icon="arrow-d">');
	//$("#submit-form").append('<input type="button" value="Export script" data-inline="true" data-icon="arrow-u">');
	$("#submit-form").append('<input type="reset" name="Reset" value="Clear" data-inline="true" data-icon="delete">');
	
	$("#submit-form").trigger('create');
	
	/// Disable dependent file
	//$(".sjf-dependents-param").attr('disabled','disabled'); 
}
/*
$(document).on( "change", '.sjf-param', function( e ){

	var id = $(this).attr("id");
	var type = $(this).attr("type");
	var paramId, depenTagId;
	if( id.indexOf( "sjf-param-" ) == 0 ){
		paramId = id.substring( 10 );
	} else {
		return;
	}
	
	switch( type ){
	case "file":
		depenTagId = "sjf-dependents-param-" + paramId;
		var depenTag = document.getElementById( depenTagId );
		
		if( this.files.length == 0 || this.files [0] . name . indexOf(".shortcut") > 0 ){
			depenTag.disabled = true;
			depenTag.value = '';
		} else {
			depenTag.removeAttribute("disabled");
		}
	break;
	}
});*/


function commandPreview( pathStr, params, txtPreview ){
	var preview = $.mobile.activePage.find( "textarea#" + txtPreview );
	if ( ( ! params ) || ( ! pathStr ) || ( ! preview ) || ( ! service.params )){
		console.error("path or service not found.");
		return;
	}
	var cmd = ( pathStr !== "" ) ? ( pathStr + " " ) : "";
	
	for( var paramid in params ) {
		var param = service.params[ paramid ];
		cmd += ( param.prefix ? param.prefix : "" );
		if( param.value != undefined && param.value != "" ){
			cmd += ( param.value ? param.value : "" );
		} else {
			cmd += '<' + ( param.name ? param.name : "" ) + '> ';
		}
	}
	preview.val( cmd );
	
	preview.trigger('change');
}
/*
function submit_genForm2( runMethod ){
	console.log("Bp: submit_genForm( runMethod ): ");
	console.log( runMethod );
	
	$( "#submit-form" ).empty();
	if( ! runMethod ){
		console.error("Error: submit_genForm( runMethod ): runMethod is UDEFINED.");
		return;
	}
	if( ! runMethod.Id ){
		console.error("Error: submit_genForm( runMethod ): runMethod.Id is UDEFINED.");
		return;
	}
		
	/// Job name
	inputTag = '';
	inputTag += '	<div data-role="fieldcontain">';
	inputTag += '         <label for="submit-job-name"><strong>Job name</strong></label>';
	inputTag += '         <input type="text" name="jobName" id="submit-job-name" value="" pattern="[^><=\\\!\@\#\$\%\^\&\?|]*" title="without [><=\!@#$%^&?] character" maxlength="300">';
	inputTag += '    </div>';
	
	/// Legend
	inputTag += '<div data-role="fieldcontain">'
	inputTag += '	<h2>Parameters:</h2>';
	inputTag += '</div>';
	$( "#submit-form" ).append( inputTag );
	
	/// RunmethodId
	inputTag = '';
	inputTag += '<div data-role="fieldcontain">';
	inputTag += 	'<input type="hidden" name="runMethodId" value="' + runMethod.Id + '">';
	inputTag += '</div>';
	$( "#submit-form" ).append( inputTag );
	
	if( ! runMethod.Parameters ) {
		console.error("Error: submit_genForm( runMethod ): runMethod.Parameters is UDEFINED.");
		return;
	}
	
	$( runMethod.Parameters ).find('Param').each(function(){
		var $param = $(this);
		
		var name;
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
		
		var requiredAttr = '';
		if( isRequired ){
			label += ' *';
			requiredAttr = " required "; 
		}
		
		inputTag = '';
		
		var labelTag = '';
		labelTag += '<label for="' + id +'">' + label ;
		labelTag += '	<a href="#submit-info-' + id + '" data-rel="popup" data-role="button" class="ui-icon-alt" data-inline="true" data-transition="pop" data-icon="info" data-theme="c" data-iconshadow="false" data-iconpos="notext">Learn more</a>';
		labelTag += '</label>';
		
		var infoTag = '';
		infoTag += '<div data-role="popup" id="submit-info-'+ id +'" class="ui-content" data-theme="e" style="max-width:350px;">';
		infoTag += '	<p>' + des + '</p>';
		infoTag += '</div>';
		switch( type ){
		
		case "bool":
			inputTag += '<select id="' + id +'" name="' + id +'" data-role="slider" data-theme="c">';
			inputTag += '	<option value="true">True</option>';
			inputTag += '	<option value="false">False</option>';
			inputTag += '</select>';
			break;
		case "byte":
			inputTag += '<label for="' + id +'">' + label ;
			inputTag += '	<a href="#submit-info-' + id + '" data-rel="popup" data-role="button" class="ui-icon-alt" data-inline="true" data-transition="pop" data-icon="info" data-theme="e" data-iconpos="notext">Learn more</a>';
			inputTag += '</label>';
			
			inputTag += '<input type="number" data-clear-btn="true" name="' + id +'" id="' + id +'" class="byte" value="" pattern="^[0-9]{0,3}" ' + requiredAttr + '>';
			inputTag += '<div data-role="popup" id="submit-info-'+ id +'" class="ui-content" data-theme="e" style="max-width:350px;">';
			inputTag += '	<p>' + des + '</p>';
			inputTag += '</div>';
			break;
		case "i16":
			inputTag += '<label for="' + id +'">' + label ;
			inputTag += '	<a href="#submit-info-' + id + '" data-rel="popup" data-role="button" class="ui-icon-alt" data-inline="true" data-transition="pop" data-icon="info" data-theme="e" data-iconpos="notext">Learn more</a>';
			inputTag += '</label>';
			inputTag += '<input type="number" data-clear-btn="true" name="' + id +'" id="' + id +'" class="i16" value="" pattern="^[0-9]{0,5}"' + requiredAttr + '>';
			inputTag += '<div data-role="popup" id="submit-info-'+ id +'" class="ui-content" data-theme="e" style="max-width:350px;">';
			inputTag += '	<p>' + des + '</p>';
			inputTag += '</div>';
			break;
		case "i32":
			inputTag += '<label for="' + id +'">' + label ;
			inputTag += '	<a href="#submit-info-' + id + '" data-rel="popup" data-role="button" class="ui-icon-alt" data-inline="true" data-transition="pop" data-icon="info" data-theme="e" data-iconpos="notext">Learn more</a>';
			inputTag += '</label>';
			inputTag += '<input type="number" data-clear-btn="true" name="' + id +'" id="' + id +'" class="i32" value="" pattern="^[0-9]{0,10}"' + requiredAttr + '>';
			inputTag += '<div data-role="popup" id="submit-info-'+ id +'" class="ui-content" data-theme="e" style="max-width:350px;">';
			inputTag += '	<p>' + des + '</p>';
			inputTag += '</div>';
			break;
		case "i64":
			//inputTag += '<label for="' + id +'">' + des +'</label>';
			inputTag += '<input type="number" data-clear-btn="true" name="' + id +'" id="' + id +'" class="i64" value=""' + requiredAttr + '>';
			break;
		case "double":
			//inputTag += '<label for="' + id +'">' + des +'</label>';
			inputTag += '<input type="number" data-clear-btn="true" name="' + id +'" id="' + id +'" class="d" value="" pattern="^\\-?[0-9]+(\\.[0-9]+)?$"' + requiredAttr + '>';
			break;
		case "string":
			//inputTag += '<label for="' + id +'">' + des + '</label>';
			inputTag += '<input type="text" data-clear-btn="true" name="' + id +'" id="' + id +'" value=""  pattern="[^><=\\\!\@\#\$\%\^\&\?|]*" title="without [><=\!@#$%^&?,] character" ' + requiredAttr + '>';
			break;
		case "file":
			if( isOutput ){
				inputTag += '<input type="text" data-clear-btn="true" name="' + id +'" id="' + id +'" value=""  pattern="[^><=\\\!\@\#\$\%\^\&\?|]*" title="without [><=\!@#$%^&?] character" ' + requiredAttr + '>';
			}else{
				//inputTag += '<label for="' + id +'">' + des + '</label>';
				inputTag += '<input type="file" data-clear-btn="true" name="' + id +'" id="' + id +'" value=""' + requiredAttr + '>';
			}
			break;
		default:
			console.log("Type " + type + " is unknow!");
			break;
		}
		if(inputTag != ""){
			inputTag = '<div data-role="fieldcontain">' + labelTag + inputTag + infoTag + '</div>';
			$( "#submit-form" ).append( inputTag );
		}
	});
	$("#submit-form").append('<input type="submit" value="Submit Job" data-inline="true" data-icon="arrow-d">');
	$("#submit-form").append('<input type="button" value="Export script" data-inline="true" data-icon="arrow-u">');
	$("#submit-form").append('<input type="reset" name="Reset" value="Clear" data-inline="true" data-icon="delete">');
	$("#submit-form").trigger('create');
}
*/
function submit_clearCacheData(){
	runMethods = {};
	submitInfo = new Object();
}

function submit_cleanData(){
	
	document.title = '';
	
	$( "#submit-title" ).html( '' );
	
	$( "#submit-cmbx-method" ).empty();
	$( "#submit-cmbx-method" ).selectmenu( 'refresh', true );
	
	$( "#submit-form" ).empty();
	
	$("#submit-form").trigger('create');
	
}

/*
 **************************************
 * Popup API
 ************************************** 
 * */
function submit_popupShow( msg, icon){
	if( ! icon ){
		icon = "check";
	}

	msg += '<img src="images/icons/'+ icon +'.png" height="32" width="32">';
	$( "#submit-popup").popup({ positionTo: "window" });
	$( "#submit-popup").html(msg).popup("option", "positionTo", "window");
	$( "#submit-popup" ).popup( "open" );
}
 