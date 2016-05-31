//HISTORY
//2013/09/06    EH0001  longnm  Add api for client

#ifndef TAG_KEY_HPP
#define TAG_KEY_HPP


/* Main tag key */
#define TAG_MSG_STR			"msg"
#define TAG_REQUEST_ID_STR  "requestId"
#define TAG_OWNER_STR       "owner"

/* Connect request */
#define TAG_USER_ID_STR 	"user"
#define TAG_PWD_STR			"pwd"

#define TAG_LOGIN_ID_STR    "loginId"
#define TAG_TOKEN_STR       "token"
#define TAG_EMAIL_STR       "email"

/* User type */
#define TAG_USER_TYPE_STR  "userType"
#define TYPE_USER_STR       "user"
#define TYPE_PROVIDER_STR   "provider"
#define TYPE_ADMIN_STR      "admin"

/* Connect response */
#define TAG_VALID_STR		"valid"
#define VALID				1
#define INVALID				0

/*	Find services	*/
#define TAG_FIND_SERVICES_STR	"findServices"
#define TAG_SERVICES_STR        "services"
#define TAG_SERVICE_STR         "service"
#define TAG_PROVIDER_STR        "provider"
#define TAG_ITEMS_STR           "items"
//EH0001
#define TAG_CANDIDATES_STR      "candidates"
#define TAG_USERS_STR           "users"
#define TAG_CURDIR_STR          "curDir"
#define TAG_ENTRIES_STR         "entries"
#define TAG_ENTRY_STR         "entry"
#define TAG_ISDIR_STR         "isDir"
//EH0001 end

/*	Get service */
//#define TAG_ITEMS_STR               "getService"

/* getRunMethodsFile */
#define TAG_NUM_OF_PACKETS_STR      "numOfPackets"

/* Submit job */
#define TAG_JOB_STR                 "job"
#define TAG_RUN_METHOD_STR          "runMethod"
#define TAG_PARAMS_STR              "params"
#define TAG_PATH_STR                "path"
#define TAG_TYPE_STR                "type"
#define LOCAL_STR                   "local"
#define REMOTE_STR                  "remote"
#define TAG_DATA_STR                "data"

//EH0001
#define TAG_JOB_ID_STR              "jobId"
#define TAG_JOB_NAME_STR              "jobName"
#define TAG_TIME_START_STR              "start"
#define TAG_TIME_END_STR              "end"
#define TAG_TIME_DURATION_STR              "duration"

//EH0001 end

//EH0011
#define TAG_SERVICE_NAME_STR        "name"
#define TAG_PRODUCER_STR            "producer"
#define TAG_DESCRIPTION_STR         "description"
#define TAG_PATH_EXCUTE_FILE_STR    "path"
#define TAG_PATH_SH_FILE_STR        "shpath"
#define TAG_NUMBER_OF_NODES_STR     "numberOfNodes"
#define TAG_NUMBER_OF_USER_STR      "numberOfUsers"
#define TAG_NUMBER_OF_WAIT_USER_STR     "numberOfWaitUsers"
#define TAG_MAX_ESLAPSED_TIME_STR     "maxElapsedTime"
#define TAG_ICON_STR     "icon"
#define TAG_KDESKACC_STR            "kdeskacc"
#define TAG_STAGEINDIRS_STR         "stageinDirs"
#define TAG_STAGEOUTDIRS_STR        "stageoutDirs"
#define TAG_PUBLICKEY_STR           "publicKey"
#define TAG_TIMEUSAGE_STR           "timeUsage"
#define TAG_TIMEUSABLE_STR          "timeUsable"

//EH0011 end

//EH0011
#define TAG_RES_CODE_STR          "result"
//EH0011 end

//EH0011
#define TAG_IS_OUTPUT_STR			"isOutput"
#define TAG_IS_REQUIRED_STR         "isRequired"
#define TAG_OPTION_NAME_STR         "prefix"
#define TAG_PARAMETER_NAME_STR      "name"
#define TAG_PARAMETER_STR			"param"
#define TAG_DEFAULT_VALUE_STR       "defaultValue"
#define TAG_ACTION_STR              "action"
//EH0011 end

/* Query job status */
#define TAG_QUERY_STATUS_STR        "queryStatus"
#define TAG_STATUS_STR              "status"

/* GetOutputFile */
#define TAG_PATH_STR                "path"
#define TAG_GET_OUTPUT_FILE_STR     "getOutputFile"


#define TAG_ID_STR                  "id"
#define TAG_FINISH_STR              "finish"
#define TAG_ACK_STR                 "ACK"
#define ACK_OK                      "OK"

/* Errors message */

//#define ERROR_AUTHORIZE_STR         1
//#define ERROR_INVALID_ACCESS_STR    2
//#define SUCCESS_AUTHORIZE_STR       2

/* JobList */
#define TAG_JOBS_STR					"jobs"

/* File transfer */
#define TAG_UPLOAD_FILE_STR             "uploadFile"
#define TAG_FROM_STR                    "from"


/* Provider info */
#define TAG_PHONE_STR                   "phone"
#define TAG_ADDRESS_STR                 "address"
#define TAG_PROVIDERS_STR               "providers"
#define TAG_REGISTERS_STR               "registers"

/* Image */
#define TAG_NAME_STR                    "name"
#define TAG_IS_OWNER_STR                "isOwner"
#define TAG_IMAGES_STR                  "images"
#define TAG_IS_PUBLIC_STR               "isPublic"
#define TAG_IS_INCLUDE_SAMPLE_STR       "isIncludeSample"
#define TAG_CLASS_STR                   "class"
#define TAG_TAR_FILE_STR                "tarfile"
#define TAG_IMAGE_STR                   "image"
#define TAG_KEYWORD_STR                 "keyword"

/*Docker tag respone */
#define DOCKER_TAG_RESPONE_NETWORK_SETTING				"NetworkSettings"
#define DOCKER_TAG_RESPONE_IP_ADDRESS             "IPAddress"

/*Time resource"*/
#define TAG_UNIT_STR                    "unit"
#define TAG_TOTAL_KD_STR                   "totalkd"
#define TAG_USAGE_KD_STR                   "usagekd"
#define TAG_REMAIN_KD_STR                  "remainkd"
#define TAG_TOTAL_KP_STR                "totalkp"
#define TAG_USAGE_KP_STR                "usagekp"
#define TAG_REMAIN_KP_STR               "remainkp"

#endif //TAG_KEY_HPP
