var imagesObj = new Object();
imagesObj.currentTab = "newimages";
imagesObj.resFindImages;
imagesObj.images = new Object();
imagesObj.selectedImageIndex;
imagesObj.baseImages = new Object();
imagesObj.selectedBaseImageIndex;
function appendImagesToList()
{
	$("#docker-images").append("<hr style=\"width:100%;float:left;\">"
						+		"<div data-role=\"fieldcontain\">"
						+		"	<label for=\"pinf-imagename\">Docker image Name</label>"
						+		"	<input type=\"text\" data-clear-btn=\"true\" name=\"pinf-imagename\" id=\"pinf-imagename\" value=\"\" pattern=\"[^><=\\\!\#@\$\%\&\?|]*\" title=\"without [><=\!#@$%&?] character\" required maxlength=\"264\">"
						+		"</div>"
						+		"<div data-role=\"fieldcontain\">"
						+			"<label for=\"pinf-upload\" >Upload Image</label>"
						+			"<input type=\"file\" data-clear-btn=\"true\" name=\"pinf-upload\" id=\"pinf-upload\"  accept=\"image/*\" value=\"\" required>"
						+			"<div style=\"margin-left:17%;\">"
						+				"<label>"
						+					"<input type=\"checkbox\" >Public"
						+				"</label>"
						+			"</div>"
						+		"</div>"
						+		"<div data-role='fieldcontain'>"
						+		"	<label for='pinf-description'>Description</label>"
						+		"	<textarea cols='40' rows='20' name='pinf-description' id='pinf-description' value=''  maxlength='7997'></textarea>"
						+		"</div>"
								);
	$("#docker-images").trigger("create");
}

$( document ).on("pagebeforeshow", "#provider-image", function(event){
	images_setTabFocus( imagesObj.currentTab );
	setProviderInfoToImagesPage();
	/// Load login name
    uLogin_success();
	
	/// Load image base link
	var imageUrl = window.location.protocol + "//" + window.location.host + "/" +  window.location.pathname + "/images/docker_images/base_image.tar";
	document.getElementById("pitu-url-baseimage-url").href =  imageUrl; 
	document.getElementById("pitu-url-baseimage-url").innerHTML = imageUrl;
	
	/// Load Dockerfile link
	var dockerfileUrl = window.location.protocol + "//" + window.location.host + "/" +  window.location.pathname + "/images/docker_images/Dockerfile";
	document.getElementById("pitu-url-dockerfile-url").href =  dockerfileUrl; 
	document.getElementById("pitu-url-dockerfile-url").innerHTML = dockerfileUrl;
	
	var inputs = document.getElementsByClassName('pinf-input');
	for(var i = 0; i < inputs.length; i++) {
		inputs[i].disabled = false;
	}
});

$( document ).on("pageshow", "#provider-image", function(event){
});

$(document).on( "click", '.provider-image-tab-btn', function(e){
	images_setTabFocus( $(this).attr( "data-tab" ) );
});

/*
function searchBaseDockerImage() {
    var tbody = $("#pid-table");
    var html = "";
	var found = 0;
	var searchingTxt = $('#provider-image-search-txt').val();
    tbody.empty();
    $("#pid-table").table("refresh");
	
	loading_show(3000);
	images_findData(1, 0, function(res) {
		loading_hide();
		images_loadBaseImagesList( res );
		images_loadBaseImagesTable(imagesObj.baseImages);
	});
}*/

$(document).on('submit', '#pid-form', function(e){
	var tagtxt = "#pidf-keyword";
	$(tagtxt).textinput("disable");
	loading_show(60000, function(){
		$(tagtxt).textinput("enable");
		loading_hide();
		return false;
	});
	
	/// Prevent default submit  
	e.preventDefault();
	
	/// Get keyword
	var keyword = $(tagtxt).val();
	
	/// Find images
	if( imagesObj.keyword !== keyword ){
		ws_findImages(1, 0, 1, keyword, function(res){
			loading_hide();
			if(res.result || res.status){
				loading_hide();
				console.log("Error: request fail!");
				$(tagtxt).textinput("enable");
				return;
			}
			imagesObj.keyword = keyword;
			images_loadBaseImagesList( res );
			images_loadBaseImagesTable(imagesObj.baseImages);
			loading_hide();
		});
	}else{
		loading_hide();
	}
	$(tagtxt).textinput("enable");
	return false;
});

$(document).on('submit', '#provider-image-newimages-form', function(e){
//	document.getElementById("pinf-submit-btn").disabled = true;
	//$("#provider-image-newimages-form").disabled = true;
	var inputs = document.getElementsByClassName('pinf-input');
	
	loading_show(3000000, function(){
		var inputs = document.getElementsByClassName('pinf-input');
		for(var i = 0; i < inputs.length; i++) {
			inputs[i].disabled = false;
		}
		//register_fail(ERROR); /// Timeout		
		return false;
	});
	
	/// Prevent default submit  
	e.preventDefault();
	
	/// Parse parameter submit
	dataString = $(this).serialize();
	console.log($(this).serializeArray());
	var serArr = $(this).serializeArray();
	
	var valid = true;
	var imageObj = new Object();
	$.each( serArr, function( index, value ) {
		//alert( index + ": " + value );
		var tagId = value.name;
		if( tagId != "" ){
			var msg, realVal;
			switch( tagId ){
			case "pinf-name":
				imageObj.name = value.value;
				break;
			case "pinf-public":
				imageObj.isPublic = value.value == "1" ? 1 : 0;
				break;
			case "pinf-description":
				imageObj.description = value.value;
				break;
			}
			
			/// Get file param "pinf-tarfile":
			var files = $('input#pinf-tarfile').prop("files")
			if ( files.length > 0 ){
				if( files[0].size > 2048576000 ) {
					uValid_setCustomValidity("pinf-tarfile", "File size maximum is 2G.");
					for(var i = 0; i < inputs.length; i++) {
						inputs[i].disabled = true;
					}
					return true;
				} else {
					imageObj.tarfile = files[0];
				}
			} else {
				console.log( "#provider-editapp-icon: No select file ");
			}
			//imageObj.filename = pinf-tarfile;
		}
	});
	
	/// Check image name is valid
	ws_checkImageNameUsable(imageObj.name, function(cnres){
		if(cnres.result == ResponseCodes.DATA_SELECT_EMPTY){
			for(var i = 0; i < inputs.length; i++) {
				inputs[i].disabled = true;
			}
			/// Submit register
			ws_createImage( imageObj, function (res) {
				if( res.uploadFile == imageObj.tarfile.name ){
					/// Show progress bar
					$("#pinf-progress-bar").empty();
					$("#pinf-progress-bar").disabled = false;
					$('<input>').appendTo('#pinf-progress-bar').attr({'name':'pinf-slider','id':'pinf-slider','data-highlight':'true','min':'0','max':'100','value':'0','type':'range'}).slider({
						create: function( event, ui ) {
							$(this).parent().find('input').hide();
							$(this).parent().find('input').css('margin-left','-9999px'); // Fix for some FF versions
							$(this).parent().find('.ui-slider-track').css('margin','0 15px 0 15px');
							$(this).parent().find('.ui-slider-handle').hide();
						}
					}).slider("refresh");
					
					var msgLbl = document.getElementById('pinf-msg');
					ws_sendFilePart(res.requestId, imageObj.tarfile, 0, function(sent, total){
						var progressBar = {
							setValue:function(id, value) {
								$(id).val(value);
								$(id).slider("refresh");
							}
						}
						console.log("Value sent / total * 100 = " + (sent / total * 100));
						progressBar.setValue('#pinf-slider', sent / total * 100);
						if(sent + 10*1024*1024 < total){
							msgLbl.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;Action in process upload file " + sent/(1024 * 1024) + "/" + Math.round(total/(1024 * 1024)) + " MB.";
						} else {
							msgLbl.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;Create image is executing it may take 3 minute to complete.";
						}
					}, function(res){
						loading_hide();
						$("#pinf-progress-bar").empty();
						if(res == 1){
							alert("Can not open file\"" + imageObj.tarfile.name + "\" !");
						}
						else if(res.result == ResponseCodes.DATA_SUCCESS){
							alert("Create image success !");
						} else if(res.result == ResponseCodes.DATA_ERROR){
							alert("Tar file invalid !");
						} else if(res.result == ResponseCodes.FILE_ACTION_ERROR || res.result == ResponseCodes.DATA_IS_EXISTED){
							alert("Tar file invalid !\n Can not upload duplicate image !");
						} else {
							alert("Create image fail!");
						}
						msgLbl.innerHTML = "";
						for(var i = 0; i < inputs.length; i++) {
							inputs[i].disabled = false;
						}
					});
				} else{
					loading_hide();
					alert("Create image fail !");
					for(var i = 0; i < inputs.length; i++) {
						inputs[i].disabled = false;
					}
				}
				return false;
			});
		} else {
			loading_hide();
			alert("Someone already has that image name. Try another?");
		}
	});

	/// Return false to prevent default submit  
	return false;
});

function images_loadImagesList(resImages)
{	
	if(resImages && resImages.images){
		if(resImages.images.length > 0){
			imagesObj.images = resImages.images;
			/*
			$.each(resImages.images, function(index){
				if(this.name == undefined){
					alert("name == undefined");
					return true;
				}
				
				var dockerImage = {};
				dockerImage.isPublic = this.isPublic;
				dockerImage.isOwner = this.isOwner;
				dockerImage.link = this.url;
				dockerImage.name = this.name;
				dockerImage.description = this.description;
				imagesObj.images[this.name] = dockerImage;
				alert("dockerImage.nam = " + dockerImage.name);
				
			});
			*/
		}else{
			imagesObj.images = [];
		}
	}
	images_updateImagesTable();
}

function images_loadBaseImagesList(resImages)
{
	if(resImages && resImages.images){
		if(resImages.images.length > 0){
			imagesObj.baseImages = resImages.images;
		}else{
			imagesObj.baseImages = [];
		}
	}
}

function images_setTabFocus( tabname ){
	$( '.provider-image-tab-btn').removeClass("ui-btn-active ui-state-persist");
	$( '#provider-image-tab-btn-' + tabname ).addClass("ui-btn-active ui-state-persist");
	//alert("images_setTabFocus(): tabname " + tabname);
	switch ( tabname ){
		case "newimages":
			$( '#provider-image-tab-newimages' ).show();
			$( '#provider-image-tab-uploadedimages' ).hide();
			$( '#provider-image-tab-downloadsearch' ).hide();
			break;
		case "uploadedimages":
			$( '#provider-image-tab-newimages' ).hide();
			$( '#provider-image-tab-uploadedimages' ).show();
			$( '#provider-image-tab-downloadsearch' ).hide();
			console.log("uploadedimages");
			loading_show(5000, function()
			{
				
			});
			//images_findData(0, 1, function(res) {
			ws_findImages(0, 1, 0, "", function(res){
				images_loadImagesList( res );
				loading_hide();
			});
			break;
		case "downloadsearch":
			$( '#provider-image-tab-newimages' ).hide();
			$( '#provider-image-tab-uploadedimages' ).hide();
			$( '#provider-image-tab-downloadsearch' ).show();
			console.log("downloadsearch");
			/*
			loading_show(5000, function()
			{
				
			});
			images_findData(1, 0, function(res) {
				loading_hide();
				images_loadBaseImagesList( res );
			});
			*/
			break;
		default:
			break;
	}
	imagesObj.currentTab = tabname;
}

function setProviderInfoToImagesPage()
{
	$('#pinf-email').val(localStorage.email);
	$('#pinf-notes').val(localStorage.description);
}

function images_showEditImagePopup(index)
{
	imagesObj.selectedImageIndex = index;
	images_setImageInfoToPopup(imagesObj.images[index]);
	$('#edit-image-popup').popup('open');
}

function images_showDeleteImagePopup(index)
{
	imagesObj.selectedImageIndex = index;
	$('#delete-image-popup').popup('open');
}

function images_updateImageBtnClick()
{
	loading_show(5000, function()
	{
	});
	var orgObj = imagesObj.images[imagesObj.selectedImageIndex];
	var imageInfo = Object();
	
	var ispublic = ($("#pief-public").val() == "1") ? 1 : 0 ;
	imageInfo.isPublic = (orgObj.isPublic != ispublic) ? ispublic : undefined;
	
	var description = $("#pief-description").val();
	imageInfo.description = (description !== orgObj.description) ? description : undefined;
	
	imageInfo.image = orgObj.name;
	if(imageInfo.isPublic == undefined && imageInfo.description == undefined){
		alert("No field has change!");
		return;
	}
	ws_updateImage(imageInfo, function(res){
		if(res.result == ResponseCodes.DATA_SUCCESS){
			if(imageInfo.isPublic != undefined){
				imagesObj.images[imagesObj.selectedImageIndex].isPublic = imageInfo.isPublic;
			}
			if(imageInfo.description != undefined){
				imagesObj.images[imagesObj.selectedImageIndex].description = imageInfo.description;
			}
			images_updateImagesTable();
			$('#edit-image-popup').popup('close');
		} else {
			alert("Update fail!");
		}
		loading_hide();
	});
}

function images_setImageInfoToPopup(imageObj)
{
	$('#pief-name').val(imageObj.name);
	$('#pief-url').val(window.location.protocol + "//" + window.location.host + "/" + window.location.pathname + "/images//" + imageObj.name + ".tar");
	$('#pief-public').val(imageObj.isPublic);
	$('#pief-public').slider('refresh');
	$('#pief-description').val(imageObj.description);
}

function images_deleteImageBtnClick()
{
	loading_show(60000, function()
	{
	});
	
	ws_deleteImage(imagesObj.images[imagesObj.selectedImageIndex].name, function(res){
		if( res.result == ResponseCodes.DATA_SUCCESS)
		{
			//delete imagesObj.images[imagesObj.selectedImageIndex];
			/// Remove item
			imagesObj.images.splice(imagesObj.selectedImageIndex, 1);
			//console.log(imagesObj.images[imagesObj.selectedImageIndex]);
			//alert("Refresh");
			images_updateImagesTable();
		} else {
			console.log("Res: " + res);
			alert("Delete image fail");
		}
		loading_hide();
		$('#delete-image-popup').popup('close');
	});
}

function images_updateImagesTable(){
	var tbody = $("#piu-table-body");
    var html = "";
	var found = 0;
	console.log("imagesObj.images. = ");
	console.log(imagesObj.images);
	if(imagesObj !== undefined && imagesObj.images !== undefined)
	{
		if(imagesObj.images.length > 0)
		{
			found = 1;
			$.each(imagesObj.images, function(index){
				if(this.name == undefined)
				{
					return true;
				}
				//window.location.protocol + "//" + window.location.host + "/" +  window.location.pathname
				href = window.location.protocol + "//" + window.location.host + "/" +  window.location.pathname + "/images/" + this.name + ".tar";
				html += '<tr>';
				//html = html + "<td>" + this.name + "</td>";
				html += '<td style="text-align:right;vertical-align:middle">' + this.name + '</td>';
				html += '<td style="text-align: center;vertical-align:middle"><a class="ui-table-cell-label" style="text-decoration:underline;" href="' + href + '" target="_blank">' + href + '</td>';
				html += '<td style="text-align: center;vertical-align:middle;white-space:initial;">' + this.description + '</td>';
				//html = html + "<td><input type=\"button\" class=\"ui-corner-all\" onclick=\"images_showEditImagePopup(\'" + index + "\')\" value=\"Edit\"></td>";
				html += '<td style="text-align: center;vertical-align:middle"><a href="#" data-role="button" data-icon="edit" onclick="images_showEditImagePopup(\'' + index + '\');" data-mini="true" >Edit</a></td>'
				//html = html + '<td style="text-align: center;vertical-align:middle"><a href="#" data-role="button" onclick="images_showEditImagePopup(\'' + index + '\');" data-mini="true">Edit</a></td>'
				html += '<td style="text-align: center;vertical-align:middle"><a href="#" data-role="button" data-icon="delete" onclick="images_showDeleteImagePopup(\'' + index + '\');" data-mini="true">Delete</a></td>'
				html += '</tr>';
			});
		}
	}
	if(found == 0)
	{
		html += '<tr>';
		html += '<td style="text-align: center;vertical-align:middle">No such uploaded image</td>';
		html += '</tr>';
	}
	tbody.empty().append(html);
	$("#piu-table").trigger('create');
}

function images_loadBaseImagesTable(baseImages){
	var tbody = $("#pid-table-body");
    var html = "";
	var found = 0;
	if(baseImages && baseImages.length > 0)
	{
		$.each(baseImages, function(index){
			found = 1;
			href = window.location.protocol + "//" + window.location.host + "/" +  window.location.pathname + "/images/" + this.name + ".tar";
			html += "<tr>";
			html += '<td style="text-align: center;vertical-align:middle">' + this.name + '</td>';
			//html += "<td style="text-align: center;vertical-align:middle"><a style=\"text-decoration:underline;\" href=\"#\">" + href + "</td>";
			html += '<td style="text-align: center;vertical-align:middle"><a class="ui-table-cell-label" style="text-decoration:underline;" href="' + href + '" target="_blank">' + href + '</td>';
			html += '<td style="text-align: center;vertical-align:middle;white-space:initial;">' + this.description + '</td>';
			//html += "<td style="text-align: center;vertical-align:middle"><form method=\"get\" action=\"" + href + "\"><input type=\"button\" class=\"ui-corner-all\" value=\"Download\" ></td>";
			html += '<td style="text-align: center;vertical-align:middle"><a href="' + href + '" data-role="button" data-mini="true" target="_blank">Download</a></td>'
			html += "</tr>";
		});
	}
	if(found == 0)
	{
		html += '<tr>';
		html += '<td style="text-align: center;vertical-align:middle">No such uploaded image</td>';
		html += '</tr>';
	}
    tbody.empty().append(html);
	$("#pid-table").trigger('create');
}

function downloadBaseDockerImage(imageId)
{
	
}
/*
function images_findData(isPublic, isOwner, callback ){
    ws_findImages(isPublic, isOwner, function( res ){
        imagesObj.resFindImages = res;
        if( callback )
		{
        	callback ( res );
        }
    });
}*/