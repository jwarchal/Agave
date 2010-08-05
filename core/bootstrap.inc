<?php
/*
 * To add phases into the bootstrap process:
 * 	1. first define a phase for your item such as BOOTSTRAP_MYPHASE
 * 	2. Insert that phase into the function bootstrap()'s switch statement
 * 	3. Add the function to this file, at the end, make sure it returns a call to bootstrap with the next phase, also set $agave->status = BOOTSTRAP_MYPHASE
 * 	4. Make sure the bootstrap function that should execute BEFORE your phase returns a call to bootstrap with your new phase
 */

//bootstrap status definitions
define('SITE_NOT_FOUND', 1);
define('SITE_OFFLINE', 2);
define('VALID_REQUEST', 3);
define('ACCESS_DENIED', 4);
define('DESTINATION_UNKNOWN', 5);

//bootstrap phase definitions
define('BOOTSTRAP_SETTINGS', 0);
define('BOOTSTRAP_AGAVE', 1);
define('BOOTSTRAP_HOST', 2); //check against include/exclude list for hosts and IPs
define('BOOTSTRAP_DB', 3); //make db connection
define('BOOTSTRAP_FAST_ROUTE', 4); //parse request, check for fast-track requests
define('BOOTSTRAP_ENV', 5); //load environment (load settings, parse important paths, set error handler, date/time)
define('BOOTSTRAP_SESSION', 6); //start user session ($user is built here)
define('BOOTSTRAP_ACCESS', 7); //access check against url
define('BOOTSTRAP_LANG', 8); //load language settings

//module status definitions
define('MODULE_LOCATION_GLOBAL', 1);
define('MODULE_LOCATION_LOCAL', 0);
define('MODULE_ENABLED', 1);
define('MODULE_NOT_ENABLED', 0);

function bootstrap($phase=BOOTSTRAP_SETTINGS, $vars=NULL) {
	switch($phase) {
		//TODO: fit caching in here somewhere
		case BOOTSTRAP_SETTINGS : $status = bootstrap_settings($vars); break;
		case BOOTSTRAP_AGAVE : $status = bootstrap_agave($vars); break;
		case BOOTSTRAP_HOST : $status = bootstrap_host($vars); break;
		case BOOTSTRAP_DB : $status = bootstrap_db($vars); break;
		case BOOTSTRAP_FAST_ROUTE : $status = bootstrap_fast_route($vars); break;
		case BOOTSTRAP_ENV : $status = bootstrap_env($vars); break;
		case BOOTSTRAP_SESSION : $status = bootstrap_session($vars); break;
		case BOOTSTRAP_ACCESS : $status = bootstrap_access($vars); break;
		case BOOTSTRAP_LANG : $status = bootstrap_lang($vars); break;
	}
	return $status;
}

function bootstrap_settings() {
	/*
	 * This function is taken from Drupal 6 bootstrap.inc - see http://api.drupal.org
	 *
	 * This is the first phase, in here Agave simply looks for the proper settings.php file, corresponding to the request received...
	 * For example, a request to www.russnet.org/homepage/look/lol will resolve to agave_root/sites/www.russnet.org/settings.php
	 * Likewise, if the site root is www.livefromrussia.org/stage1, the request will resolve to agave_root/sites/www.livefromrussia.org.stage1/settings.php
	 */
	global $settings;

	$confdir = 'sites';
	$uri = explode('/', $_SERVER['SCRIPT_NAME'] ? $_SERVER['SCRIPT_NAME'] : $_SERVER['SCRIPT_FILENAME']);
	$server = explode('.', implode('.', array_reverse(explode(':', rtrim($_SERVER['HTTP_HOST'], '.')))));
	for ($i = count($uri) - 1; $i > 0; $i--) {
		for ($j = count($server); $j > 0; $j--) {
			$dir = implode('.', array_slice($server, -$j)) . implode('.', array_slice($uri, 0, $i));
			
			if(file_exists("$confdir/$dir/settings.php")) {
				global $initFile;
				$initFile = "$confdir/$dir/settings.php";
				return bootstrap(BOOTSTRAP_HOST);
			}
		}
	}
	
	return SITE_NOT_FOUND;
}

function bootstrap_host() {
	/*
	 * Second phase, a quick check against any banned hosts specified in your settings.php file
	 */
	global $settings;
	if(isset($settings['ip-include']) && !in_array($_SERVER['REMOTE_ADDR'], $settings['ip-include'])) banned_location();
	if(isset($settings['ip-exclude']) && in_array($_SERVER['REMOTE_ADDR'], $settings['ip-exclude'])) banned_location();
	
	return bootstrap(BOOTSTRAP_AGAVE);
}

function bootstrap_agave() {
	
	/*
	 * Third phase: based on what's in settings.php, we determine core paths for agave
	 * 
	 */
	global $agave, $settings, $db_info, $initFile;
	include($initFile);
	//TODO: we'll be changing this to point at core/db/db.object
	include('./core/db/olddb.object');
        include('./core/db/newdb.object');
	include('./core/agave.object');
	include("./core/dispatcher.object");
	$agave = new agave();
	$agave->connect($db_info);

	//TODO: initialize PDO db stuff here, when it's ready
	
	//core agave paths
	$protocol = isset($_SERVER['HTTPS']) ? 'https://' : 'http://';
	$url = $_SERVER['HTTP_HOST'].$_SERVER['SCRIPT_NAME'];
	$agave->url = substr($protocol.$url, 0, -9); //chops off "index.php", it MAY get rewritten with "?dest=" in the _env phase
	$agave->web_dir = $_SERVER['DOCUMENT_ROOT']."/"; //avoid using this for anything, not used anywhere, please don't use it unless you have a very good reason
	$agave->core_url = substr($protocol.$url, 0, -9);
	$agave->core_dir = explode("/", dirname(__FILE__)); //chops off "index.php" //this one does NOT get rewritten with ?dest= - it's a URL to be used for files that are in a subdirectory of agave root
	array_pop($agave->core_dir);
	$agave->core_dir = implode("/", $agave->core_dir)."/";

	//project paths
	$agave->proj_dir = $agave->core_dir.dirname($initFile)."/";
	$agave->proj_url = $agave->core_url.dirname($initFile)."/";

	//project uploaded file paths
	$agave->upfiles_dir = $agave->proj_dir."files/";
	$agave->upfiles_url = $agave->proj_url."files/";

	//resolve request with dispatcher
	$GLOBALS['objects']['dispatcher'] = new dispatcher;
	if(!empty($_GET['dest'])) {
		$agave->destination = rtrim($_GET['dest'], "/");
		$agave->current_location = $agave->destination;
		$agave->uri = $GLOBALS['objects']['dispatcher']->resolveRequest($agave->destination);
	}


	$agave->status = BOOTSTRAP_AGAVE;
	return bootstrap(BOOTSTRAP_ENV);
}

function bootstrap_env() {
	global $agave, $settings;
	$agave->loadSettings(); //loads settings data from cache or rebuilds from system_variables table

	if($agave->setting('development_mode')) $agave->cache = FALSE;

	$agave->loadModules(); //loads module data structure from cache, or builds from .info files if necessary
 
	set_error_handler('agave_error_handler');
	date_default_timezone_set('America/New_York');	//TODO: change to get timezone setting from settings

	//add ?dest= back into $agave->url if clean URLS are enabled, disabled by default
	if(!$agave->setting('clean_urls') || $agave->setting('clean_urls') == FALSE) $agave->url = $agave->url."?dest=";

	//if we haven't resolved the request yet, it's because we're at the home page
	if(!$agave->destination) {
		$agave->destination = $agave->setting('home_page');
		$agave->current_location = $agave->destination;
		$agave->uri = $GLOBALS['objects']['dispatcher']->resolveRequest($agave->destination);
	}
	$agave->current_url = $agave->url.$agave->destination;

	//TODO: access log here

	if($agave->uri['validRequest'] && $agave->uri['fast_route']) {
		$GLOBALS['objects']['dispatcher']->route($agave->uri);
		$agave->stop();
	}
	$agave->status = BOOTSTRAP_ENV;
	return bootstrap(BOOTSTRAP_SESSION);
}

function bootstrap_session() {
	global $agave;
	include("./core/user.object");
	include("./core/session.object");

	$GLOBALS['objects']['session'] = new session;
	
	//start the users session - global user is built here
	$GLOBALS['objects']['session']->start();

	//needed to wait until after session was created to throw our 404 error for logging purposes
	if(!$agave->uri['validRequest']) return DESTINATION_UNKNOWN;

	//start the users session and set important session data
	$_SESSION['referrer'] = (isset($_SERVER['HTTP_REFERER'])) ? $_SERVER['HTTP_REFERER'] : $agave->url;
	$_SESSION['agent'] = $_SERVER['HTTP_USER_AGENT'];
	$_SESSION['destination'] = $agave->uri['absoluteDest'];
	$_SESSION['previousLocation'] = ($_SESSION['referrer'] != $_SESSION['destination']) ? $_SESSION['referrer'] : $_SESSION['previousLocation']; //TODO: this isn't taking into account stuff in $_GET string
	if(!empty($_POST)) $_SESSION['previousPost'] = $_POST;
		
	//TODO: handle user time-outs... how?

	$agave->status = BOOTSTRAP_SESSION;
	return bootstrap(BOOTSTRAP_ACCESS);
}

function bootstrap_access() {
	global $agave;

	if(!empty($agave->uri['access']) && !$agave->user->access($agave->uri['access'])) return ACCESS_DENIED;
	
	$agave->status = BOOTSTRAP_ACCESS;
	return bootstrap(BOOTSTRAP_LANG);
}

function bootstrap_lang() {
	global $agave;

	//check language domain settings, site settings, user settings (in that order)
	//TODO: load translation file

	$agave->status = BOOTSTRAP_LANG;
	return VALID_REQUEST;
}


function banned_location() {
	global $settings;
	header('HTTP/1.1 403 Forbidden');
	die("<h1>HTTP/1.1 403 Forbidden</h1><h2>Your location has been banned from accessing <em>".$_SERVER['SERVER_NAME']."</em>.</h2>");
}

function site_not_found() {
	header('HTTP/1.1 404 Not Found');
	die("<h1>HTTP/1.1 404 Not Found</h1><h2>Agave could not find the site you requested: <em>".$_SERVER['SERVER_NAME']."</em>.</h2>");
}

function site_offline() {
	global $agave;
	header('HTTP/1.1 404 Not Found');
	die("<h1>HTTP/1.1 404 Not Found</h1><h2>Agave could not establish a database connection to <em>".$_SERVER['SERVER_NAME']."</em>.</h2><p>$agave->dbError</p>");
}

function agave_error_page($type=NULL, $message=FALSE) {
	//The purpose of this function is to kill the processing of a request midstream, yet still render a full page with the necessary message
	//The most common need for this would be for "404" errors or "ACCESS_DENIED" errors
	//common errors (note that these aren't real HTTP responses, we use these codes for convenience because they correspond to actual errors that would occur if Apache were to handle this scenario on the file system):
	//404 - content not found on server (or in database)
	//403 - access denied
	global $agave;
	$t = $agave->load('themer');
	$_SESSION['agave_error_page'] = TRUE;

	$message = "<h2>Error: $type</h2>".$message;
	if(!$message) $message .= "<h5>A serious error occured - please contact the site administrator if this is a recurring issue</h5>";
	$t->output = $message."<h4></h4><hr /><h5>URL info: </h5><pre>".print_r($agave->uri, TRUE)."</pre>";

	$type = ($type) ? $type : "unknown";
	$agave->log($type, $message);

	//set headers if necessary
	//TODO: abstract out these error codes, get a complete list
	if($type=='404') {
		$t->page['title'] = "Content Not Found (404)";
		header('HTTP/1.1 404 Not Found');
	}
	if($type=='403') {
		$t->page['title'] = "Access Denied (403)";
		header('HTTP/1.1 403 Forbidden');
	}
	if($type=='415') {
		$t->page['title'] = "Unsupported Media Type (415)";
		header('HTTP/1.1 415 Unsupported Media Type');
	}
	
	//use page template to markup error page
	$t->output = $t->theme(NULL, $t->pageTemplate);

	unset($_SESSION['agave_error_page']);

	//render error page
	$agave->stop($t->output."<p style='padding: 5px;color: #fff'>".agave_timer_return()."</p>");
}

function agave_error_handler($errno, $errstr, $errfile, $errline) {
	global $agave;
	$types = array( //thanks drupal, seriously
		1 => 'error',
		2 => 'warning',
		4 => 'parse error',
		8 => 'notice',
		16 => 'core error',
		32 => 'core warning',
		64 => 'compile error',
		128 => 'compile warning',
		256 => 'user error',
		512 => 'user warning',
		1024 => 'user notice',
		2048 => 'strict warning',
		4096 => 'recoverable fatal error'
	);

	$message = "<code>PHP ".$types[$errno]."</code>: \"$errstr\" in <em>$errfile</em> on line <b>$errline</b>";
	
	$agave->message('error', $message);

    return true;
}

function agave_timer_start() {
	global $agave_timer; 
	$agave_timer = microtime(TRUE);
}

function agave_timer_return() {
	global $agave_timer;
	return substr((microtime(TRUE) - $agave_timer), 0, 7);
}

function __autoload($object) {
	/*
	 * Provides support for autoloading in classes that are being extended or implemented
	 */
	global $agave;
	$file = $agave->getObjectFile($object);
	if(!$file) die("Autoload error - could not load <em>$object</em> object.<hr><h3>URI info:</h3><pre>".print_r($agave->uri, TRUE)."</pre><h3>Module info:</h3><pre>".print_r($agave->setting('module_data'), TRUE)."</pre>");
	if(file_exists($file)) include($file);
}

function t($string) {
	global $agave;
	if($agave->languagesLoaded) {
		//check against dictionary, return translated string
	}
	else return $string;
}