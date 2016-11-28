#include <iostream>
#include "ReqUtils.hpp"

using namespace std;
RequestType ReqUtils::GetID(std::string name)
{
    //std::cout<<"GetID("<<name<<")"<<std::endl;
    RequestType id = REQ_UNKNOW;
	if(name == REQ_CONNECT_STR)
		id = REQ_CONNECT;
	else if(name == REQ_LOGOUT_STR)
		id = REQ_LOGOUT;
	else if(name == REQ_FIND_SERVICES_STR)
		id = REQ_FIND_SERVICES;
	else if(name == REQ_GET_SERVICE_STR)
		id = REQ_GET_SERVICE;
    else if(name == REQ_GET_RUN_METHODS_FILE_STR)
        id = REQ_GET_RUN_METHODS_FILE;
    else if(name == REQ_SUBMIT_JOB_STR)
        id = REQ_SUBMIT_JOB;
    else if(name == REQ_QUERY_STATUS_STR)
        id = REQ_QUERY_STATUS;
    else if(name == REQ_GET_OUTPUT_FILE_STR)
        id = REQ_GET_OUTPUT_FILE;
    else if(name == REQ_JOB_LIST_STR)
        id = REQ_JOB_LIST;
    else if(name == REQ_DELETE_JOB_STR)
        id = REQ_DELETE_JOB;
    //else if(name == REQ_RESTART_JOB_STR)
    //    id = REQ_RESTART_JOB;
    else if(name == REQ_CANCEL_JOB_STR)
        id = REQ_CANCEL_JOB;
    else if(name == REQ_ID_STR)
        id = REQ_ID;

	else if(name == REQ_GET_ALL_SERVICES_STR)
        id = REQ_GET_ALL_SERVICES;
    else if(name == REQ_GET_SERVICES_PROVIDER_STR)
        id = REQ_GET_SERVICES_PROVIDER;
    else if(name == REQ_REGISTER_SERVICE_STR)
        id = REQ_REGISTER_SERVICE;
    else if(name == REQ_GET_USERS_REGISTER_SERVICE_STR)
        id = REQ_GET_USERS_REGISTER_SERVICE;
    else if(name == REQ_GET_USERS_SERVICE_STR)
        id = REQ_GET_USERS_SERVICE;
    else if(name == REQ_GET_SERVICES_USER_REGISTER_STR)
        id = REQ_GET_SERVICES_USER_REGISTER;
    else if(name == REQ_ADD_USER_SERVICE_STR)
        id = REQ_ADD_USER_SERVICE;
    else if(name == REQ_REMOVE_USER_SERVICE_STR)
        id = REQ_REMOVE_USER_SERVICE;
    else if(name == REQ_REMOVE_CANDIDATE_SERVICE_STR)
        id = REQ_REMOVE_CANDIDATE_SERVICE;
    else if(name == REQ_GET_SERVICES_REGISTER_STR)
        id = REQ_GET_SERVICES_REGISTER;
    else if(name == REQ_CANCEL_SERVICE_REGISTER_STR)
        id = REQ_CANCEL_SERVICE_REGISTER;
    else if(name == REQ_GET_ENTRIES_JOB_STR)
        id = REQ_GET_ENTRIES_JOB;
    else if(name == REQ_GET_INFORMATION_JOBS_STR)
        id = REQ_GET_INFORMATION_JOBS;
    else if(name == REQ_GET_INFORMATION_JOB_STR)
        id = REQ_GET_INFORMATION_JOB;
    else if(name == REQ_USER_REMOVE_SERVICE_STR)
        id = REQ_USER_REMOVE_SERVICE;
    else if(name == REQ_RESUBMIT_JOB_STR)
        id = REQ_RESUBMIT_JOB;

    else if(name == REQ_CREATE_SERVICE_STR)
        id = REQ_CREATE_SERVICE;
    else if(name == REQ_GET_PARAM_OF_SERVICE_STR)
        id = REQ_GET_PARAM_OF_SERVICE;
    else if(name == REQ_UPDATE_SERVICE_STR)
        id = REQ_UPDATE_SERVICE;
    else if(name == REQ_DELETE_SERVICE_STR)
        id = REQ_DELETE_SERVICE;
    else if(name == REQ_UPLOAD_FILE_ICON_STR)
        id = REQ_UPLOAD_FILE_ICON;
    else if(name == REQ_CHECK_PATH_SERVICE_STR)
        id = REQ_CHECK_PATH_SERVICE;

	else if(name == REQ_GET_PROVIDERS_STR)
		id = REQ_GET_PROVIDERS;
	else if(name == REQ_GET_PROVIDER_REGISTERS_STR)
		id = REQ_GET_PROVIDER_REGISTERS;
	else if(name == REQ_ACCEPT_PROVIDER_REGISTERS_STR)
		id = REQ_ACCEPT_PROVIDER_REGISTERS;
	else if(name == REQ_REJECT_PROVIDER_REGISTERS_STR)
    id = REQ_REJECT_PROVIDER_REGISTERS;
  else if(name == REQ_REJECT_PROVIDERS_STR)
    id = REQ_REJECT_PROVIDERS;
  else if(name == REQ_REJECT_PROVIDERS_STR)
    id = REQ_REJECT_PROVIDERS;

    else if(name == REQ_UPLOAD_FILE_STR)
        id = REQ_UPLOAD_FILE;
	else if(name == REQ_FIND_IMAGES_STR)
		id = REQ_FIND_IMAGES;
	else if(name == REQ_CREATE_IMAGE_STR)
		id = REQ_CREATE_IMAGE;
	else if(name == REQ_UPDATE_IMAGE_STR)
        id = REQ_UPDATE_IMAGE;
    else if(name == REQ_DELETE_IMAGE_STR)
        id = REQ_DELETE_IMAGE;
  else if(name == REQ_CHECK_IMAGE_NAME_USABLE_STR)
    id = REQ_CHECK_IMAGE_NAME_USABLE;
	else if(name == REQ_REGISTER_PROVIDER_STR)
    id = REQ_REGISTER_PROVIDER;
  else if(name == REQ_REJECT_PROVIDER_STR)
    id = REQ_REJECT_PROVIDER;
  else if(name == REQ_UPDATE_PROVIDER_USABLE_TIME_STR)
      id = REQ_UPDATE_PROVIDER_USABLE_TIME;
  else if(name == REQ_GET_PROVIDER_TIME_RESOURCE_STR)
    id = REQ_GET_PROVIDER_TIME_RESOURCE;

    //cout<< "End GetID():" << id << endl;
	return id;
}

void ReqUtils::GenRequestId(std::string & pre, std::string & id)
{
    srand (time(NULL));

    string appendStr = pre;

    FILE *urandom;
    unsigned int seed;


    for(int i = 0; i < 10; i++)
    {
        urandom = fopen ("/dev/urandom", "r");

        if (urandom == NULL) {
            fprintf (stderr, "Cannot open /dev/urandom!\n");
        }
        else
        {
            fread (&seed, sizeof (seed), 1, urandom);
            fclose (urandom);
        }

        srand (seed);

        int re = rand() % 10;

        stringstream ssret;

        ssret.str("");

        ssret << re;

        appendStr += ssret.str();
    }
    id = appendStr;

    //    id = "";

//    time_t rawtime;

//    struct tm * timeinfo;

//    char buffer [80];

//    time ( &rawtime );

//    timeinfo = localtime ( &rawtime );

//    strftime (buffer,80,"%Y%m%d%H%M%S",timeinfo);

//    cout << "RequestId:" << buffer << endl;

//    id = string(buffer);
}

/*
 * Check request permission by user group
 */
bool ReqUtils::CheckPermissionRequest(const RequestType& req, const TypeUser& usr){
	bool isAllow = false;
	switch(usr){
	case ADMIN_GROUP:
		std::cout << "ADMIN_GROUP" << std::endl;
        switch (req){
        case REQ_CONNECT:
        case REQ_LOGOUT:
		case REQ_GET_PROVIDERS:
		case REQ_GET_PROVIDER_REGISTERS:
		case REQ_ACCEPT_PROVIDER_REGISTERS:
		case REQ_REJECT_PROVIDER_REGISTERS:
    case REQ_REJECT_PROVIDERS:
          case REQ_UPDATE_PROVIDER_USABLE_TIME:

			isAllow = true;
			break;
		default:
			break;
		}
		break;
	case PROVIDER_GROUP:
		std::cout << "PROVIDER_GROUP" << std::endl;
		isAllow = true;
		switch (req){
		case REQ_UNKNOW:
		case REQ_GET_PROVIDERS:
		case REQ_GET_PROVIDER_REGISTERS:
		case REQ_ACCEPT_PROVIDER_REGISTERS:
		case REQ_REJECT_PROVIDER_REGISTERS:
		case REQ_REGISTER_PROVIDER:
    case REQ_REJECT_PROVIDERS:
			isAllow = false;
			break;
		default:
			break;
		}
		break;
	case WAIT_PROVIDER_GROUP:
	case USER_GROUP:

		std::cout << "USER_GROUP | WAIT_PROVIDER_GROUP" << std::endl;
		switch (req){
		case REQ_CONNECT:
		case REQ_LOGOUT:
			/// Service
		case REQ_FIND_SERVICES:
		case REQ_GET_ALL_SERVICES:
		case REQ_REGISTER_SERVICE:
        case REQ_GET_SERVICES_REGISTER:
		case REQ_USER_REMOVE_SERVICE:
		case REQ_CANCEL_SERVICE_REGISTER:
		case REQ_GET_SERVICE:
		case REQ_GET_PARAM_OF_SERVICE:
        case REQ_UPLOAD_FILE:

			/// Job
		case REQ_SUBMIT_JOB:
		case REQ_RESUBMIT_JOB:
		case REQ_QUERY_STATUS:
		case REQ_GET_OUTPUT_FILE:
		case REQ_JOB_LIST:
		case REQ_DELETE_JOB:
		case REQ_RESTART_JOB:
		case REQ_GET_INFORMATION_JOBS:
		case REQ_GET_INFORMATION_JOB:
		case REQ_CANCEL_JOB:
		case REQ_GET_ENTRIES_JOB:
		case REQ_ID:
		case REQ_CHECK_NAME_USABLE:
    case REQ_REGISTER_PROVIDER:
			isAllow = true;
			break;
		default:
			break;
		}
		break;
	default:
		switch (req){
		case REQ_CONNECT:
		case REQ_LOGOUT:
		case REQ_REGISTER_PROVIDER:
			isAllow = true;
			break;
		default:
			break;
		}
	}
	return isAllow;
}
