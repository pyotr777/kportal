#ifndef REQ_TYPE_HPP
#define REQ_TYPE_HPP

using namespace std;

#define REQ_CONNECT_STR 		"connect"
#define REQ_LOGOUT_STR			"logout"
#define REQ_FIND_SERVICES_STR	"findServices"
#define REQ_GET_SERVICE_STR		"getService"
#define REQ_GET_RUN_METHODS_FILE_STR		"getRunMethodsFile"
#define REQ_SUBMIT_JOB_STR                  "submitJob"
#define REQ_QUERY_STATUS_STR                "queryStatus"
#define REQ_GET_OUTPUT_FILE_STR             "getOutputFile"
#define REQ_JOB_LIST_STR                    "jobList"
#define REQ_DELETE_JOB_STR                  "deleteJob"
#define REQ_RESTART_JOB_STR                 "restartJob"
#define REQ_CANCEL_JOB_STR                  "cancelJob"
typedef enum
{
	REQ_UNKNOW	= 0,
	REQ_CONNECT,
	REQ_LOGOUT,
	REQ_FIND_SERVICES,
	REQ_GET_SERVICE,
	REQ_GET_RUN_METHODS_FILE,
    REQ_SUBMIT_JOB,
    REQ_QUERY_STATUS,
    REQ_GET_OUTPUT_FILE,
    REQ_JOB_LIST,
    REQ_RESTART_JOB,
    REQ_CANCEL_JOB
}  REQ_TYPE ;

#endif //REQ_TYPE_HPP
