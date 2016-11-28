var RESULTS = {
	SUCCESS : 0,
	ERR_UNKNOW : 1
};

/*
 * Just test
 */
///localStorage.loginId =  "loginID_test";
//localStorage.email = "anlabprovider@gmail.com";
//localStorage.userType  = "provider";

/*
 * Error code
 */
var ResponseCodes = {
    DATA_SUCCESS : 0,
    ACTION_WAIT_ICON : 1,
    ACTION_WAIT_INPUT : 2,
	ACTION_WAIT_TAR : 3,
    /// Database error: ( 10 - 29 )
    DATA_ERROR	: 10,
    DATA_ERROR_OPEN_DB : 11,
    DATA_ERROR_CLOSE_DB : 12,
    DATA_ERROR_CREATE_TB : 13,
    DATA_ERROR_INSERT_DB : 14,
    DATA_ERROR_SELECT_DB : 15,
    DATA_ERROR_UPDATE_DB : 16,
    DATA_ERROR_DELETE_DB : 17,
    DATA_SELECT_EMPTY : 18,
    DATA_ERROR_START_TRANSACTION : 19,
    DATA_ERROR_END_TRANSACTION : 20,
    
    DATA_IS_EXISTED : 21,
    
    ERROR_CREATE_JOB_FOLDER : 50,
    ERROR_SUBMIT_JOB : 51,
    ERROR_JOB_DONT_INIT : 52,
    ERROR_STOP_JOB : 53,
    ERROR_JOB_NOT_EXIST : 54,
    ERROR_DIR_NOT_EXIST : 55,
    ERROR_DUPLICATE_PATH_EXCUTE : 56,
    ERROR_FX10_RETURN : 57,
	
    PERMISSION_DENIED : 100,

    REQUEST_SUCCESS : 150,
    REQUEST_UNKNOW_HOST : 151,
    REQUEST_SOCKET_ERROR : 152,
    REQUEST_CONNECT_ERROR : 153,
    REQUEST_SEND_REQUEST_ERROR : 154,
	REQUEST_RECEIVE_REQUEST_ERROR : 155,
    REQUEST_DOCKER_ERROR : 156,
	
	FILE_ACTION_SUCCESS : 200,
	FILE_ACTION_ERROR : 201,

	DOCKER_CREATE_CON_SUCCESS : 250,
	DOCKER_CREATE_CON_ERROR : 251,
	ERROR_TIMEQUOTA_OVER : 300,
    ERROR_KDESK_ACC_EXISTS : 301,

  ERROR_SERVICE_INVALID_NAME : 350,
  ERROR_SERVICE_EXEPATH_NOTFOUND : 351,
  ERROR_SERVICE_SHPATH_NOTFOUND : 352,
  ERROR_SERVICE_SLAVEDAEMON_NOTFOUND : 353,
  ERROR_SERVICE_STAGEINDIR_NOTFOUND : 354,
  ERROR_SERVICE_PRESHPATH_NOTFOUND: 355,
  ERROR_SERVICE_POSTSHPATH_NOTFOUND: 356
};

/*
 * Authorize code
 */
var AuthorizeCodes = {
    ERR_UNKNOW : 0,
    ERR_NO_AUTHENTICATION : 1,
    ERR_INVALID_ACCESS : 2
};


var SUCCESS 							= 0,
	ERROR								= -1,
	ERR_READ_FILE 						= -2,
	ERR_INVALID_SHORTCUT_FILE_FORMAT 	= -3,
	ERR_SERVER_RESPONSE					= -4
	;
var gl_error = {};

function gl_setError(code, detail){
	gl_error.code = code;
	gl_error.detail = detail;
}

var NUMBER_OF_ITEMS_EACH_PAGE			= 10;
var NOB_REQUEST_ID              = 8   /// Number of requestId byte
var NOB_SENT_BYTE               = 4   /// Number of sent byte
var NOB_TOTAL_BYTE              = 4   /// Number of total byte
var NOB_PARAM_ID_BYTE           = 4   /// Number of paramid
