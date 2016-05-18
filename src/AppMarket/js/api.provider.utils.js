
/*
 * Handler change parameter type 
 */
$( document ).on( "change", "#ppp-type", function( e ){
	var defaultId = $(this).attr( 'data-default' ),
	defaultTag = $.mobile.activePage.find( "input#" + defaultId ),
	attrs;
	//alert("defaultId = " + defaultId );
	var attrs;
	if( defaultTag ){
		attrs = pu_attrsByType($( this ).val());
		/*
		switch( $( this ).val() ){
		case "bool":
			type = "text";
			pattern = "true|false|0|1";
			title = "Values: true|false|0|1";
			break;
		case "byte":
			type = "number";
			pattern = "^[-]{0,1}[1-9][0-9]{0,2}";
			title = "Values: (-128 to 127)";
			break;
		case "i16":
			type = "number";
			pattern = "^[-]{0,1}[1-9][0-9]{0,3}";
			title = "Values: (-32,768 to 32,787)";
			break;
		case "i32":
			type = "number";
			pattern = "^[-]{0,1}[1-9][0-9]{0,9}";
			title = "Values: (-2,147,483,684 to 2,147,483,683)";
			break;
		case "i64":
			type = "number";
			pattern = "^[-]{0,1}[1-9][0-9]{0,3}";
			title = "Values: (-9,223,372,036,854,775,808 to +9,223,372,036,854,775,807)";
			break;
		case "double":
			type = "number";
			pattern = "^[-]{0,1}[1-9][0-9]{0,3}";
			title = "Values: (1.7E +/- 308 (15 digits))";
			break;
		case "string": 
			type = "text";
			pattern="[^><]*"; 
			title="without [><] characters";
			break;
		case "file":
//			/*type = "text";
//			pattern = "";
//			title = "character ";
//			break;
		default:
			//defaultTag.addClass('ui-disabled');
			//defaultTag.addAttr("disabled");
			//defaultTag.prop("disabled", false);
			defaultTag.textinput("disable");
			break;
		}
		*/
		//alert("change type = " + type + ", pattern = " + pattern + ", title = " + title );
		defaultTag.textinput("enable");
		if( Object.size(attrs) == 0 ) {
			defaultTag.textinput("disable");
		}
		
		if ( attrs.type ) {
			defaultTag.attr("type", attrs.type);
		}
		if ( attrs.pattern ){
			defaultTag.attr("pattern", attrs.pattern);
		}
		if ( attrs.title ){
			defaultTag.attr("title", attrs.title);
		}
		console.log("change type = " + defaultTag.attr("type") + ", pattern = " + defaultTag.attr("pattern") + ", title = " + defaultTag.attr("title") );
	} else {
		console.error("defaultTag not found");
	}
});

function pu_attrsByType( paramType ){
	var attrs = {};
	switch( paramType ){
	case "bool":
		attrs.type = "text";
		attrs.pattern = "true|false|0|1";
		attrs.title = "Values: true|false|0|1";
		break;
	case "byte":
		attrs.type = "number";
		attrs.pattern = "^[-]{0,1}[1-9][0-9]{0,2}";
		attrs.title = "Values: (-128 to 127)";
		break;
	case "i16":
		attrs.type = "number";
		attrs.pattern = "^[-]{0,1}[1-9][0-9]{0,3}";
		attrs.title = "Values: (-32,768 to 32,787)";
		break;
	case "i32":
		attrs.type = "number";
		attrs.pattern = "^[-]{0,1}[1-9][0-9]{0,9}";
		attrs.title = "Values: (-2,147,483,684 to 2,147,483,683)";
		break;
	case "i64":
		attrs.type = "number";
		attrs.pattern = "^[-]{0,1}[1-9][0-9]{0,3}";
		attrs.title = "Values: (-9,223,372,036,854,775,808 to +9,223,372,036,854,775,807)";
		break;
	case "double":
		attrs.type = "number";
		attrs.pattern = "^[-]{0,1}[1-9][0-9]{0,3}";
		attrs.title = "Values: (1.7E +/- 308 (15 digits))";
		break;
	case "string": 
		attrs.type = "text";
		attrs.pattern="[^><]*"; 
		attrs.title="without [><] characters";
		break;
	case "file":
		/*type = "text";
		pattern = "";
		title = "character ";
		break;*/
	default:
		//defaultTag.addClass('ui-disabled');
		//defaultTag.addAttr("disabled");
		//defaultTag.prop("disabled", false);
		//defaultTag.textinput("disable");
		break;
	}
	return attrs;
}
/*
 * Handler text change / clear form event
 */
$( document ).on( "click", ".ppp-button", function( e ){
	//$.mobile.activePage.find(".ppp-button").button('disable');
});


$( document ).on( "change", ".ppp-input", function( e ){
	$.mobile.activePage.find(".ppp-button").button('enable');
});

/*
 * Hanler collapse  of provider-collapsible
 */
$(document).on( "collapse", ".provider-collapsible", function( event, ui ){
    //alert('Close');
    
});
$(document).on( "expand", ".provider-collapsible", function( e, ui ){
	/*if( e.handled !== true ) // This will prevent event triggering more then once
    {
        console.log('expand = '+  $(this).attr('id'));
		e.handled = true;
		return;
    }*/
	if( $.mobile.activePage ) {
		$.mobile.activePage.find(".provider-collapsible").attr('data-collapsed', "true");
		$(this).attr('data-collapsed', "false");
	}
});

/*
 * Handler img is error: not found src image
 */
$(document).ready(function () {
    $('img').on("error", function(){
    	//alert( "Handler for .error() called." );
    	$(this).attr( "src", "images/no-image.png" );
    });
});


/*
 * ****************************************************
 * 		Handler create, edit, delete param
 * ****************************************************
 */
//var gl_deleteParamCallback = {}; // Callback function is call when Delete (on delete popup) button is clicked

/*
 * Refresh preview textbox
 */

function pu_refreshCommandPreview( txtPreview, txtPath, service ){
	//console.log(" pu_refreshCommandPreview( txtPreview, txtPath, service )" + txtPreview + ", " +  txtPath );
	var path = $.mobile.activePage.find("input#" + txtPath);
	var preview = $.mobile.activePage.find( "textarea#" + txtPreview );
	//alert( "input#" + txtPath + " = " +  path.val() );
	if ( ( ! service ) || ( ! path ) || ( ! preview ) || ( ! service.params )){
		console.error("path or service not found.");
		return;
	}
	var pathStr = uString_escapePath( path.val() );
	var cmd = ( pathStr !== "" ) ? ( pathStr + " " ) : "";
	for( var paramid in service.params ) {
		var param = service.params[paramid];
		if ( param.action && param.action == "deleteparam"){
			continue;
		}
		cmd += ( param.prefix ? param.prefix : "" ) + '<' + ( param.name ? param.name : "" ) + '> ';
	}
	//alert("pu_refreshCommandPreview: cmd " + cmd);
	preview.val( cmd );
	
	preview.trigger('change');
}


/*
 * Refresh parameter table
 */
function pu_refreshParamTable(from, tableName, params ){
	//alert("pu_refreshParamTable");
	var table = $.mobile.activePage.find("table#" + tableName + " tbody" );
	
	/// Clear table
	table.empty();
	
	/// Create rows
	var rows = "",
		noparam = 0; //Object.size(params);
	
	for( paramid in params ) {
		//console.log(paramid + " = " + params[paramid]);
		var param = params[paramid];
		//console.log(param);
		if ( param.action && param.action == "deleteparam"){
			continue;
		}
		rows += '	\
				<tr>	\
				    <td style="text-align:left;vertical-align:middle"><p data-rel="external" class="long-name" >' + ( param.name ? param.name : "" ) + '</p></td>	\
				    <td style="text-align:left;vertical-align:middle">' + ( param.type ? param.type : "" ) + '</td>	\
				    <td style="text-align:center;vertical-align:middle"><a class="provider-editparam-btn" data-paramid="' + ( param.id ? param.id : "" ) + '" data-from="' + from + '" data-role="button" data-rel="popup" data-icon="edit" data-iconpos="notext" data-theme="c">Edit</a></td>	\
				    <td style="text-align: center;vertical-align:middle"><a class="provider-deleteparam-btn" data-paramid="' + ( param.id ? param.id : "" ) + '" data-from="' + from + '" data-rel="popup" data-role="button" data-icon="delete" data-iconpos="notext" data-theme="e" data-transition="pop"  data-position-to="window">Delete</a></td>	\
				</tr>	\
		';
		noparam ++ ;
	}
	if( noparam == 0 ){
		rows += '	\
				<tr>	\
				    <td>(No parameter)</td>	\
				    <td></td>	\
				    <td></td>	\
				    <td></td>	\
				</tr>	\
				';
	}
	
	/// Add last row
	rows += '	\
			<tr>	\
			    <td><a class="provider-addparam-btn" href="#provider-newparam-popup" data-role="button" data-rel="popup" data-icon="add" data-inline="true" data-mini="true" data-position-to="window" >Add</a></td>	\
			    <td></td>	\
			    <td></td>	\
			    <td></td>	\
			</tr>	\
	';
	//console.log("rows: " + rows);
	
	/// Append rows & refresh table
	table
	// Append the new rows to the body
    .append( rows )
    // Call the refresh method
    .closest( "table#" + tableName )
    .table()
    .table( "refresh" )
    // Trigger if the new injected markup contain links or buttons that need to be enhanced
    .trigger( "create" );
	
	return true;
}

/*
 * Generate param id
 */

function pu_genParamId( params ){
	var newId = 0;
	for( var paramid in params ) {
		if ( parseInt( paramid ) >= newId ){
			newId = parseInt( paramid );
		}
	}
	var newIdStr = ( parseInt ( newId ) + 1 ).toString();		
	while(newIdStr.length < 4)
	{
		newIdStr = "0" + newIdStr;
	}
	console.log("newIdStr = " + newIdStr);
	return newIdStr;
}

/*
 * Generate preview parameter
 */

$(document).on("change", ".ppp-cmd-element", function(e) {
	///This will prevent event triggering more then once
	/*if(e.handled !== true) {
        e.handled = true;
        return true;
    }*/
	var previewid = $(this).attr("data-preview"),
		nameid = $(this).attr("data-name"),
		prefixid = $(this).attr("data-prefix");
	
	if( ! previewid ){
		console.err( " previewid is emtyp.");
		return true;
	}
	
	if( ! nameid ){
		nameid = $(this).attr("id");
	}
	
	if( ! prefixid ) {
		prefixid = $(this).attr("id");
	}
	
	//console.log("previewid = " + previewid + ", nameid = " + nameid + ", prefixid = " + prefixid );
	var prefix = $.mobile.activePage.find( "#" + prefixid ).val(),
		name = $.mobile.activePage.find( "#" + nameid ).val();
	//console.log("preview cmd = " + prefix  + "<" + name + ">" );
	$.mobile.activePage.find( "#" + previewid ).val( pu_genParamPreview ( prefix, name ) );
	//$.mobile.activePage.find( "#" + previewid ).val( prefix  + ( name ? ("<" + name + ">") : "" ) ).trigger('change');
});

function pu_genParamPreview( prefix , name ){
	return ( prefix ? prefix : "" ) + ( name ? ("<" + name + ">") : "" );
}

/*
 * Click delete button in parameter table
 */
$(document).on( "click", '.provider-deleteparam-btn', function(){
	//alert("click");
	var paramId = $(this).attr( 'data-paramid' ),
		paramidStr = paramId ? ( ' data-paramid="' + paramId + '" ') : "",
		from =  $(this).attr( 'data-from' ),
		fromStr = from ? ( ' data-from="' + from + '" ') : "";
	//alert( paramidStr + ", " + fromStr);
	var type = "parameter",
	popUp = '\
				<div data-role="popup" id="provider-deleteparam-popup" data-overlay-theme="a" data-theme="c" data-dismissible="false" style="max-width:400px;" class="ui-corner-all">	\
					<div data-role="header" data-theme="e" class="ui-corner-top">	\
					    <h1>Delete ' + type + ' ?</h1>	\
					</div>	\
					<div data-role="content" data-theme="d" class="ui-corner-bottom ui-content">	\
					    <h3 class="ui-title">Are you sure you want to delete this  ' + type + ' ?</h3>	\
					    <p>This action cannot be undone.</p>	\
					    <a data-role="button" data-inline="true" data-rel="back" data-theme="c">Cancel</a>	\
					    <a id="provider-deleteparam-yes-btn" ' + paramidStr + fromStr + 'data-role="button" data-inline="true" data-rel="back" data-transition="flow" data-theme="b">Delete</a>	\
					</div>	\
				</div>	\
	';
	//console.log("popUp: " + popUp);
	$.mobile.activePage.append( popUp ).trigger( "pagecreate" );
	
	/// Release after close popup
	$( "#provider-deleteparam-popup" ).bind({   
		popupafterclose: function(event, ui) {	$( this ).remove();}
	});
	
	/// Show popup
	$ ( "#provider-deleteparam-popup" ).popup("open");
});

/*
 * Process delete parameter in table
 */
$(document).on( "click", '#provider-deleteparam-yes-btn', function(e){
	/*///This will prevent event triggering more then once
	if(e.handled !== true) {
        e.handled = true;
        return true;
    }*/
	console.log("Delete row event");;
	
	var paramId = $(this).attr( 'data-paramid' ),
	from = $(this).attr( 'data-from' );
	
	console.log('paramId = ' + paramId + ', from = ' + from );
	
	if ( paramId && from ) {
		switch( from ){
		case "newapp":
			provider_deleteParamCallback(paramId);
		break;
		case "editapp":
			papp_deleteParamCallback(paramId);
		break;
		default:
			console.err("Unknow from attribute.");
		break;
		}
		
		
		//gl_deleteParamCallback[ from ](paramId);
		/*console.log('providerObj.service.params[ paramId ]: ' + providerObj.service.params[ paramId ]);
		delete providerObj.service.params[ paramId ];
		console.log('providerObj.service.params[ paramId ]: ' + providerObj.service.params[ paramId ]);
		
		provider_refreshParamTable( providerObj.service.params );*/
		return;
	} else {
		console.err("Error: ParamID, From attr is not found");
	}
});

var gl_paramTypes = { "bool":1, "byte":1, "i16":1, "i32":1, "i64":1, "double":1, "string":1, "file":1 };
//var gl_paramPopupCallback;	/// Callback function is called when Done button is clicked

$(document).on( "click", '.provider-editparam-btn', function(e){
	///This will prevent event triggering more then once
	/*if(e.handled !== true) {
		alert("click1");
        e.handled = true;
        return true;
    }*/
	var paramId = $(this).attr( 'data-paramid' ),
	from = $(this).attr( 'data-from' );
	
	console.log("provider-editparam-btn: Click: ParamId = " + paramId + ", from = " + from );
	var param;
	switch( from ){
	case "editapp":
		console.log("pappObj.service.params");
		console.log(pappObj.service.params)
		param = pappObj.service.params[ paramId ];
		break;
	case "newapp":
		param = providerObj.service.params[ paramId ];
		break;
	default:
		console.error("Error: Unknow from tag.");
		break;
	}
	//providerObj.service.params[ paramId ];
	//console.log(param);
	
	/*param.type = "i64";
	param.isRequired = true;
	param.isOutput = true;
	param.prefix = "--size";
	param.descript = "Mean of parameter \n end line";
	param.defaultValue = "100";*/
	var attrs = pu_attrsByType(param.type);
	preview = param.preview ? param.preview : ( pu_genParamPreview( param.prefix , param.name ) );
	var popUp = ' \
	<div data-role="popup" id="provider-editparam-popup" data-theme="b" class="ui-corner-all">	\
		<a href="#" data-rel="back" data-role="button" data-theme="c" data-icon="delete" data-iconpos="notext" class="ui-btn-right">Close</a>	\
	    <form class="provider-param-form"  data-ajax="false" enctype="multipart/form-data"> 	\
			<input type="hidden" name="ppp-action" value="editparam" >	\
			<input type="hidden" name="ppp-from" value="' + from + '" >	\
			<input type="hidden" name="ppp-id" value="' + paramId + '" >	\
	        <div style="padding:10px 20px;">	\
	            <h3>Edit parameter</h3>	\
	            <label for="ppp-name" class="ui-hidden-accessible">Name:</label>	\
	            <input type="text" name="ppp-name" id="ppep-name" class="ppp-cmd-element ppp-input" data-preview="ppep-preview" data-prefix="ppep-prefix" value="' + ( param.name ? param.name : "" ) + '" placeholder="name" pattern="[^><=\\\!\@\#\$\%\&\?|]*" title="without [><=\!@#$%&?] character" required data-clear-btn="true" maxlength="80">	\
	            <label for="ppp-type" class="ui-hidden-accessible ppp-input">Type:</label>	\
				<select class="ppp-input" name="ppp-type" id="ppp-type" data-default="ppep-default-value"  data-mini="true">	\
					<option value="">Choose type...</option>	\
				    <option value="bool"' + (( gl_paramTypes[ param.type ] && param.type == "bool" ) ? 'selected="selected"' : ""  )+ '>bool</option>	\
				    <option value="byte"' + (( gl_paramTypes[ param.type ] && param.type == "byte" ) ? 'selected="selected"' : "" ) + '>byte</option>	\
				    <option value="i16"' + (( gl_paramTypes[ param.type ] && param.type == "i16" ) ? 'selected="selected"' : "" ) + '>i16</option>	\
				    <option value="i32"' + (( gl_paramTypes[ param.type ] && param.type == "i32" ) ? 'selected="selected"' : "" ) + '>i32</option>	\
				    <option value="i64"' + (( gl_paramTypes[ param.type ] && param.type == "i64" ) ? 'selected="selected"' : "" ) + '>i64</option>	\
				    <option value="double"' + (( gl_paramTypes[ param.type ] && param.type == "double" ) ? 'selected="selected"' : "" ) + '>double</option>	\
				    <option value="string"' + (( gl_paramTypes[ param.type ] && param.type == "string" ) ? 'selected="selected"' : "" ) + '>string</option>	\
				    <option value="file"' + (( gl_paramTypes[ param.type ] && param.type == "file" ) ? 'selected="selected"' : "" ) + '>file</option>		\
				</select>	\
				<label for="ppep-default-value" class="ui-hidden-accessible">Default value:</label>	\
	            <input type="' + ( attrs.type ? attrs.type : "text" ) 
	            	+ '" class="ppp-input" name="ppp-default-value" id="ppep-default-value" ' 
	            	+ '  value="' + ( param.defaultValue ? param.defaultValue : "") 
	            	+ '" pattern="' + ( attrs.pattern ? attrs.pattern : "" ) 
	            	+ '" title="' + ( attrs.title ? attrs.title : "" ) 
	            	+ '" placeholder="default value">	\
	            <label for="ppp-descript" class="ui-hidden-accessible"><strong>Description:</strong></label>	\
	        	<textarea cols="20" rows="40" class="ppp-input" name="ppp-descript" id="ppp-descript" placeholder="description" >' + ( param.descript ? param.descript : "" )  + '</textarea>	\
	            <fieldset data-role="controlgroup" data-mini="true">	\
					<legend><b>Attributes:</b></legend>	\
					<input type="checkbox" class="ppp-input" name="ppp-required" id="ppp-required" ' + ( param.isRequired ? 'checked="true"' : "" ) + '>	\
			        <label for="ppp-required" >Required</label>	\
			        <input type="checkbox" class="ppp-input" name="ppp-output" id="ppp-output" ' + ( param.isOutput ? 'checked="true"' : "" ) + '>	\
			        <label for="ppp-output">Output</label>	\
		        </fieldset>	\
		        <label for="ppp-prefix"><strong>Prefix:</strong></label>	\
		        <input type="text" class="ppp-cmd-element ppp-input" name="ppp-prefix" id="ppep-prefix" class="ppp-cmd-element" data-name="ppep-name" data-preview="ppep-preview" data-prefix="ppep-prefix" value="' + ( param.prefix ? param.prefix : "" ) + '" placeholder="ex: --size" pattern="[A-Za-z\-._0-9 ]*" title="Only support [A-Za-z\-._0-9 ] character">\
	            <div class="submit-form-border">	\
		            <label for="ppp-preview" class="ui-hidden-accessible"><strong>Preview:</strong></label>	\
		        	<textarea cols="20" rows="10" name="ppp-preview" id="ppep-preview" placeholder="preview" readonly>' + preview + '</textarea>	\
		        	<button class="ppp-button" type="submit" data-icon="check" data-inline="true" disabled>Done</button>	\
		        	<button class="ppp-button" type="reset" data-icon="delete" data-inline="true" disabled>Undo</button>	\
	        	</div>	\
	        </div>	\
	    </form>	\
	</div>	\
    ';
	/// Create the popup. Trigger "pagecreate" instead of "create" 
	/// because currently the framework doesn't bind the enhancement of toolbars 
	/// to the "create" event (js/widgets/page.sections.js)
	$.mobile.activePage.append( popUp ).trigger( "pagecreate" );
	
	/// Release after close popup
	$( "#provider-editparam-popup" ).bind({   
		popupafterclose: function(event, ui) {	$( this ).remove();}
	});
	/// Show popup
	$ ( "#provider-editparam-popup" ).popup("open");
});

/*
 * Submit new parameter
 */
//$(document).on('submit', '#provider-paramnew-form', function( e ){
$(document).on('submit', '.provider-param-form', function( e ){

	/*///This will prevent event triggering more then once
	if(e.handled !== true) {
        e.handled = true;
        return false;
    }*/
	
	/// Prevent default submit  
	e.preventDefault();
	console.log("Bp: Submit job occur !");
	
	/// Parse parameter submit
	dataString = $(this).serialize();
	console.log( "Bp: onSubmit: " + dataString );
	var serArr = $(this).serializeArray(),
		from = "",
		param = {}
		;
	for ( var i in serArr ){
		var value = serArr[ i ].value;
		
		if( ! value ) {
			continue;
		}
		//console.log("name = " + serArr[ i ].name + ", value = " + value);
		switch ( serArr[ i ].name ){
		case "ppp-action":
			param.action = value;
			break;
		case "ppp-from":
			from = value;
			break;
		case "ppp-id":
			param.id = value;
			break;
		case "ppp-name":
			param.name = value;
			break;
		case "ppp-type":
			param.type = value;
			break;
		case "ppp-default-value":
			param.defaultValue = value;
			break;
		case "ppp-required":
			param.isRequired = true;
			break;
		case "ppp-output":
			param.isOutput = true;
			break;
		case "ppp-prefix":
			param.prefix = value;
			break;
		case "ppp-descript":
			param.descript = value;
			break;
		case "ppp-preview":
			param.preview = value;
			break;
		}
		if( serArr[ i ].name ==  "action" ){
			runMethodId = value;
			break;
		}
	}
	switch(from){
	case "newapp":
		this.reset();
		provider_addEditParamCallback(param);
		break;
	case "editapp":
		this.reset();
		//alert("eddit app")
		papp_addEditParamCallback(param);
		break;
	default:
		console.log("Error: from attr unknow.");
		break;
	}
	$.mobile.activePage.find(".ppp-button").button('disable');
	
/*	switch (action){
	case "paramnew":
		//console.log("param");
		//console.log(param);
		/// Generate param
		param.id = provider_genParamId( providerObj.service.params );
		//providerObj.service.params[param.id] = param;
		//console.log( "providerObj.service.params = ");
		//console.log( providerObj.service.params );
		/// Refresh table after close popup
		//$( "#provider-paramnew-popup" ).unbind(popupafterclose);
		$( "#provider-paramnew-popup" ).unbind().bind({   
			popupafterclose: function(event, ui) {
				//provider_refreshParamTable(providerObj.service.params);
				//provider_refreshCommandPreview(providerObj.service);
				gl_paramPopupCallback(param);
				}
		});
		
		$ ( "#provider-paramnew-popup" ).popup("close");
		//provider_refreshParamTable(providerObj.service.params);
		//provider_refreshCommandPreview(providerObj.service);
		break;
	case "editparam":
		console.log("param = ");
		console.log(param);
		providerObj.service.params[param.id] = param;
		$ ( "#provider-editparam-popup" ).popup("close");
		gl_paramPopupCallback(param);
		break;	
	default:
		console.error("Unkow action !");
		break;
	}*/
	
	
	/// Return true to prevent default submit
	//return true;
	return false;
});

$(document).on('reset', '.provider-param-form', function( e ){
	$.mobile.activePage.find(".ppp-button").button('disable');
});

/*
 * Create service description xml file
 */

function pu_serviceToXml( service ){
	function paramsToXML(params){
		var pxml = "";
		for( var paramid in params ) {
			var param = params [paramid];
			pxml += '\
				<Param ' + ( service.isRequired ? 'isRequired="true"' : '' ) + ' ' + ( service.isOutput ? 'isOutput="true"' : "" ) + '>\
					<Id>' + ( param.id ? param.id : "" ) + '</Id>\
					<Description>' + ( param.descript ? param.descript : "" ) + '</Description>\
					<Type>' + ( param.type ? param.type : "" ) + '</Type>\
					' + ( param.prefix ? ('<Prefix>' + param.prefix + '</Prefix>') : "" ) + '\
				</Param>\
				';
		}
		return pxml;
	}
	///<Service name="' + ( service.name ? service.name : "" ) + '" producer="' + ( service.producer ? service.producer : "" ) + '">\
	var xml = '\
		<Service name="' + ( service.name ? service.name : "" ) + '">\
			<Description>' + ( service.descript ? uString_escapeHtml( service.descript ) : "(No description)" ) + '</Description>\
			<Id>' + ( service.id ? service.id : "" ) + '</Id>\
			<Path>' + ( service.path ? service.path : "" ) + '</Path>\
			<Icon>' + ( (service.icon && service.icon.name) ? service.icon.name : "" ) + '</Icon>\
			<MaxElapsedTime>' + ( service.maxElapsedTime ? service.maxElapsedTime : "" ) + '</MaxElapsedTime>\
			<NoNodes>' + ( service.noNodes ? service.noNodes : "" ) + '</NoNodes>\
			<Parameters>\
				' + paramsToXML(service.params) + '\
			</Parameters>\
		</Service>';
	return xml;
}

/*
 * Parse service description xml file to service object
 */
function pu_xmlToService( methodFile ){
	var service = {};
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
	
	/// Retrieve data
	if( xml.children('Service') ){
		if( xml.children('Service').attr('name') ){
			service.name = xml.children('Service').attr('name');
		}
		if( xml.children('Service').attr('producer') ){
			service.producer = xml.children('Service').attr('producer');
		}
		if( xml.children('Service').attr('provider') ){
			service.producer = xml.children('Service').attr('provider');
		}
		var serviceXml = xml.children('Service');
		if ( serviceXml.children("Id") ){
			service.id = serviceXml.children('Id').text();
		}
		if ( serviceXml.children("Path") ){
			service.path = serviceXml.children('Path').text();
		}
		if ( serviceXml.children("Icon") ){
			service.icon = {};
			service.icon.name = serviceXml.children('Icon').text();
		}
		if ( serviceXml.children("MaxElapsedTime") ){
			service.maxElaptedTime = serviceXml.children('MaxElapsedTime').text();
		}
		if ( serviceXml.children("NoNodes") ){
			service.noNodes = serviceXml.children('NoNodes').text();
		}
		if ( serviceXml.children("Description") ){
			service.descript = serviceXml.children('Description').text();
		}
		if ( serviceXml.children("Parameters") ){
			var paramsXml = serviceXml.children('Parameters');
			service.params = {};
			
			$( paramsXml ).find( "Param" ).each(function(){
				var param = {};
					paramXml = $(this);
				if( paramXml.attr( "isRequired" ) && paramXml.attr( "isRequired" ) == "true" ){
					param.isRequired = true;
				}
				if( paramXml.attr( "isOutput" ) && paramXml.attr( "isOutput" ) == "true" ){
					param.isOutput = true;
				}
				if( paramXml.children('Id') ){
					param.id = paramXml.children('Id').text();
				}
				if( paramXml.children('Description') ){
					param.descript = paramXml.children('Description').text();
				}
				if( paramXml.children('Type') ){
					param.type = paramXml.children('Type').text();
				}
				if( paramXml.children('Prefix') ){
					param.prefix = paramXml.children('Prefix').text();
				}
				service.params[param.id] = param;
			});
		}
	}else{
		console.err("puService_xmlToService( methodFile ): Service tag not found.");
	}
	return service;
}

/*
 * Parse service to json
 */
function pu_serviceToJson( service ){
	var json = new Object();
	json.name = service.name;
	//json.producer = service.producer;
	json.path = service.path;
	json.icon = ( service.icon && service.icon.name ) ? service.icon.name : "";
	json.maxElaptedTime = service.maxElaptedTime;
	json.noNodes = service.noNodes;
	json.descript = service.descript;
	
	return json;
}
//
///*
// * Load image when 
// */
//function pu_showImageURL( input, output, getDataCallback ) {
//    if (input.files && input.files[0]) {
//        var reader = new FileReader();
//
//        reader.onload = function (e) {
//            $( '#' + output ).attr( 'src', e.target.result );
//                //.width(150)
//                //.height(200);
//            console.log(" e.target.result = ");
//            console.log( e.target.result );
//            if( getDataCallback ){
//            	getDataCallback(e.target.result);
//            }
//        };
//
//        reader.readAsDataURL(input.files[0]);
//    }    
//}

/*
 * Backup data provider pages
 */
function pu_backupData() {
	console.log("pu_backupData()");
	//papp_backupData();
	//provider_backupData();
	if( pappObj != undefined ){
		sessionStorage.setItem ( "pappObj", JSON.stringify( pappObj ) );
	}
	if( providerObj != undefined ){
		sessionStorage.setItem ( "providerObj", JSON.stringify( providerObj ) );
	}
	if( padetailObj != undefined ){
		sessionStorage.setItem ( "padetailObj", JSON.stringify( padetailObj ) );
	}
	/*	console.log(pappObj);
	console.log(JSON.stringify( pappObj ));
	console.log(padetailObj);
	console.log(JSON.stringify( padetailObj ));
	console.log(providerObj);
	console.log(JSON.stringify( providerObj ));
	alert("pu_backupData");*/
}

function pu_restoreData() {
	console.log("pu_restoreData()");
	//papp_restoreData();
	//provider_restoreData();
	pappObj = JSON.parse( sessionStorage.getItem ( "pappObj" ) );
	if( pappObj.service 
			&& pappObj.service.iconfile 
			&& pappObj.service.iconfile.name == undefined 
			&& pappObj.service.icontag ){
		var files = $( "#" + pappObj.service.icontag ).prop("files");
		//console.log("files");
		//console.log(files);
		if( files.length > 0 ){
			pappObj.service.iconfile = files[0];
		} else {
			console.log("pu_restoreData: pappObj.service.iconfile error get file");
		}
	}
	providerObj = JSON.parse( sessionStorage.getItem ( "providerObj" ) );
	if( providerObj.service.iconfile 
			&& providerObj.service.iconfile.name == undefined 
			&& providerObj.service.icontag ){
		var files = $( "#" + providerObj.service.icontag ).prop("files");
		//console.log("files");
		//console.log(files);
		if( files.length > 0 ){
			providerObj.service.iconfile = files[0];
		} else {
			console.log("pu_restoreData: providerObj.service.iconfile  error get file");
		}
	}

	padetailObj = JSON.parse( sessionStorage.getItem ( "padetailObj" ) );
	if( padetailObj && padetailObj.service && padetailObj.service.iconfile 
			&& padetailObj.service.iconfile.name == undefined 
			&& padetailObj.service.icontag ){
		var files = $( "#" + padetailObj.service.icontag ).prop("files");
		//console.log("files");
		//console.log(files);
		if( files.length > 0 ){
			padetailObj.service.iconfile = files[0];
		} else {
			console.log("pu_restoreData: providerObj.service.iconfile error get file");
		}
	}

	
}


/*
 * Backup / restore data
 */
/*function papp_backupData(){
	console.log( " papp_backupData() ");
	sessionStorage.setItem ( "pappObj", JSON.stringify(pappObj) );
}

function papp_restoreData(){
	console.log( " papp_restoreData() ");
	pappObj = JSON.parse( sessionStorage.getItem ( "pappObj" ) );
	console.log("pappObj = ");
	console.log( pappObj );
}*/

/*
 * Set F5 flag
 */
function pu_setF5(){
	providerObj.f5 = true;
	pappObj.f5 = true;
	padetailObj.f5 = true;
}

function pu_loadTimeResources(callback){
	ws_getProviderTimeResource(function(res){
		document.getElementById("pt-remain").innerHTML = res.usage + " (" + res.unit + ")";
		document.getElementById("pt-total").innerHTML = res.total + " (" + res.unit + ")";
		if(callback != undefined)
			callback();
	});
}
