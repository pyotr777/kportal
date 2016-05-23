#ifndef TAG_KEY_HPP
#define TAG_KEY_HPP

/* Main tag key */
#define TAG_MSG_STR			"msg"
#define TAG_OWNER_STR       "owner"

/* Connect request */
#define TAG_USER_ID_STR 	"user"
#define TAG_PWD_STR			"pwd"

/* Connect response */
#define TAG_VALID			"valid"
#define VALID				1
#define INVALID				0

/*	Find services	*/
#define TAG_FIND_SERVICES_STR	"findServices"
#define TAG_SERVICES_STR	"services"
#define TAG_SERVICE_STR		"service"
#define TAG_PROVIDER_STR	"provider"
#define TAG_ITEMS_STR		"items"

/*	Get service *
//#define TAG_ITEMS_STR		"getService"

/* getRunMethodsFile */
#define TAG_NUM_OF_PACKETS_STR	"numOfPackets"

/* Submit job */
#define TAG_JOB_STR                 "job"
#define TAG_RUN_METHOD_STR          "runMethod"
#define TAG_PARAMS_STR              "params"
#define TAG_PATH_STR                "path"
#define TAG_TYPE_STR                "type"
#define LOCAL_STR                   "local"
#define REMOTE_STR                  "remote"
#define TAG_DATA_STR                "data"

#define TAG_ACK_STR                 "ACK"
#define ACK_OK                      "OK"

/* Query job status */
#define TAG_QUERY_STATUS_STR        "queryStatus"
#define TAG_STATUS_STR              "status"

/* GetOutputFile */
#define TAG_PATH_STR                "path"
#define TAG_GET_OUTPUT_FILE_STR     "getOutputFile"
#define TAG_ID_STR                  "id"
#define TAG_REQUEST_ID_STR          "requestId"
#define TAG_OWNER_ID_STR            "ownerId"
#define TAG_FINISH_STR              "finish"

/* JobList */
#define TAG_JOBS_STR                "jobs"



#endif //TAG_KEY_HPP
