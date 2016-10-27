
/*
 * Set invalid message
 */
function uValid_setCustomValidity( idTag, msg ){
	document.getElementById(idTag).setCustomValidity(msg);
	$.mobile.activePage.find( "#" + idTag ).focus();
}

/*
 * Replace html character of text
 */
function uString_escapeHtml( text ){
	  return text
	      .replace(/&/g, "&amp;")
	      .replace(/</g, "&lt;")
	      .replace(/>/g, "&gt;")
	      .replace(/"/g, "&quot;")
	      .replace(/'/g, "&#039;");
}

function uString_escapePath( path ){
	if(path == undefined){
		return "";
	}
	return path.trim().replace(/ /g, "\\ ");
}
/*
 * Convert response code to message
 */

function uResponseCode_toMessage( code ){
	var msg;
	switch( code ){
	case ResponseCodes.ERROR_FX10_RETURN:
		msg = "Resource value is out of range: node.";
		break;
	default:
		msg = "";
		break;
	}
	return msg;
}
/*
 *  *******************************************************
 *  Hanler check file size input file type
 *  *******************************************************
 */
$(document).on( "change", 'input[ type="file" ]', function( e ){
    var fileObj = this.files[0];
    /// < 100M ~ 104857600 bit
    if( fileObj && fileObj.size > 2048576000 ){
    	e.target.setCustomValidity('The selected file size is more than 200 M.');
    	return;
    } else {
    	e.target.setCustomValidity("");
    }
} );

/*
 * *******************************************************
 * 	Backup/Restore data page
 * *******************************************************
 */
function uStorage_backupData(){
	sessionStorage.setItem("homeObj", JSON.stringify( homeObj ));
	sessionStorage.setItem("marketObj", JSON.stringify( marketObj ));
	if(detailObj !== undefined)
		sessionStorage.setItem("detailObj", JSON.stringify( detailObj ));
	if( adetailObj ){
		sessionStorage.setItem("adetailObj", JSON.stringify( adetailObj ));
	}
	if(appObj !== undefined)
		sessionStorage.setItem("appObj", JSON.stringify( appObj ));
	if(asubmitObj !== undefined)
		sessionStorage.setItem("asubmitObj", JSON.stringify( asubmitObj ));

	if(typeof(jobDetailObj) != "undefined")
		sessionStorage.setItem("jobDetailObj", JSON.stringify( jobDetailObj ));
	if(jobsObj !== undefined)
		sessionStorage.setItem("jobsObj", JSON.stringify( jobsObj ));
	if( db_services ){
		sessionStorage.setItem("db_services", JSON.stringify( db_services ));
	}
}

function uStorage_restoreData(){
	homeObj = JSON.parse( sessionStorage.getItem ( "homeObj" ) );
	marketObj = JSON.parse( sessionStorage.getItem ( "marketObj" ) );
	detailObj = JSON.parse( sessionStorage.getItem ( "detailObj" ) );
	adetailObj = JSON.parse( sessionStorage.getItem ( "adetailObj" ) );
	appObj = JSON.parse( sessionStorage.getItem ( "appObj" ) );
	asubmitObj = JSON.parse( sessionStorage.getItem ( "asubmitObj" ) );
	detailObj = JSON.parse( sessionStorage.getItem ( "detailObj" ) );
	jobDetailObj = JSON.parse( sessionStorage.getItem ( "jobDetailObj" ) );
	jobsObj = JSON.parse( sessionStorage.getItem ( "jobsObj" ) );
	var jsonStr = sessionStorage.getItem ( "db_services" );
	if( jsonStr && jsonStr !== "" ){
		db_services = JSON.parse( jsonStr );
	}
}

/*
 * *******************************************************
 *		Login button handler
 * *******************************************************
 */

function uLogin_success(){
	$(".login-txt").text(localStorage.email);
	$(".login-btn").buttonMarkup({ icon: "delete" });
	$(".login-btn").attr("href", LOGOUT);
	$(".login-btn").attr("target", "_blank");
}

function uLogin_resetBtn(){
	$(".login-txt").text("Login");
	$(".login-btn").buttonMarkup({ icon: "check" });
	$(".login-btn").removeAttr("target");//, "_blank");
}

$(document).on('click', '.login-btn', function(e){
/*	///This will prevent event triggering more then once
	if(e.handled !== true){
        e.handled = true;
        return true;
    }*/
	//alert("this.text = " + $.mobile.activePage.find("#login-txt").text());
	if( $.mobile.activePage.find("#login-txt").text() === "Login" ){
		//console.log("Bp: login now !");
		homeObj.currentPage = "#home";
		home_login( home_oa2LoginCallback );
	}else{
		//console.log("Bp: logout now !");
		home_logout();
	}
});

/*
 * *******************************************************
 * Generate icon path
 * *******************************************************
 */
function uIcon_genIconPath( serviceId ){
	var ran = Math.random();
	return ( ( serviceId && serviceId !== "" ) ? ( "images/" + serviceId + ".png?" + ran ) : "images/no-image.png" );
}

/*
 * *******************************************************
 * 						Multi times click
 * *******************************************************
 */

/*///This will prevent event triggering more then once
	if(e.handled !== true)
    {
        e.handled = true;
        return true;
    }
*/

/*
 * *******************************************************
* 						Object until
 * *******************************************************
 */

/*
 * Size of object / Number of properties / Size of associative array
 */
Object.size = function(obj) {
    var size = 0, key;
    for (key in obj) {
        if (obj.hasOwnProperty(key)) size++;
    }
    return size;
};

/*
 * *******************************************************
* 						File utils
 * *******************************************************
 */
/*
function ufile_displayAsImage(file, containerid) {
	if (typeof FileReader !== "undefined") {
		var container = document.getElementById(containerid),
		    img = document.createElement("img"),
		    reader;
		container.appendChild(img);
		reader = new FileReader();
		reader.onload = (function (theImg) {
			return function (evt) {
				theImg.src = evt.target.result;
			};
		}(img));
		reader.readAsDataURL(file);
	}
}*/


function ufile_displayAsImage(file, imgTagID) {
	console.log("Bp: ufile_displayAsImage(file, imgTagID) ");
	console.log(file);
	console.log(imgTagID);
	if (typeof FileReader !== "undefined" && file && imgTagID) {
		var img = document.getElementById(imgTagID),
		    //img = document.createElement("img"),
		    reader;
		//container.appendChild(img);
		reader = new FileReader();
		reader.onload = (function (theImg) {
			return function (evt) {
				theImg.src = evt.target.result;
			};
		}(img));
		reader.readAsDataURL(file);
	}
}

function ufile_getExtension( fullName ){
	var extension = fullName.substr( (fullName.lastIndexOf('.') +1) );
	return extension;
}

function ufile_isImageExtension( filename ){
	return (/\.(gif|jpg|jpeg|tiff|png)$/i).test(filename.toLowerCase());
}

function ufile_isBinaryExtension( filename ){
	return (/\.(bin|binary)$/i).test(filename.toLowerCase());
}


function ufile_isTextExtension( filename ){
	return (/\.(text|txt)$/i).test(filename.toLowerCase());
}

function ufile_isXmlExtension( filename ){
	return (/\.(xml)$/i).test(filename.toLowerCase());
}

/*
 * Get file name inside full path
 */
function ufile_getFileName( fullPath ){
	var filename = fullPath.replace(/^.*[\\\/]/, '');
	return filename;
}

/*
 *
 */
function ufile_isSafari(){
  var ua = navigator.userAgent.toLowerCase();
  if (ua.indexOf('safari') != -1) {
    if (ua.indexOf('chrome') > -1) {
      return false;
    } else {
      return true; // Safari
    }
  }
}

/*
 *	Util save BufferArray to local file
 */
function ufile_saveDownloadFile( fileNameToSaveAs, data ){
	console.log("SAVE DOWNLOAD FILE");
	console.log("fileNameToSaveAs = " + fileNameToSaveAs);

	var ie = navigator.userAgent.match(/MSIE\s([\d.]+)/),
        ie11 = navigator.userAgent.match(/Trident\/7.0/) && navigator.userAgent.match(/rv:11/),
        ieVer=(ie ? ie[1] : (ie11 ? 11 : -1));
	if (ie && ieVer < 10) {
        console.log("No blobs on IE ver<10");
        return;
    }
	/// Check file type

	var dataBlob;
	if(data instanceof Blob){
		dataBlob = data;
	}else{
		var ext = ufile_getExtension(fileNameToSaveAs);
		var mime = uMime_getMimeByExt(ext);
        dataBlob = new Blob([data],{'type': mime, endings: 'native'});
	}

    if (ie || ie11) {
		/// IE api to save file
        window.navigator.msSaveBlob(dataBlob, fileNameToSaveAs);
    } else {
        var downloadLink = document.createElement("a");
        downloadLink.download = fileNameToSaveAs;
        downloadLink.innerHTML = "Download File";
		var isSafari = Object.prototype.toString.call(window.HTMLElement).indexOf('Constructor') > 0;
		if(isSafari){
			var fr = new FileReader();
			fr.onload = function (e){
				downloadLink.href = e.target.result;
				downloadLink.target = '_blank';
				downloadLink.onclick = function (event)
				{
					document.body.removeChild(event.target);
				}
				downloadLink.style.display = "none";
				document.body.appendChild(downloadLink);
				downloadLink.click();
			};
			fr.readAsDataURL(dataBlob);
		} else {
			if (window.webkitURL != null) {
				// Chrome allows the link to be clicked
				// without actually adding it to the DOM.
				downloadLink.href = window.webkitURL.createObjectURL(dataBlob);
				downloadLink.target = '_blank';
			} else {
				// Firefox requires the link to be added to the DOM
				// before it can be clicked.
				downloadLink.href = window.URL.createObjectURL(dataBlob);
				downloadLink.target = '_blank';
				downloadLink.onclick = function (event)
				{
					document.body.removeChild(event.target);
				}
				downloadLink.style.display = "none";
				document.body.appendChild(downloadLink);
			}

			downloadLink.click();
		}
    }
	return;
}

/*
 * *******************************************************
 * 						Menu button handler utils
 * *******************************************************
 */
$(document).on('click', '.menu-btn', function(e){
/*	///This will prevent event triggering more then once
	if(e.handled !== true){
        e.handled = true;
        return true;
    }*/
	//alert("this.text = " + $.mobile.activePage.find("#login-txt").text());
	if( $.mobile.activePage.find("#login-txt").text() === "Login" ){
		//console.log("Bp: login now !");
		homeObj.currentPage = "#home";
		home_login( home_oa2LoginCallback );
	}else{
		//console.log("Bp: logout now !");
		home_logout();
	}
});

/*
 * *******************************************************
 * 						Convert utils
 * *******************************************************
 */
function ab2str(buf) {
	return String.fromCharCode.apply(null, new Uint8Array(buf));
}

function ab2ui32(buf) {
	return new Uint32Array(buf);
}

function str2ab(str) {
  var buf = new ArrayBuffer(str.length); // 2 bytes for each char
  var bufView = new Uint8Array(buf);
  for (var i=0, strLen=str.length; i<strLen; i++) {
    bufView[i] = str.charCodeAt(i);
  }
  return buf;
}

function uint2ab(x) {
	var ab = new ArrayBuffer(4)
	var view = new Uint32Array(ab);
	view[0] = x;
	return ab;
}
/*
 * *******************************************************
 * 				Remove provider permission
 * *******************************************************
 */
function uRegister_rejectProvider(){
	ws_removeRegister(function(res){
		if(res.result == ResponseCodes.DATA_SUCCESS){
			localStorage.userType = "user";
			$.mobile.changePage( "#home", { reloadPage: true, transition: "none"} );
		} else {
			alert("Remove register fail !");
		}
	});
}
/*
 * *******************************************************
 * 				Mime types convert from extension
 * *******************************************************
 */
function uMime_getMimeByExt(extensionStr) {
    var extToMimes = {
        'mp4' : 'movie/mp4',
		'323' : 'text/h323',
		'*' : 'application/octet-stream',
		'acx' : 'application/internet-property-stream',
		'ai' : 'application/postscript',
		'aif' : 'audio/x-aiff',
		'aifc' : 'audio/x-aiff',
		'aiff' : 'audio/x-aiff',
		'asf' : 'video/x-ms-asf',
		'asr' : 'video/x-ms-asf',
		'asx' : 'video/x-ms-asf',
		'au' : 'audio/basic',
		'avi' : 'video/x-msvideo',
		'axs' : 'application/olescript',
		'bas' : 'text/plain',
		'bcpio' : 'application/x-bcpio',
		'bin' : 'application/octet-stream',
		'bmp' : 'image/bmp',
		'c' : 'text/plain',
		'cat' : 'application/vnd.ms-pkiseccat',
		'cdf' : 'application/x-cdf',
		'cdf' : 'application/x-netcdf',
		'cer' : 'application/x-x509-ca-cert',
		'class' : 'application/octet-stream',
		'clp' : 'application/x-msclip',
		'cmx' : 'image/x-cmx',
		'cod' : 'image/cis-cod',
		'cpio' : 'application/x-cpio',
		'crd' : 'application/x-mscardfile',
		'crl' : 'application/pkix-crl',
		'crt' : 'application/x-x509-ca-cert',
		'csh' : 'application/x-csh',
		'css' : 'text/css',
		'dcr' : 'application/x-director',
		'der' : 'application/x-x509-ca-cert',
		'dir' : 'application/x-director',
		'dll' : 'application/x-msdownload',
		'dms' : 'application/octet-stream',
		'doc' : 'application/msword',
		'dot' : 'application/msword',
		'dvi' : 'application/x-dvi',
		'dxr' : 'application/x-director',
		'eps' : 'application/postscript',
		'etx' : 'text/x-setext',
		'evy' : 'application/envoy',
		'exe' : 'application/octet-stream',
		'fif' : 'application/fractals',
		'flr' : 'x-world/x-vrml',
		'gif' : 'image/gif',
		'gtar' : 'application/x-gtar',
		'gz' : 'application/x-gzip',
		'h' : 'text/plain',
		'hdf' : 'application/x-hdf',
		'hlp' : 'application/winhlp',
		'hqx' : 'application/mac-binhex40',
		'hta' : 'application/hta',
		'htc' : 'text/x-component',
		'htm' : 'text/html',
		'html' : 'text/html',
		'htt' : 'text/webviewhtml',
		'ico' : 'image/x-icon',
		'ief' : 'image/ief',
		'iii' : 'application/x-iphone',
		'ins' : 'application/x-internet-signup',
		'isp' : 'application/x-internet-signup',
		'jfif' : 'image/pipeg',
		'jpe' : 'image/jpeg',
		'jpeg' : 'image/jpeg',
		'jpg' : 'image/jpeg',
		'js' : 'application/x-javascript',
		'latex' : 'application/x-latex',
		'lha' : 'application/octet-stream',
		'lsf' : 'video/x-la-asf',
		'lsx' : 'video/x-la-asf',
		'lzh' : 'application/octet-stream',
		'm13' : 'application/x-msmediaview',
		'm14' : 'application/x-msmediaview',
		'm3u' : 'audio/x-mpegurl',
		'man' : 'application/x-troff-man',
		'mdb' : 'application/x-msaccess',
		'me' : 'application/x-troff-me',
		'mht' : 'message/rfc822',
		'mhtml' : 'message/rfc822',
		'mid' : 'audio/mid',
		'mny' : 'application/x-msmoney',
		'mov' : 'video/quicktime',
		'movie' : 'video/x-sgi-movie',
		'mp2' : 'video/mpeg',
		'mp3' : 'audio/mpeg',
		'mpa' : 'video/mpeg',
		'mpe' : 'video/mpeg',
		'mpeg' : 'video/mpeg',
		'mpg' : 'video/mpeg',
		'mpp' : 'application/vnd.ms-project',
		'mpv2' : 'video/mpeg',
		'ms' : 'application/x-troff-ms',
		'msg' : 'application/vnd.ms-outlook',
		'mvb' : 'application/x-msmediaview',
		'nc' : 'application/x-netcdf',
		'nws' : 'message/rfc822',
		'oda' : 'application/oda',
		'p10' : 'application/pkcs10',
		'p12' : 'application/x-pkcs12',
		'p7b' : 'application/x-pkcs7-certificates',
		'p7c' : 'application/x-pkcs7-mime',
		'p7m' : 'application/x-pkcs7-mime',
		'p7r' : 'application/x-pkcs7-certreqresp',
		'p7s' : 'application/x-pkcs7-signature',
		'pbm' : 'image/x-portable-bitmap',
		'pdf' : 'application/pdf',
		'pfx' : 'application/x-pkcs12',
		'pgm' : 'image/x-portable-graymap',
		'pko' : 'application/ynd.ms-pkipko',
		'pma' : 'application/x-perfmon',
		'pmc' : 'application/x-perfmon',
		'pml' : 'application/x-perfmon',
		'pmr' : 'application/x-perfmon',
		'pmw' : 'application/x-perfmon',
		'pnm' : 'image/x-portable-anymap',
		'pot' : 'application/vnd.ms-powerpoint',
		'ppm' : 'image/x-portable-pixmap',
		'pps' : 'application/vnd.ms-powerpoint',
		'ppt' : 'application/vnd.ms-powerpoint',
		'prf' : 'application/pics-rules',
		'ps' : 'application/postscript',
		'pub' : 'application/x-mspublisher',
		'qt' : 'video/quicktime',
		'ra' : 'audio/x-pn-realaudio',
		'ram' : 'audio/x-pn-realaudio',
		'ras' : 'image/x-cmu-raster',
		'rgb' : 'image/x-rgb',
		'rmi' : 'audio/mid',
		'roff' : 'application/x-troff',
		'rtf' : 'application/rtf',
		'rtx' : 'text/richtext',
		'scd' : 'application/x-msschedule',
		'sct' : 'text/scriptlet',
		'setpay' : 'application/set-payment-initiation',
		'setreg' : 'application/set-registration-initiation',
		//'sh' : 'application/x-sh',
		'sh' : 'text/plain',
		'shar' : 'application/x-shar',
		'sit' : 'application/x-stuffit',
		'snd' : 'audio/basic',
		'spc' : 'application/x-pkcs7-certificates',
		'spl' : 'application/futuresplash',
		'src' : 'application/x-wais-source',
		'sst' : 'application/vnd.ms-pkicertstore',
		'stl' : 'application/vnd.ms-pkistl',
		'stm' : 'text/html',
		'sv4cpio' : 'application/x-sv4cpio',
		'sv4crc' : 'application/x-sv4crc',
		'svg' : 'image/svg+xml',
		'swf' : 'application/x-shockwave-flash',
		't' : 'application/x-troff',
		'tar' : 'application/x-tar',
		'tcl' : 'application/x-tcl',
		'tex' : 'application/x-tex',
		'texi' : 'application/x-texinfo',
		'texinfo' : 'application/x-texinfo',
		'tgz' : 'application/x-compressed',
		'tif' : 'image/tiff',
		'tiff' : 'image/tiff',
		'tr' : 'application/x-troff',
		'trm' : 'application/x-msterminal',
		'tsv' : 'text/tab-separated-values',
		'txt' : 'text/plain',
		'uls' : 'text/iuls',
		'ustar' : 'application/x-ustar',
		'vcf' : 'text/x-vcard',
		'vrml' : 'x-world/x-vrml',
		'wav' : 'audio/x-wav',
		'wcm' : 'application/vnd.ms-works',
		'wdb' : 'application/vnd.ms-works',
		'wks' : 'application/vnd.ms-works',
		'wmf' : 'application/x-msmetafile',
		'wps' : 'application/vnd.ms-works',
		'wri' : 'application/x-mswrite',
		'wrl' : 'x-world/x-vrml',
		'wrz' : 'x-world/x-vrml',
		'xaf' : 'x-world/x-vrml',
		'xbm' : 'image/x-xbitmap',
		'xla' : 'application/vnd.ms-excel',
		'xlc' : 'application/vnd.ms-excel',
		'xlm' : 'application/vnd.ms-excel',
		'xls' : 'application/vnd.ms-excel',
		'xlt' : 'application/vnd.ms-excel',
		'xlw' : 'application/vnd.ms-excel',
		'xof' : 'x-world/x-vrml',
		'xpm' : 'image/x-xpixmap',
		'xwd' : 'image/x-xwindowdump',
		'z' : 'application/x-compress',
		'zip' : 'application/zip',
		'shortcut' : 'text/plain'
    }
	console.log("uMime_getMimeByExt: extension = " + extensionStr );
	var ext = extensionStr.toLowerCase();
	if (extToMimes.hasOwnProperty(ext)) {
		console.log("mime type = " + extToMimes[ext]);
		return extToMimes[ext];
	}
	if(ufile_isSafari()){
		return extToMimes['txt'];
	} else {
		return extToMimes['*'];
	}

};
