//HISTORY
//2013/09/06    EH0001  longnm  Add api for client
//2013/09/24	EH0002	tuannp	Update the submit job api
//2013/09/27	EH0003	tuannp	Cancel register service api
//2013/09/27+26	EH0004	tuannp	Reconnect when connection unavaiable
//2013/09/27	EH0005	tuannp	Job status
//2013/09/27	EH0006	tuannp	Job detail
//2013/10/14	EH0007	tuannp	Remove app
//2013/10/16	EH0008	tuannp	Submit with shortcut file
//2013/10/24	EH0009  tuannp  Send binary file

var ws;

/*
 * For test
 * */
//var ws_connectCallback;
//var url = "ws://localhost:9003/kportal";		// just local test
//var url = "ws://192.168.1.30:9005/kportal";		// test server on lan
//var url = "wss://192.168.1.40:9004/kportal";		// Sercue test server on lan
//var url = "wss://bounceme.anlab.info:9005/kportal";	// secure release test internet
//var url = "ws://bounceme.anlab.info:9005/kportal";	// release test internet

/*
 * For release
 * */
//var ws_connectCallback;
//var url = "ws://localhost:9003/kportal";		// just local test
//var url = "ws://192.168.1.40:9004/kportal";		// test server on lan
//var url = "wss://192.168.1.40:9004/kportal";		// Sercue test server on lan
//var url = "wss://bounceme.anlab.info:9004/kportal";	// secure release test internet
//var url = "ws://bounceme.anlab.info:9004/kportal";	// release test internet

/*
 * Read from config
 * */
var url = config.websocket_uri;


var requestID = Math.floor((Math.random()*100)+1);
var requests = new Array();	// List of request
var reconnectCount = 0;
/// EH0009
var fileRequestQueue = [];
/// EH009 end

function genRequestID(){
	var idStr = (requestID++).toString();
	if(requestID >= 99999999)
	{
		requestID = Math.floor((Math.random()*100)+1);
	}
	while(idStr.length < 8)
	{
		idStr = "0" + idStr;
	}
	return idStr;
}

function showDialog(title, message){
	document.getElementById("popup_title").innerHTML = title;
	document.getElementById("popup_messages").innerHTML = message+ "<br />";
	$.mobile.changePage('#popup', {transition: 'pop', role: 'dialog'});
}

function ws_connect ( callback ) {
	//ws_connectCallback = callback;
	//alert("WS_CONNECT: url = " + url);
	console.log("WS_CONNECT: url = " + url);
	try
	{
		if ("WebSocket" in window) {
	        ws = new WebSocket(url);
	    } else if ("MozWebSocket" in window) {
	        ws = new MozWebSocket(url);
	    } else {
	    	error_showPage("Websocket error", "This Browser does not support WebSockets<br />");
	        return;
	    }
		
	} catch(exception){  
		console.log(exception);
		error_showPage("Websocket", exception.message);
		return;
	}
    ws.onopen = function(e) {
    	/// EH0006 : reconnect when websocket timeout
    	if( reconnectCount ){
    		reconnectCount = 0;
    		/// Relogin when reconnect
    		if (ws || ws.readyState == 1) {
    			if( localStorage.loginId && localStorage.email && localStorage.userType ){
	    			ws_login2( localStorage.loginId, function( res ){
	    				if(res.valid == 1){
	    					console.log("onopen : relogin is SUCCESS.");
						if( callback ){
				                        callback(true);
				                } else {
							console.log("Callback is empty!");
							//home_loginCallback(true);
							//home_loginCallback
							$.mobile.loading("hide");
						}

	    				}else{
	    					console.log("Error: reopen : login valid  is fail.");
	        				//error_showPage("LoginId not found", "User need back to home page and login again. Try again.<br />");
	    					cleanUserInfo();
	    					$.mobile.changePage('#home');
	    				}
	    			});
    			} else {
				console.log("NO login");
				$.mobile.loading("hide");
			}
    		}
    	}else{
    		if( callback ){
        		callback(true);
    		} else {
			console.log("Callback is empty 2");
			$.mobile.loading("hide");
		}
    	}
    	/// EH0006 end
    };
    
    ws.onerror = function(e) {
    	/// EH0006 : reconnect when websocket timeout
    	if( reconnectCount < 3){
    		console.log("ws.onerror: e = ");
    		console.log(e);
    		reconnectCount ++;
    		ws_connect(callback);
    		return;
    	}
    	//alert("Connection has timeout");
    	//alert("More than 3 time");
    	/// EH0006 end
        //document.getElementById("messages").innerHTML += "Client: An error occured, see console log for more details.<br />";
    	error_showPage("Websocket", 'Client: An error occured, see console log for more details.');
    	//error_showPage("Websocket", e.data);
    };
    
    ws.onclose = function(e) {
        //console.log(e.data);
    	//document.getElementById("messages").innerHTML += "Client: The connection to "+url+" was closed.<br />";
        //error_showPage("Websocket","Socket is closed.");
       if(reconnectCount < 3){
    		console.log("Reconnect times: " + reconnectCount);
    		reconnectCount ++;
    		ws_connect(callback);
    		return;
    	}
    	console.log("Websocket closed!");
	$.mobile.loading("hide");
    };
    
    ws.onmessage = function(e) {
    	console.log("Bp: message before parse: ");
    	console.log(e.data);
    	var str = e.data;
    	//console.log("e.data = " + e.data);
    	//str = str.replace ( "\n", " " );
    	//console.log("Bp: message after replace: ");
    	//console.log(str);
    	
    	/// EH0009
    	/// Check binary data
    	if( typeof (e.data) == "object"){
    		console.log ( "Message client opcode binary: " );
    		console.log(e.data);
    		processBinaryData(e.data);
        	return;
    	}
    	/// EH0009 end
    	
		var message = JSON.parse(str);
		if( message.data ){
			console.log("Server response: Data packet: ");
		}else{
			console.log("Server response: " + JSON.stringify(message));
		}
		
		if(!( message.requestId in requests) || message.requestId == undefined ) {
			console.error("Request tag is missing or request not exits!");
			return;
		}
		processResponse(message);
    };
}


function processBinaryData( data ){
	console.log("processBinaryData");
	if(data.length < NOB_REQUEST_ID){
		console.log("RequestID tag header field is missing!");
		return;
	}
	if(data.length < NOB_REQUEST_ID + NOB_SENT_BYTE){
		console.log("Sent tag header field is missing!");
		return;
	}
	
	if(data.length < NOB_REQUEST_ID + NOB_SENT_BYTE + NOB_TOTAL_BYTE){
		console.log("Sent tag header field is missing!");
		return;
	}
	
	var rdb = new FileReader();
	rdb.onloadend = function(event) {
		var contents = event.target.result,
        error = event.target.error;
		if (error != null) {
			console.error("File could not be read! Code " + error.code);
			return;
		}
		console.log("Contents: " + contents);
		var requestId = ab2str(contents.slice(0, NOB_REQUEST_ID));
		console.log("requestId : " + requestId);
		var sentNtotal = contents.slice(NOB_REQUEST_ID, NOB_REQUEST_ID + NOB_SENT_BYTE + NOB_TOTAL_BYTE);
		var uint32s = ab2ui32(sentNtotal);
		if(uint32s.length < 2){
			console.log("NOB_SENT_BYTE && NOB_TOTAL_BYTE invalid");
			return;
		}
		var sentNoByte = uint32s[0];	
		var totalNoByte = uint32s[1];
		console.log("sentNoByte : " + sentNoByte + ", totalNoByte : " + totalNoByte);
		if( requests[requestId].callback != undefined){
			requests[requestId].callback(requestId, sentNoByte, totalNoByte, contents.slice(NOB_REQUEST_ID + NOB_SENT_BYTE + NOB_TOTAL_BYTE));
		}
	};
	rdb.readAsArrayBuffer(data);
}

function processBinaryData1( data ){
	console.log("Bp: processBinaryData( data ): fileRequestQueue = ");
	console.log( fileRequestQueue );
	
	/// Save data to request
	var fileRequest;
	if( data && fileRequestQueue &&  fileRequestQueue.length > 0 ){
		fileRequest = fileRequestQueue.shift();
		console.log("Bp: processBinaryData( data ):fileRequest = ");
		console.log( fileRequest );
		if( fileRequest.requestId && requests[ fileRequest.requestId ].response ){
			requests[ fileRequest.requestId ].response.data = data;	
//			var reader = new FileReader();
//			reader.onloadend = function() {
//				if( reader.error ){
//					console.error("Bp: processBinaryData( data ) ERROR read data.");
//					return;
//				}
//				
//				console.log("Byte:");
//			    console.log(reader.result);
//			    
//			    requests[ fileRequest.requestId ].response.data = reader.result;
//			    if( callback ) { 
//			    	callback ();
//			    }
//			};
//			
//			switch ( fileRequest.dataType ){
//			case "Text":
//				//alert("readAsText");
//				reader.readAsText(data);
//				break;
//			case "BinaryString":
//				//alert("readAsBinaryString");
//				reader.readAsBinaryString(data);
//				break;
//			default:
//				requests[ fileRequest.requestId ].response.data = data;	
//				if( callback ) { 
//			    	callback ();
//			    }
//				break;
//			}
			
		} else {
			console.error("Bp: processBinaryData( data ): request need receive data not found.");
			if( callback ) { 
		    	callback ( -1 );
		    }
			return;
		}
	} else {
		console.error("Bp: processBinaryData( data ): fileRequestQueue is UNDEFINED or EMPTY, or data is UNDEFINED");
		if( callback ) { 
	    	callback ( -2 );
	    }
		return;
	}
}
/// EH0009 end

/* 
 * { “valid”: 1, “userType”: userType } 
 * { “valid”: 1, “userType”: userType, “loginID”: loginID }
 * */
function processResponse(msg){
	/*
	 * if(msg.valid != undefined){
		if(msg.valid == 0){
			//login_error("Login fail !");
			home_loginError();
		}else{
			if(msg.loginID){
				localStorage.loginID = msg.loginID;
			}
			
			localStorage.userType = msg.userType;
			
			home_loginSucess();
		}
	}*/
	/*
	Object.size = function(obj) {
	    var size = 0, key;
	    for (key in obj) {
	        if (obj.hasOwnProperty(key)) size++;
	    }
	    return size;
	};
	
	/*
	if( Object.size( msg ) == 2 ){
		if( msg.status != undefined && msg.requestId ){
			switch( msg.status ){
			case AuthorizeCodes.ERR_INVALID_ACCESS:
				error_showPage("Access Denied", "User do\'not have permission to use service. Please register service");
				return;
				break;
			case AuthorizeCodes.ERR_NO_AUTHENTICATION:
				cleanUserInfo();
				error_showPage("Authentication", "Please re-login.");
				return;
				break;
			}
		}
	}
	*/
	switch(msg.result){
	case ResponseCodes.PERMISSION_DENIED:
		ws_login2(localStorage.loginId, function(res){
			localStorage.userType = res.userType;
			error_showPage("Access Denied", "Account do not permit the action!");
		});
	break;
	}
	var requestType = requests[msg.requestId].request.msg;
	switch (requestType) {
	case "connect":
		processConnect(msg);
		break;
	case "logout":
		processLogout(msg);
		break;
	case "findServices":
    	processFindServices(msg);
        break;
	case "getRunMethodsFile":
	case "getOutputFile":
		processRecieveFile(msg);
	    break;
	default:
		console.log("Bp: requestType: " + requestType + ", callback.");
		if( requests[ msg.requestId ].callback ){
			console.log( "Bp: requests[ msg.requestId ]" );
			console.log( requests[ msg.requestId ] );
			if ( requests[ msg.requestId ].callback ){
				//alert("callback");
				requests[ msg.requestId ].callback(msg);
			}else{
				console.log("There are no callback function.");
			}
			
		}else{
			console.log("processResponse(msg): request response not found.");
		}
		
		break;
	}
}

function processLogout(msg){
	requests[ msg.requestId ].callback(msg);
}

function processConnect(msg){
	requests[msg.requestId].callback(msg);
}

function processRecieveFile(msg){
	if( ! requests[ msg.requestId ].response)
		requests[ msg.requestId ].response = new Object();
	if(msg.path){
		requests[ msg.requestId ].response.path = msg.path;
		
		/// EH0009
		fileRequestQueue.push( { "requestId" : msg.requestId } );
		/// EH0009 end
		
		return;
	}
	
	/// EH0009
//	if(msg.data){
//		if( ! requests[ msg.requestId ].response.data)
//			requests[ msg.requestId ].response.data = msg.data;
//		else
//			requests[ msg.requestId] .response.data += msg.data;
//		return;
//	}
	/// EH0009 end
	
	if(msg.finish){
		requests[ msg.requestId ].response.finish = msg.finish;
		/// EH0009
		var callback = requests[ msg.requestId ].callback;
		var data = requests[ msg.requestId ].response.data;
		if( ! callback ) {
			console.error("Bp: processRecieveFile(msg) callback of request is UNDEFINED");
			return;
		}
		//callback( requests[ msg.requestId ].response.data, msg.finish == 1);
		var reader = new FileReader();
		reader.onloadend = function() {
			if( reader.error ){
				console.error("Bp: processRecieveFile(msg) ERROR read data.");
				callback( data , false );
				return;
			}
			
			//console.log("Byte:");
		    //console.log(reader.result);/
		    
		    //requests[ fileRequest.requestId ].response.data = reader.result;
		    if( callback ) { 
		    	callback( reader.result, msg.finish == 1);
		    }
		};
		
		var requestType = requests[msg.requestId].request.msg;
		switch ( requestType ){
		case "getRunMethodsFile":
			//reader.readAsBinaryString(data);
			reader.readAsText(data);
			break;
		default:
			/// Default data is Blob object ~ ArrayBuffer
			//requests[ fileRequest.requestId ].response.data = data;	
			if( callback ) { 
				callback( data, msg.finish == 1);
		    }
			break;
		}
		
		/// EH0009 end
		return;
	}
}

function processFindServices(msg){
	if(msg.services.items != undefined){
		console.log(msg);
		console.log(msg.services.items);
		requests[ msg.requestId ].callback( msg.services.items );
		//home_loadServices(msg.services.items);
	}else {
		error_showPage("Websocket", "The respose message is missing required tag.");
		console.error(msg);
	}
}

function disconnect() {
    ws.close();
    document.getElementById("server_url").disabled = false;
    document.getElementById("toggle_connect").innerHTML = "Connect";
}

/*
function toggle_connect() {
    if (document.getElementById("server_url").disabled === false) {
        connect();
    } else {
        disconnect();
    }
}*/

var handler = false;
function send(jsonObj, callbackFunction) {
    if (ws === undefined || ws.readyState != 1) {
    	if ( handler ) {
    		//alert( "Reconnecting !");
    		handler = true;
    		return;
    	}
    	handler = true;
    	/// EH0004
    	console.log("Error: websocket connection unavaiable.");
    	console.log("Reconnect now!");
    	ws_connect(function(){
    		if ( ws || ws.readyState == 1 ) {
    			console.log("Reconnect success !");
    			if( localStorage.loginId && localStorage.email && localStorage.userType ){
	    			ws_login2( localStorage.loginId, function( res ){
	    				if( res.valid == 1 ){
	    					_send(jsonObj, callbackFunction);
	    					handler = false;
	    				}else{
	    					//console.error("Error: send(jsonObj, callbackFunction) : login valid  is fail.");
	    					handler = false;
	        				//error_showPage("LoginId not found", "User need back to home page and login again. Try again.<br />");
	    					/// Clean and back to home page
	    					cleanUserInfo();
	    					uLogin_resetBtn();
	    					$.mobile.changePage('#home');
	        				return;
	    				}
	    			});
    			}else{
    				if( jsonObj.msg == "connect" ){
    					_send(jsonObj, callbackFunction);
    					handler = false;
    				}else{
        				//console.error("Error: send(jsonObj, callbackFunction) : reconnect loginID not exist.");
        				//error_showPage("LoginId not found", "User need back to home page and login again. Try again.<br />");
        				handler = false;
        				/// Clean and back to home page
    					cleanUserInfo();
    					uLogin_resetBtn();
    					$.mobile.changePage('#home');
        				return;
    				}
    			}
    			
    		}else{
    			console.error("Reconnect fail.");
    			error_showPage("Websocket error", "Can not establish a connection to the server.<br />");
    			handler = false;
    			return;
    		}
    	});
        return;
        /// EH0004 end
    } else {
    	_send(jsonObj, callbackFunction);
    	handler = false;
    }
}

function _send(jsonObj, callbackFunction) {
	var resreq = new Object();
	resreq.request = jsonObj;
	resreq.callback = callbackFunction;
	
	requests[ jsonObj.requestId ] = resreq;
	var msg = JSON.stringify(jsonObj);
	console.log("Client request:" + msg);	
	ws.send(msg);
}

/* ****************************
 * API for client
 * *****************************/

/* { “requestID”: requestID, “msg”: “logout”, "loginId": loginID} */
function ws_logout(loginID, callback){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "logout";
	request.loginId = loginID;
	
	send(request, callback);
}

function ws_login(access_token, email){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "connect";
	request.user = email;
	
	request.pwd = pwd;
	send(request);
}	

/* { “msg”: “connect”, “email”: email, “token”: token } */
function ws_login1(email, token, callback){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "connect";
	request.email = email;
	request.token = token;
	send(request, callback);
}

/* { “msg”: “connect”, “loginID”: loginID } */
function ws_login2(loginId, callback){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "connect";
	request.loginId = loginId;
	send(request, callback);
}

/*{ “requestID”: requestID, “msg”: “findServices”, [“provider”: providerID] }*/
function ws_findServices( callback ) {
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "findServices";
	send(request, callback);
}

function ws_getRunMethodFile(service, callback){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "getRunMethodsFile";
	request.service = service;
	send(request, callback);
}

/*{ “requestID”: requestID,
	 “msg”: “submitJob”,
	 “service”: serviceID,
	 “runMethod”: runMethodID,
	 “jobName”: jobName,
	 “params”: {
	  	“1”: “out.txt”,
	  	“2”: 4.5,
	  	“3”: 5,
	}}*/


function ws_submitJob( info, callback ){
	console.log("ws_submitJob( info, callback )");
	console.log(info);
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "submitJob";
	request.service = info.serviceId;
	//request.runMethod = info.runMethodId;
	request.maxElapsedTime = info.maxElapsedTime;
	request.numberOfNodes = info.numberOfNodes;
	request.jobName = info.jobName;
	request.params = {};
	if( info.params ){
		var fileParams = new Array();
		$.each( info.params, function( key, value ){
			console.log("Bp: ws_submitJob: key = " + key + ", value = ");
			console.log(value);
			if( value.type == "file" 
				/*&& value.isOutput != true*/ ){
//				/// Get list file need send.
//				if( ufile_getExtension ( value.value[0].name ) == "shortcut" ){

				fileParams [ fileParams.length ] = { "key": key, "value": value.value }
				
//				var param = new Object();
//				param.path = value.value[0].name;
//				param.type = "local";
//				request.params [ key ] = param;
				
//				genFileParameter(value, function(param){
//					if ( param < 0 ){
//						/// Gen file fail
//						console.error("Error: ws_submitJob( info, callback ): genFileParameter() fail.");	
//					}else{
//						request.params [ key ] = param;
//					}
//				});
			} else {
				//request.params [ key ] = value.value ;
				request.params [ key ] = value ;
			}
		});
		//EH0002 end
		console.log("Bp: fileParams ");
		console.log(fileParams);
		genFileParameterRecursive( fileParams, 0, request.params, function( result ){
			if( result == 0 ){
				send(request, callback);
			} else {
				callback (result)
				//alert("Error: genFileParameterRecursive() fail.");
			}
		} );
	}
	
}

/// EH0008
function genFileParameterRecursive( fileParams, index, requestParams , callback ){
	
	if( fileParams.length == 0  || index >= fileParams.length ){
		console.log("Bp: genFileParameterRecursive: finish.");
		callback ( SUCCESS );
		return;
	}
	console.log("BP: genFileParameterRecursive: fileParams[ index ] = ");
	console.log(fileParams[ index ]);
	if( fileParams[index] && fileParams[ index ].value[0] && fileParams[ index ].key && fileParams[ index ].value[0].name ){
			genFileParameter( fileParams[ index ].value, function (param){
				if ( param < 0 ){
					/// Gen file fail
					console.error("Error: ws_submitJob( info, callback ): genFileParameter() fail.");
					callback ( param );
					return;
				}else{
					requestParams [ fileParams[ index ].key ] = param;
					genFileParameterRecursive( fileParams, index + 1, requestParams, callback)
				}
			} );
	} else {
		//alert("Error: onSubmit: value.value[0] is UNDEFINED.");
		callback ( ERROR );
		return;
	}
}

function genFileParameter( files, callback ){
	console.log("Bp: genFileParameter( files, callback ): files = ");
	console.log(files);
	
	var param = new Object();
	extension = ufile_getExtension ( files[0].name );
	console.log(  extension );
	fileInfo = files[0];
	if( extension.toLowerCase() == "shortcut" ){
		var reader = new FileReader();
		reader.onloadend = function (bytes) {
			console.log('Bp: reader.error = ');
			console.log(reader.error);
			if( reader.error ){
				callback( ERR_READ_FILE);
				gl_setError( ERR_READ_FILE, reader.error);
				return;
			}
			
			binaryString = reader.result;
			
			console.log("Bp: genFileParameter( files ): byte:");
		    
			console.log(binaryString);
			
			var shortcutJSON;
			try{
				shortcutJSON = JSON.parse(binaryString);
				if( shortcutJSON.path && shortcutJSON.job ){
					param.path = shortcutJSON.path;
					param.job = shortcutJSON.job;
				}else{
					param.path = fileInfo.name;
					param.type = "local";					
				}
				callback ( param );
		    }catch(e){
		    	callback ( ERR_INVALID_SHORTCUT_FILE_FORMAT );
		    }
		
		};
	    reader.readAsBinaryString(fileInfo);
	}else{
		param.path = fileInfo.name;
		param.type = "local";
		callback ( param );
	}
}
/// EH0008 end

//EH0002

/*
 * Req2 Block binary: requestId/fileId/binarydata
 * Res2 {"requestId":"xxxxxxx", "result":"0/1"}
 */
 
 function ws_sendFile(requestId, fileInfo, callback){
	console.log( "Bp: ws_sendFile( requestId, fileInfo, fileId, callback )"  + "(" + requestId + ", " );
	console.log( fileInfo );
	console.log( ", " );
	console.log( callback );
	console.log( ")");
	var reader = new FileReader();
	reader.onloadend = function() {
		if( reader.error ){
			//gl_setError( ERR_READ_FILE, reader.error );
			callback(1);
			return;
		}
		//ws.send(reader.result);
		requests[requestId] = new Object();
		requests[requestId].callback = callback;
		requests[requestId].request = new Object();
		requests[requestId].request.msg = "";
		//ws.send(binaryString);
		var rid = str2ab(requestId);
		var blob = new Blob([rid, reader.result]);
		var rdb = new FileReader();
		rdb.onloadend = function() {
			ws.send(rdb.result);
		};
		rdb.readAsArrayBuffer(blob);
	};
	reader.readAsArrayBuffer(fileInfo);
}

/*
 * Req2 Block binary: requestId/fileId/sentbyte size/total/binarydata
 * Res2 {"requestId":"xxxxxxx", "from": sentByteSize, "result":"0/1"}
 */
function ws_sendFilePart(requestId, fileInfo, sentSize, callbackPart, callbackFinish){
	console.log( "Bp: ws_sendFilePart( requestId, fileInfo, part, callback )"  + "( requestId = " + requestId + ", sendedPart = " + sentSize + ", " );
	var blocksize = 10*1024*1024; // 10M
	var total = fileInfo.size;
	var sendSize = sentSize + blocksize;
	if(sentSize < fileInfo.size){
		var isLastPart = false;
		if( sendSize >= fileInfo.size){
			/// Last part file
			isLastPart = true;
			blocksize = fileInfo.size - sentSize;
			sendSize = sentSize + blocksize;
		}
		var reader = new FileReader();
		reader.onloadend = function() {
			if( reader.error ){
				//gl_setError( ERR_READ_FILE, reader.error );
				callbackFinish(1);
				return;
			}
			requests[requestId] = new Object();
			requests[requestId].callback = isLastPart ? callbackFinish : function(res){
				if(res.result == ResponseCodes.DATA_SUCCESS && res.from == sendSize){
					callbackPart(sendSize, fileInfo.size);
					ws_sendFilePart(requestId, fileInfo, sendSize, callbackPart, callbackFinish);
				} else {
					console.error("ERROR: Server write file fail. Res = ");
					console.log(res);
					callbackFinish(2);
				}
			};
			requests[requestId].request = new Object();
			requests[requestId].request.msg = "";
			
			/// Create binary message
			var rid = str2ab(requestId);
			var sendAb = uint2ab(sendSize);
			var totalAb = uint2ab(fileInfo.size);
			
			var blob = new Blob([rid, sendAb, totalAb, reader.result]);
			var rdb = new FileReader();
			rdb.onloadend = function() {
				ws.send(rdb.result);
			};
			rdb.readAsArrayBuffer(blob);
		};
		reader.readAsArrayBuffer(fileInfo.slice(sentSize, sendSize));
	} else {
		console.log("End sent file");
		return;
	}
}
/*
 * Req2 Block binary: requestId/fileId/sentbyte size/total/binarydata
 * Res2 {"requestId":"xxxxxxx", "from": sentByteSize, "result":"0/1"}
 */
function ws_sendFileParamPart(requestId, paramId, fileInfo, sentSize, callbackPart, callbackFinish){
	console.log( "Bp: ws_sendFilePart( requestId, fileInfo, part, callback )"  + "( requestId = " + requestId + ", sendedPart = " + sentSize + ", " );
	var blocksize = 10*1024*1024; // 10M
	var total = fileInfo.size;
	var sendSize = sentSize + blocksize;
	if(sentSize < fileInfo.size){
		var isLastPart = false;
		if( sendSize >= fileInfo.size){
			/// Last part file
			isLastPart = true;
			blocksize = fileInfo.size - sentSize;
			sendSize = sentSize + blocksize;
		}
		var reader = new FileReader();
		reader.onloadend = function() {
			if( reader.error ){
				//gl_setError( ERR_READ_FILE, reader.error );
				callbackFinish(1);
				return;
			}
			requests[requestId] = new Object();
			requests[requestId].callback = isLastPart ? callbackFinish : function(res){
				if(res.result == ResponseCodes.DATA_SUCCESS && res.from == sendSize){
					callbackPart(sendSize, fileInfo.size);
					ws_sendFileParamPart(requestId, paramId, fileInfo, sendSize, callbackPart, callbackFinish);
				} else {
					console.error("ERROR: Server write file fail. Res = ");
					console.log(res);
					callbackFinish(2);
				}
			};
			requests[requestId].request = new Object();
			requests[requestId].request.msg = "";
			
			/// Create binary message
			var rid = str2ab(requestId);
			var pid = str2ab(paramId);
			var sendAb = uint2ab(sendSize);
			var totalAb = uint2ab(fileInfo.size);
			
			var blob = new Blob([rid, sendAb, totalAb, pid, reader.result]);
			var rdb = new FileReader();
			rdb.onloadend = function() {
				ws.send(rdb.result);
			};
			rdb.readAsArrayBuffer(blob);
		};
		reader.readAsArrayBuffer(fileInfo.slice(sentSize, sendSize));
	} else {
		console.log("End sent file");
		return;
	}
}

/* { “requestID”: requesteID, “path”: ”src/test1.cpp” } */
function ws_sendFileName(requestId, fileName){
	var request = new Object();
	request.requestId = requestId;
	request.path = fileName;
	send(request);
}

/*{ “requestID”: requesteID, “data”: binary_data }*/
function ws_sendFileData(requestId, data){
//	var request = new Object();
//	request.requestId = requestId;
//	request.data = data;
//	send(request);
	ws.send(data);
}

/*	 { “requestID”: requestID, “finish”: 1 } */
function ws_sendFileFinish( requestId, result, callback ){
	var request = new Object();
	request.requestId = requestId;
	request.finish = result;
	send(request, callback);
}

//EH0002 end

/*{ “requestID”: requestID, “msg”: “getServicesHaveCandidates” }*/
function ws_getWaitingServices ( callback ){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "getWaitingServices";
	send(request, callback);
}

function ws_getAllServices ( callback ){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "getAllServices";
	send(request, callback);
}

/* { “requestID”: requestID, “msg”: “registerService”, “service”: serviceID } */
function ws_registerService( serviceId, callback ){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "registerService";
	request.service = serviceId;
	send(request, callback);
}

/// EH0003

function ws_cancelRegistering( serviceId, callback ) {
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "cancelRegistering";
	request.service = serviceId;
	send(request, callback);
}
/// EH0003 end

/// EH0005

/* { “requestID”: requestID, “msg”: “getJobsByFilter”, “service”: serviceID } */
function ws_getJobsByFilter( filterInfo, callback ){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "getJobsByFilter";
	request.service = filterInfo.serviceId;
	request.textFilter = filterInfo.textFilter;
	request.begin = filterInfo.begin;
	request.count = filterInfo.count;
	send(request, callback);
}

/* { “requestID”: requestID, “msg”: “getJobsByService”, “service”: serviceID } */
function ws_getJobsByService( serviceId, callback ){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "getJobsByService";
	request.service = serviceId;
	send(request, callback);
}
//
///* { “requestID”: requestID, “msg”: “getJobsByUser” } */
//function ws_getJobsByUser( callback ){
//	var request = new Object();
//	request.requestId = genRequestID();
//	request.msg = "getJobsByUser";
//	send(request, callback);
//}

/// EH0005 end

/// EH0006
/*
 * Request:  { “requestId”: requestId , “msg”: “getJobInfos”, “service”: service ,“job”: jobID }
 * Response: ..
 * */
function ws_getJobInfos( jobId, serviceId, callback ){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "getJobInfos";
	request.service = serviceId;
	request.job = jobId;
	send(request, callback);
}
/*
 * Request:  { “requestId”: requestId , “msg”: “getEntries”, “job”: jobID , "curDir":currentDirectoryToGetEntries}
 * Response: ..
 * */
function ws_getEntriesOfJob( jobId, curDir, callback ){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "getEntries";
	request.job = jobId;
	request.curDir = curDir;
	send(request, callback);
}
/*
 * Request:  { “msg”: “cancelJob”, “job”: jobID }
 * Response: { “job”: jobID, “status”: status }
 * */
function ws_cancelJob( jobId , callback ){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "cancelJob";
	request.job = jobId;
	send(request, callback);
}

/*
 * Request:  { “msg”: “restartJob”, “job”: jobID }
 * Response: { “job”: jobID, “status”: status }
 * */
function ws_restartJob( jobId , callback ){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "resubmitJob";//"restartJob";
	request.job = jobId;
	send(request, callback);
}

/*
 * Request:  { “msg”: “deleteJob”, “job”: jobID }
 * Response: { “job”: jobID, “status”: status }
 * */
function ws_deleteJob( jobId , callback ){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "deleteJob";
	request.job = jobId;
	send(request, callback);
}
/* 
 * Request:  { “msg”: “getOutputFile”, “job”: jobID, “path”: entryPath }
 * Response: 
 * */
function ws_getOutputFile( jobId , entry, callback ){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "getOutputFile";
	request.job = jobId;
	request.path = entry;
	
	//send(request, callback);
	var res = {};
	var parts = [];
	res.result = ResponseCodes.DATA_ERROR;
	var getOutputFilePart = function(requestId, sentNoByte, totalNoByte, data){
		if(sentNoByte == totalNoByte){
			parts.push(data);
			var ext = ufile_getExtension(entry);
			var mime = uMime_getMimeByExt(ext);
            var blob = new Blob(parts, {'type': mime, endings: 'native'});
			res.filedata = blob;
			res.result = ResponseCodes.FILE_ACTION_SUCCESS
			callback(res);
		} else if(sentNoByte < totalNoByte){
			//alert("continue");
			parts.push(data);
			//callbackPart(sentNoByte, totalNoByte);
			request.from = sentNoByte;
			send(request, getOutputFilePart);
		} else {
			callback(res);
		}
	};
	send(request, getOutputFilePart);
}

function ws_userRemoveService( serviceId, callback){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "userRemoveService";
	request.service = serviceId;
	send(request, callback);
}

function ws_getService( serviceId, callback){
    var request = new Object();
    request.requestId = genRequestID();
    request.msg = "getService";
    request.service = serviceId;
    send(request, callback);
}

function ws_getServices(callback){
    var request = new Object();
    request.requestId = genRequestID();
    request.msg = "getServices";
    send(request, callback);
}

function ws_getServicesHaveCandidate(callback){
    var request = new Object();
    request.requestId = genRequestID();
    request.msg = "getServicesHaveCandidates";
    send(request, callback);
}

function ws_getCandidatesOfService(service, callback){
    var request = new Object();
    request.requestId = genRequestID();
    request.msg = "getCandidates";
    request.service = service;
    send(request, callback);
}

function ws_getUsersOfService(service, callback){
    var request = new Object();
    request.requestId = genRequestID();
    request.msg = "getUsers";
    request.service = service;
    send(request, callback);
}

function ws_removeUserOfService( service, user, callback){
    var request = new Object();
    request.requestId = genRequestID();
    request.msg = "removeUser";
    request.service = service;
    request.user = user
    send(request, callback);
}

function ws_addUser(service, user, callback){
    var request = new Object();
    request.requestId = genRequestID();
    request.msg = "addUser";
    request.service = service;
    request.user = user;
    send(request, callback);
}

function ws_removeCandidateOfService(service,user,callback) {
    var request = new Object();
    request.requestId = genRequestID();
    request.msg = "removeCandidate";
    request.service = service;
    request.user = user;
    send(request, callback);
}
//EH0001 end
/*
 *	Send request create new service 
 */
function ws_createService( service, callback ){
	var request = new Object();
    request.requestId = genRequestID();
    request.msg = "createService";
    //request.provider = service.provider;
    request.name = service.name;
    request.path = service.path;
    request.shpath = service.shpath;
    request.image = service.image;
	// stagin folders
	request.stageinDirs = service.stageinDirs;
	request.stageoutDirs = service.stageoutDirs;
	
	// stagout folders
    //request.producer = service.producer;
    if( service.status == true ){
    	request.status = true;
    }
    //request.icon = ( service.icon && service.icon.name ) ? service.icon.name : "";
    if ( service.iconfile ) {
    	//console.log(" service.iconfile = ");
    	//console.log( service.iconfile );
    	//alert("ws_createService");
    	request.icon = service.iconfile.name;
    }
	request.maxElapsedTime = service.maxElapsedTime;
	request.numberOfNodes = service.noNodes;
	request.description = service.descript;
    request.params = {};
    request.params.items = [];
    console.log( "service = " );
    console.log( service );
    $.each( service.params, function (index) {
    	request.params.items.push( 
    			{ 
    				"id" : this.id,
    				"name" : this.name, 
    				"type" : this.type, 
    				"defaultValue" : this.defaultValue,
    				"isRequired" : this.isRequired,
    				"isOutput" : this.isOutput,
    				"prefix" : this.prefix,
    				"description" : this.descript,
    				} );
    });
    send(request, callback);
}

/*
 * Send request create update services info
 */
function ws_updateService( service, callback ){
    
	var request = new Object();
	request.requestId = genRequestID();
    request.msg = "updateService";
    request.service = service.id;
    //request.updates = {};
    //request.updates.items = service.updates;
    //console.log("service = ");
    //console.log(service);
	//alert("ws_updateService");
    for ( i = 0 ; i < service.updates.length ; i ++ ) {
    	var key = service.updates[ i ];
    	switch( key ){
    	case "noNodes":
    		service.updates[ i ] = "numberOfNodes";
    		request [ "numberOfNodes" ] = service [ key ];
    		break;
    	case "descript":
    		service.updates[ i ] = "description";
    		request [ "description" ] = service [ key ];
    		break;
    	default:
    		request [ key ] = service [ key ];
    		console.log(" request [ key ] ");
    		console.log( request [ key ] );
    		break;
    	}
    	
    }
    /// Parameter
    request.params = {}; request.params.items = [];
    if ( service.params && Object.size( service.params ) > 0 ){
	    $.each( service.params, function (index) {
	    	if ( this.action ) {
	    	   	request.params.items.push( 
	        			{ 
	        				"action" : this.action,
	        				"id" : this.id,
	        				"name" : this.name, 
	        				"type" : this.type, 
	        				"defaultValue" : this.defaultValue,
	        				"isRequired" : this.isRequired,
	        				"isOutput" : this.isOutput,
	        				"prefix" : this.prefix,
	        				"description" : this.descript,
	        				} );
	    	}
	    });
    }
    //console.log(request);
    send(request, callback);
}

/*
 * Get parameter of service
 */
function ws_getParameters( service, callback ) {
    var request = new Object();
    request.requestId = genRequestID();
    request.msg = "getParameters";
    request.service = service;
    send(request, callback);
}

/*
 * 
 */
function ws_uploadIcon( requestId, serviceId, fileInfo, callback ){
	console.log( "Bp: ws_uploadIcon( requestId, serviceId, fileInfo, callback )"  + "(" + requestId + ", serviceId = " + serviceId );
	console.log( fileInfo );
	console.log( ", " );
	console.log( callback );
	console.log( ")");
	
	/// Send file name
	//ws_sendFileName(requestId, fileInfo.name);
	var requestn = new Object();
	requestn.requestId = requestId;
	requestn.msg = "uploadIcon";
	//requestn.path = fileInfo.name;
	requestn.path = serviceId + ".png";
	
	send(requestn);
	
	/// Send data
	var reader = new FileReader();
	reader.onloadend = function() {
		if( reader.error ){
			gl_setError( ERR_READ_FILE, reader.error );
			alert("Error read file");
			return;
		}
		binaryString = reader.result;
		//console.log("Byte:");
	    //console.log(binaryString);
	    
	    ws_sendFileData(requestId, binaryString);
	    
	    /// Send finish
	    //ws_sendFileFinish( requestId, 1, callback );
	    var requestf = new Object();
	    requestf.requestId = requestId;
	    requestf.msg = "uploadIcon";
	    requestf.finish = 1;//ResponseCodes.DATA_SUCCESS;
		send(requestf, callback);
	};
	reader.readAsArrayBuffer(fileInfo);
}
/*
function ws_sendFile( requestId, fileInfo, callback ){
	console.log( "Bp: ws_sendFile( requestId, fileInfo, callback )"  + "(" + requestId + ", " );
	console.log( fileInfo );
	console.log( ", " );
	console.log( callback );
	console.log( ")");
	
	ws_sendFileName(requestId, fileInfo.name);
	
	var reader = new FileReader();
	reader.onloadend = function() {
		if( reader.error ){
			gl_setError( ERR_READ_FILE, reader.error );
			submit_fail(ERR_READ_FILE);
			return;
		}
		binaryString = reader.result;
		console.log("Byte:");
	    console.log(binaryString);
	    
	    ws_sendFileData(requestId, binaryString);
	    
	    ws_sendFileFinish( requestId, 1, callback );
	};
	reader.readAsArrayBuffer(fileInfo);
}
*/
/*
 * Send request create update services info
 */
function ws_deleteService( serviceId, callback ){    
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "deleteService";
	request.service = serviceId;
	send(request, callback);
}

/* 
 * { “requestID”: requesteID, “path”: ”src/test1.cpp” }
 * { “requestID”: requesteID, “data”: binary_data }
 * 
function ws_sendFile( requestId, fileInfo, callback ){
	console.log( "Bp: ws_sendFile( requestId, fileInfo, callback )"  + "(" + requestId + ", " );
	console.log( fileInfo );
	console.log( ", " );
	console.log( callback );
	console.log( ")");
	if( ! fileInfo ){
		alert("Error: ws_sendFile: file not read.");
		return;
	}
	
	/// { “requestID”: requesteID, “path”: ”src/test1.cpp” }
	var request = new Object();
	request.requestId = requestId;
	request.path = fileInfo.name;
	send(request);
	
	/// { “requestID”: requesteID, “data”: binary_data }
	var reader = new FileReader();
	reader.onloadend = function() {
		if( reader.error ){
			callback (ERR_READ_FILE, reader.error);
			return;
		}
		
		binaryString = reader.result;
		console.log("Byte:");
	    console.log(binaryString);
	    
	    var datamsg = new Object();
	    datamsg.requestId = requestId;
	    datamsg.data = binaryString;
	    send(datamsg);
	};
    reader.readAsBinaryString(fileInfo);
}*/

function ws_checkPathOfService ( path, callback ){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "checkPathOfService";
	request.path = path;
	send(request, callback);
}


/*
 * Get list of image of provider
 * Req: { "requestID" : requestID, "msg": "getImages", "isPublic":0/1, "isOwner":0/1, "isIncludeSample":0/1, "keyword":fileterKeyword}
 * Res: {
        "requestID" : requestID,
        "images" : [
            { "image" : imageId01, "isPublic" : 0/1, "url" : downloadUrl01, "name" : imageName01, "description" : imageDescription01,},
            { "image" : imageId02, "isPublic" : 0/1, "url" : downloadUrl02, "name" : imageName02, "description" : imageDescription02,},
            ...
        ]
        }
 */
function ws_findImages(isPublic, isOwner, isIncludeSample, keyword, callback){
    var request = new Object();
    request.requestId = genRequestID();
    request.msg = "findImages";
	request.isOwner = isOwner;
	request.isPublic = isPublic;
	request.isIncludeSample = isIncludeSample;
	request.keyword = keyword;
    send(request, callback);
}
/*
 * Check image name is usable
 * Req:
 * { "requestID" : requestID, "msg": "checkImageNameUsable",
		"class": 1/2/3 <-> image/job/service,
		"name": "nameString"
	}
 * Res: { "requestID" : requestID, "result" : resultId }
 *
 */
 function ws_checkImageNameUsable(nameStr, callback){
	var request = new Object();
    request.requestId = genRequestID();
    request.msg = "checkImageNameUsable";
	request.name = nameStr;
	send(request, callback);
}
/*
-> Client request : 
	{ "requestID" : requestID, "msg": "createImage",
		"name", imageName,
		"isPublic" : 0/1, 
		"filename" : localFileName,
		"description" : imageDescription
	}
<- Server response: 
	{ "requestID" : requestID, "result" : resultId }
*/
function ws_createImage(imageInfo, callback)
{
	var request = new Object();
    request.requestId = genRequestID();
    request.msg = "createImage";
	request.name = imageInfo.name;
	request.isPublic = imageInfo.isPublic;
	request.tarfile = imageInfo.tarfile.name;	
	request.description = imageInfo.description;
    send(request, callback);
}

/*
 * Update a image
 * Req:
 * { "requestID" : requestID, "msg": "updateImage",
		"image":"imageName"
		"isPublic" : 0/1,
		"description" : imageDescription
	}
 * Res: { "requestID" : requestID, "result" : resultId }
 */
function ws_updateImage(imageInfo, callback)
{
	var request = new Object();
	request.requestId = genRequestID();
    request.msg = "updateImage";
	for (var attrname in imageInfo) { 
		request[attrname] = imageInfo[attrname]; 
	}
    send(request, callback);
}

function ws_deleteImage(imageId, callback)
{
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "deleteImage";
	request.image = imageId;
	send(request, callback);
}

/*
 -> Client request : 
	{ "requestID" : requestID, "msg": "registerProvider", "phone": "phoneNumer", "address":"address", "kdeskacc":"kdeskaccname", "description":"notes" }
 <- Server response: 
	{ "requestID" : requestID, "result" : resultId }
 */
function ws_registerProvider(regObj, callback){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "registerProvider";
	request.phone = regObj.phone;
	request.address = regObj.address;
	request.kdeskacc = regObj.kdeskacc;
	request.description = regObj.description;
	send(request, callback);
}

/*
 * Get list of provider register
 * Req: { "requestID": requestID, "msg": "getProviderRegisters" }
 * Res: {
    "requestID" : requestID,
    "registers" : [
        { "user" : userEmail01, "phone": "phoneNumber", "address":"", "description" : providerDescription },
        { "user" : userEmail01, "phone": "phoneNumber", "address":"", "description" : providerDescription },
        ...
    ]
    "
}
 */
function ws_getProviderRegisters(callback){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "getProviderRegisters";
	send(request, callback);
}

/*
 * Get list of provider register
 * Req: { "requestID": requestID, "msg": "getProviderRegisters" }
 * Res: {
    "requestID" : requestID,
    "registers" : [
        { "user" : userEmail01, "phone": "phoneNumber", "address":"", "description" : providerDescription },
        { "user" : userEmail01, "phone": "phoneNumber", "address":"", "description" : providerDescription },
        ...
    ]
    "
}
 */
function ws_getProviders(callback){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "getProviders";
	send(request, callback);
}

/*
 * Get list of provider register
 * Req: { "requestID": requestID, "msg": "rejectProviderRegisters"
          "registers" : [
                "email" : userEmail01,
                "email" : userEmail01,
                ...
        ]}
 * Res: {
    "requestID" : requestID,
    "result" : ""}
 */
 
function ws_rejectProviderRegisters(registers, callback){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "rejectProviderRegisters";
	request.registers = registers;
	send(request, callback);
}

function ws_rejectProviderRegister(email, callback){
	var register = new Object();
	register.email = email;
	var registers = new Array();
	registers[0] = register;
	ws_rejectProviderRegisters(registers, callback);
}

/*
 * Get list of provider register
 * Req: { "requestID": requestID, "msg": "acceptProviderRegisters"
          "registers" : [
                "email" : userEmail01,
                "email" : userEmail01,
                ...
        ]}
 * Res: {
    "requestID" : requestID,
    "result" : ""}
 */
function ws_acceptProviderRegisters(registers, callback){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "acceptProviderRegisters";
	request.registers = registers;
	send(request, callback);
}

function ws_acceptProviderRegister(email, callback){
	var register = new Object();
	register.email = email;
	var registers = new Array();
	registers[0] = register;
	ws_acceptProviderRegisters(registers, callback);
}
/*
 * Get list of provider register
 * Req: { "requestID": requestID, "msg": "rejectProviders"
          "providers" : [
                "email" : userEmail01,
                "email" : userEmail01,
                ...
        ]}
 * Res: {
    "requestID" : requestID,
    "result" : ""}
 */
function ws_rejectProviders(providers, callback){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "rejectProviders";
	request.providers = providers;
	send(request, callback);
}

function ws_rejectProvider(email, callback){
	var provider = new Object();
	provider.email = email;
	var providers = new Array();
	providers[0] = provider;
	ws_rejectProviders(providers, callback);
}

function ws_updateTimeUsable(email, timeUsable, callback){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "updateProviderUsableTime";
	request.email = email;
	request.timeUsable = timeUsable;
	send(request, callback);
}

/*
 *  Remove register provider
-> Client request :
	{ "requestID" : requestID, "msg": "rejectProvider"}
<- Server response:
	{ "requestID" : requestID, "result" : resultId }
 *
 */
function ws_removeRegister(callback){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "rejectProvider";
	send(request, callback);
}

/*
 * Get resource usage & usable time
 * -> Client request : 
 *	{ "requestID" : requestID, "msg": "getProviderTimeResource" }
 * <- Server response: 
 *	{ "requestID" : requestID, "unit":{Ms, s, h},"total" : total_time, "usage" : usage_time, "remain":remain_time }
 *
 */
 function ws_getProviderTimeResource(callback){
	var request = new Object();
	request.requestId = genRequestID();
	request.msg = "getProviderTimeResource";
	send(request, callback);
 }
 
