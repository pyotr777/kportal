#ifndef REQ_TYPE_HPP
#define REQ_TYPE_HPP

using namespace std;

#define REQ_CONNECT_STR							"connect"
#define REQ_LOGOUT_STR							"logout"
#define REQ_FIND_SERVICES_STR					"findServices"
#define REQ_GET_SERVICE_STR						"getService"
#define REQ_GET_RUN_METHODS_FILE_STR			"getRunMethodsFile"
#define REQ_SUBMIT_JOB_STR						"submitJob"
#define REQ_QUERY_STATUS_STR					"queryStatus"
#define REQ_GET_OUTPUT_FILE_STR					"getOutputFile"
#define REQ_JOB_LIST_STR						"jobList"
#define REQ_DELETE_JOB_STR						"deleteJob"
//#define REQ_RESTART_JOB_STR					"restartJob"
#define REQ_CANCEL_JOB_STR						"cancelJob"
#define REQ_ID_STR								"id"

#define REQ_GET_ALL_SERVICES_STR				"getAllServices"
#define REQ_GET_SERVICES_PROVIDER_STR           "getServices"
#define REQ_REGISTER_SERVICE_STR				"registerService"
#define REQ_GET_USERS_REGISTER_SERVICE_STR      "getCandidates"
#define REQ_GET_USERS_SERVICE_STR               "getUsers"

#define REQ_GET_SERVICES_USER_REGISTER_STR		"getServicesHaveCandidates"
#define REQ_ADD_USER_SERVICE_STR				"addUser"
#define REQ_REMOVE_USER_SERVICE_STR				"removeUser"
#define REQ_GET_SERVICES_REGISTER_STR			"getWaitingServices"
#define REQ_CANCEL_SERVICE_REGISTER_STR         "cancelRegistering"

#define REQ_GET_ENTRIES_JOB_STR                 "getEntries"
#define REQ_REMOVE_CANDIDATE_SERVICE_STR        "removeCandidate"

#define REQ_GET_INFORMATION_JOBS_STR            "getJobsByService"
#define REQ_GET_INFORMATION_JOB_STR             "getJobInfos"

#define REQ_USER_REMOVE_SERVICE_STR             "userRemoveService"
#define REQ_RESUBMIT_JOB_STR                    "resubmitJob"

#define REQ_CREATE_SERVICE_STR                  "createService"
#define REQ_UPDATE_SERVICE_STR                  "updateService"
#define REQ_DELETE_SERVICE_STR                  "deleteService"
#define REQ_GET_PARAM_OF_SERVICE_STR            "getParameters"
#define REQ_UPLOAD_FILE_ICON_STR                "uploadIcon"
#define REQ_CHECK_PATH_SERVICE_STR              "checkPathOfService"
#define REQ_CHECK_SERVICE_NAME_USABLE_STR       "checkServiceNameUsable"

#define REQ_UPLOAD_FILE_STR                     "uploadFile"
#define REQ_GET_PROVIDERS_STR					"getProviders"
#define REQ_GET_PROVIDER_REGISTERS_STR          "getProviderRegisters"
#define REQ_ACCEPT_PROVIDER_REGISTERS_STR       "acceptProviderRegisters"
#define REQ_REJECT_PROVIDER_REGISTERS_STR       "rejectProviderRegisters"
#define REQ_REJECT_PROVIDERS_STR				"rejectProviders"
#define REQ_UPDATE_PROVIDER_USABLE_TIME_STR				"updateProviderUsableTime"

#define REQ_REGISTER_PROVIDER_STR		        "registerProvider"
#define REQ_REJECT_PROVIDER_STR					"rejectProvider"

#define REQ_FIND_IMAGES_STR                     "findImages"
#define REQ_CREATE_IMAGE_STR                    "createImage"
#define REQ_UPDATE_IMAGE_STR                    "updateImage"
#define REQ_DELETE_IMAGE_STR                    "deleteImage"
#define REQ_GET_IMAGE_FILE_STR                  "getImageFile"
#define REQ_CHECK_IMAGE_NAME_USABLE_STR         "checkImageNameUsable"

#define REQ_GET_PROVIDER_TIME_RESOURCE_STR      "getProviderTimeResource"


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
    REQ_DELETE_JOB,
    REQ_RESTART_JOB,
    REQ_CANCEL_JOB,
    REQ_ID,
    REQ_GET_ALL_SERVICES,
    REQ_GET_SERVICES_PROVIDER,
    REQ_REGISTER_SERVICE,
    REQ_GET_USERS_REGISTER_SERVICE,
    REQ_GET_USERS_SERVICE,
    REQ_GET_SERVICES_USER_REGISTER,
    REQ_ADD_USER_SERVICE,
    REQ_REMOVE_USER_SERVICE,
    REQ_GET_SERVICES_REGISTER,
    REQ_CANCEL_SERVICE_REGISTER,
    REQ_GET_ENTRIES_JOB,
    REQ_REMOVE_CANDIDATE_SERVICE,
    REQ_GET_INFORMATION_JOBS,
    REQ_GET_INFORMATION_JOB,
    REQ_USER_REMOVE_SERVICE,
    REQ_RESUBMIT_JOB,
    REQ_CREATE_SERVICE,
    REQ_UPDATE_SERVICE,
    REQ_DELETE_SERVICE,
    REQ_GET_PARAM_OF_SERVICE,
    REQ_UPLOAD_FILE_ICON,
    REQ_CHECK_PATH_SERVICE,
	REQ_UPLOAD_FILE,

	REQ_GET_PROVIDERS,
	REQ_GET_PROVIDER_REGISTERS,
    REQ_ACCEPT_PROVIDER_REGISTERS,
    REQ_REJECT_PROVIDER_REGISTERS,
	REQ_REJECT_PROVIDERS,
  REQ_UPDATE_PROVIDER_USABLE_TIME,

	REQ_FIND_IMAGES,
	REQ_CREATE_IMAGE,
	REQ_UPDATE_IMAGE,
	REQ_CHECK_NAME_USABLE,
	REQ_DELETE_IMAGE,
	REQ_GET_IMAGE_FILE,
	REQ_CHECK_IMAGE_NAME_USABLE,
	REQ_CHECK_SERVICE_NAME_USABLE,
	REQ_REGISTER_PROVIDER,
  REQ_REJECT_PROVIDER,
  REQ_GET_PROVIDER_TIME_RESOURCE

}  RequestType;
#endif //REQ_TYPE_HPP
