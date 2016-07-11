<!DOCTYPE html> 
<html> 
<head>
	
	<link rel="icon" type="image/gif" href="https://www.google.com/a/anlab.info/images/logo.gif?alpha=1&service=google_black" />
	<meta name="viewport" charset="UTF-8" content="width=device-width, initial-scale=1" >
	<!-- link rel="stylesheet" href="http://code.jquery.com/mobile/1.2.1/jquery.mobile-1.2.1.min.css" />  -->
	<!-- <script src="http://code.jquery.com/jquery-1.8.3.min.js"></script>  -->
	<!-- <script src="http://code.jquery.com/mobile/1.2.1/jquery.mobile-1.2.1.min.js"></script>  -->
	
	<!-- <link rel="stylesheet" href="https://code.jquery.com/mobile/1.3.2/jquery.mobile-1.3.2.min.css" />
	<script src="https://code.jquery.com/jquery-1.9.1.min.js"></script>
	<script src="https://code.jquery.com/mobile/1.2.1/jquery.mobile-1.2.1.min.js"></script>
	 -->
	<link rel="stylesheet" href="jquery.mobile-1.3.2/jquery.mobile-1.3.2.min.css" />
	<script src="js/jquery-1.9.1.min.js"></script>
	<script src="jquery.mobile-1.3.2/jquery.mobile-1.3.2.min.js"></script>
	
	<script src="https://apis.google.com/js/client.js?onload=handleClientLoad"></script>
	
	<script src="js/api.constant.js" type="text/javascript"></script>
	<script src="js/api.init.js" type="text/javascript"></script>
	<script src="js/api.utils.js" type="text/javascript"></script>
	<script src="js/api.error.page.js" type="text/javascript"></script>
	<script src="js/api.popup.js" type="text/javascript"></script>
	<script>
		var config = new Object();
<?php
		$handle = fopen("log_kp.txt", 'a+');
		if ($handle) {
			fputs($handle, "abc\n");
			fclose($handle);
		}

		$xml = simplexml_load_file("config.xml");
		//print_r($xml);
		$websocket_uri = $xml -> websocket -> uri;
		$oauth20_clientid = $xml -> oauth20 -> clientid;
		$oauth20_redirect = $xml -> oauth20 -> redirect;
	
		echo "config.websocket_uri = \"".$websocket_uri."\";";
		echo "config.oauth20_clientid = \"".$oauth20_clientid."\";";
		echo "config.oauth20_redirect = \"".$oauth20_redirect."\";";
?>
	</script>
	<script src="js/api.OAuth2.0.js" type="text/javascript"></script>
	<script src="js/api.websocket.js" type="text/javascript"></script>
	<script src="js/api.loading.page.js" type="text/javascript"></script>
	<script src="js/api.home.page.js" type="text/javascript"></script>
	<script src="js/api.register.page.js" type="text/javascript"></script>
	<script src="js/api.jobs.page.js" type="text/javascript"></script>
	<script src="js/api.job.detail.page.js" type="text/javascript"></script>
	<script src="js/api.market.page.js" type="text/javascript"></script>
	<script src="js/api.detail.page.js" type="text/javascript"></script>
	<script src="js/api.search.page.js" type="text/javascript"></script>
	<script src="js/api.app.page.js" type="text/javascript"></script>
	<script src="js/api.app.submit.page.js" type="text/javascript"></script>
	<script src="js/api.app.detail.page.js" type="text/javascript"></script>
	
	<script src="js/api.provider.utils.js" type="text/javascript"></script>
	<script src="js/api.provider.page.js" type="text/javascript"></script>
	<script src="js/api.provider.app.detail.page.js" type="text/javascript"></script>
	<script src="js/api.provider.app.page.js" type="text/javascript"></script>
	
	<script src="js/api.provider.app.user.page.js" type="text/javascript"></script>
	<script src="js/api.provider.app.waitlist.page.js" type="text/javascript"></script>
	<script src="js/api.provider.page.js" type="text/javascript"></script>
	<script src="js/api.provider.waitlist.page.js" type="text/javascript"></script>
	<script src="js/api.provider.image.page.js" type="text/javascript"></script>
	
	<script src="js/api.admin.page.js" type="text/javascript"></script>
	
	<link rel="stylesheet" type="text/css" href="css/init.css" />
	<link rel="stylesheet" type="text/css" href="css/home.css" />
	<link rel="stylesheet" type="text/css" href="css/market.css" />
	<link rel="stylesheet" type="text/css" href="css/detail.css" />
	<link rel="stylesheet" type="text/css" href="css/app.submit.css" />
	<link rel="stylesheet" type="text/css" href="css/provider.image.css" />
	
</head>

<body>

<!-- Home page ------------------------------------------>
<div data-role="page" id="home" class="swipe-page" data-dom-cache="true" data-theme="c" data-next="jobs" data-url="home">
	<div data-role="panel" id="home-menu-panel" data-display="overlay" data-shadow="false" data-tolerance="0,0">
		<h2 class="hmp-user">User</h2>
		<h2 class="hmp-provider" hidden>Provider</h2>
		<ul id="home-list-menu-user"class="hmp-user" data-role="listview" data-inset="true" data-divider-theme="d">
			<li><a href="#register">Register provider</a></li>
		</ul>
		<ul id="home-list-menu-provider" class="hmp-provider" data-role="listview" data-inset="true" data-divider-theme="d" hidden>
			<li data-role="list-divider">Go to page</li>
			<li><a href="#provider">Provider</a></li>
			<!--li><a href="#provider-image">Image Management</a></li-->
			<li data-role="list-divider">Actions</li>
			<li><a href="#" onclick="uRegister_rejectProvider()">Remove Register</a></li>
		</ul>
	</div> 
	<div data-role="header" data-theme="c" data-position="fixed" data-id="hdr" data-tap-toggle="false">
		<h1>Home</h1>
		<a id="home-menu-btn" class="menu-btn ui-btn-hidden" data-role="button" href="#home-menu-panel"  data-icon="bars" data-iconpos="notext" data-theme="c" data-inline="true" data-panel="home-menu-panel">Menu</a>
		<!--a href="#admin" class="ui-btn-left login-btn" data-role="button" data-icon="menu"/ -->
		<!-- <a href="#" id="home-btn-login" class="ui-btn-right" data-role="button" data-inline="true" data-icon="check" data-transition="pop"> 
			<label id="signinText">Login</label>
		</a> -->
		<a href="#" id="login-btn" class="ui-btn-right login-btn" data-role="button" style=" width:20%;min-width:130px;" data-inline="true" data-icon="check" data-transition="pop"> 
			<label id="login-txt" class="login-txt" style=" width:40%;min-width:350px;">Login</label>
		</a>
	</div><!-- /header  -->
	
	<div data-role="content">
		<div id="trivia-city" class="trivia ui-content" data-role="popup" data-position-to="window" data-tolerance="50,30,30,30" data-theme="d">
            <a href="" data-rel="back" data-role="button" data-theme="a" data-icon="delete" data-iconpos="notext" class="ui-btn-right">Close</a>
            <p>Here some text.</p>
        </div><!-- /popup -->
        
		<!-- <div class="ui-grid-c" id="grid"> -->
		<div class="ui-grid-b" id="grid">
			<div class="ui-block-a">
				<div data-role="controlgroup"  align="center">
					<a id="home-btn-market" href="#" rel="external"><img class="app-icon" src="images/market.png" alt="market" /></a>Market
				</div>
			</div>
		</div><!-- /grid -->
		
	</div><!-- /content -->
	    
	<div data-role="footer" data-theme="c" style="height:45px;" data-position="fixed" data-fullscreen="true" data-id="ftr" data-tap-toggle="false">
		<div data-role="group" style="align:center;" class="center-wrapper" >
			<a data-role="button" id="home-bar-btn-home" class="selection-bar ui-btn-active ui-state-persist" data-inline="true" data-transition="slide">Home</a>
			<a data-role="button" id="home-bar-btn-jobs" href="#jobs" class="bar-home-jobs selection-bar" data-inline="true" data-transition="slidefade" data-direction="reverse">Jobs</a>
		</div>
	</div>
	<div data-role="popup" id="register-popup" data-overlay-theme="a" class="ui-content message-popup">
    	<p id="register-popup-message">This is a completely basic popup, no options set.</p>
	</div>
</div>

<!-- Register provider page ------------------------------------------>
<div data-role="page" id="register" data-dom-cache="true" data-theme="c">
	<div data-role="header" data-theme="c" data-position="fixed" data-id="hdr" data-tap-toggle="false">
		<a href="#home" data-role="button" data-icon="back">Back</a>		
		<h1 data-mini = "true">Register as Content Provider</h1>	
	</div><!-- /header -->
	<div data-role="content">
		<form id="register-form" data-ajax="false" enctype="multipart/form-data">
			<ul data-role="listview" data-inset="true">
				<li data-role="fieldcontain">
					<label for="rf-email" style="margin-left: 10px;margin-right: auto !important;" readonly><b>Email Address </b></label>
					<input type="text" data-mini="true" data-theme="e" name="rf-email" id="rf-email" value="anlabprovider@anlab.info"  class="ui-disabled"/>
				</li>
				<li data-role="fieldcontain">
					<label for="rf-address" style="margin-left: 10px;margin-right: auto !important;"><b>Address</b></label>
					<input type="text" data-clear-btn="true" data-mini="true" name="rf-address" id="rf-address" value=""/>
				</li>
				<li data-role="fieldcontain">
					<label for="rf-phone" style="margin-left: 10px;margin-right: auto !important;" ><b>Phone Number </b></label>
					<input type="text" data-clear-btn="true" data-mini="true" name="rf-phone" id="rf-phone" value=""/>
				</li>
				<li data-role="fieldcontain">
					<label for="rf-descript" style="margin-left: 10px;margin-right: auto !important;"><b>Notes</b></label>
					<textarea cols="40" rows="8" name="rf-descript" id="rf-descript" data-mini="true"></textarea>
				</li>
				<li data-role="fieldcontain">
						<label for="rf-kdeskacc" style="margin-left: 10px;margin-right: auto !important;"><b>K login user name</b></label>
						<input type="text" data-clear-btn="true" data-mini="true" name="rf-kdeskacc" id="rf-kdeskacc" value="" required/>
				</li>

				<li class="ui-body ui-body-b" >
					<fieldset class="ui-grid-a">
							<!--div class="ui-block-a"><button type="reset" data-theme="d">Cancel</button></div-->
							<div class="ui-block-b"><button type="submit" data-theme="b" id="rf-submit">Register</button></div>
					</fieldset>
				</li>
			</ul>
		</form>
	</div><!-- /content -->
	
	<div data-role="footer" data-theme="c" style="height:45px;" data-position="fixed" data-fullscreen="true" data-id="ftr" data-tap-toggle="false">
		<div data-role="group" style="align:center;" class="center-wrapper" >
			<a data-role="button" id="home-bar-btn-home" class="selection-bar ui-btn-active ui-state-persist" data-inline="true" data-transition="slide">Home</a>
			<a data-role="button" id="home-bar-btn-jobs" href="#jobs" class="bar-home-jobs selection-bar" data-inline="true" data-transition="slidefade" data-direction="reverse">Jobs</a>
		</div>
	</div>
	<div data-role="popup" class="message-popup" id="home-popup">
		<p id="home-popup-message">This is a completely basic popup, no options set.</p>
	</div>
	
	<iframe name='home_iframe' id="home_iframe" style='display:none'></iframe>
</div>
 
<!-- Error page ------------------------------------------>

<div data-role="page" id="error" data-dom-cache="true" data-theme="c">
	<div data-role="header" data-theme="c" data-position="fixed" data-id="hdr" data-tap-toggle="false">
		<a href="#home" data-role="button" data-icon="home">Home</a>
		<h1>Error</h1>
		<a id="error-refresh-btn" data-role="button" class="ui-btn-right" data-inline="true" data-icon="refresh" data-iconpos="notext" data-mini="true"></a>		
	</div>
	<div data-role="content">
		<h1 id="error_title">Error title</h1>
		<p id="error_messages">Detail infomations</p>
	</div><!-- /content -->
</div>

<!-- Market page ------------------------------------------>

<div data-role="page" id="market">
	<div data-role="header" data-theme="c" data-position="fixed" data-id="hdr" data-tap-toggle="false">
		<a href="#home" data-role="button" data-icon="home">Home</a>
		<a href="#market-search" data-role="button" data-transition="slidedown" data-icon="search"  data-iconpos="notext" data-theme="c" data-inline="true" data-mini="true">Search</a>
		<h1>Market</h1>
		<div data-role="navbar">
            <ul>
                <li><a href="#" class="market-tab-btn ui-btn-active ui-state-persist" id="market-tab-btn-installed" data-tab="installed">Installed</a></li>
                <li><a href="#" class="market-tab-btn" id="market-tab-btn-applied" data-tab="applied">Applied</a></li>
                <li><a href="#" class="market-tab-btn" id="market-tab-btn-noninstall"  data-tab="noninstall">Noninstall</a></li>
            </ul>
        </div><!-- /navbar -->
	</div><!-- /header -->
	
	<div id="market-tab-installed" data-role="content" class="ui-body ui-body-c ui-hide" >
		<div class="card-list" id="card-list-installed">
			<!-- <div class="card">
				<div class="card-content">
					<a class="card-click-target" href="index4.html"></a>
					<div class="card-cover">
						<div class="cover-image-container">
							<div class="cover-outer-align">
								<div class="cover-inner-align">
									<a href="#detail1"><img class="card-cover-image" src="images/mc.png" /></a>
								</div>
							</div>
						</div>
					</div>
					<div class="details">
						<a class="title" href="#detail" title="Model Checking" >
								market-tab-installed
								<span class="paragraph-end"></span>
						</a>
						<div class="subtitle-container">
							<span class="price-container">
								<span class="price">Install</span>
							</span>
						</div>
					</div>
				</div>
			</div> -->
		</div> 
	</div><!-- /content -->
	<div id="market-tab-applied" data-role="content" class="ui-body ui-body-c">
		<div class="card-list" id="card-list-applied">
			
		</div> 
	</div><!-- /content -->
	<div id="market-tab-noninstall" data-role="content" class="ui-body ui-body-c">
		<div class="card-list" id="card-list-noninstall">
			
		</div> 
	</div><!-- /content -->
	
</div>

<!-- Search page  ------------------------------------------>

<div data-role="page" id="market-search" >
	<div data-role="header" data-theme="c" data-position="fixed" data-id="hdr" data-tap-toggle="false">
		<a href="#market" data-role="button" data-icon="back">Back</a>
		<h1>Search service</h1>
	</div>
	<div data-role="content" id="ms-content">
		<div id="ms-content">
		</div>
		<!--ul id="search-list" data-role="listview" data-filter="true" data-filter-reveal="true" data-filter-placeholder="Search service..."-->
		    <!--li><a href="#detail">
		        <img src="images/mc.png">
		        <h2>Model Checking</h2>
		        <p>Anlab</p></a>
		        <span class="ui-li-count price">Cancel</span>
		    </li>
		    <li><a href="#detail">
		        <img src="images/60/calendar.png">
		        <h2>Geant 4</h2>
		        <p>Nuclear Instruments and Methods in Physics Research</p></a>
		        <span class="ui-li-count price">Uninstall</span>
		    </li>
		    <li><a href="#detail">
		        <h2>Service 03</h2>
		        <p>Producer 02</p></a>
		        <span class="ui-li-count price">Install</span>>
		    </li-->
		</ul>
	</div>
	<div data-role="popup" class="message-popup" id="search-popup">
		<p id="search-popup-message">This is a completely basic popup, no options set.</p>
	</div>
</div>

<!-- Detail page ------------------------------------------>

<div data-role="page" id="detail">

	<div data-role="header" data-theme="c" >
		<a href="#market" data-role="button" data-icon="back">Back</a>
		<a id="detail-refresh" class="ui-btn-right" href="#" data-role="button" data-icon="refresh" data-iconpos="notext"></a>
		<h1 id="detail-title">Market</h1>
	</div><!-- /header -->
	
	<div data-role="content" >
        <div class="ui-grid-a">
            <div class="ui-block-a" style="width:25%; min-width:220px; align:center;">
                <div id="detail-image"> 
                	<!-- <img src="images/mc.png" alt="mc" /> -->
                </div>
            </div>
            <div class="ui-block-b" style="width:70%; min-width:500px;">
                <div style="float:left;padding-right:10%;width:95%;align:left;">
                    <h1 id="detail-name" class="long_name"></h1>
                   <!--  <span><h3><i>Producer: </i><span id="detail-producer"></span></h3></span> -->
                    <span><h3><i>Provider: </i><span id="detail-provider"></span></h3></span>
                    <hr>
                    <span id="detail-container-button">
                    	<a data-role="button" id="detail-button" data-icon="delete" data-theme="c" data-textonly="false" data-textvisible="true" data-inline="true">
                    		<label id="detail-button-label"></label>
                    	</a>
                    	<!-- <button id="detail-uninstall" data-icon="delete" data-theme="c" data-textonly="false" data-textvisible="true" data-inline="true">UNINSTALL</button>
                    	<button id="detail-cancel" data-icon="delete" data-theme="c" data-textonly="false" data-textvisible="true" data-inline="true">CANCEL</button>
                    	<button id="detail-install" data-icon="check" data-theme="c" data-textonly="false" data-textvisible="true" data-inline="true">INSTALL</button>
                    	<button id="detail-open" data-icon="check" data-theme="c" data-textonly="false" data-textvisible="true" data-inline="true">OPEN</button> -->					    
					</span>
                </div> 
            </div>
	        <div class="ui-block-a" style="width:100%">
	        <hr/>
				<h2>Description</h2>
				<p id="detail-description" style="text-indent: 30px;font:300 13px/1.4 'Roboto',Arial,sans-serif" > 
				<!-- Property checking is used for verification instead of equivalence checking when two descriptions are not functionally equivalent. Particularly, during refinement, the specification is complemented with the details that are unnecessary in the higher level specification. Yet, there is no need to verify the newly introduced properties against the original specification. It is not even possible. Therefore, the strict bi-directional equivalence check is relaxed to one-way property checking. The implementation or design is regarded a model of the circuit whereas the specifications are properties that the model must satisfy.[1]
	An important class of model checking methods have been developed for checking models of hardware and software designs where the specification is given by a temporal logic formula. Pioneering work in the model checking of temporal logic formulae was done by E. M. Clarke and E. A. Emerson[2][3][4] and by J. P. Queille and J. Sifakis.[5] Clarke, Emerson, and Sifakis shared the 2007 Turing Award for their work on model checking.[6][7]
	Model checking is most often applied to hardware designs. For software, because of undecidability (see computability theory) the approach cannot be fully algorithmic; typically it may fail to prove or disprove a given property.
	The structure is usually given as a source code description in an industrial hardware description language or a special-purpose language. Such a program corresponds to a finite state machine (FSM), i.e., a directed graph consisting of nodes (or vertices) and edges. A set of atomic propositions is associated with each node, typically stating which memory elements are one. The nodes represent states of a system, the edges represent possible transitions which may alter the state, while the atomic propositions represent the basic properties that hold at a point of execution.
	Formally, the problem can be stated as follows: given a desired property, expressed as a temporal logic formula p, and a structure M with initial state s, decide if . If M is finite, as it is in hardware, model checking reduces to a graph search. -->
				</p>
	        </div>
        </div>
        
    </div>
    <div data-role="popup" id="detail-popup" data-overlay-theme="a" class="ui-content message-popup">
    	<p id="detail-popup-message">This is a completely basic popup, no options set.</p>
	</div>
</div>

<!-- App page ------------------------------------------>

<div data-role="page" id="app">
	<div data-role="header" data-theme="c" data-position="fixed" data-id="hdr" data-tap-toggle="false">
		<a href="#home" data-role="button" data-icon="back" data-icon="back">Back</a>
		<h1 id="app-title"></h1>
		
		<div class="ui-btn-right" data-role="controlgroup" data-type="horizontal">
			<a id="app-btn-refresh" href="#" data-role="button" data-icon="refresh" data-iconpos="notext">Refresh</a>
			<a href="#app-status-menu" data-role="button" data-rel="popup" data-icon="bars" data-iconpos="notext">Menu</a>
		</div>
		<!-- <div data-role="navbar">
		    <ul>
		        <li><a href="#app" class="ui-btn-active ui-state-persist" >Status</a></li>
		        <li><a href="#app-submit">Submit Job</a></li>
		        <li><a href="#app-detail">Details</a></li>
		    </ul>
		</div> /navbar -->
	</div><!-- /header -->
	
	<div data-role="content">
		<!-- form id="app-filter-form">
		     <label for="app-filter-text" class="ui-hidden-accessible">Search:</label>
		     <input type="search" name="app-filter-text" id="app-filter-text" value="">
		</form -->
		<ul id="app-list-jobs" data-role="listview" data-inset="true"><!-- data-filter="true" data-filter-placeholder="Search jobs..."  data-inset="true"-->
		    <!-- <li>
	    		<a>
			    	<h2>Job01</h2>
			    	<p>Submit date: &nbsp;&nbsp; <strong>20/12/20135:59:07 PM</strong></p>
			    	<p class="ui-li-aside">Duration&nbsp;&nbsp;<strong>1:06:24:40</strong></p>
			    </a>
			    <span class="ui-li-count">Running</span>
	    	</li>
	    	<li>
	    		<a>
			    	<h2>Job02</h2>
			    	<p>Submit date: &nbsp;&nbsp; <strong>20/12/20135:59:07 PM</strong></p>
			    	<p class="ui-li-aside">Duration&nbsp;&nbsp;<strong>1:06:24:40</strong></p>
			    </a>
			    <span class="ui-li-count">Running</span>			    	
	    	</li>
	    	<li>
	    		<a>
			    	<h2>Job03</h2>
			    	<p>Submit date: &nbsp;&nbsp; <strong>20/12/20135:59:07 PM</strong></p>
			    	<p class="ui-li-aside">Duration&nbsp;&nbsp;<strong>1:06:24:40</strong></p>
		    	</a>
		    	<span class="ui-li-count">Finished</span>			    	
	    	</li> -->
		    
		</ul>
		<p id="app-list-message" align="center">&nbsp</p>
		<!-- center>
			<div data-role="controlgroup" data-type="horizontal" data-mini="true">
			    <a id="app-previous-page-btn" href="#" data-role="button" data-iconpos="notext" data-icon="arrow-l" data-theme="c">Previous</a>
			    <a id="app-current-page-btn" data-role="button" data-theme="c">3/4</a>
			    <a id="app-next-page-btn" href="#" data-role="button" data-iconpos="notext" data-icon="arrow-r" data-theme="c">Next</a>
			</div>
		</center>
		 -->
	</div>
	<div data-role="popup" id="app-status-menu" class="app-menu" data-theme="d">
        <ul data-role="listview" data-inset="true" style="min-width:80px;" data-theme="d">
            <li data-role="divider" data-theme="c">Choose an action</li>
            <li data-icon="false"><a class="app-menu-btn-remove" data-tab="status" href="#">Remove this app</a></li>
            <li data-icon="false"><a href="#" data-rel="back">Cancel</a></li>
        </ul>
	</div>
	<div data-role="popup" id="app-popup" class="ui-content message-popup" data-overlay-theme="a">
    	<p id="app-popup-message">This is a completely basic popup, no options set.</p>
	</div>
</div>


<div data-role="page" id="app-submit">
	<div data-role="header" data-theme="c" data-position="fixed" data-id="hdr" data-tap-toggle="false">
		<a href="#home" data-role="button" data-icon="back">Back</a>
		<h1 id="submit-title"></h1>
		<div class="ui-btn-right" data-role="controlgroup" data-type="horizontal">
			<a id="app-submit-btn-refresh" href="#" data-role="button" data-icon="refresh" data-iconpos="notext">Refresh</a>
			<a href="#app-submit-menu" data-role="button" data-rel="popup" data-icon="bars" data-iconpos="notext">Menu</a>
		</div>
		<div data-role="navbar">
		    <ul>
		        <li><a href="#app">Status</a></li>
		        <li><a href="#app-submit" class="ui-btn-active ui-state-persist">Submit Job</a></li>
		        <li><a href="#app-detail">Details</a></li>
		    </ul>
		</div><!-- /navbar -->
	</div><!-- /header -->
	<div data-role="content">
		<!-- <div data-role="fieldcontain">
		    <label for="submit-cmbx-method"><strong>Run methods</strong></label>
		    <select name="runMethodId" id="submit-cmbx-method" data-mini="true">
		    </select>
		</div> -->
		<!-- <div class="submit-form-border"> -->
			<form id="submit-form" data-ajax="false" enctype="multipart/form-data">
			</form>
		<!-- </div> -->
	
	</div><!-- /content -->
	
	<!-- <div data-role="popup" class="message-popup" id="submit-popup">
		<p id="submit-popup-message" class="ui-body-d" style="padding:2em;">
			This is a completely basic popup, no options set.
			<a href="#" data-rel="popup" data-role="button" class="ui-icon-alt" data-inline="true" data-transition="pop" data-icon="info" data-theme="e" data-iconpos="notext"></a>
		</p>
	</div> -->
	
	<div data-role="popup" id="app-submit-menu" class="app-menu" data-theme="d">
        <ul data-role="listview" data-inset="true" style="min-width:150px;" data-theme="d">
            <li data-role="divider" data-theme="c">Choose an action</li>
            <li data-icon="false"><a class="app-menu-btn-remove" data-tab="submit" href="#">Remove this app</a></li>
            <li data-icon="false"><a href="#" data-rel="back">Cancel</a></a></li>
        </ul>
	</div>
	<div data-role="popup" id="submit-popup" data-overlay-theme="a" class="ui-content message-popup">
    	<p id="submit-popup-message">This is a completely basic popup, no options set.</p>
	</div>
</div>

<div data-role="page" id="app-detail">
	<div data-role="header" data-theme="c" data-id="hdr" data-tap-toggle="false">
	<!-- <div data-role="header" data-theme="c" data-position="fixed" data-id="hdr" data-tap-toggle="false"> -->
		<a href="#home" data-role="button" data-icon="back" data-icon="back">Back</a>
		<!-- <a id="app-detail-refresh" class="ui-btn-right" href="#" data-role="button" data-icon="refresh" data-iconpos="notext"></a> -->
		<h1 id="app-detail-name"></h1>
		
		<div class="ui-btn-right" data-role="controlgroup" data-type="horizontal">
			<a href="#" id="app-detail-btn-refresh" data-role="button" data-icon="refresh" data-iconpos="notext">Refresh</a>
			<a href="#app-detail-menu" data-role="button" data-rel="popup" data-icon="bars" data-iconpos="notext">Menu</a>
		</div> 
		<div data-role="navbar" data-theme="b">
		    <ul>
		        <li><a href="#app">Status</a></li>
		        <li><a href="#app-submit">Submit Job</a></li>
		        <li><a href="#app-detail" class="ui-btn-active ui-state-persist">Details</a></li>
		    </ul>
		</div><!-- /navbar -->
	</div><!-- /header -->
	
	<div data-role="content" >
        <div class="ui-grid-a" style="padding-top:5px;padding-bottom:5px;padding-right:5px;padding-left:5px;">
            <div class="ui-block-a" style="width:25%; min-width:220px; text-align:center;">
                <div id="app-detail-image"> 
                </div>
            </div>
            <div class="ui-block-b" style="width:70%; min-width:500px; margin: 0 10px 0 0;">
                <div style="float:left;padding-right:10%;width:95%;align:left;">
                    <!-- <h1 id="app-detail-title"></h1> -->
                    <!-- <span><h3><i>Service ID: </i></h3><h2><span id="app-detail-service-id"></span><h2></span> -->
                    
                    <span><h3><i>ServiceID: </i><span id="app-detail-service-id"></span></h3></span>
                    <!-- <span><h3><i>Producer: </i><span id="app-detail-producer"></span></h3></span> -->
                    <span><h3><i>Provider: </i><span id="app-detail-provider"></span></h3></span>
                </div> 
            </div>
	        <div class="ui-block-a" style="width:100%">
	        <hr/>
				<h2>Description</h2>
				<p id="app-detail-description" style="text-indent: 30px;font:300 13px/1.4 'Roboto',Arial,sans-serif" > 
				<!-- Property checking is used for verification instead of equivalence checking when two descriptions are not functionally equivalent. Particularly, during refinement, the specification is complemented with the details that are unnecessary in the higher level specification. Yet, there is no need to verify the newly introduced properties against the original specification. It is not even possible. Therefore, the strict bi-directional equivalence check is relaxed to one-way property checking. The implementation or design is regarded a model of the circuit whereas the specifications are properties that the model must satisfy.[1]
	An important class of model checking methods have been developed for checking models of hardware and software designs where the specification is given by a temporal logic formula. Pioneering work in the model checking of temporal logic formulae was done by E. M. Clarke and E. A. Emerson[2][3][4] and by J. P. Queille and J. Sifakis.[5] Clarke, Emerson, and Sifakis shared the 2007 Turing Award for their work on model checking.[6][7]
	Model checking is most often applied to hardware designs. For software, because of undecidability (see computability theory) the approach cannot be fully algorithmic; typically it may fail to prove or disprove a given property.
	The structure is usually given as a source code description in an industrial hardware description language or a special-purpose language. Such a program corresponds to a finite state machine (FSM), i.e., a directed graph consisting of nodes (or vertices) and edges. A set of atomic propositions is associated with each node, typically stating which memory elements are one. The nodes represent states of a system, the edges represent possible transitions which may alter the state, while the atomic propositions represent the basic properties that hold at a point of execution.
	Formally, the problem can be stated as follows: given a desired property, expressed as a temporal logic formula p, and a structure M with initial state s, decide if . If M is finite, as it is in hardware, model checking reduces to a graph search.
				 -->
				 </p>
	        </div>
        </div>
    </div> <!-- /content -->
    
    <div data-role="footer" data-theme="c" style="height:5px;" data-position="fixed" data-id="ftr" data-fullscreen="true">
    </div><!-- /footer --> 
    <div data-role="popup" id="app-detail-menu" class="app-menu" data-theme="d">
        <ul data-role="listview" data-inset="true" style="min-width:150px;" data-theme="d">
            <li data-role="divider" data-theme="c">Choose an action</li>
            <li data-icon="false"><a class="app-menu-btn-remove" data-tab="detail" href="#">Remove this app</a></li>
            <li data-icon="false"><a href="#" data-rel="back">Cancel</a></a></li>
        </ul>
	</div>
	<div data-role="popup" id="app-detail-popup" data-overlay-theme="a" class="ui-content message-popup">
    	<p id="app-detail-popup-message">This is a completely basic popup, no options set.</p>
	</div>
</div>

<!-- Jobs page ------------------------------------------>

<div data-role="page" id="jobs" class="swipe-page" data-dom-cache="true" data-theme="c" data-prev="home" data-url="jobs">
	<div data-role="header" data-theme="c" data-position="fixed" data-id="hdr" data-tap-toggle="false">
		<h1>Jobs</h1>
		<a class="jobs-btn-refresh ui-btn-right" data-tab="jobs" href="#" data-role="button" data-icon="refresh" data-iconpos="notext">Refresh</a>
        <!-- <a href="#home" data-role="button" data-icon="home">Home</a> -->
		<div data-role="navbar">
		    <ul>
		    	<li><a  id="jobs-tab-btn-running" class="ui-btn-active ui-state-persist" >Running</a></li>
		        <li><a class="jobs-tab-btn" href="#jobs-finished">Finished</a></li>
		    </ul>
		</div><!-- /navbar -->
	</div><!-- /header -->
	<div data-role="content">
		<div id="jobs-services-list" data-role="collapsible-set" data-theme="e" data-content-theme="d">
			
			<!-- <div data-role="collapsible" data-collapsed="false">
	        	<h2>Model Checking (2)</h2>
				<ul data-role="listview" data-filter="true" data-filter-placeholder="Search jobs..." data-inset="true">
				<ul data-role="listview">
					<li data-role="list-divider" data-theme="c" >Total jobs<span class="ui-li-count">2</span></li>
			    	<li>
			    		<a>
					    	<h2>Job01</h2>
					    	<p>Submit date: &nbsp;&nbsp; <strong>20/12/20135:59:07 PM</strong></p>
					    	<p class="ui-li-aside">Duration&nbsp;&nbsp;<strong>1:06:24:40</strong></p>
					    </a>				    	
			    	</li>
			    	<li>
			    		<a>
					    	<h2>Job02</h2>
					    	<p>Submit date: &nbsp;&nbsp; <strong>20/12/20135:59:07 PM</strong></p>
					    	<p class="ui-li-aside">Duration&nbsp;&nbsp;<strong>1:06:24:40</strong></p>
					    </a>				    	
			    	</li>
			    	<li>
			    		<a>
					    	<h2>Job03</h2>
					    	<p>Submit date: &nbsp;&nbsp; <strong>20/12/20135:59:07 PM</strong></p>
					    	<p class="ui-li-aside">Duration&nbsp;&nbsp;<strong>1:06:24:40</strong></p>
				    	</a>				    	
			    	</li>
				</ul>
			</div>
			<div data-role="collapsible">
	        	<h2>Geant 4 (3)</h2>
				<ul data-role="listview" data-filter="true" data-filter-placeholder="Search jobs..." data-inset="true">
				
				<ul data-role="listview">
				    <li>
			    		<a>
					    	<h2>Job101</h2>
					    	<p>Submit date: &nbsp;&nbsp; <strong>20/12/20135:59:07 PM</strong></p>
					    	<p class="ui-li-aside">Duration&nbsp;&nbsp;<strong>1:06:24:40</strong></p>
					    </a>				    	
			    	</li>
			    	<li>
			    		<a>
					    	<h2>Job102</h2>
					    	<p>Submit date: &nbsp;&nbsp; <strong>20/12/20135:59:07 PM</strong></p>
					    	<p class="ui-li-aside">Duration&nbsp;&nbsp;<strong>1:06:24:40</strong></p>
					    </a>				    	
			    	</li>
			    	<li>
			    		<a>
					    	<h2>Job103</h2>
					    	<p>Submit date: &nbsp;&nbsp; <strong>20/12/20135:59:07 PM</strong></p>
					    	<p class="ui-li-aside">Duration&nbsp;&nbsp;<strong>1:06:24:40</strong></p>
				    	</a>				    	
			    	</li>
				</ul>
			</div> -->
		</div>
		<div id="jobs-message" style="text-align:center"></div>
	</div><!-- /content -->
	<div data-role="footer" data-theme="c" style="height:45px;" data-position="fixed" data-fullscreen="true" data-id="ftr" data-tap-toggle="false">
		<div data-role="group" style="align:center;" class="center-wrapper" >
			<a data-role="button" href="#home" class="bar-home-jobs selection-bar" data-inline="true" data-transition="slide" data-direction="reverse">Home</a>
			<a data-role="button" id="jobs-bar-btn-jobs" class="selection-bar ui-btn-active ui-state-persist" data-inline="true" data-transition="slidefade">Jobs</a>
		</div>
	</div>
</div>

<div data-role="page" id="jobs-finished" class="swipe-page" data-dom-cache="true" data-theme="c" data-prev="home" data-url="jobs-finished">
	<div data-role="header" data-theme="c" data-position="fixed" data-id="hdr" data-tap-toggle="false">
		<h1>Jobs</h1>
        <a class="jobs-btn-refresh ui-btn-right" data-tab="jobs-finished" href="#" data-role="button" data-icon="refresh" data-iconpos="notext">Refresh</a>
        <!-- <a href="#" data-role="button" data-rel="back" data-icon="back">Back</a> -->
		<div data-role="navbar">
		    <ul>
		        <li><a class="jobs-bar-btn" href="#jobs">Running</a></li>
		        <li><a id=jobs-tab-btn-finished" class="ui-btn-active ui-state-persist">Finished</a></li>
		    </ul>
		</div><!-- /navbar -->
	</div><!-- /header -->
	<div data-role="content">
		<div id="jobs-finished-services-list" data-role="collapsible-set" data-theme="e" data-content-theme="d">
		</div>
		<div id="jobs-finished-message" style="text-align:center"></div>
	</div><!-- /content -->
	<div data-role="footer" data-theme="c" style="height:45px;" data-position="fixed" data-fullscreen="true" data-id="ftr" data-tap-toggle="false">
		<div data-role="group" style="align:center;" class="center-wrapper" >
			<a data-role="button" href="#home"class="bar-home-jobs selection-bar" data-inline="true" data-transition="slide" data-direction="reverse">Home</a>
			<a data-role="button" class="selection-bar ui-btn-active ui-state-persist" data-inline="true" data-transition="slidefade">Jobs</a>
		</div>
	</div><!-- /footer -->
</div>

<!-- Job detail ------------------------------------------>

<div data-role="page" id="job-detail" data-theme="c">
	<div data-role="header" data-theme="c" data-position="fixed" data-id="hdr" data-tap-toggle="false">
		<a href="#" data-role="button" data-rel="back" data-icon="back">Back</a>
		<a id="job-detail-refresh" class="ui-btn-right" href="#" data-role="button" data-icon="refresh" data-iconpos="notext"></a>
		
		<h1 id="job-detail-title">(Job name)</h1>
	</div><!-- /header -->
	<div data-role="header" data-theme="c" style="overflow:hidden;">
	 	<a href="#" data-icon="arrow-l" data-iconpos="left" data-rel="back" data-transition="slide" data-direction="reverse">Back</a>
	</div>
	<div data-role="content">
		<div data-role="collapsible-set" data-theme="e" data-content-theme="c">
	    	<div data-role="collapsible" data-collapsed="false" id="jdc-jobinfo">
	    		<h2>Job info</h2>
				<ul id="job-detail-list-info" data-role="listview" data-theme="d" data-divider-theme="d"  data-collapsed="true" >
				</ul>
			</div>
		    <div data-role="collapsible" data-collapsed="true" id="jdc-browse">
				<h2>Browse directory</h2>
				<!-- <ul id="job-detail-list-browse"  data-role="listview" data-split-icon="custom" data-split-theme="d" data-inset="true"> -->
				<ul id="job-detail-list-browse" data-role="listview" data-theme="d" data-inset="true">
				</ul>
			</div>
		</div>
	</div><!-- /content -->
	<div data-role="footer" data-theme="c" style="height:5px;" data-fullscreen="true">
	</div><!-- /footer -->
	<div data-role="popup" id="jdetail-popup" data-overlay-theme="a" class="ui-content message-popup">
    	<p id="jdetail-popup-message">This is a completely basic popup, no options set.</p>
	</div>
</div>



<!-- Provider home page ------------------------------------------>
<div data-role="page" id="provider" data-theme="c">
	<div data-role="panel" id="provider-menu-panel" data-display="overlay"> 
		<h2>Provider</h2>
		<ul data-role="listview" data-inset="true" data-divider-theme="d">
			<li data-role="list-divider">Go to page</li>
			<li><a href="#home">User</a></li>
			<li><a href="#provider-image">Image Management</a></li>
			<li data-role="list-divider">Actions</li>
			<li><a href="#" onclick="uRegister_rejectProvider()">Remove Register</a></li>
			<li><label id="provider-publickey-lbl" for="provider-publickey" style="margin-left: 10px;margin-right: auto !important;"><b>Public Key</b></label>
			<textarea cols="40" rows="8" name="provider-publickey" id="provider-publickey" data-mini="true" readonly></textarea></li>
		</ul>
	</div>
	<div data-role="header" data-theme="c" data-position="fixed">
		<a class="menu-btn" href="#provider-menu-panel" data-role="button" data-icon="bars" data-iconpos="notext" data-theme="c" data-inline="true" data-panel="home-menu-panel">Menu</a>
		<!--div class="ui-grid-b">
			<a href="#" id="removeRegister-btn" class="removeRegister-btn ui-corner-all" data-role="button" style=" width:10%;min-width:100px;" data-inline="true" data-transition="pop">Remove Register</a>
			<a href="#" id="userPage-btn" class="userPage-btn ui-corner-all" data-role="button" style=" width:10%;min-width:100px;" data-inline="true" data-transition="pop">User Page</a>
			<a href="#provider-image" id="imagesMng-btn" class="imagesMng-btn ui-corner-all" data-role="button" style=" width:10%;min-width:100px;" data-inline="true" data-transition="pop">Image Management</a>
		</div-->
		
		<a href="#" id="login-btn" class="ui-btn-right login-btn" data-role="button" style=" width:20%;min-width:130px;" data-inline="true" data-icon="check" data-transition="pop"> 
			<label id="login-txt" class="login-txt" style=" width:40%;min-width:350px;"></label>
		</a>
		<h1>Provider</h1>
		<!--div style="text-align: right;font-size: 15px; font-weight=bold"">
			<label>The remain usable time:<span id="pt-remain" style="text-align: right;font-size: 18px;font-weight=bold"> 0000(Ms)</span>&nbsp;&nbsp;</label>
			</br>
			<label>Total usable time:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span id="pt-total" style="text-align: right;font-size: 18px;font-weight=bold">0000(Ms)</span>&nbsp;&nbsp;</label>
		</div-->
		<div data-role="navbar">
            <ul>
                <li><a id="provider-tab-btn-quotatime" href="#" data-role="tab" class="provider-tab-btn" data-tab="quotatime" data-transition="slide">Time Quota</a></li>
                <li><a id="provider-tab-btn-apps" href="#" data-role="tab" class="provider-tab-btn" data-tab="apps" data-transition="slide">Apps</a></li>
                <li><a id="provider-tab-btn-waitlist" href="#" data-role="tab" class="provider-tab-btn" data-tab="waitlist" data-transition="slide" >Wait List</a></li>
                <li><a id="provider-tab-btn-newapp" href="#" data-role="tab" class="provider-tab-btn" data-tab="newapp" data-transition="slide" class="ui-btn-active ui-state-persist">New App</a></li>
            </ul>
        </div>
	</div><!-- /header -->
	<!-- New App tab -->
	<div id="provider-tab-newapp" data-role="content">
		<form id="provider-newapp-form" data-ajax="false" enctype="multipart/form-data">
		<div data-role="collapsible-set" data-theme="e" data-content-theme="c">
	    	<div id="provider-properties-collapsible" class="provider-collapsible" data-role="collapsible" data-collapsed="false">
    			<h2>Properties</h2>
	    		<ul data-role="listview" data-theme="d" data-divider-theme="d">
		    		<li>
		    		<!-- <div data-role="fieldcontain">
						<label for="provider-newapp-id">Id</label>
						<input type="text" name="provider-newapp-id" id="provider-newapp-id" value="">
					</div> -->
					<div data-role="fieldcontain">
						<label for="pnaf-name">Name *</label>
						<input type="text" name="pnaf-name" id="pnaf-name" value="" data-clear-btn="true" maxlength="300" required pattern="[^><=\\\!\@\#\$\%\&\?|]*" title="without [><=\!@#$%&?] character" >
					</div>
					<div data-role="fieldcontain">
						<label for="provider-newapp-path"><h2>Application Path</h2></label>
					</div>
					<div data-role="fieldcontain">
						<label for="pnaf-image">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Docker Image Name *</label>
						<select name="pnaf-image" id="pnaf-image" required>
							<!--option value="1">RunMethod01</option>
					        <option value="2">RunMethod02</option>
					        <option value="3">RunMethod03</option>
					        <option value="4">... New method ...</option-->
						</select>
					</div>
					<div data-role="fieldcontain">
						<label for="pnaf-path">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Path *</label>
						<input type="text" data-clear-btn="false" name="pnaf-path" id="pnaf-path" value="" pattern="[^><$]*" title="without [^><$] character" data-clear-btn="true" placeholder="/path/to/executable" required>
					</div>
					<div data-role="fieldcontain">
                                                <label for="provider-newapp-path"><h2>Script File Path</h2></label>
                                        </div>

					<div data-role="fieldcontain">
						<label for="pnaf-shpath">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Job script template</label>
						<input type="text" data-clear-btn="false" name="pnaf-shpath" id="pnaf-shpath" value="" title="without [^><$] character" data-clear-btn="true" placeholder="/path/to/sh/file">
					</div>
                                        <div data-role="fieldcontain">
                                                <label for="pnaf-preshpath">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pre-processing command</label>
                                                <input type="text" data-clear-btn="false" name="pnaf-preshpath" id="pnaf-preshpath" value="" title="without [^><$] character" data-clear-btn="true" placeholder="/path/to/sh/file">
                                        </div>
                                        <div data-role="fieldcontain">
                                                <label for="pnaf-postshpath">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Post-processing command</label>
                                                <input type="text" data-clear-btn="false" name="pnaf-postshpath" id="pnaf-postshpath" value="" title="without [^><$] character" data-clear-btn="true" placeholder="/path/to/sh/file">
                                        </div>
					<!-- <div data-role="fieldcontain">					
						<label for="provider-newapp-producer">Producer</label>
						<input type="text" data-clear-btn="true" name="provider-newapp-producer" id="provider-newapp-producer" value="">
					</div> -->
					<div data-role="fieldcontain">
						<label for="pnaf-stgin">Folder stage in</label>
						<input type="text" name="pnaf-stgin" id="pnaf-stgin" placeholder="/path/to/folder" data-clear-btn="true" maxlength="300" pattern="[^><=\\\!\@\#\$\%\&\?| ]*" title="without [><=\!@#$%&? ] character" >
					</div>
					<!--p class="folder-stage"> specify another folder</p-->
					<div data-role="fieldcontain">
						<label for="pnaf-stgout">Folder stage out</label>
						<input type="text" name="pnaf-stgout" id="pnaf-stgout" placeholder="folder name" data-clear-btn="true" maxlength="300" pattern="[^><=\\\!\@\#\$\%\&\?| ]*" title="without [><=\!@#$%&?] character" >
					</div>
					<!--p class="folder-stage"> specify another folder</p-->
					<div data-role="fieldcontain">
						<label for="pnaf-icon">Icon</label>
						<input type="file" data-clear-btn="true" name="pnaf-icon" id="pnaf-icon" value="" accept="image/*">
					</div>
					<div data-role="fieldcontain">	
						<label for="pnaf-elapsed">Max elapsed time (s)</label>
						<input type="number" data-clear-btn="true" name="pnaf-elapsed" id="pnaf-elapsed" value="" pattern="[0-9]{1,10}" required>	
					</div>
					<div data-role="fieldcontain">	
						<label for="pnaf-nodes">Number of nodes</label>
						<input type="number" data-clear-btn="true" name="pnaf-nodes" id="pnaf-nodes" value="" pattern="[0-9]{1,10}" required >
					</div>
					<div data-role="fieldcontain">	
						<label for="pnaf-descript">Description</label>
						<textarea cols="40" rows="8" name="pnaf-descript" id="pnaf-descript"></textarea>
					</div>
					<!--div data-role="fieldcontain">
						<label for="pnaf-publish" >Publish</label>
						<select name="pnaf-publish" id="pnaf-publish" data-role="slider">
							<option value="0">Off</option>
							<option value="1">On</option>
						</select>
					</div-->
					</li>
				</ul>
			</div>
			
		    <div id="provider-runmethod-collapsible" class="provider-collapsible" data-role="collapsible" data-collapsed="true">
				<h2>Run Method</h2>
				
				<!-- <ul id="job-detail-list-browse"  data-role="listview" data-split-icon="custom" data-split-theme="d" data-inset="true"> -->
				
				<ul data-role="listview" data-theme="d" data-divider-theme="d" >
					<!-- <li data-role="list-divider">Properties</li> -->
					<!-- <li>
						<label for="submit-cmbx-method">Run methods</label>
						<select name="runMethodId" id="submit-cmbx-method" data-mini="true">
							<option value="1">RunMethod01</option>
					        <option value="2">RunMethod02</option>
					        <option value="3">RunMethod03</option>
					        <option value="4">... New method ...</option>
						</select>
					</li> -->
					<!-- <li data-role="fieldcontain">
						<!-- <label for="name">Name</label>
						<input type="text" data-clear-btn="false" name="name" id="name" value="cp_abc"> -->
						<!-- <label for="provider-newapp-path">Path</label>
						<input type="text" data-clear-btn="false" name="provider-newapp-path" id="provider-newapp-path" value="/usr/local/cp"> -->
						<!-- <label for="textarea-13">Description</label>
						<textarea cols="40" rows="8" name="textarea-13" id="textarea-13"></textarea> 
					</li> -->
					<li data-role="list-divider">Parameters</li>
					<li data-role="fieldcontain">
					<!-- <table data-role="table" id="table-column-toggle" data-mode="columntoggle" class="ui-responsive table-stroke"> -->
					<table data-role="table" id="provider-newapp-params-table" data-mode="reflow" class="ui-responsive table-stroke" style="min-width:230px;width:100%">
						<thead>
							<tr class="ui-bar-d">
								<th><abbr title="* is required, out is output">Name</abbr></th></th>
								<!-- <th data-priority="2">Description</th> -->
								<th>Type</th>
								<!-- <th data-priority="2">is Requied/Output</th> -->
								<th>Edit</th>
								<th>Delete</th>
							</tr>
						</thead>
						<tbody>
							<tr>	
							    <td>(No parameter)</td>	
							    <td></td>	
							    <td></td>	
							    <td></td>	
							</tr>	
							<!-- 
						  <tr>
						    <th>name1&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</th>
						    <td>file</td>
						    <!-- <td><a class="provider-param-edit-btn" href="#provider-param-popup" data-paramid="paramid01" data-role="button" data-rel="popup" data-icon="edit" data-iconpos="notext" data-theme="c" data-inline="true" data-rel="popup" data-position-to="window" data-inline="true" data-theme="a" data-transition="pop"></a></td> -->
						    <!--<td><a class="provider-param-edit-btn" data-paramid="paramid01" data-role="button" data-rel="popup" data-icon="edit" data-iconpos="notext" data-theme="c" data-inline="true" data-rel="popup" data-position-to="window" data-inline="true" data-theme="a" data-transition="pop"></a></td>
						    <td><a class="provider-newapp-param-delete-btn" data-paramid="paramid01" data-role="button" data-icon="delete" data-iconpos="notext" data-theme="c" data-inline="true" data-mini="true"></a></td>
						  </tr>
						  <tr>
						    <th>name2</th>
						    <td>string</td>
						    <td><a class="provider-param-edit-btn" data-paramid="paramid01" data-role="button" data-rel="popup" data-icon="edit" data-iconpos="notext" data-theme="c" data-inline="true"></a></td>
						    <td><a class="provider-newapp-param-delete-btn" data-paramid="paramid01" data-rel="popup" data-role="button" data-inline="true" data-icon="delete" data-iconpos="notext" data-theme="c" data-transition="pop"  data-position-to="window">Delete</a></td>
						  </tr>
						  <tr>
						    <th>name3</th>
						    <td>i32</td>
						    <td><a class="provider-param-edit-btn" data-paramid="paramid01" data-role="button" data-rel="popup" data-icon="edit" data-iconpos="notext" data-theme="c" data-inline="true"></a></td>
						    <td><a class="provider-newapp-param-delete-btn" data-paramid="paramid01" data-rel="popup" data-role="button" data-inline="true" data-icon="delete" data-iconpos="notext" data-theme="c" data-transition="pop"  data-position-to="window">Delete</a></td>
						  </tr>
						  <!-- 
						  <tr>
						    <th>name4</th>
						    <td>i64</td>
						    <td><a href="#" data-role="button" data-icon="edit" data-iconpos="notext" data-theme="c" data-inline="true"></a></td>
						    <td><a href="#" data-role="button" data-icon="delete" data-iconpos="notext" data-theme="c" data-inline="true"></a></td>
						  </tr> -->
						  <tr>
						    <th><a class="provider-newapp-param-add-btn" href="#provider-newparam-popup" data-role="button" data-rel="popup" data-icon="add" data-inline="true" data-mini="true" data-position-to="window" >Add</a></th>
						    <td></td>
						    <td></td>
						    <td></td>
						  </tr>
						  
						</tbody>
					</table>
					</li>
					<!--
					<li data-role="list-divider">Default outputs</li>
					<li data-role="fieldcontain">
					
					<table data-role="table" id="table-column-toggle" data-mode="reflow" class=" ui-responsive table-stroke">
					<!-- <table data-role="table" id="movie-table" data-mode="reflow" class="ui-responsive table-stroke" style="min-width:230px"> -->
						<!--<thead>
							<tr class="ui-bar-d">
								<!-- <th ><abbr title="* is required, out is output">Name</abbr></th></th> -->
								<!-- <th data-priority="2">Description</th> -->
								<!-- <th>Path &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</th>
								<!-- <th>Type</th> -->
								<!-- <th data-priority="2">is Requied/Output</th> -->
								
								<!--<th>Edit</th>
								<th>Delete</th>
							</tr>
						</thead>
						<tbody>
						  <tr>
						    <td>output.txt&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						    </td>
						    <!-- <td>file</td> -->
						    <!--<td><a href="#provider-param-popup" data-role="button" data-icon="edit" data-iconpos="notext" data-theme="c" data-inline="true" data-rel="popup" data-position-to="window" data-inline="true" data-theme="a" data-transition="pop"></a></td>
						    <td><a href="#popupDialog" data-rel="popup" data-position-to="window" data-role="button" data-inline="true" data-transition="pop" data-icon="delete" data-theme="c" data-icon="delete" data-iconpos="notext">Delete</a></td>
						  </tr>
						  <tr>
						    <td>Result/output.txt</td>
						    <!-- <td>file(stdout)</td> -->
						   <!-- <td><a href="#provider-param-popup" data-role="button" data-icon="edit" data-rel="popup"  data-iconpos="notext" data-theme="c" data-inline="true"></a></td>
						    <td><a href="#" data-role="button" data-icon="delete" data-iconpos="notext" data-theme="c" data-inline="true"></a></td>
						  </tr>
						  <tr>
						    <td>Data</td>
						    <!-- <th>directory</th> -->
						   <!-- <td><a href="#" data-role="button" data-icon="edit" data-iconpos="notext" data-theme="c" data-inline="true"></a></td>
						    <td><a href="#" data-role="button" data-icon="delete" data-iconpos="notext" data-theme="c" data-inline="true"></a></td>
						  </tr>
						 <!--  <tr>
						    <td>Results</td>
						    <th>directory</th>
						    <td><a href="#" data-role="button" data-icon="edit" data-iconpos="notext" data-theme="c" data-inline="true"></a></td>
						    <td><a href="#" data-role="button" data-icon="delete" data-iconpos="notext" data-theme="c" data-inline="true"></a></td>
						  </tr>
						  <tr> -->
						   <!-- <th><a href="#" data-role="button" data-icon="add" data-theme="c" data-inline="true" data-mini="true">Add</a></th>
						    <!-- <td></td> -->
						   <!-- <td></td>
						    <td></td>
						  </tr>  
						</tbody>
					</table>
	
					</li>-->
					<li data-role="list-divider">Preview</li>
					<li>
						<label for="provider-newapp-preview" class="ui-hidden-accessible">Description</label>
						<textarea readonly cols="40" rows="8" name="provider-newapp-preview" id="provider-newapp-preview"></textarea>
					</li>
				</ul>
			</div>
		</div>
		<div>
			<div id="provider-submit-msg" style="color:red;text-indent: 30px;font:300 13px/1.4 'Roboto',Arial,sans-serif"></div>
			<p>
			    <button id="provider-newapp-submit-btn" type="submit" data-inline="true" data-icon="check" disabled>Next</button>
			    <button id="provider-newapp-reset-btn" type="reset" data-inline="true" disabled>Clear</button>
			</p>
		</div>
		</form>
	</div><!-- /content -->
	
	<!-- List app tab -->
	<div id="provider-tab-apps" data-role="content">
		<ul id="provider-apps-list" data-role="listview" data-filter="true" data-filter-placeholder="Search service..." data-inset="true">
		  <!--  <li><a href="#provider-app-detail?id=service-test&from=apps">
		        <img src="images/service_mc.png">
		        <h2>Model Checking</h2>
		        <p>Anlab</p>
		        <p class="ui-li-count price">Public</p>
		        <span class="ui-li-aside count">12</span>
		        </a>
		    </li>
		    <li><a href="#provider-app-detail?id=service-test&from=apps">
		        <img src="images/service_g4.png">
		        <h2>Geant 4</h2>
		        <p>Nuclear Instruments and Methods in Physics Research</p>z
		        <p class="ui-li-count price" ><strong>Private</strong></p>
		        <span class="ui-li-aside ">0</span>
		        </a>
		    </li>
		    <li><a href="#provider-app-detail?id=service-test&from=apps">
		    	<img src="images/service_cp.png">
		        <h2>Service 03</h2>
		        <p>Producer 02</p>
		        <span class="ui-li-aside "><strong>0</strong></span>
		        <p class="ui-li-count price">Private</p>
		        </a>
		    </li> -->
		</ul>
	</div>
	<!-- Wait list tab -->
	<div id="provider-tab-waitlist"  data-role="content">
		<ul id="provider-wait-list" data-role="listview" data-filter="true" data-filter-placeholder="Search service..." data-inset="true">
		  <!--  <li><a href="#provider-app?id=service_test&from=waitlist">
		        <img src="images/service_mc.png">
		        <h2>Model Checking</h2>
		        <p>Anlab</p>
		        <span class="ui-li-count "><strong>3</strong></span>
		        </a>
		    </li> -->
		    <!-- <li><a href="#detail">
		        <img src="images/service_g4.png">
		        <h2>Geant 4</h2>
		        <p>Nuclear Instruments and Methods in Physics Research</p>
		        
		        <p class="ui-li-count price" ><strong>Private</strong></p>
		        <span class="ui-li-aside ">0</span>
		        </a>
		        
		    </li>
		    <li><a href="#detail">
		    	<img src="images/service_cp.png">
		        <h2>Service 03</h2>
		        <p>Producer 02</p>
		        <span class="ui-li-aside "><strong>0</strong></span>
		        <p class="ui-li-count price">Private</p>
		        </a>
		        
		    </li> -->
		</ul>
	</div>
	
	<!-- Time quota -->
	<div id="provider-tab-quotatime"  data-role="content">
		<!--div style="text-align: left;font-size: 15px; font-weight=bold""-->
			<!--abel>The remain usable time:</label><span id="pt-remain" style="text-align: right;font-size: 18px;font-weight=bold"> 0000(Ms)</span>
			<label>Total usable time:</label><span id="pt-total" style="text-align: right;font-size: 18px;font-weight=bold">0000(Ms)</span-->
			<!--form-->
				<div data-role="fieldcontain">
					 <label for="ptq-remain-kdesk" id="ptq-remain-kdesk-lbl">The remain usable time on K-Computer (Ms)</label>
					 <input type="number" data-mini="true" name="ptq-remain-kdesk" id="ptq-remain-kdesk" value="" readonly>
				</div>
				<div data-role="fieldcontain">
					 <label for="ptq-total-kdesk" id="ptq-total-kdesk-lbl">Total usable time on K-Computer (Ms)</label>
					 <input type="number" data-mini="true" name="ptq-total-kdesk" id="ptq-total-kdesk" value="" readonly>
				</div>
				<div data-role="fieldcontain">
					 <label for="ptq-remain-kpserver" id="ptq-remain-kpserver-lbl">The remain usable time (Ms)</label>
					 <input type="number" data-mini="true" name="ptq-remain-kpserver" id="ptq-remain-kpserver" value="" readonly>
				</div>
				<div data-role="fieldcontain" >
					 <label for="ptq-total-kpserver" id="ptq-total-kpserver-lbl">Total usable time (Ms)</label>
					 <input type="number" data-mini="true" name="ptq-total-kpserver" id="ptq-total-kpserver" value="" readonly>
				</div>
			<!--/form-->
		<!--/div-->
	</div>
	
	<!-- Popup input default output data -->
	<!--<div data-role="popup" id="provider-outputnew-popup" data-theme="b" class="ui-corner-all">
	    <form>
	        <div style="padding:10px 20px;">
	            <h3>New entry</h3>
	            <label for="un" class="ui-hidden-accessible">Path:</label>
	            <input type="text" name="name" id="un" value="" placeholder="path">
				
				<strong>Attributes:</strong>
		        <input type="checkbox" name="checkbox-h-2b" id="checkbox-h-2b">
		        <label for="checkbox-h-2b">Directory</label>
		        <input type="checkbox" name="checkbox-h-2c" id="checkbox-h-2c">
		        <label for="checkbox-h-2c">Stdout</label>
		        
	            <label for="textarea2"><strong>Description:</strong></label>
	        	<textarea cols="20" rows="40" name="textarea2" id="textarea2" placeholder="description" ></textarea>
	        	
	            <button type="submit" data-icon="check">Done</button>
	        </div>
	    </form>
	</div>-->
	
	<!-- Popup input parameter data -->
	<div data-role="popup" id="provider-newparam-popup" data-theme="b" class="ui-corner-all"">
		<a data-rel="back" data-role="button" data-theme="c" data-icon="delete" data-iconpos="notext" class="ui-btn-right">Close</a>
	   	<div style="padding:0px 15px;">
	        <form class="provider-param-form" data-ajax="false" enctype="multipart/form-data">
	        	<input type="hidden" name="ppp-action" value="newparam" >
	        	<input type="hidden" name="ppp-from" value="newapp" >	
	            <h2 id="ppp-title">New parameter</h2>
	            <label for="ppp-name" class="ui-hidden-accessible">Name:</label>
	            <input type="text" name="ppp-name" id="ppp-name" class="ppp-cmd-element ppp-input" data-preview="ppp-preview" data-prefix="ppp-prefix" value="" placeholder="name" pattern="[^><=\\\!\@\#\$\%\&\?|]*" title="without [><=\!@#$%&?] character" required data-clear-btn="true" maxlength="80">
	            
				<label for="ppp-type" class="ui-hidden-accessible">Type</label>
				<select name="ppp-type" class="ppp-input" id="ppp-type" data-default="ppp-default-value" data-mini="true" required >
					<option value="">Choose type...</option>
					<option value="bool">bool</option>
				    <option value="byte">byte</option>
				    <option value="i16">i16</option>
				    <option value="i32">i32</option>
				    <option value="i64">i64</option>
				    <option value="double">double</option>
				    <option value="string">string</option>
				    <option value="file">file</option>
				</select>
				<label for="pnpp-default-value" class="ui-hidden-accessible">Default value:</label>
	            <input type="text" name="ppp-default-value" class="ppp-input" id="ppp-default-value" value="" placeholder="default value" disabled="disabled" maxlength=50>
				<label for="ppp-descript" class="ui-hidden-accessible"><strong>Description:</strong></label>
	        	<textarea cols="20" rows="40" name="ppp-descript" class="ppp-input" id="ppp-descript" placeholder="description" ></textarea>
	        	
				<fieldset data-role="controlgroup" data-mini="true">
   					<legend><b>Attributes:</b></legend>
					<input type="checkbox" class="ppp-input" name="ppp-required" id="ppp-required">
			        <label for="ppp-required">Required</label>
			        <input type="checkbox" class="ppp-input" name="ppp-output" id="ppp-output">
			        <label for="ppp-output">Output</label>
		        </fieldset>
		        <label for="ppp-prefix"><strong>Option prefix:</strong> (include " ", "=" ...)</label>
		        <input type="text" name="ppp-prefix" id="ppp-prefix" class="ppp-cmd-element ppp-input" data-preview="ppp-preview" data-name="ppp-name" value="" placeholder="ex: --size" pattern="[A-Za-z\-._0-9 ]*" title="Only support [A-Za-z\-._0-9 ] character">
		        <div class="submit-form-border">
		            <label for="ppp-preview" class="ui-hidden-accessible"><strong>Preview:</strong></label>
		        	<textarea cols="20" rows="10" class="ppp-input" name="ppp-preview" id="ppp-preview" placeholder="preview" readonly></textarea>
		        	<button class="ppp-button" type="submit" data-icon="check" data-inline="true" disabled="disabled" data-rel="back">Done</button>
		        	<button class="ppp-button" type="reset" data-icon="delete" data-inline="true" disabled="disabled">Clear</button>
	        	</div>
	        </form>
	    </div>
	</div>
	
	<!-- <div data-role="popup" id="provider-delete-popup" data-overlay-theme="a" data-theme="c" data-dismissible="false" style="max-width:400px;" class="ui-corner-all">
	    <div data-role="header" data-theme="e" class="ui-corner-top">
	        <h1>Delete page?</h1>
	    </div>
	    <div data-role="content" data-theme="d" class="ui-corner-bottom ui-content">
	        <h3 class="ui-title">Are you sure you want to delete this page?</h3>
	        <p>This action cannot be undone.</p>
	        <a href="#" data-role="button" data-inline="true" data-rel="back" data-theme="c">Cancel</a>
	        <a href="#" id="pdp-delete-button" data-role="button" data-inline="true" data-rel="back" data-transition="flow" data-theme="b">Delete</a>
	    </div>
	</div>-->
</div>

<!-- Provider app detail page -------------------------------------------->
<div data-role="page" id="provider-app-detail">

	<div data-role="header" data-theme="c">
	<a data-role="button" data-rel="back" data-icon="arrow-l" data-transition="slide" data-direction="reverse">Back</a>
		<h1 id="padetail-title">Market</h1>
	</div><!-- /header -->
	
	<div data-role="content" >
        <div class="ui-grid-a">
            <div class="ui-block-a" style="width:25%; min-width:220px; max-width=align:center;">
                <!-- <div id="padetail-image"> --> 
                	<img id="padetail-image" src="images/no-image.png" style="max-width: 100%; max-height: 320px" />
                <!-- </div> -->
            </div>
            <div class="ui-block-b" style="width:70%; min-width:500px;">
                <div style="float:left;padding-right:10%;width:95%;align:left;">
                    <h1 id="padetail-name" class="detail-name"></h1>
                    <!-- <span><h3><i>Producer: </i><span id="padetail-producer">Anlab</span></h3></span> -->
                    <span><h3><i>Provider: </i><span id="padetail-provider"></span></h3></span>
                    <hr>
                    <span id="padetail-container-button">
                    	<a id="padetail-backtoedit" data-role="button" data-icon="edit" data-inline="true" >Back to Edit</a>
                    	<a id="padetail-unpublic" href="#provider-app-detail-popup" data-role="button" data-icon="delete" data-inline="true" data-rel="popup" data-position-to="window">Un-Publish</a>
                    	<a id="padetail-public" data-role="button" data-icon="check" data-inline="true">Publish</a>
                    	<a id="padetail-save" data-role="button" data-icon="check" data-inline="true">Save</a>
                    	<a id="padetail-delete" href="#provider-app-detail-delete-popup" data-role="button" data-icon="delete" data-inline="true" data-rel="popup" data-position-to="window">Delete</a>
					</span>
					<div id="padetail-msg" style="color:red;text-indent: 30px;font:300 13px/1.4 'Roboto',Arial,sans-serif"></div>
                </div> 
            </div>
	        <div class="ui-block-a" style="width:100%">
	        <hr/>
				<h2>Description</h2>
				<p id="padetail-description" style="text-indent: 30px;font:300 13px/1.4 'Roboto',Arial,sans-serif" > 
				Property checking is used for verification instead of equivalence checking when two descriptions are not functionally equivalent. Particularly, during refinement, the specification is complemented with the details that are unnecessary in the higher level specification. Yet, there is no need to verify the newly introduced properties against the original specification. It is not even possible. Therefore, the strict bi-directional equivalence check is relaxed to one-way property checking. The implementation or design is regarded a model of the circuit whereas the specifications are properties that the model must satisfy.[1]
	An important class of model checking methods have been developed for checking models of hardware and software designs where the specification is given by a temporal logic formula. Pioneering work in the model checking of temporal logic formulae was done by E. M. Clarke and E. A. Emerson[2][3][4] and by J. P. Queille and J. Sifakis.[5] Clarke, Emerson, and Sifakis shared the 2007 Turing Award for their work on model checking.[6][7]
	Model checking is most often applied to hardware designs. For software, because of undecidability (see computability theory) the approach cannot be fully algorithmic; typically it may fail to prove or disprove a given property.
	The structure is usually given as a source code description in an industrial hardware description language or a special-purpose language. Such a program corresponds to a finite state machine (FSM), i.e., a directed graph consisting of nodes (or vertices) and edges. A set of atomic propositions is associated with each node, typically stating which memory elements are one. The nodes represent states of a system, the edges represent possible transitions which may alter the state, while the atomic propositions represent the basic properties that hold at a point of execution.
	Formally, the problem can be stated as follows: given a desired property, expressed as a temporal logic formula p, and a structure M with initial state s, decide if . If M is finite, as it is in hardware, model checking reduces to a graph search.
				</p>
	        </div>
        </div>
    </div>
    <div data-role="popup" id="provider-app-detail-popup" data-overlay-theme="a" data-theme="c" data-dismissible="false" style="max-width:400px;" class="ui-corner-all">
	    <div data-role="header" data-theme="a" class="ui-corner-top">
	        <h1>Un-Publish App?</h1>
	    </div>
	    <div data-role="content" data-theme="d" class="ui-corner-bottom ui-content">
	        <h3 class="ui-title">This will hide your service from the public site. Are you sure ?</h3>
	        <p>This action cannot be undone.</p>
	        <a href="#" data-role="button" data-inline="true" data-rel="back" data-theme="c">Cancel</a>
	        <a id="padp-ok" href="#" data-role="button" data-inline="true" data-rel="back" data-transition="flow" data-theme="b">Un-Publish</a>
	    </div>
	</div>
	<div data-role="popup" id="provider-app-detail-delete-popup" data-overlay-theme="a" data-theme="c" data-dismissible="false" style="max-width:400px;" class="ui-corner-all">
	    <div data-role="header" data-theme="a" class="ui-corner-top">
	        <h1>Delete App?</h1>
	    </div>
	    <div data-role="content" data-theme="d" class="ui-corner-bottom ui-content">
	        <h3 class="ui-title">This will delete your service from the server and its jobs. Are you sure ?</h3>
	        <p>This action cannot be undone.</p>
	        <a href="#" data-role="button" data-inline="true" data-rel="back" data-theme="c">Cancel</a>
	        <a id="paddp-delete-btn" href="#" data-role="button" data-inline="true" data-rel="back" data-transition="flow" data-theme="b">Delete</a>
	    </div>
	</div>
</div>

<!-- Provider app page -------------------------------------------->
<div data-role=page id="provider-app">
	<!-- <div data-role="header" data-theme="c" style=""> -->
	<div data-role="header" data-theme="c" data-position="fixed" data-id="hdr" data-tap-toggle="false">
		<a id="papp-back-btn" data-role="button" data-icon="arrow-l" data-transition="slide" data-direction="reverse">Back</a>
		<a href="#" id="login-btn" class="ui-btn-right login-btn" data-role="button" style=" width:20%;min-width:130px;" data-inline="true" data-icon="check" data-transition="pop"> 
			<label id="login-txt" class="login-txt" style=" width:40%;min-width:350px;"></label>
		</a>
		<h2 id="papp-title">Service name</h2>
		
		<!--div style="text-align: right;font-size: 15px; font-weight=bold"">
			<label>The remain usable time:<span id="pt-remain" style="text-align: right;font-size: 18px;font-weight=bold"> 0000(Ms)</span>&nbsp;&nbsp;</label>
			</br>
			<label>Total usable time:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span id="pt-total" style="text-align: right;font-size: 18px;font-weight=bold">0000(Ms)</span>&nbsp;&nbsp;</label>
		</div-->
		<div data-role="navbar">
		    <ul>
				<li><a id="papp-tab-btn-users" href="#" data-role="tab" class="papp-tab-btn" data-tab="users" data-transition="slide">Users</a></li>
	            <li><a id="papp-tab-btn-waitlist" href="#" data-role="tab" class="papp-tab-btn" data-tab="waitlist" data-transition="slide" >Wait List</a></li>
	            <li><a id="papp-tab-btn-details" href="#" data-role="tab" class="papp-tab-btn" data-tab="details" data-transition="slide" class="ui-btn-active ui-state-persist">Details</a></li>
	        </ul>
		</div><!-- /navbar -->
	</div> <!-- header -->
	
	<!-- Tab list user -->
	<div data-role="content" id="papp-tab-users">
		<form id="papp-adduser-form" data-ajax="false" enctype="multipart/form-data">
		<table style='width:100%'>
			<tr>
			    <td>
					<label for="papp-adduser-email" class="ui-hidden-accessible"><strong>User email</strong></label>
		            <input type="email" name="papp-adduser-email" id="papp-adduser-email" value="" placeholder="ex: abc@gmail.com" data-clear-btn="true">
			    </td>
			    <td style='font-size:80%; width:7em'>
			         <input id="papp-adduser-btn" type='submit' value='Add' />
			    </td>
			</tr>
			<tr>
				<td>
					<p id="papp-adduser-msg" style='font-size:100%; color:red;'></p>
				</td>
			</tr>
		</table>
		</form>
		<br/>
		<ul id="papp-users-list" data-role="listview" data-filter="true" data-filter-placeholder="Search user..."  data-inset="true" data-split-theme="e" data-split-icon="delete" data-split-theme="c" data-inset="true">
			<!-- <li><a href="#">user002@gmail.com</a><a class="papp-deleteuser-btn" data-user="email" data-rel="popup" data-position-to="window" data-transition="pop">Delete</a></li>
			<li><a href="#">user003@gmail.com</a><a class="papp-deleteuser-btn" href="#purchase" data-rel="popup" data-position-to="window" data-transition="pop">Delete</a></li>		    
			<li><a href="#">tanaka@gmail.com</a><a class="papp-deleteuser-btn" href="#purchase" data-rel="popup" data-position-to="window" data-transition="pop">Delete</a></li>
			<li><a href="#">tanaka@gmail.com</a><a class="papp-deleteuser-btn" href="#purchase" data-rel="popup" data-position-to="window" data-transition="pop">Delete</a></li>
		    <li><a href="#">nakamura@gmail.com</a><a class="papp-deleteuser-btn" href="#purchase" data-rel="popup" data-position-to="window" data-transition="pop">Delete</a></li> -->
		</ul>
	</div><!-- /content -->
	
	<!-- Tab wait list tab -->
	<div data-role="content" id="papp-tab-waitlist">
		<ul id="papp-wait-list" data-role="listview" data-filter="true" data-filter-placeholder="Search user..."  data-theme="c" style="margin-bottom: 50px;" data-inset="true">
			<!--<li>
				<a href="#">user002@gmail.com</a>
				<a class="papp-deleteuser-btn split-button-custom" data-role="button" data-icon="delete" data-theme= "e" data-iconpos="notext">Delete</a>
				<a class="papp-acceptuser-btn split-button-custom" data-role="button" data-icon="check" data-iconpos="notext">Accept</a>
				<a href="#" style="display: none;"></a>
			</li><li>
				<a href="#">user003@gmail.com</a>
				<a onclick="alert('click');" class="split-button-custom" data-role="button" data-icon="delete" data-theme= "e" data-iconpos="notext">1st link</a>
				<a onclick="alert('click');" class="split-button-custom" data-role="button" data-icon="check" data-iconpos="notext">2nd link</a>
				<a href="#" style="display: none;">Dummy</a>
			</li><li>
				<a href="#">user004@gmail.com</a>
				<a onclick="" class="split-button-custom" data-role="button" data-icon="delete" data-theme= "e" data-iconpos="notext">1st link</a>
				<a onclick="" class="split-button-custom" data-role="button" data-icon="check" data-iconpos="notext">2nd link</a>
				<a href="#" style="display: none;">Dummy</a>
			</li><li>
				<a href="#">user005@gmail.com</a>
				<a onclick="" class="split-button-custom" data-role="button" data-icon="delete" data-theme= "e" data-iconpos="notext">1st link</a>
				<a onclick="" class="split-button-custom" data-role="button" data-icon="check" data-iconpos="notext">2nd link</a>
				<a href="#" style="display: none;">Dummy</a>
			</li>-->
		</ul>
	</div>	<!-- content -->
	<!-- <div data-role="popup" id="papp-deleteuser-popup" data-theme="d" style="height:20;padding:0;">
    	<div data-role="controlgroup" data-type="horizontal">
            <a href="#" data-role="button" data-icon="check">Accept</a>
            <a href="#" data-role="button" data-icon="delete">Reject</a>
		</div>
	</div> -->
	
	<!-- Tab properties tab -->
	<div data-role="content" id="papp-tab-details">
		<form id="provider-editapp-form" data-ajax="false" enctype="multipart/form-data">
		<div data-role="collapsible-set" data-theme="e" data-content-theme="c">
	    	<div class="provider-collapsible" data-role="collapsible" data-collapsed="false">
    			<h2>Properties</h2>
    			<ul data-role="listview" data-theme="d" data-divider-theme="d">
		    		<li>
		    		<div data-role="fieldcontain">
						<label for="provider-editapp-id">Id</label>
						<input type="text" name="provider-newapp-id" id="provider-editapp-id" value="" readonly>
					</div>
					<div data-role="fieldcontain">
						<label for="provider-editapp-name">Name</label>
						<input type="text" name="provider-newapp-name" id="provider-editapp-name" class="provider-editapp-input" value=""  pattern="[^><=\\\!\@\#\$\%\&\?|]*" title="without [><=\!@#$%&?] character" data-clear-btn="true" required maxlength="300">
					</div>
					<div data-role="fieldcontain">
						<label for="provider-editapp-path"><h2>Application Path</h2></label>
					</div>
					<div data-role="fieldcontain">
						<label for="provider-editapp-image">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Docker Image</label>
						<select name="provider-newapp-image" id="provider-editapp-image" class="provider-editapp-input" required>
							<!--option value="1">RunMethod01</option>
					        <option value="2">RunMethod02</option>
					        <option value="3">RunMethod03</option>
					        <option value="4">... New method ...</option-->
						</select>
					</div>
					<div data-role="fieldcontain">
						<label for="provider-editapp-path">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Path</label>
						<input type="text" data-clear-btn="false" name="provider-newapp-path" id="provider-editapp-path" class="provider-editapp-input" value="" pattern="[^><$]*" title="without [><$] character" data-clear-btn="true" required>
					</div>
					<div data-role="fieldcontain">
                                                <label for="provider-editapp-path"><h2>Script File Path</h2></label>
                                        </div>
					<div data-role="fieldcontain">
                                                <label for="provider-editapp-shpath">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Job script template</label>
                                                <input type="text" data-clear-btn="false" name="provider-newapp-shpath" id="provider-editapp-shpath" class="provider-editapp-input" value="" pattern="[^><$]*" title="without [><$] character" data-clear-btn="true" placeholder="/path/to/sh/file">
                                        </div>
                                        <div data-role="fieldcontain">
                                                <label for="provider-editapp-preshpath">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pre-processing command</label>
                                                <input type="text" data-clear-btn="false" name="provider-newapp-preshpath" id="provider-editapp-preshpath" class="provider-editapp-input"  value="" title="without [^><$] character" data-clear-btn="true" placeholder="/path/to/sh/file">
                                        </div>
                                        <div data-role="fieldcontain">
                                                <label for="provider-editapp-postshpath">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Post-processing command</label>
                                                <input type="text" data-clear-btn="false" name="provider-newapp-postshpath" id="provider-editapp-postshpath" class="provider-editapp-input" value="" title="without [^><$] character" data-clear-btn="true" placeholder="/path/to/sh/file">
                                        </div>
					<div data-role="fieldcontain">
						<label for="provider-editapp-icon">Icon</label>
						<input type="file" data-clear-btn="true" name="provider-newapp-icon" id="provider-editapp-icon" class="provider-editapp-input" value="" accept="image/*">
					</div>
					<div data-role="fieldcontain">
						<label>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</label>
						<center><img id="provider-editapp-icon-view" src="images/geant4_program.png" height="100px" width="100px" /></center>
					</div>
					<div data-role="fieldcontain">
						<label for="provider-editapp-stgin">Folder stage in</label>
						<input type="text" name="provider-newapp-stgin" id="provider-editapp-stgin" placeholder="path name" data-clear-btn="true" maxlength="300" class="provider-editapp-input"  pattern="[^><=\\\!\@\#\$\%\&\?| ]*" title="without [><=\!@#$%&? ] character" >
					</div>
					<div data-role="fieldcontain">
						<label for="provider-editapp-stgout">Folder stage out</label>
						<input type="text" name="provider-newapp-stgout" id="provider-editapp-stgout" placeholder="path name" data-clear-btn="true" maxlength="300" class="provider-editapp-input" pattern="[^><=\\\!\@\#\$\%\&\?| ]*" title="without [><=\!@#$%&?] character" >
					</div>
					
					<div data-role="fieldcontain">	
						<label for="provider-editapp-elapsed">Max elapsed time (s)</label>
						<input type="number" data-clear-btn="true" name="provider-newapp-elapsed" id="provider-editapp-elapsed"  class="provider-editapp-input" value="" pattern="[0-9]{1,10}" required>	
					</div>
					<div data-role="fieldcontain">	
						<label for="provider-editapp-nodes">Number of nodes</label>
						<input type="number" data-clear-btn="true" name="provider-newapp-nodes" id="provider-editapp-nodes"  class="provider-editapp-input" value="" pattern="[0-9]{1,10}" required >
					</div>
					<div data-role="fieldcontain">	
						<label for="provider-editapp-descript">Description</label>
						<textarea cols="40" rows="8" name="provider-newapp-descript" id="provider-editapp-descript" class="provider-editapp-input" ></textarea>
					</div>
					</li>
				</ul>
	    		<!--ul id="job-detail-list-info" data-role="listview" data-theme="d" data-divider-theme="d"  data-collapsed="true" >
		    		<li> 
		    		<div data-role="fieldcontain">
						<label for="provider-editapp-id">Id</label>
						<input type="text" name="provider-newapp-id" id="provider-editapp-id" value="" readonly>
					</div>
					<div data-role="fieldcontain">
						<label for="provider-editapp-name">Name</label>
						<input type="text" name="provider-newapp-name" id="provider-editapp-name" class="provider-editapp-input" value=""  pattern="[^><=\\\!\@\#\$\%\&\?|]*" title="without [><=\!@#$%&?] character" data-clear-btn="true" required maxlength="300">
					</div>
					<div data-role="fieldcontain">
						<label for="provider-editapp-path">Path</label>
						<input type="text" data-clear-btn="false" name="provider-newapp-path" id="provider-editapp-path" class="provider-editapp-input" value="" pattern="[^><$]*" title="without [><$] character" data-clear-btn="true" required>
					</div>
					<div data-role="fieldcontain">
						<label for="provider-editapp-icon">Icon</label>
						<input type="file" data-clear-btn="true" name="provider-newapp-icon" id="provider-editapp-icon" class="provider-editapp-input" value="" accept="image/*">
					</div>
					<div data-role="fieldcontain">
						<label>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</label>
						<center><img id="provider-editapp-icon-view" src="images/geant4_program.png" height="100px" width="100px" /></center>
					</div>
					<!--div data-role="fieldcontain">
						<label for="provider-editapp-elapsed">Max elapsed time (ms)</label>
						<input type="number" data-clear-btn="true" name="provider-newapp-elapsed" id="provider-editapp-elapsed" class="provider-editapp-input" value="" pattern="[0-9]*" required>	
					</div>
					<div data-role="fieldcontain">	
						<label for="provider-editapp-nodes">Number of nodes</label>
						<input type="number" name="provider-newapp-nodes" id="provider-editapp-nodes" class="provider-editapp-input" value="" pattern="[0-9]*" required >
					</div-- >
					<div data-role="fieldcontain">	
						<label for="provider-editapp-descript">Description</label>
						<textarea cols="40" rows="8" name="provider-newapp-descript" id="provider-editapp-descript" class="provider-editapp-input" ></textarea>
					</div>
					</li>
				</ul-->
			</div>
			
		    <div class="provider-collapsible" data-role="collapsible" data-collapsed="true">
				<h2>Run Method</h2>
				
				<!-- <ul id="job-detail-list-browse"  data-role="listview" data-split-icon="custom" data-split-theme="d" data-inset="true"> -->
				
				<ul data-role="listview" data-theme="d" data-divider-theme="d" >
					<li data-role="list-divider">Parameters</li>
					<li>
					<!-- <table data-role="table" id="table-column-toggle" data-mode="columntoggle" class="ui-responsive table-stroke"> -->
						<table data-role="table" id="provider-editapp-params-table" data-mode="reflow" class="ui-body-d ui-shadow table-stripe ui-responsive" style="width:100%" >
							<thead>
								<tr class="ui-bar-d">
									<th><abbr title="* is required, out is output">Name</abbr></th></th>
									<!-- <th data-priority="2">Description</th> -->
									<th>Type</th>
									<!-- <th data-priority="2">is Requied/Output</th> -->
									<th align="center"></th>
									<th align="center"></th>
								</tr>
							</thead>
							<tbody id="pep-table-body">
							  <tr>
							    <td><span>データファイル&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
							    <td>file</td>
							    <!-- <td><a class="provider-param-edit-btn" href="#provider-param-popup" data-paramid="paramid01" data-role="button" data-rel="popup" data-icon="edit" data-iconpos="notext" data-theme="c" data-inline="true" data-rel="popup" data-position-to="window" data-inline="true" data-theme="a" data-transition="pop"></a></td> -->
							    <td><a class="provider-param-edit-btn" data-paramid="paramid01" data-role="button" data-rel="popup" data-icon="edit" data-iconpos="notext" data-theme="c" data-inline="true" data-rel="popup" data-position-to="window" data-inline="true" data-theme="a" data-transition="pop"></a></td>
							    <td><a class="provider-newapp-param-delete-btn" data-paramid="paramid01" data-rel="popup" data-role="button" data-inline="true" data-icon="delete" data-iconpos="notext" data-theme="e" data-transition="pop"  data-position-to="window">Delete</a></td>
							  </tr>
							  <tr>
							    <td>出力ファイル</td>
							    <td>string</td>
							    <td><a class="provider-param-edit-btn" data-paramid="paramid01" data-role="button" data-rel="popup" data-icon="edit" data-iconpos="notext" data-theme="c" data-inline="true"></a></td>
							    <td><a class="provider-newapp-param-delete-btn" data-paramid="paramid01" data-rel="popup" data-role="button" data-inline="true" data-icon="delete" data-iconpos="notext" data-theme="e" data-transition="pop"  data-position-to="window">Delete</a></td>
							  </tr>
							  <tr>
							    <td>ブロックサイズ</td>
							    <td>i32</td>
							    <td><a class="provider-param-edit-btn" data-paramid="paramid01" data-role="button" data-rel="popup" data-icon="edit" data-iconpos="notext" data-theme="c" data-inline="true"></a></td>
							    <td><a class="provider-newapp-param-delete-btn" data-paramid="paramid01" data-rel="popup" data-role="button" data-inline="true" data-icon="delete" data-iconpos="notext" data-theme="e" data-transition="pop"  data-position-to="window">Delete</a></td>
							  </tr>
							  <!-- 
							  <tr>
							    <td>name4</td>
							    <td>i64</td>
							    <td><a href="#" data-role="button" data-icon="edit" data-iconpos="notext" data-theme="c" data-inline="true"></a></td>
							    <td><a href="#" data-role="button" data-icon="delete" data-iconpos="notext" data-theme="c" data-inline="true"></a></td>
							  </tr> -->
							  <tr>
							    <td><a class="provider-newapp-param-add-btn" href="#provider-newparam-popup" data-role="button" data-rel="popup" data-icon="add" data-inline="true" data-mini="true" data-position-to="window" >Add</a></td>
							    <td></td>
							    <td></td>
							    <td></td>
							  </tr>
							  
							</tbody>
						</table>
					</li>
					<li data-role="list-divider">Preview</li>
					<li>
						<label for="provider-newapp-preview" class="ui-hidden-accessible">Description</label>
						<textarea readonly cols="40" rows="8" name="provider-newapp-preview" id="provider-editapp-preview"></textarea>
					</li>
				</ul>
			</div>
		</div>
		<div>
			<div id="papp-submit-msg" style="color:red;text-indent: 30px;font:300 13px/1.4 'Roboto',Arial,sans-serif"></div>
			<p>
			    <button type="submit" id="provider-editapp-submit-btn" data-inline="true" data-icon="check" disabled >Next</button>
			    <a data-role="button" id="provider-editapp-undo-btn" class="ui-disabled" data-inline="true">Undo</a>
			</p>
		</div>
		</form>
	</div><!-- /content -->
	
	<!-- Popup input parameter data -->
	<div data-role="popup" id="provider-newparam-popup" data-theme="b" class="ui-corner-all"">
		<a href="#" data-rel="back" data-role="button" data-theme="c" data-icon="delete" data-iconpos="notext" class="ui-btn-right">Close</a>
	   	<div style="padding:0px 15px;">
	        <form class="provider-param-form" data-ajax="false" enctype="multipart/form-data">
	        	<input type="hidden" name="ppp-action" value="newparam" >
	        	<input type="hidden" name="ppp-from" value="editapp" >	
	            <h2 id="ppp-title">New parameter</h2>
	            <label for="ppp-name" class="ui-hidden-accessible">Name:</label>
	            <input type="text" name="ppp-name" id="ppp-name" class="ppp-cmd-element ppp-input" data-preview="ppp-preview" data-prefix="ppp-prefix" value="" placeholder="name" pattern="[^><=\\\!\@\#\$\%\&\?|]*" title="without [><=\!@#$%&?] character" required data-clear-btn="true" required maxlength="80">
	            
				<label for="ppp-type" class="ui-hidden-accessible">Type</label>
				<select name="ppp-type" class="ppp-input" id="ppp-type" data-default="ppp-default-value" data-mini="true" required>
					<option value="">Choose type...</option>
					<option value="bool">bool</option>
				    <option value="byte">byte</option>
				    <option value="i16">i16</option>
				    <option value="i32">i32</option>
				    <option value="i64">i64</option>
				    <option value="double">double</option>
				    <option value="string">string</option>
				    <option value="file">file</option>
				</select>
				<label for="ppp-default-value" class="ui-hidden-accessible">Default value:</label>
	            <input type="text" class="ppp-input" name="ppp-default-value" id="ppp-default-value" value="" placeholder="default value" disabled>
				<label for="ppp-descript" class="ui-hidden-accessible"><strong>Description:</strong></label>
	        	<textarea cols="20" rows="40" class="ppp-input" name="ppp-descript" id="ppp-descript" placeholder="description" ></textarea>
	        	
				<fieldset data-role="controlgroup" data-mini="true">
   					<legend><b>Attributes:</b></legend>
					<input type="checkbox" class="ppp-input" name="ppp-required" id="ppp-required">
			        <label for="ppp-required">Required</label>
			        <input type="checkbox" class="ppp-input" name="ppp-output" id="ppp-output">
			        <label for="ppp-output">Output</label>
		        </fieldset>
		        <label for="ppp-prefix"><strong>Option prefix:</strong> (include " ", "=" ...)</label>
		        <input type="text" name="ppp-prefix" id="ppp-prefix" class="ppp-cmd-element ppp-input" data-preview="ppp-preview" data-name="ppp-name" value="" placeholder="ex: --size" pattern="[A-Za-z\-._0-9 ]*" title="Only support [A-Za-z\-._0-9 ] character" maxlength="50">
		        <div class="submit-form-border">
		            <label for="ppp-preview" class="ui-hidden-accessible"><strong>Preview:</strong></label>
		        	<textarea cols="20" rows="10" name="ppp-preview" id="ppp-preview" placeholder="preview" readonly></textarea>
		        	<button class="ppp-button" type="submit" data-icon="check" data-inline="true" disabled>Done</button>
		        	<button class="ppp-button" type="reset" data-icon="delete" data-inline="true" disabled>Clear</button>
	        	</div>
	        </form>
	    </div>
	</div>
</div>

<!-- Images Management home page ------------------------------------------>
<div data-role=page id="provider-image">
	<div data-role="header" data-theme="c" data-position="fixed">
		<a href="#provider" class="ui-btn-icon-left" data-role="button" data-inline="true" data-icon="arrow-l">Back</a>
		<a href="#" id="login-btn" class="ui-btn-right login-btn" data-role="button" style=" width:20%;min-width:130px;" data-inline="true" data-icon="check" data-transition="pop"> 
			<label id="login-txt" class="login-txt" style=" width:40%;min-width:350px;"></label>
		</a>
		<h1>Images Management</h1>
		<div data-role="navbar" id="provider-image-tabs">
			<ul>
				<li><a id="provider-image-tab-btn-newimages" href="#" data-role="tab" class="provider-image-tab-btn ui-btn-active ui-state-persist" data-tab="newimages" data-transition="slide">Create New Images</a></li>
				<li><a id="provider-image-tab-btn-uploadedimages" href="#" data-role="tab" class="provider-image-tab-btn" data-tab="uploadedimages" data-transition="slide" >Manage Uploaded Images</a></li>
				<li><a id="provider-image-tab-btn-downloadsearch" href="#" data-role="tab" class="provider-image-tab-btn" data-tab="downloadsearch" data-transition="slide" >Download & Search Public Base Images</a></li>
			</ul>
		</div>
	</div><!-- /header -->
	
	<!-- New images tab-->
	<div id="provider-image-tab-newimages" data-role="content">
		<form id="provider-image-newimages-form" data-ajax="false" enctype="multipart/form-data">
		<div data-theme="e" data-content-theme="c">
			<ul id="images-detail-list-info" data-role="listview" data-theme="d" data-divider-theme="d">
				<li> 
				<div data-role="fieldcontain">
					<label for="pinf-email">Email address</label>
					<input type="text" name="pinf-email" id="pinf-email" class="ui-disabled" value="" data-clear-btn="false">
				</div>
				<!--div data-role="fieldcontain">
					<label for="pinf-username">System User Name</label>
					<input type="text" name="pinf-username" id="pinf-username" class="ui-disabled" value="" data-clear-btn="false" required>
				</div-->
				<!--div data-role="fieldcontain">					
					<label for="pinf-notes">Notes</label>
					<textarea cols="40" rows="20" data-clear-btn="true" name="pinf-notes" id="pinf-notes" value="" maxlength="7997" class="ui-disabled"></textarea>
				</div-->
				<div>
					<div id="docker-images">
						<div data-role="fieldcontain">
							<label for="pinf-name">Docker Image Name<a href="#popupInfo" data-rel="popup" data-role="button" class="ui-icon-alt" data-inline="true" data-transition="pop" data-icon="info" data-theme="e" data-iconpos="notext">Learn more</a></label>
							<input type="text" data-clear-btn="true" name="pinf-name" id="pinf-name" class="pinf-input" value="" pattern="[a-z][a-z0-9._+-]+" title="Name only allow use [a-z0-9.-_] characters.\n And must begin with a letter[a-z]." required maxlength="264">
							<div data-role="popup" id="popupInfo" class="ui-content" data-theme="e" style="max-width:350px;">
							  <p>The name use for repository inside docker. <br>So only allow use [a-z0-9.-_]* character.<br/>Must begin with a letter[a-z].<br/><strong>Only lower case.</strong></p>
							</div>
						</div>
						<div data-role="fieldcontain">
							<label for="pinf-tarfile" >Upload Image</label>
							<input type="file" data-clear-btn="true" name="pinf-tarfile" id="pinf-tarfile" class="pinf-input" value="" required>
						</div>
						<div data-role="fieldcontain">
							<label for="pinf-public" >Public</label>
							<select name="pinf-public" id="pinf-public" class="pinf-input" data-role="slider">
								<option value="0">Off</option>
								<option value="1">On</option>
							</select>
						</div>
						<div data-role='fieldcontain'>
							<label for='pinf-description'>Description</label>
							<textarea cols='40' rows='20' name='pinf-description' id='pinf-description' class="pinf-input" value=''  maxlength='7997'></textarea>
						</div>
						<div data-role='fieldcontain' id="pinf-progress-bar" class="ui-hide-label">
						</div>
						<div data-role='fieldcontain'>
								<label id="pinf-msg" style="float:center;"></label>
						</div>
					</div>
					<div style="margin-left:17%;display:inline-block;">
						<!--div>
							<a href="#" onclick="appendImagesToList()">Upload another Docker image</a>
						</div-->
						<div style="float:left;">
							<button id="pinf-submit-btn" class="pinf-input" type="submit">Upload</button>
							<!--button id="pinf-submit-btn" class="show-page-loading-msg" data-theme="a" data-textonly="false" data-textvisible="true" data-msgtext="Loading theme a">Upload</button-->
						</div>
					</div>
				</div>
				</li>
			</ul>
		</div>
		</form>
	</div>
	
	<!-- Uploaded images tab-->
	<div id="provider-image-tab-uploadedimages" data-role="content">
		<ul>
			<div>
				<label class="ui-block-a">Default Base image&nbsp;&nbsp;&nbsp;&nbsp;</label>
				<a class="ui-block-b"></a>
				<a class="ui-block-c" id="pitu-url-baseimage-url" href="#" data-ajax="false"></a>
			</div>
			<br/>
			<div>
				<label class="ui-block-a">Default Dockerfile&nbsp;&nbsp;&nbsp;&nbsp;</label>
				<a class="ui-block-b"></a>
				<a class="ui-block-c" id="pitu-url-dockerfile-url" href="#" data-ajax="false"></a>
			</div>
			
		</ul>
		<br/>
		<br/>
		<!-- Table of Docker Images -->
		<table data-role="table" 
			   id="piu-table" 
			   data-mode="Reflow" 
			   class="ui-body-d ui-shadow table-stripe ui-responsive">
			   <tbody id="piu-table-body"></tbody>
		</table>
			   
		<!-- Edit Docker Image Popup -->	   
		<div data-role="popup" id="edit-image-popup">
			<div data-role="header" data-theme="d">
				<h1>Edit Docker Image</h1>
			</div>
			<div data-role="main" class="ui-content" data-theme="d">
				<label for="pief-name">Docker Image Name</label>
				<input type="text" name="pief-name" id="pief-name" value="" readonly class="ui-disabled">
		
				<label for="pief-url">Download Url</label>
				<input type="text" name="pief-url" id="pief-url" value="" readonly class="ui-disabled">
				<div data-role="fieldcontain">
				<label for="pief-public">Public</label>
				<select name="pief-public" id="pief-public" data-role="slider">
					<option value="0">Off</option>
					<option value="1">On</option>
				</select>
				</div>
				<label for='pief-description'>Description</label>
				<textarea cols='40' rows='20' name='pief-description' id='pief-description' value=''  maxlength='7997'></textarea>
				<!--p>
					<label id="edit-image-name-lbl"> Docker image Name</label><br><br>
					<a href="#" id="edit-link-a">Link</a><br><br>
					<label><input type="checkbox" name="edit-public-cbx" id="edit-public-cbx">Public</label><br>
					<label style="font-weight:bold">Description</label><br>
					<textarea name="description-txt" id="description-txt" value=""></textarea>
				</p -->
			</div>
			<div data-role="footer" data-theme="d">
				<div class="ui-grid-d">
					<a class="ui-block-a"></a>
					
					<div class="ui-block-b">
						<a href="#" id="delete-image-btn" data-role="button" onclick="images_updateImageBtnClick();" class="ui-corner-all ui-btn-inline ui-shadow" data-icon="edit" data-theme="b">Save</a>
					</div>
					<a class="ui-block-b"></a>
					<div class="ui-block-d">
						<a href="#" id="cancel-delete-image-btn" data-role="button" class="ui-corner-all ui-shadow ui-btn-inline" data-rel="back" data-icon="delete">Cancel</a>
					</div>
					<a class="ui-block-e"></a>
				</div>
			</div>
		</div>
		
		<!-- Delete Docker Image Popup -->
		<div data-role="popup" id="delete-image-popup" style="width:120%;">
			<div data-role="header" data-theme="d">
				<h1>Confirm Deleting Docker Image</h1>
			</div>
			<div data-role="main" class="ui-content" data-theme="d">
				<p>
					<h2>Do you really want to delete this Docker image?</h2>
				</p>
			</div>
			<div data-role="footer" data-theme="d">
				<div class="ui-grid-c">
					<a class="ui-block-a"></a>
					<a class="ui-block-b"></a>
					<div class="ui-block-c">
						<a href="#" id="delete-image-btn" data-role="button" onclick="images_deleteImageBtnClick();" class="ui-corner-all ui-btn-inline ui-shadow" data-icon="delete" data-theme="b">Delete</a>
					</div>
					<div class="ui-block-d">
						<a href="#" id="cancel-delete-image-btn" data-role="button" class="ui-corner-all ui-shadow ui-btn-inline" data-rel="back" data-icon="back" >Cancel</a>
					</div>
				</div>
			</div>
		</div>
	</div>
	
	<!-- Download & Search images tab-->
	<div id="provider-image-tab-downloadsearch" data-role="content">
		<form id="pid-form" data-ajax="false" enctype="multipart/form-data">
			<ul data-role="listview" data-inset="true">
				<li data-role="fieldcontain">
					<label for="pidf-keyword">Search public image:</label>
					<input type="search" name="pidf-keyword" id="pidf-keyword" placeholder="Search by name">
				</li>
			</ul>
		</form>
		<!-- Table of Docker Images -->
		<table data-role="table" 
			   id="pid-table" 
			   data-mode="Reflow" 
			   class="ui-body-d ui-shadow table-stripe ui-responsive">
			   <tbody id="pid-table-body"></tbody>
		</table>
	</div>
</div>

<!-- Admin page ------------------------------------------>
<div data-role="page" id="admin">
	<div data-role="header" data-theme="c" data-position="fixed" data-id="hdr" data-tap-toggle="false">
		<!--a href="#provider" class="ui-btn-icon-left" data-role="button" data-inline="true" data-icon="arrow-l">Back</a-->
		<h1>Admin</h1>
		<a href="#" id="login-btn" class="ui-btn-right login-btn" data-role="button" style=" width:20%;min-width:130px;" data-inline="true" data-icon="check" data-transition="pop"> 
			<label id="login-txt" class="login-txt" style=" width:40%;min-width:350px;">Login</label>
		</a>
		<div data-role="navbar">
			<ul>
				<li><a href="#" class="admin-tab-btn ui-btn-active ui-state-persist" id="admin-tab-btn-wait" data-tab="wait">Wait list</a></li>
				<li><a href="#" class="admin-tab-btn" id="admin-tab-btn-provider" data-tab="provider">Provider</a></li>
			</ul>
		</div><!-- /navbar -->
	</div><!-- /header -->
	<div id="admin-tab-wait" data-role="content" class="ui-body ui-body-c">
		<ul id="admin-card-list-wait" data-role="listview" data-filter="true" data-filter-placeholder="Search user..."  data-theme="c" style="margin-bottom: 50px;" data-inset="true">
		<!--div class="admin-card-list" id="admin-card-list-wait">
		</div-->
		</ul>
	</div><!-- /content -->
	<div id="admin-tab-provider" data-role="content" class="ui-body ui-body-c">
		<ul id="admin-card-list-provider" data-role="listview" data-filter="true" data-filter-placeholder="Search user..."  data-theme="c" style="margin-bottom: 50px;" data-inset="true">
		<!--div class="admin-card-list" id="admin-card-list-provider">
		</div--> 
		</ul>
	</div><!-- /content -->
	<!-- Delete provider -->
	<div data-role="popup" id="admin-delete-popup" style="width:120%;">
		<div data-role="header" data-theme="d">
			<h1>Confirm Deleting Provider Image</h1>
		</div>
		<div data-role="main" class="ui-content" data-theme="d">
			<p>
				<h2>Do you really want to delete this provider image?</h2>
			</p>
		</div>
		<div data-role="footer" data-theme="d">
			<div class="ui-grid-c">
				<a class="ui-block-a"></a>
				<a class="ui-block-b"></a>
				<div class="ui-block-c">
					<a href="#" id="delete-image-btn" data-role="button" onclick="admin_deleteImageBtnClick();" class="ui-corner-all ui-btn-inline ui-shadow">Delete</a>
				</div>
				<div class="ui-block-d">
					<a href="#" id="cancel-delete-image-btn" data-role="button" class="ui-corner-all ui-shadow ui-btn-inline" data-rel="back">Cancel</a>
				</div>
			</div>
		</div>
	</div>
			   
	<!-- Register Detail Popup -->	   
	<div data-role="popup" id="admin-register-popup">	
		<a href="#" data-rel="back" data-role="button" data-theme="c" data-icon="delete" data-iconpos="notext" class="ui-btn-right">Close</a>
		<div data-role="header" data-theme="d">
			<h1>Provider Register Detail</h1>
		</div>
		<div data-role="main" class="ui-content" data-theme="d">
			<label for="arp-email" style="margin-left: 10px;margin-right: auto !important;"><b>Email Address </b></label>
			<input type="text" data-mini="true" data-theme="e" name="arp-email" id="arp-email" value="" readonly/>
			<label for="arp-address" style="margin-left: 10px;margin-right: auto !important;"><b>Address</b></label>
			<input type="text" data-mini="true" name="arp-address" id="arp-address" value="" readonly/>
			<label for="arp-phone" style="margin-left: 10px;margin-right: auto !important;" ><b>Phone Number </b></label>
			<input type="text" data-mini="true" name="arp-phone" id="arp-phone" value="" readonly/>
			<label for="arp-descript" style="margin-left: 10px;margin-right: auto !important;"><b>Notes</b></label>
			<textarea cols="40" rows="8" name="arp-descript" id="arp-descript" data-mini="true" readonly></textarea>
			<label for="arp-kacc" style="margin-left: 10px;margin-right: auto !important;"><b>K-Computer Account</b></label>
			<input type="text" data-mini="true" name="arp-kacc" id="arp-kacc" value="" readonly/>
			<label id="arp-publickey-lbl" for="arp-publickey" style="margin-left: 10px;margin-right: auto !important;"><b>Public Key</b></label>
			<textarea cols="40" rows="8" name="arp-publickey" id="arp-publickey" data-mini="true" readonly></textarea>
			<label id="arp-timeusable-lbl" for="arp-timeusable" style="margin-left: 10px;margin-right: auto !important;" ><b>Time Usable (Ms)</b></label>
			<!--input type="time" data-clear-btn="false" name="arp-timeusable" id="arp-timeusable" value=""-->
			<input type="number" data-clear-btn="false" name="arp-timeusable" id="arp-timeusable" step="0.001" value="">
			<!--input type="number" data-clear-btn="false" name="arp-timeusable" pattern="[0-9]*,." id="arp-timeusable" step="any" value=""-->
			<a href="#" id="arp-register-accept-btn" data-role="button" data-inline="true" data-theme="b" data-icon="check" onclick="admin_acceptRegisterBtnClick();" data-rel="back">Accept</a>
			<a href="#" id="arp-register-reject-btn" data-role="button" data-inline="true" data-theme="d" data-icon="delete" onclick="admin_rejectRegisterBtnClick();" data-rel="back">Reject</a>
			<a href="#" id="arp-provider-reject-btn" data-role="button" data-inline="true" data-theme="d" data-icon="delete" onclick="admin_rejectProviderBtnClick();" data-rel="back">Reject</a>
			<a href="#" id="arp-provider-apply-btn" data-role="button" data-inline="true" data-theme="d" data-icon="check" onclick="admin_applyroviderBtnClick();" data-rel="back">Apply</a>
		</div>
	</div>
	
</div>
</body>
</html>
