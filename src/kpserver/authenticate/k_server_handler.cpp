#include "../../include/libjson/libjson.h"
#include "../stdafx.h"

using namespace websocketkserver;
extern std::string ICON_FOLDER_PATH;
extern std::string APPMARKET_DOMAIN;
extern std::map<std::string, ImageDocker> SampleImages;

template <typename request_endpoint_type>
void request<request_endpoint_type>::process()
{
	std::string threadId = boost::lexical_cast<std::string>(boost::this_thread::get_id());
	std::cout << "THREADID = " << threadId << std::endl;

    if (msg->get_opcode() == websocketpp::frame::opcode::BINARY) {
        cout << "Message client opcode binary: " << msg << endl;
        processBinaryData();
        return;
    }
    if (msg->get_opcode() != websocketpp::frame::opcode::TEXT) {
        return;
    }
    std::string json = msg->get_payload();
    std::cout<<"Message clien content:"<<std::endl<<json<<std::endl;
    JSONNode n;
    try
    {
        n = libjson::parse(json);
    }
    catch(const std::invalid_argument& e)
    {
        std::cerr << "Invalid argument: " << e.what() << '\n';
        return;
    }

    std::string jc = n.write_formatted();

    std::cout<<"JSONNode content:"<<std::endl<<jc<<std::endl;

    JSONNode::const_iterator i = n.begin();
    string requestId = "";
    if(i != n.end() && i -> name() == TAG_REQUEST_ID_STR && i -> type() != JSON_NODE && i -> type() != JSON_ARRAY)
    {
        requestId = i -> as_string();
    }
    else
    {
        cout << "Error: Invalid message: Miss requestId tag.\n";
        return;
    }

    i++;
    if (i == n.end()){
        cout << "Error: Invalid message: Miss msg tag.\n";
        return;
    }

    std::string node_name = i -> name();
    std::string node_value = i -> as_string();
    //cout << "Bp: node_name: " << node_name << ", node_value: " << node_value << endl;
    if (!(node_name == TAG_MSG_STR && node_value != ""))
    {
//        if(node_name == TAG_ID_STR)
//        {
        if (strlen((msg -> get_payload()).c_str()) > 0) {
            std::cout << "message from client " << con << " have data." << std::endl;
        }
        MessageHeader h(requestId,REQ_UNKNOW);
        processId(h, i, n);
//        }
        cout << "request tag is invalid";
        return;
    } else {
        std::cout << "message from client " << con << ": " << msg -> get_payload() << std::endl;
    }

    RequestType type = ReqUtils::GetID(node_value);
    MessageHeader header(requestId, type);
/*
    if( type != REQ_GET_RUN_METHODS_FILE
            && type != REQ_REGISTER_SERVICE
            && type != REQ_CANCEL_SERVICE_REGISTER
            && type != REQ_CONNECT
      )
    {
        if( ! authorizeUserService(header, i, n))
        {
            cout << "process authentize fail." << endl;
            return;
        } else {
            cout << "process authentize success" << endl;
        }
    }
*/

	if(! ReqUtils::CheckPermissionRequest(type, cs -> getUserType())){
        sendResult(header, (int)PERMISSION_DENIED);
		return;
	}

    switch(type)
    {

    case REQ_CONNECT:
        processConnectRequest(header, i, n);
        break;

    case REQ_LOGOUT:
        processLogoutRequest(header, i, n);
        break;

    case REQ_FIND_SERVICES:
        processFindServices(header, i, n);
        break;

    case REQ_GET_SERVICE:
        processGetService(header, i, n);
        break;
/*
    case REQ_GET_RUN_METHODS_FILE:
        processGetRunMethodsFile(header, i, n);
        break;
*/
    case REQ_SUBMIT_JOB:
        processSubmitJob(header, i, n);
        break;

    case REQ_QUERY_STATUS:
        processQueryStatus(header, i, n);
        break;

    case REQ_GET_OUTPUT_FILE:
        processGetOutputFile(header, i, n);
        break;

    case REQ_JOB_LIST:
        processJobList(header, i, n);
        break;

    case REQ_DELETE_JOB:
        processDeleteJob(header, i, n);
        break;

    //case REQ_RESTART_JOB:
    //    processRestartJob(header, i, n);
    //    break;

    case REQ_CANCEL_JOB:
        processCancelJob(header, i, n);
        break;

    case REQ_GET_ALL_SERVICES:
        processGetAllServices(header, i, n);
        break;

    case REQ_GET_SERVICES_PROVIDER:
        processGetServicesOfProvider(header, i, n);
        break;

    case REQ_REGISTER_SERVICE:
        processUserRegisterService(header, i, n);
        break;

    case REQ_GET_USERS_REGISTER_SERVICE:
        processRegisterUsersOfService(header, i, n);
        break;

    case REQ_GET_USERS_SERVICE:
        processGetUsersOfService(header, i, n);
        break;

    case REQ_GET_SERVICES_USER_REGISTER:
        processGetServicesHaveCandidates(header, i, n);
        break;

    case REQ_ADD_USER_SERVICE:
        processAddUserToService(header, i, n);
        break;

    case REQ_REMOVE_USER_SERVICE:
        processRemoveUserFromService(header, i, n);
        break;

    case REQ_REMOVE_CANDIDATE_SERVICE:
        processRemoveCandidateFromService(header, i, n);
        break;

    case REQ_GET_SERVICES_REGISTER:
        processGetRegisterServicesOfUser(header, i, n);
        break;

    case REQ_CANCEL_SERVICE_REGISTER:
        processCancelRegisterServiceByUser(header, i, n);
        break;

    case REQ_GET_ENTRIES_JOB:
        processGetEntriesOfJob(header, i, n);
        break;

    case REQ_GET_INFORMATION_JOBS:
        processGetInfomationJobsOfUserByService(header, i, n);
        break;

    case REQ_GET_INFORMATION_JOB:
        processGetDetailInfomationJob(header, i, n);
        break;

    case REQ_USER_REMOVE_SERVICE:
        processUserRemoveService(header, i, n);
        break;

    case REQ_RESUBMIT_JOB:
        processResubmitJob(header, i, n);
        break;

    case REQ_CREATE_SERVICE:
        processCreateService(header, i, n);
        break;

    case REQ_GET_PARAM_OF_SERVICE:
        processGetParametersOfService(header, i, n);
        break;

    case REQ_UPDATE_SERVICE:
        processUpdateService(header, i, n);
        break;

    case REQ_DELETE_SERVICE:
        processDeleteService(header, i, n);
        break;

    case REQ_UPLOAD_FILE_ICON:
        processUploadIconStep(header, i, n);
        break;

    case REQ_CHECK_PATH_SERVICE:
        processCheckPathOfService(header, i, n);
        break;

	case REQ_CHECK_SERVICE_NAME_USABLE:
		processCheckServiceNameUsable(header, i, n);
		break;

	case REQ_GET_PROVIDERS:
		processGetProviders(header, i, n);
		break;

	case REQ_GET_PROVIDER_REGISTERS:
		processGetProviderRegisters(header, i, n);
		break;

	case REQ_ACCEPT_PROVIDER_REGISTERS:
		processAcceptProviderRegisters(header, i, n);
		break;

    case REQ_REJECT_PROVIDER_REGISTERS:
        processRejectProviderRegisters(header, i, n);
		break;

	case REQ_REJECT_PROVIDERS:
		processRejectProviders(header, i, n);
		break;

	case REQ_FIND_IMAGES:
		processFindImages(header, i, n);
		break;

	case REQ_CREATE_IMAGE:
		processCreateImage(header, i, n);
		break;

	case REQ_UPDATE_IMAGE:
		processUpdateImage(header, i, n);
		break;

	case REQ_DELETE_IMAGE:
		processDeleteImage(header, i, n);
		break;

	case REQ_CHECK_IMAGE_NAME_USABLE:
		processCheckImageNameUsable(header, i, n);
		break;

	case REQ_REGISTER_PROVIDER:
		processRegisterProvider(header, i, n);
		break;

	case REQ_REJECT_PROVIDER:
		processRejectProvider(header, i, n);
		break;

  case REQ_GET_PROVIDER_TIME_RESOURCE:
    processGetProviderTimeResource(header, i, n);
    break;

  case REQ_UPDATE_PROVIDER_USABLE_TIME:
    processUpdateProviderUsableTime(header, i, n);
    break;

    default:
        std::cout<<"Unknow request:"<<(int)type<<std::endl;
        break;
    }
}

/*
 * Convert to standard string for message
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::convertStandardStr(std::string &string_src) {
    std::string listStr[8] = {"\\","\"","/","\b", "\f", "\n", "\r", "\t"};
    std::string listReplaceStr[8] = {"\\\\","\\\"","\\/","\\b", "\\f", "\\n", "\\r", "\\t"};
    for (int i = 0; i < 8; i++) {
        int pos = 0;
        while (pos != -1) {
            pos = string_src.find(listStr[i],pos);
            if (pos != -1) {
                string_src.replace(pos,listStr[i].length(),listReplaceStr[i]);
                pos += listReplaceStr[i].length();
            }
        }
    }
}

/*
 * Process send file from server to client
 */
template <typename request_endpoint_type>
bool request<request_endpoint_type>::sendFile(std::string& fileName,/* std::string& requestId,*/ MessageHeader& header)
{
    cout << "Bp: sendFile():" << fileName << endl;
    bool result = false;
    long size = 0;

    ifstream file (fileName.c_str(), ios::in | ios::binary);


    /// Create path string to create message
    std::string localPathStr = FileUtils::GetFileName(fileName);

    if (file.is_open() && file.good() && FileUtils::IsFile(fileName))
    {
		/*
        JSONNode nf(JSON_NODE);
        nf.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
        nf.push_back(JSONNode(TAG_PATH_STR, localPathStr));

        std::string msgf = nf.write_formatted();
        std::cout << "Msgf: \n" << msgf << std::endl;
        con -> send(msgf);
		sleep(1);*/

        //std::ifstream file (fileName.c_str(), ios::in | ios::binary);
        //if ( file.is_open() && file.good() ){
            /// Read file size
            file.seekg(0,ifstream::end);
            size = file.tellg();
            file.seekg(0);
            std::cout << "Bp: File size: " << size << std::endl;

			if( size > 1000000000 ) /// ~ 1000 M
            {
                std::cout << "Error: file size requied less than 100 M " << std::endl;
                return result;
            }

            /// Read data
            char *data;
            data = new char[size];
            file.read(data, size);

            /// Send data
            std::string str(reinterpret_cast< char const* >(data), size);
            con -> send(str, websocketpp::frame::opcode::BINARY);

            /// Release
            delete[] data;
            data = NULL;

            file.close();

            /// Success
            result = true;
        }

    else
    {
        cout << "Can not open file to read: " << fileName << endl;
    }
    return result;
}

/*
 * Send error message
 */
//template <typename request_endpoint_type>
//void request<request_endpoint_type>::sendError(std::string requestId, int errorCode){
//    JSONNode n(JSON_NODE);
//    n.push_back(JSONNode(TAG_REQUEST_ID_STR, requestId));
//    n.push_back(JSONNode(TAG_STATUS_STR, errorCode));

//    std::string msg = n.write_formatted();
//	std::cout<<"JSONNode content:"<<std::endl<<msg<<std::endl;
//    con -> send(msg);
//}

//template <typename request_endpoint_type>
//void request<request_endpoint_type>::sendError(MessageHeader& header, int errorCode){
//	sendError(header.getRequestID(), errorCode);
//}

/*
 * Send result response
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::sendResult(MessageHeader& header, int resultCode){
	JSONNode n(JSON_NODE);
	n.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
	n.push_back(JSONNode(TAG_RES_CODE_STR, resultCode));
	std::string msg = n.write_formatted();
	std::cout<<"JSONNode content:"<<std::endl<<msg<<std::endl;
	con -> send(msg);
}

/*
 * Check job resource
 */
template <typename request_endpoint_type>
bool request<request_endpoint_type>::checkResourceJob(Job new_job, Job old_job) {
    bool isHasService = false, isHasJobName = false;
    JSONNode n;
    try
    {
		n = libjson::parse(old_job.getStrJsonOfJob());
    }
    catch(const std::invalid_argument& e)
    {
        std::cerr << "Invalid argument: " << e.what() << '\n';
        return true;
    }
    cout << "checkResourceJob jsonString: " << n.write_formatted() << endl;
    JSONNode::const_iterator i = n.begin();
    //++i;
    //++i;
    i = n.find(TAG_SERVICE_STR);
    if(i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        isHasService = true;
    //++i;
    //if(i != n.end() && i -> name() == TAG_RUN_METHOD_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
    //    isHasRunMethod = true;
    //++i;
    i = n.find(TAG_JOB_NAME_STR);
    if(i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        isHasJobName = true;
    //++i;
    i = n.find(TAG_PARAMS_STR);
    if(i != n.end() && i -> type() == JSON_NODE)
    {
        JSONNode::const_iterator ip = (*i).begin();
        while(ip != (*i).end())
        {
            if(ip != (*i).end() && ip -> type() == JSON_NODE)
            {
                string path = "", type = "";
                bool isHasPath = false, isLocalType = false;
                JSONNode::const_iterator ipc = (*ip).find(TAG_PATH_STR);
                if(ipc != (*ip).end() && ipc -> type() != JSON_ARRAY && ipc -> type() != JSON_NODE)
                {
                    path = ipc -> as_string();
                    isHasPath = true;
                }
                ipc = (*ip).find(TAG_TYPE_STR);
                if(ipc != (*ip).end() && ipc -> type() != JSON_ARRAY && ipc -> type() != JSON_NODE)
                {
                    type = ipc -> as_string();
                    isLocalType = (strcmp(type.data(), LOCAL_STR) == 0);
                }
                if(isHasPath && isLocalType)
                {
					string fullPath_new = new_job.getPathJob();
					string fullPath_old = old_job.getPathJob();
					/// create path;
					if(path.size() > 0 && path.data()[0] == '/')
					{
						fullPath_new += path;
						fullPath_old += path;
					}
					else
					{
						fullPath_new = fullPath_new + string("/") + path;
						fullPath_old = fullPath_old + string("/") + path;
					}
					cout << "fullPath_new:" << fullPath_new << endl;

					ResponseCode ret = FILE_ACTION_SUCCESS;
					DataManager data_manager(PATH_OF_DATABASE);
					if ( data_manager.connectDB() == DATA_SUCCESS) {
						User user(cs->getUserId(), USER_GROUP);
						ret = data_manager.getUser(user);
						if (ret != DATA_SUCCESS) {
							return false;
						}
						std::string containerId = user.getContainerId();
						std::string host;
						ret = NetworkUtil::getIpAddressOfContainer(containerId, host);
						if (ret != REQUEST_SUCCESS) {
							return false;
						}
						ret = NetworkUtil::ProcessDownloadFile(host, Container_Com_Port, fullPath_old, fullPath_new);
						if (ret != FILE_ACTION_SUCCESS) {
							return false;
						}
					}
                }
            }
            ip ++;
        }
    } else {
        cout << "processCheckResourceJob: don't have param." << endl;
    }
    //return requestId;
    return true;
}

/*
 * Insert new service to DB & send response to client
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::insertService(MessageHeader& header,Service &service) {
    ResponseCode ret;
    ret = cs -> createService(service);
    JSONNode resNode;
    resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
	resNode.push_back(JSONNode(TAG_SERVICE_STR, service.getServiceID()));
    resNode.push_back(JSONNode(TAG_RES_CODE_STR, ret));

    /// Send response to client
    std::string response = resNode.write_formatted();
    std::cout<<"Response content:"<< std::endl << response << std::endl;

    con -> send(response);
}

/*
 * Update service to DB & send response to client
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::editService(MessageHeader& header,Service &service) {
        ResponseCode ret;
        ret = cs -> updateService(service);
        JSONNode resNode;
        resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
		resNode.push_back(JSONNode(TAG_SERVICE_STR, service.getServiceID()));
        resNode.push_back(JSONNode(TAG_RES_CODE_STR, ret));

        /// Send response to client
        std::string response = resNode.write_formatted();
        std::cout<<"Response content:"<< std::endl << response << std::endl;

        con -> send(response);
    }

/*
 * Send request to client to get file
 * { "requestId":"xxxxxx", "uploadFile":"filename"}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::requestUploadFile(MessageHeader& header, std::string fileName){
	JSONNode resNode;
	resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
	resNode.push_back(JSONNode(TAG_UPLOAD_FILE_STR, fileName));

	/// Send response to client
	std::string response = resNode.write_formatted();
	std::cout<<"Response content:"<< std::endl << response << std::endl;

	con -> send(response);
}


/*
 * Send request to client to get file
 * { "requestId":"xxxxxx", "uploadFile":"filename"}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::requestUploadFileParam(MessageHeader& header, param_job param){
	std::cout << "Upload file param\n";
	if(param.getType() != ARG_LOCAL){
		cout << "File type not ARG_LOCAL\n";
		return;
	}
	JSONNode resNode;
	resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
	resNode.push_back(JSONNode(TAG_UPLOAD_FILE_STR, param.getValue()));
	resNode.push_back(JSONNode(TAG_PARAMETER_STR, param.getParamId()));

	/// Send response to client
	std::string response = resNode.write_formatted();
	std::cout<<"Response content:"<< std::endl << response << std::endl;

	con -> send(response);
}

/*
 * Send create image result to client
 * { "requestId":"xxxxxx", "uploadFile":"filename"}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::responseCreateImage(MessageHeader &header, std::string &imageId, ResponseCode &ret){
    JSONNode resNode;
    resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    resNode.push_back(JSONNode(TAG_IMAGE_STR, imageId));
    resNode.push_back(JSONNode(TAG_RES_CODE_STR, ret));

    /// Send response to client
    std::string response = resNode.write_formatted();
    std::cout<<"Response content:"<< std::endl << response << std::endl;

    con -> send(response);
}

/*
 * Send create image result to client
 * { "requestId":"xxxxxx", "uploadFile":"filename"}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::responseCreateService(MessageHeader &header, std::string serviceId, ResponseCode &ret){
	JSONNode resNode;
	resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
	resNode.push_back(JSONNode(TAG_SERVICE_STR, serviceId));
	resNode.push_back(JSONNode(TAG_RES_CODE_STR, ret));

	/// Send response to client
	std::string response = resNode.write_formatted();
	std::cout<<"Response content:"<< std::endl << response << std::endl;

	con -> send(response);
}

/*
 * Send create image result to client
 * { "requestId":"xxxxxx", "uploadFile":"filename"}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::responseSubmitJob(MessageHeader &header, std::string jobId, ResponseCode &ret){
	JSONNode resNode;
	resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
	resNode.push_back(JSONNode(TAG_JOB_STR, jobId));
	resNode.push_back(JSONNode(TAG_RES_CODE_STR, ret));

	/// Send response to client
	std::string response = resNode.write_formatted();
	std::cout<<"Response content:"<< std::endl << response << std::endl;
	con -> send(response);
}

/*
 * Process permission user for request accept
 */
template <typename request_endpoint_type>
bool request<request_endpoint_type>::authorizeUserService(MessageHeader& header, JSONNode::const_iterator i, JSONNode& n)
{
    bool isAuthorize = true;
    //if (m_connections.find( con ) != m_connections.end())
    //{
        std::string serviceId = "";
        i = n.find(TAG_SERVICE_STR);
        if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
            serviceId = i -> as_string();
        } else {
            serviceId = "";
        }

        //check user Id
        //ClientSession *cs = &m_connections[con];
        if (cs->getUserId().length() == 0) {            std::cout << "Id is empty\n";
            JSONNode resNode;

            resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));

            resNode.push_back(JSONNode(TAG_STATUS_STR, (int)ERR_NO_AUTHENTICATION));

            std::string resReport = resNode.write_formatted();

            std::cout<<"JSONNode response error:"<<std::endl<<resReport<<std::endl;

            con -> send(resReport);
            return false;
        }

    //} else {
    //    isAuthorize = false;
    //}
    if (!isAuthorize) {
        JSONNode resNode;

        resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));

        resNode.push_back(JSONNode(TAG_STATUS_STR, (int)ERR_INVALID_ACCESS));

        std::string resReport = resNode.write_formatted();

        std::cout<<"JSONNode response error:"<<std::endl<<resReport<<std::endl;

        con -> send(resReport);
    }
    return isAuthorize;
}

/*
 * Process login request
 * Req: {"requestId":"21","msg":"connect","email":"userEmail","token":"gmailToken}
 * Res: {"requestId" : "21", "valid" : 1}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processConnectRequest(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
    std::cout << "CONNECT" << std::endl;

    std::string response;
    std::string key, valStr;
    std::string userId = "", loginId = "", token = "";
    bool isNeedAuth = false;
    bool isValid = false;
    JSONNode resNode(JSON_NODE);

    ++i;
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
    {
        if( i -> name() == TAG_EMAIL_STR ){
            key = i -> name();
            valStr = i -> as_string();
            userId = valStr;
            isNeedAuth = true;
        }
        else if( i -> name() == TAG_LOGIN_ID_STR )
        {
            key = i -> name();
            valStr = i -> as_string();
            loginId = valStr;
        }
        else
        {
            std::cout << "Error: loginID || email is emty or missing.\n";
            return;
        }
    }

    if(isNeedAuth)
    {
        ++i;
        if (i != n.end()  && i -> name() == TAG_TOKEN_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        {
            key = i -> name();
            valStr = i -> as_string();
            token = valStr;
        }
        else
        {
            std::cout << "Error: Token tag is emty or missing.\n";
            return;
        }

        /// Authentication
        if(GmailUtils::Oauth20(userId, token))
        {
            GmailUtils::GenLoginID(loginId);
            std::cout << "LoginID: " << loginId << ", email: " << userId << std::endl;
			std::string userType = string(TYPE_USER_STR);
			User usr(userId);
			ResponseCode ret = cs -> getInfoUser(usr);

			if (ret == DATA_SUCCESS && (int)(usr.getType() & (char)ADMIN_GROUP)) {
				userType = string(TYPE_ADMIN_STR);
			} else if (ret == DATA_SUCCESS && (int)(usr.getType() & (char)PROVIDER_GROUP)) {
				userType = string(TYPE_PROVIDER_STR);
			}
            UserInfoObject userInfo(userId, userType);
            //m_users.insert(std::pair<std::string, UserInfoObject>(loginId, userInfo));
            lm -> addLoginInfo(loginId, userInfo);

            /// Set user info to connection
            std::cout << "setUserID " << userId.c_str() << std::endl;
            cs -> setUserId(userId);
            std::cout << "setLogined TRUE" << std::endl;
            cs -> setLogined(true);

            //// Send response report num of packet data
            resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
            resNode.push_back(JSONNode(TAG_VALID_STR, VALID));
            resNode.push_back(JSONNode(TAG_USER_TYPE_STR, userType));
            resNode.push_back(JSONNode(TAG_LOGIN_ID_STR, loginId));
            resNode.push_back(JSONNode(TAG_PUBLICKEY_STR, usr.getPublicKey()));
            isValid = true;
        }
        else
        {
            std::cout << "Error: email && token info not match.\n";
        }
    }
    else if(loginId != "")
    {
        UserInfoObject uio = lm -> getUserInfo(loginId);
        if(uio.getEmail() != "")
        {
			std::string userType = string(TYPE_USER_STR);
			User usr(uio.getEmail());
			std::cout << "LoginID: " << loginId << ", email: " << uio.getEmail() << std::endl;

			ResponseCode ret = cs -> getInfoUser(usr);
			if(ret == DATA_SUCCESS){
			std::cout << (int)usr.getType() << "\n (char)usr.getType() & (char)ADMIN_GROUP =="
					  << (int)((char)usr.getType() & (char)ADMIN_GROUP)
					  << ", (char)ADMIN_GROUP = " << ADMIN_GROUP << std::endl;
			} else {
				std::cout << "ERROR: getInfoUser() ret = " << ret << std::endl;
			}
			if (ret == DATA_SUCCESS && (int)(usr.getType() & (char)ADMIN_GROUP)) {
				userType = string(TYPE_ADMIN_STR);
			} else if (ret == DATA_SUCCESS && (int)(usr.getType() & (char)PROVIDER_GROUP)) {
				userType = string(TYPE_PROVIDER_STR);
			}
            ///	Send response report num of packet data
            resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
            resNode.push_back(JSONNode(TAG_VALID_STR, VALID));
            resNode.push_back(JSONNode(TAG_USER_TYPE_STR, userType));
            resNode.push_back(JSONNode(TAG_PUBLICKEY_STR, usr.getPublicKey()));       

            /// Set user info to connection
      std::cout << "before set user info to connection: uio -> getEmail() "<<
                    uio.getEmail() << std::endl;
      if(cs != NULL)
        try{
            cs -> setUserId( uio.getEmail() );
            cs -> setLogined(true);

            std::cout << "after set user info to connection\n";
            isValid = true;
      } catch (int& ex){
        std::cout << "[ERR] Exception caught durring set user info " << ex << std::endl;
      }
        }
    }
    if( ! isValid )
    {
        resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
        resNode.push_back(JSONNode(TAG_VALID_STR, INVALID));
    }

    response = resNode.write_formatted();
     std::cout <<" before send login response\n";
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
    return;
}

/*
 * Process logout request
 * Req: {"requestId":"21","msg":"logout","loginId":"201501130618280638729148"}
 * Res: {"requestId" : "21", "valid" : 1}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processLogoutRequest(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
    std::cout << "LOGOUT" << std::endl;
    std::string loginId = "";
    cs -> setLogined(false);
    ++ i;
    if( i != n.end() && i -> name() == TAG_LOGIN_ID_STR)
    {
        loginId = i -> as_string();
    }
    else
    {
        cout << "Error: loginId tag is missing or empty.";
        return;
    }
    /// Response to client: { “requestID”: requestID, “valid”: 1 }
    JSONNode resNode(JSON_NODE);
    resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));

    //if(m_users.erase(loginId))
    lm -> removeUserInfo(loginId);
    resNode.push_back(JSONNode(TAG_VALID_STR, 1));
    string response = resNode.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Process findServices request
 * Req: {"requestId":"21","msg":"findServices"}
 * Res: {"requestId" : "21", services : items [{"service":{"serviceId" : "001",..}},{},...]}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processFindServices(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
    std::string response;
    std::cout<<"FIND_SERVIES"<<std::endl;

    ++i;
    std::string userEmail = cs -> getUserId();
	std::vector<Service > services;
    //ResponseCode ret =
    cs -> getServicesOfUser(userEmail, services);
    for (unsigned int i = 0; i < services.size(); i++) {
		if (!services[i].getStatus()) {
            services.erase (services.begin()+i);
            i--;
        }
    }

    JSONNode serArr(JSON_ARRAY);
    serArr.set_name(TAG_ITEMS_STR);

    for (unsigned int i = 0; i < services.size(); i++) {
        serArr.push_back(JsonUtils::toJSONNode(services[i]));
    }

    JSONNode itemsNode(JSON_NODE);
    itemsNode.set_name(TAG_SERVICES_STR);

    itemsNode.push_back(serArr);

    JSONNode resNode(JSON_NODE);
    resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    resNode.push_back(itemsNode);

    response = resNode.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;

    con -> send(response);
}

/*
 * Get all public service for normal user
 * Req: {"requestId":"xxxxxxxx","msg":"getAllServices"}
 * Res: {"requestId" : "xxxxxxxx", ....}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processGetAllServices(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n) {
    std::cout << "GET_ALL_SERVICES request" << std::endl;
    JSONNode rn;
    std::string response;
    //push tag request id
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    //push tag services
    JSONNode servicesNode(JSON_NODE);
    servicesNode.set_name(TAG_SERVICES_STR);
    //add list item
    JSONNode itemArr(JSON_ARRAY);
    itemArr.set_name(TAG_ITEMS_STR);

    //Get list all service of k-portal
	std::vector<Service > services;
    //ResponseCode ret =
    cs -> getAllServices(services);

    if (services.size() == 0) {
        cout << "services empty";
    }
    for (unsigned int i = 0; i < services.size(); i++) {
        itemArr.push_back(JsonUtils::toJSONNode(services[i]));
    }
    servicesNode.push_back(itemArr);
    rn.push_back(servicesNode);

    // Convert to send reply
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Process findServices request
 * Req: {"requestId":"xxxxxxxx","msg":"getService", "serviceId":"serviceID"}
 * Res: {"requestId" : "xxxxxxxx", "name":"ServiceName", ....}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processGetService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
    std::cout<< "GET_SERVICE request" << std::endl;
    ++i;
	Service service;
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
		service.setServiceID(i -> as_string());
    else
        return;

    ResponseCode ret = cs -> getService(service);
    JSONNode rn(JSON_NODE);
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));

    if (ret == DATA_SUCCESS) {
        JSONNode ser_node = JsonUtils::toJSONNode(service);
        rn.push_back((JSONNode)(*ser_node.find(TAG_SERVICE_STR)));
        rn.push_back((JSONNode)(*ser_node.find(TAG_SERVICE_NAME_STR)));
        rn.push_back((JSONNode)(*ser_node.find(TAG_PRODUCER_STR)));
        rn.push_back((JSONNode)(*ser_node.find(TAG_STATUS_STR)));
        rn.push_back((JSONNode)(*ser_node.find(TAG_NUMBER_OF_NODES_STR)));
        rn.push_back((JSONNode)(*ser_node.find(TAG_NUMBER_OF_USER_STR)));
        rn.push_back((JSONNode)(*ser_node.find(TAG_NUMBER_OF_WAIT_USER_STR)));
        rn.push_back((JSONNode)(*ser_node.find(TAG_DESCRIPTION_STR)));
        rn.push_back((JSONNode)(*ser_node.find(TAG_PATH_STR)));
        rn.push_back((JSONNode)(*ser_node.find(TAG_PROVIDER_STR)));
        rn.push_back((JSONNode)(*ser_node.find(TAG_MAX_ESLAPSED_TIME_STR)));
    } else {
        rn.push_back(JSONNode(TAG_RES_CODE_STR, ret));
    }

    std::string response = rn.write_formatted();

    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;

    con -> send(response);
}

/*
 * Process findServices request
 * Req: {"requestId":"xxxxxxxx","msg":"getParameters", "service":"serviceID"}
 * Res: {"requestId" : "xxxxxxxx", "name":"ServiceName", ....}
 * {
    "requestId" : "23",
    "params" : {
        "items" : [
            {
                "id" : "1",
                "service" : "00982200751396238712",
                "name" : "Input file",
                "description" : "",
                "isOutput" : false,
                "isRequired" : true,
                "prefix" : "",
                "type" : "file",
                "defaultValue" : ""
            },
            ...
        ]
    }

 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processGetParametersOfService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n) {
    std::cout << "GET_PARAMETERS request" << std::endl;
    JSONNode rn;
    std::string response;
    //push tag request id
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    //push tag params
    JSONNode paramsNode(JSON_NODE);
    paramsNode.set_name(TAG_PARAMS_STR);
    //add list item
    JSONNode itemArr(JSON_ARRAY);
    itemArr.set_name(TAG_ITEMS_STR);

    ///Get list service of provider
    std::string serviceId;
    i = n.find(TAG_SERVICE_STR);
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
        serviceId = i->as_string();
    } else {
        serviceId = "";
    }
    std::vector<Parameter> listParam;
    //ResponseCode ret =
    cs -> getAllParametersOfService(serviceId,listParam);
    for (std::vector<Parameter >::const_iterator it = listParam.begin(); it != listParam.end(); it++)
    {
        itemArr.push_back(JsonUtils::toJSONNode((Parameter)* it));
    }
    paramsNode.push_back(itemArr);
    rn.push_back(paramsNode);

    /// Convert to send reply
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Get jobs by service
 * Req: {"requestId":"xxxxxxxx","msg":"getJobsByService","service":"serviceID"}
 * Res: {"requestId" : "xxxxxxxx", "name":"ServiceName", ....}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processJobList(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
    std::string response;

	std::list<Job> jobs = cs -> getJobs(n.write_formatted());

    JSONNode jobsArr(JSON_ARRAY);
    jobsArr.set_name(TAG_ITEMS_STR);

	for (std::list<Job>::iterator it = jobs.begin(); it != jobs.end(); it++)
    {
        jobsArr.push_back(JsonUtils::toJSONNode((Job )*it));
    }

    JSONNode itemsNode(JSON_NODE);
    itemsNode.set_name(TAG_JOBS_STR);
    itemsNode.push_back(jobsArr);

    JSONNode resNode(JSON_NODE);
    resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    resNode.push_back(itemsNode);

    response = resNode.write_formatted();

    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;

    con -> send(response);
}

/*
 * Process get Jobs by service
 * Req: {"requestId":"24","msg":"getJobsByService","service":"00982200751396238712"}
 * Res: {"requestId":"24",
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processGetInfomationJobsOfUserByService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n) {
    cout << "GET_JOBS_BY_SERVICE" << endl;
    ++i;
    std::string serviceId = "";
    if (i != n.end() && i -> name() == TAG_SERVICE_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        serviceId = i -> as_string();

    /// push tag request id
    JSONNode rn;
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));

    /// push tag services
    JSONNode jobsNode(JSON_NODE);
    jobsNode.set_name(TAG_JOBS_STR);

    ///add list item
    JSONNode itemArr(JSON_ARRAY);
    itemArr.set_name(TAG_ITEMS_STR);

    /// add user to service
    std::string userId = cs -> getUserId();
	std::vector<Job > listJob;
    //ResponseCode ret =
    cs -> getJobsOfUsersByService(serviceId, userId, listJob);
    for (unsigned int i = 0; i < listJob.size(); i++)
    {
        JSONNode item(JSON_NODE);
		item.push_back(JSONNode(TAG_JOB_STR, listJob[i].getId()));
        cout << "jobName final: " << listJob[i].getJobName() << endl;
        //item.push_back(JSONNode(TAG_JOB_NAME_STR, listJob[i].getJobName()));
        JSONNode jSon_JobName;
        string strJsonJobName = string("{\"") + TAG_JOB_NAME_STR + "\" : \"" + listJob[i].getJobName() + "\"}";
        try {
            jSon_JobName = libjson::parse(strJsonJobName);
        } catch(const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << '\n';
        }
        item.push_back(*(jSon_JobName.begin()));

        item.push_back(JSONNode(TAG_STATUS_STR, listJob[i].getStatus()));
        item.push_back(JSONNode(TAG_TIME_START_STR, listJob[i].getTimeStartJob()));
        item.push_back(JSONNode(TAG_TIME_END_STR, listJob[i].getTimeEndJob()));
        item.push_back(JSONNode(TAG_TIME_DURATION_STR, listJob[i].getDuration()));

        itemArr.push_back(item);
    }
    jobsNode.push_back(itemArr);
    rn.push_back(jobsNode);

    /// Convert to send reply
    std::string response;
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Process get detail infomation job
 * Req: {"requestId":"xxxxxxxx","msg":"getJobInfos","job":"jobID"}
 * Res: {"requestId" : "xxxxxxxx", "job":"jobID", ....}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processGetDetailInfomationJob(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n) {
    cout << "GET_JOB_INFOS" << endl;
    ++i;
    std::string serviceId = "";
    std::string jobId = "";
    if (i != n.end() && i -> name() == TAG_SERVICE_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        serviceId = i -> as_string();
    ++i;
    if (i != n.end() && i -> name() == TAG_JOB_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        jobId = i -> as_string();

    //add user to service
    //std::string userId = cs -> getUserId();
	Job job;
	job.setId(jobId);
    //ResponseCode ret =
	cs -> getInfoOfJob(job);
    cout << "jobName final: " << job.getJobName() << endl;

    //push tag request id
    JSONNode rn;
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
	rn.push_back(JSONNode(TAG_JOB_STR, job.getId()));

    //view job name
    const char* text = job.getJobName().c_str();
    for(size_t i = 0; i < strlen(text); ++i)
    {
        std::cout << " " << static_cast<unsigned int>(static_cast<unsigned char>(text[i])) ;
    }
    cout << endl;
    //end
    //rn.push_back(JSONNode(TAG_JOB_NAME_STR, job.getJobName()));

    rn.push_back(JSONNode(TAG_STATUS_STR, job.getStatus()));
    rn.push_back(JSONNode(TAG_TIME_START_STR, job.getTimeStartJob()));
    rn.push_back(JSONNode(TAG_TIME_END_STR, job.getTimeEndJob()));
    rn.push_back(JSONNode(TAG_TIME_DURATION_STR, job.getDuration()));
    //EH0011
//    rn.push_back(JSONNode(TAG_NUMBER_OF_NODES_STR, job.getNumberOfNode()));
//    rn.push_back(JSONNode(TAG_MAX_ESLAPSED_TIME_STR, job.getMaxElapsedTime()));
    //EH0011 end
    //rn.push_back(JSONNode(TAG_RUN_METHOD_STR, job.getRunmethodId()));
    JSONNode jSon_JobName;
    string strJson = string("{\"") + TAG_JOB_NAME_STR + "\" : \"" + job.getJobName() + "\"}";
    try {
        jSon_JobName = libjson::parse(strJson);
    } catch(const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << '\n';
    }
    rn.push_back(*(jSon_JobName.begin()));

    JSONNode paramsNode;
    try
    {
		paramsNode = libjson::parse(job.getStrJsonParam());
        paramsNode.set_name(TAG_PARAMS_STR);
        rn.push_back(paramsNode);
    }
    catch(const std::invalid_argument& e)
    {
        std::cerr << "Invalid argument: " << e.what() << '\n';
        JSONNode itemArr(JSON_ARRAY);
        itemArr.set_name(TAG_PARAMS_STR);
        rn.push_back(itemArr);
    }
    //convert to send reply
    std::string response;
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Process download job output file
 * Req: {"requestId":"xxxxxxxx","msg":"getOutputFile","job":"jobID",...}
 * Res: {"requestId" : "xxxxxxxx", "job":"jobID", ....}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processGetOutputFile(MessageHeader& header, JSONNode::const_iterator& i, JSONNode & n)
{
    std::cout << "GET_OUTPUT_FILE" << std::endl;

	std::string jobId, fileName;
	int fromOffset = 0;

    i = n.find(TAG_JOB_STR);
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
        jobId = i -> as_string();
    } else {
        jobId = "";
    }

	i = n.find(TAG_PATH_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		fileName = i -> as_string();
	} else {
		fileName = "";
	}

	i = n.find(TAG_FROM_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		fromOffset = i -> as_int();
	} else {
		fromOffset = 0;
	}

	Job job(jobId);
	cs -> getInfoOfJob(job);
	std::string pathFile =  job.getPathJob() + PATH_SEPARATOR + fileName;

	/// Get part to send
	ResponseCode ret = FILE_ACTION_SUCCESS;
	DataManager data_manager(PATH_OF_DATABASE);
	if ( data_manager.connectDB() == DATA_SUCCESS) {
		User user(cs->getUserId(), USER_GROUP);
		ret = data_manager.getUser(user);
		data_manager.disConnectDB();
		if (ret != DATA_SUCCESS) {
			std::cout << "ERROR: getUser, ret:" << ret << endl;
			sendResult(header, ret);
			return;
		}

		std::string containerId = user.getContainerId();
		std::string host;
		ret = NetworkUtil::getIpAddressOfContainer(containerId, host);
		if (ret != REQUEST_SUCCESS) {
			std::cout << "ERROR: getIpAddressOfContainer, ret:" << ret << endl;
			sendResult(header, ret);
			return;
		}

		FileNetworkUtils fnu (host, Container_Com_Port, pathFile);
		size_t filesize, readlength;
		size_t size_buf = 1024 * 1024 * 10; // 10M
		std::string buffer;

		ret = fnu.Open();
		if (ret != FILE_ACTION_SUCCESS) {
			std::cout << "ERROR: Open FileNetwork, ret:" << ret << endl;
			sendResult(header, ret);
			return;
		}
		ret = fnu.GetSize(filesize);

		fnu.setOffset(fromOffset);
		ret = fnu.Read(size_buf, readlength, buffer);

		if (ret != FILE_ACTION_SUCCESS) {
			std::cout << "ERROR: read file, ret:" << ret << endl;
			sendResult(header, ret);
			ret = fnu.Close();
			return;
		}
		ret = fnu.Close();
		if (ret != FILE_ACTION_SUCCESS) {
			std::cout << "ERROR: close file, ret:" << ret << endl;
			sendResult(header, ret);
			return;
		}
		int sentn = (fromOffset + readlength);
		int totaln = (int)filesize;

		std::string str = header.getRequestID();
		str += std::string((char*)&sentn, sizeof(int));
		str += std::string((char*)&totaln, sizeof(int));
		str += buffer;

		/// Send data
		con -> send(str, websocketpp::frame::opcode::BINARY);
	}
	/*
    std::string fileName;
    if (jobId.length() > 0 && pathFile.length() > 0) {
        fileName = cs -> getOutputFile(jobId, pathFile);
    } else {
        fileName = "";
    }

    //std::string fileName = "/home/tuannp/test/images.jpg";  // just test

    cout << "FileName:" << fileName << endl;

    Results result = FAIL;
    if (fileName.length() > 0) {
        if(sendFile(fileName, header))
        {
            result = SUCCESS;
        }
    }

	/*	Send response report num of packet data	* /
    JSONNode resNode(JSON_NODE);
    resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    resNode.push_back(JSONNode(TAG_FINISH_STR, (int)result));

    std::string resReport = resNode.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<resReport<<std::endl;
	con -> send(resReport);*/
}

/*
 * Process submit a job
 * Request: { “msg”: “submitJob”, ....}
 * Response: { “job”: jobID, “status”: status }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processSubmitJob(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n){
    std::cout << "SUBMIT_JOB" << std::endl;

    /// Start job
	Job kp_job;

    ResponseCode ret = cs -> InitJob(n,kp_job);
    if (ret == DATA_SUCCESS) {
      ret = cs-> submitJob(kp_job);
      std::cout << "after submit job\n";
      responseSubmitJob(header, kp_job.getId(), ret);
    } else if (ret == ACTION_WAIT_INPUT) {
		cs -> setWorkStatus(ret);
		for( int i = 0; i < kp_job.getParamJsonStr() -> size(); i ++){
			if(kp_job.getParamJsonStr() -> at(i).getType() == ARG_LOCAL){
				requestUploadFileParam(header, kp_job.getParamJsonStr() -> at(i));
				return;
			}
		}
	} else {
		sendResult(header, ret);
	}
}

/*
 * Process submit a job
 * Request: { “msg”: “resubmitJob”, ....}
 * Response: { “job”: jobID, “status”: status }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processResubmitJob(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
  std::cout << "RESUBMIT_JOB" << std::endl;
  ++i;
  std::string jobId = "";
  if (i != n.end() && i -> name() == TAG_JOB_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
    jobId = i -> as_string();

    Job old_job;
    old_job.setId(jobId);
    cs -> getInfoOfJob(old_job);
    //old_job.setPath(string(DEFAULT_JOB_FOLDER) + string("/") + jobId);

    std::cout << "Parse old job info: \n" << old_job.getStrJsonOfJob().c_str();
    Job job;
    JSONNode node;
    ResponseCode ret = DATA_SUCCESS;
    try{
      node = libjson::parse(old_job.getStrJsonOfJob());
    } catch (...){
      ret = DATA_ERROR;
      std::cout << "Parsing error: " << old_job.getStrJsonOfJob().c_str() << std::endl;
    }
    if(ret == DATA_SUCCESS) {
      std::cout << "Begin init a new job from old job\n";
      ret = cs -> InitJob(node,job);
    }
    if(ret == DATA_SUCCESS || ret == ACTION_WAIT_INPUT)
    {
        job.setHeader(header);
		cout << "JobId: " << job.getId() << ", requestId: " << job.getHeader().getRequestID()<< endl;
        //bool isNeedReceive = processSubmitJobCheckReceiveFile(header, i, n);
        bool checkResource = checkResourceJob(job,old_job);
        if(checkResource)
        {
            //string jobid = job.getJobId();
            //processSubmitJob(header, jobid);
            ret = cs -> submitJob(job);
            JSONNode resNode;
            resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
			resNode.push_back(JSONNode(TAG_JOB_STR, job.getId()));
            resNode.push_back(JSONNode(TAG_RES_CODE_STR, ret));

            /// Send response to client
            std::string response = resNode.write_formatted();
            std::cout<<"Response content:"<< std::endl << response << std::endl;

            con -> send(response);
        } else {
            JSONNode resNode;
            resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
			resNode.push_back(JSONNode(TAG_JOB_STR, job.getId()));
            resNode.push_back(JSONNode(TAG_RES_CODE_STR, DATA_ERROR));

            /// Send response to client
            std::string response = resNode.write_formatted();
            std::cout<<"Response content:"<< std::endl << response << std::endl;

            con -> send(response);
        }
    }
    else
    {
        // Start job fail
        cout << "InitJob fail\n";
        //
        JSONNode resNode;
        resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
		resNode.push_back(JSONNode(TAG_JOB_STR, job.getId()));
        resNode.push_back(JSONNode(TAG_RES_CODE_STR, ret));

        /// Send response to client
        std::string response = resNode.write_formatted();
        std::cout<<"Response content:"<< std::endl << response << std::endl;

        con -> send(response);
    }

}

/*
 * Process submit a job
 * Request: { "requestId": "xxxxxxx", "msg": "path”/"finish"/.., ....}
 * Response: {  }
 */

/*
 * Process submit a job
 * Request: { “msg”: “submitJob”, ....}
 * Response: { “job”: jobID, “status”: status }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processId(MessageHeader& header, JSONNode::const_iterator& i, JSONNode & n)
{
//    //JSONNode::const_iterator i = n.begin();
//    //cout << "Bp: processId: 1" << endl;
//    std::string requestId = "", path = "", binData = "";
//    int result = 0;
//    bool /*isHasId = false,*/ isHasPath = false, isHasData = false , isHasFinish;
////    if (i != n.end() && i -> name() == TAG_ID_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
////    {
////        requestId = i -> as_string();
////        isHasId = true;
////    }
////    ++i;
//    if (i != n.end() && i -> name() == TAG_PATH_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
//    {
//        path = i -> as_string();
//        isHasPath = true;
//    }
//    else if(i != n.end() && i -> name() == TAG_DATA_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
//    {
//        binData = i -> as_string();
//        //binData = i -> as_binary();
//        isHasData = true;
//    }
//    else if(i != n.end() && i -> name() == TAG_FINISH_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
//    {
//        result = i -> as_int();
//        isHasFinish = true;
//    }

//    if((!isHasPath) && (!isHasData) && (!isHasFinish))
//        return;
//	std::map<std::string,KP_Job>*jobs = cs -> getRequestJobs();
//    requestId = header.getRequestID();

//    if (jobs -> find(requestId) == jobs -> end())
//    {
//        cout << "Error: RequestId: "<< requestId << " not found. " << endl;
//        return;
//    }
//    if(isHasPath)
//    {
//        string jobpath = (*jobs)[requestId].getPath();
//        string fullPath = jobpath;
//        /// create path;
//        if(path.size() > 0 && path.data()[0] == '/')
//        {
//            fullPath += path;
//        }
//        else
//        {
//            fullPath = fullPath + string("/") + path;
//        }
//        cout << "FullPath:" << fullPath << endl;
//        string dir = fullPath.substr(0,fullPath.find_last_of("/"));
//        cout << "Directory: " << dir << endl;
//        FileUtils::CreateDirectory(dir);
//        ofstream fout2(fullPath.c_str(), ios::trunc | ios::out | ios::binary);

//        if(fout2.is_open() && fout2.good())
//        {
//            /// EH0004
//            cs->enqueueFileReceive(requestId, fullPath);
//            /// EH0004 end
//            fout2.close();
//        }
//        else
//        {
//            cout << "Error: Can't create file: " << fullPath << endl;
//            /// EH0004
//            sendError (requestId, ERR_CREATE_FILE);
//            /// EH0004 end
//        }
//        (*jobs)[requestId].getFiles() -> push_back(fullPath);
//        cout << "Request is a path" << endl;

//    }
//    else if(isHasData)
//    {
//        vector<string>* files = (*jobs)[requestId].getFiles();
//        cout << "requestId received data:" << requestId << endl;
//        if(files -> size() > 0)
//        {
//            string fileName = files -> back();
//            //cout << "Bp: filename:" << fileName << endl;
//            ofstream fout(fileName.c_str(), ios::app | ios::out | ios::binary);
////            cout << "Data write file: size = " << (long)binData.size() << endl ;//<< ", data = " << binData << endl;
//            if(fout.is_open() && fout.good())
//            {
//                fout.write(binData.data(), binData.size());
//                fout.close();
//            }
//            else
//            {
//                cout << "Error: Can't write file: " << fileName << endl;
//            }
//        }
//        else
//        {
//            cout << "Files size is 0 " << endl;
//        }
//        cout << "Request is a data" << endl;
//    }
//    else if(isHasFinish)
//    {
//        if(result == (int)SUCCESS)
//        {
//            map<string,KP_Job>::iterator it = jobs -> find(requestId);
//            if(it != jobs -> end())
//            {
//                //string jobId = (*jobs)[requestId].getJobId();
//                //MessageHeader header = (*jobs)[requestId].getHeader();
//                //cout << "Submit job: id = " << jobId << ", requestId: " << header.getRequestID()<< endl;
//                //processSubmitJob(header, jobId);
//                ResponseCode ret = cs -> submitJob(it->second);
//                JSONNode resNode;
//                resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
//                resNode.push_back(JSONNode(TAG_JOB_STR, (it->second).getJobId()));
//                resNode.push_back(JSONNode(TAG_RES_CODE_STR, ret));

//                /// Send response to client
//                std::string response = resNode.write_formatted();
//                std::cout<<"Response content:"<< std::endl << response << std::endl;

//                con -> send(response);
//                jobs -> erase(it);
//            }
//            else
//                cout << "Error: request id not found on jobs." << endl;
//        }
//        else
//        {
//            /// Tranfer file fail --> need remove file recieved
//            cout << "Tranfer file fail!\n";
//        }
//        cout << "Request is finish" << endl;
//    }
//    else
//    {
//        cout << "Unknow message: " << endl << n.write_formatted();
//    }
}

/*
 * Process query job status
 * Request: { “msg”: “queryStatus”, “job”: jobID }
 * Response: { “job”: jobID, “status”: status }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processQueryStatus(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
    cout << "QUERY_STATUS" << endl;
    ++i;
    std::string jobId = "";
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        jobId = i -> as_string();

    std::string msg = n.write_formatted();
    KP_JobStatus status = cs -> queryStatus(msg);

    JSONNode resNode;
    resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    resNode.push_back(JSONNode(TAG_JOB_STR, jobId));
    resNode.push_back(JSONNode(TAG_STATUS_STR, (int)status));

    std::string response = resNode.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl << response <<std::endl;
    con -> send(response);
}

/*
 * Process get list file of job folder.
 * Request: { “msg”: “queryStatus”, “job”: jobID }
 * Response: { “job”: jobID, “status”: status }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processGetEntriesOfJob(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n) {
    cout << "GET_ENTRIES" << endl;
    ++i;
    std::string jobId = "";
    std::string curDir = "";
    if (i != n.end() && i -> name() == TAG_JOB_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        jobId = i -> as_string();
    ++i;
    if (i != n.end() && i -> name() == TAG_CURDIR_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        curDir = i -> as_string();
    //push tag request id
    JSONNode rn;
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    //push tag services
    JSONNode entriesNode(JSON_NODE);
    entriesNode.set_name(TAG_ENTRIES_STR);
    //add list item
    JSONNode itemArr(JSON_ARRAY);
    itemArr.set_name(TAG_ITEMS_STR);
    //check user
    map<string,bool> listEntry;
    //ResponseCode ret =
    cs -> getListEntryOfJob(jobId,curDir,listEntry);
    for (map<string,bool>::iterator it=listEntry.begin(); it != listEntry.end(); it++)
    {
        JSONNode item(JSON_NODE);
        //
        JSONNode jSon_FileName;
        string strJsonFileName = string("{\"") + TAG_ENTRY_STR + "\" : \"" + curDir +  (curDir == "" ? "" : "/") + (*it).first + "\"}";
        try {
            jSon_FileName = libjson::parse(strJsonFileName);
        } catch(const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << '\n';
        }
        item.push_back(*(jSon_FileName.begin()));
        //
        //item.push_back(JSONNode(TAG_ENTRY_STR, (*it).first));
        if ((*it).second)
            item.push_back(JSONNode(TAG_ISDIR_STR, 1));
        else item.push_back(JSONNode(TAG_ISDIR_STR, 0));
        itemArr.push_back(item);
    }
    entriesNode.push_back(itemArr);
    rn.push_back(entriesNode);
    //convert to send reply
    std::string response;
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Process delete a job
 * Req: {"requestId":"xxxxxxxx","msg":"processDeleteJob","job":"jobID"}
 * Res: {"requestId" : "xxxxxxxx", "job":"jobID"}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processDeleteJob(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
    cout << "DELETE_JOB" << endl;
    ++i;
    std::string jobId = "";
    if (i != n.end() && i -> name() == TAG_JOB_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        jobId = i -> as_string();

	Job job;
	job.setId(jobId);
    ResponseCode ret = cs -> deleteJob(job);

    JSONNode rn;
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
	rn.push_back(JSONNode(TAG_JOB_STR, job.getId()));
    //rn.push_back(JSONNode(TAG_STATUS_STR, (int)job->getStatus()));
    rn.push_back(JSONNode(TAG_RES_CODE_STR, ret));
    std::string response;
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
void processRestartJob(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
    cout << "RESTART_JOB" << endl;
    ++i;
    std::string jobId = "";
    if (i != n.end() && i -> name() == TAG_JOB_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        jobId = i -> as_string();

	ClientSession cs = m_connections[con];
	KP_Job* job = NULL;
	job = cs -> restartJob(jobId, n.write_formatted());
	if(job == NULL)
		job = new KP_Job(jobId);

    JSONNode rn;
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    rn.push_back(JSONNode(TAG_JOB_STR, job->getJobId()));
	rn.push_back(JSONNode(TAG_STATUS_STR, (int)job->getStatus()));
    std::string response;
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
	con -> send(response);
	delete job;
}
*/


/*
 * Process cancel a job
 * Req: {"requestId":"xxxxxxxx","msg":"cancelJob","job":"jobID"}
 * Res: {"requestId" : "xxxxxxxx", "job":"jobID"}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processCancelJob(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
    std::cout << "CANCEL_JOB" << std::endl;
    ++i;
    std::string jobId = "";
    if (i != n.end() && i -> name() == TAG_JOB_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        jobId = i -> as_string();

	Job job;
	job.setId(jobId);
    ResponseCode ret = cs -> cancelJob(job);
    JSONNode rn;
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
	rn.push_back(JSONNode(TAG_JOB_STR, job.getId()));
    rn.push_back(JSONNode(TAG_STATUS_STR, (int)job.getStatus()));
    rn.push_back(JSONNode(TAG_RES_CODE_STR, ret));
    std::string response;
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Process get all service register of user
 * Req: {"requestId":"xxxxxxxx","msg":"getWaitingServices"}
 * Res: {"requestId" : "xxxxxxxx", }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processGetRegisterServicesOfUser(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n) {
    std::cout << "GET_WAITING_SERVICES" << std::endl;
    //push tag request id
    JSONNode rn;
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    //push tag services
    JSONNode servicesNode(JSON_NODE);
    servicesNode.set_name(TAG_SERVICES_STR);
    //add list item
    JSONNode itemArr(JSON_ARRAY);
    itemArr.set_name(TAG_ITEMS_STR);

    //Get list services have candidates
    std::string userId = cs -> getUserId();
	std::vector<Service> listServices;
    //ResponseCode ret =
    cs -> getServicesWaitToUseOfUser(userId, listServices);
    for (unsigned int i = 0; i < listServices.size(); i++)
    {
        //JSONNode item(JSON_NODE);
        //item.push_back(JSONNode(TAG_SERVICE_STR, listServices[i]));
        //itemArr.push_back(item);
        itemArr.push_back(JsonUtils::toJSONNode(listServices[i]));
    }
    servicesNode.push_back(itemArr);
    rn.push_back(servicesNode);

    //convert to send reply
    std::string response;
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Process delete service register
 * Req: {"requestId":"xxxxxxxx","msg":"cancelRegistering"}
 * Res: {"requestId" : "xxxxxxxx", }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processCancelRegisterServiceByUser(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n) {
    cout << "CANCEL_REGISTERING" << endl;
    ++i;
    std::string serviceId = "";
    if (i != n.end() && i -> name() == TAG_SERVICE_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        serviceId = i -> as_string();
    //push tag request id
    ResponseCode ret = DATA_ERROR;
    JSONNode rn;
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    if (serviceId.length() == 0) {
        rn.push_back(JSONNode(TAG_RES_CODE_STR, ret));
    } else {
        //remove service from wait list
        std::string userEmail = cs -> getUserId();
        ret = cs -> removeRegisterServiceOfUser(serviceId,userEmail);
        rn.push_back(JSONNode(TAG_RES_CODE_STR, ret));
    }
    //convert to send reply
    std::string response;
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Get all service of a provider
 * Req: {"requestId":"xxxxxxxx","msg":"getServices"}
 * Res: {"requestId" : "xxxxxxxx", }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processGetServicesOfProvider( MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n) {
    cout << "getServices" << endl;
    JSONNode rn;
    std::string response;
    //push tag request id
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    //push tag services
    JSONNode servicesNode(JSON_NODE);
    servicesNode.set_name(TAG_SERVICES_STR);
    //add list item
    JSONNode itemArr(JSON_ARRAY);
    itemArr.set_name(TAG_ITEMS_STR);

    //Get list service of provider
    std::string providerEmail = cs -> getUserId();
	std::vector<Service > services;
    //ResponseCode ret =
    cs -> getAllServicesOfProvider(providerEmail, services);
    for (unsigned int i = 0; i < services.size(); i++) {
        itemArr.push_back(JsonUtils::toJSONNode(services[i]));
    }
    servicesNode.push_back(itemArr);
    rn.push_back(servicesNode);
    //end push tag services

    //convert to send reply
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Add user to wait register list of service
 * Req: {"requestId":"xxxxxxxx","msg":"registerService","service":"serviceId"}
 * Res: {"requestId" : "xxxxxxxx", }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processUserRegisterService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n) {
    //code add user to wait service here
    cout << "registerService" << endl;
    ++i;
    std::string serviceId = "";
    if (i != n.end() && i -> name() == TAG_SERVICE_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        serviceId = i -> as_string();

    //Add user to candidate list
    cout << "serviceId_first: " << serviceId << endl;
    ResponseCode ret = cs -> registerService(cs -> getUserId(),serviceId);
    //
    JSONNode rn;
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    rn.push_back(JSONNode(TAG_RES_CODE_STR, ret));

    //convert to send reply
    std::string response;
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Uuser remove service in list app
 * Req: {"requestId":"xxxxxxxx","msg":"userRemoveService","service":"serviceId"}
 * Res: {"requestId" : "xxxxxxxx", }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processUserRemoveService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n) {
    cout << "userRemoveService" << endl;
    ++i;
    std::string serviceId = "";
    if (i != n.end() && i -> name() == TAG_SERVICE_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        serviceId = i -> as_string();
    //push tag request id
    ResponseCode ret = DATA_ERROR;
    JSONNode rn;
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    if (serviceId.length() == 0) {
        rn.push_back(JSONNode(TAG_RES_CODE_STR, ret));
    } else {
        //add user to service
        std::string userId = cs -> getUserId();
        ret = cs -> removeRegisterServiceOfUser(serviceId,userId);
        rn.push_back(JSONNode(TAG_RES_CODE_STR, ret));
    }
    //convert to send reply
    std::string response;
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Get list user waitting register a service
 * Req: {"requestId":"xxxxxxxx","msg":"getCandidates","service":"serviceId"}
 * Res: {"requestId" : "xxxxxxxx", }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processRegisterUsersOfService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n) {
    cout << "getCandidates" << endl;
    ++i;
    std::string serviceId = "";
    if (i != n.end() && i -> name() == TAG_SERVICE_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        serviceId = i -> as_string();
    //push tag request id
    JSONNode rn;
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    //push tag candidates
    JSONNode candidatesNode(JSON_NODE);
    candidatesNode.set_name(TAG_CANDIDATES_STR);
    //add list item
    JSONNode itemArr(JSON_ARRAY);
    itemArr.set_name(TAG_ITEMS_STR);
    //Get list candidate user list
    std::string providerEmail = cs -> getUserId();
    //test
    /*
    unsigned pos = providerId.find("@");
    providerId = providerId.substr (0, pos);
    */
    vector<string> listUsers;
    //ResponseCode ret =
    cs -> getAllCandidateUsersOfService(serviceId,providerEmail, listUsers);
    for (unsigned int i = 0; i < listUsers.size(); i++)
    {
        JSONNode item(JSON_NODE);
        item.push_back(JSONNode(TAG_USER_ID_STR, listUsers[i]));
        itemArr.push_back(item);
    }
    candidatesNode.push_back(itemArr);
    rn.push_back(candidatesNode);

    //convert to send reply
    std::string response;
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Get list user of service.
 * Req: {"requestId":"xxxxxxxx","msg":"getCandidates","service":"serviceId"}
 * Res: {"requestId" : "xxxxxxxx", }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processGetUsersOfService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n) {
    cout << "getUsers" << endl;
    ++i;
    std::string serviceId = "";
    if (i != n.end() && i -> name() == TAG_SERVICE_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        serviceId = i -> as_string();
    //push tag request id
    JSONNode rn;
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    //push tag users
    JSONNode usersNode(JSON_NODE);
    usersNode.set_name(TAG_USERS_STR);
    //add list item
    JSONNode itemArr(JSON_ARRAY);
    itemArr.set_name(TAG_ITEMS_STR);
    //Get list user list
    std::string providerEmail = cs -> getUserId();

    cout << "start get users of service" << endl;
    vector<string> listUsers = cs -> getAllUserOfService(serviceId,providerEmail);
    for (unsigned int i = 0; i < listUsers.size(); i++)
    {
        JSONNode item(JSON_NODE);
        item.push_back(JSONNode(TAG_USER_ID_STR, listUsers[i]));
        itemArr.push_back(item);
    }
    usersNode.push_back(itemArr);
    rn.push_back(usersNode);

    //convert to send reply
    std::string response;
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Get services have candidates of provider
 * Req: {"requestId":"xxxxxxxx","msg":"getServicesHaveCandidates","service":"serviceId"}
 * Res: {"requestId" : "xxxxxxxx", }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processGetServicesHaveCandidates(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n) {
    cout << "getServicesHaveCandidates" << endl;
    //push tag request id
    JSONNode rn;
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    //push tag services
    JSONNode servicesNode(JSON_NODE);
    servicesNode.set_name(TAG_SERVICES_STR);
    //add list item
    JSONNode itemArr(JSON_ARRAY);
    itemArr.set_name(TAG_ITEMS_STR);
    //Get list services have candidates
    //std::string providerId = cs -> getProviderIDFromEmail(cs -> getUserId());
    std::string providerEmail = cs -> getUserId();
    //test
    /*
    unsigned pos = providerId.find("@");
    providerId = providerId.substr (0,pos);
    */

	std::vector<Service> listServices;
    //ResponseCode ret =
    cs -> getServicesHaveCandidates(providerEmail,listServices);
    for (unsigned int i = 0; i < listServices.size(); i++)
    {
        //JSONNode item(JSON_NODE);
        //item.push_back(JSONNode(TAG_SERVICE_STR, listServices[i]));
        //itemArr.push_back(item);
        itemArr.push_back(JsonUtils::toJSONNode(listServices[i]));
    }
    servicesNode.push_back(itemArr);
    rn.push_back(servicesNode);

    //convert to send reply
    std::string response;
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Provider add user to the service
 * Req: {"requestId":"xxxxxxxx","msg":"addUser","service":"serviceId","email":"userEmail"}
 * Res: {"requestId" : "xxxxxxxx", }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processAddUserToService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n) {
    cout << "ADD_USER" << endl;
    ++i;
    std::string serviceId = "";
    std::string userId = "";
    if (i != n.end() && i -> name() == TAG_SERVICE_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        serviceId = i -> as_string();
    ++i;
    if (i != n.end() && i -> name() == TAG_USER_ID_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        userId = i -> as_string();
    //push tag request id
    ResponseCode ret = DATA_ERROR;
    JSONNode rn;
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    if (serviceId.length() == 0 || userId.length() == 0) {
        rn.push_back(JSONNode(TAG_RES_CODE_STR, ret));
    } else {
        //add user to service
//        std::string providerEmail = cs -> getUserId();
		ret = cs -> addUserToService(serviceId,userId);
        rn.push_back(JSONNode(TAG_RES_CODE_STR, ret));
    }
    //convert to send reply
    std::string response;
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Provider remove user from the service
 * Req: {"requestId":"xxxxxxxx","msg":"removeUser","service":"serviceId","email":"userEmail"}
 * Res: {"requestId" : "xxxxxxxx", }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processRemoveUserFromService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n) {
    cout << "REMOVE_USER" << endl;
    ++i;
    std::string serviceId = "";
    std::string userId = "";
    if (i != n.end() && i -> name() == TAG_SERVICE_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        serviceId = i -> as_string();
    ++i;
    if (i != n.end() && i -> name() == TAG_USER_ID_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        userId = i -> as_string();
    //push tag request id
    ResponseCode ret = DATA_ERROR;
    JSONNode rn;
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    if (serviceId.length() == 0 || userId.length() == 0) {
        rn.push_back(JSONNode(TAG_RES_CODE_STR, ret));
    } else {
        //add user to service
        //std::string providerId = cs -> getProviderIDFromEmail(cs -> getUserId());
        std::string providerEmail = cs -> getUserId();
        //test
        /*
        unsigned pos = providerId.find("@");
        providerId = providerId.substr (0,pos);
        */
        ret = cs -> removeUserToService(providerEmail,serviceId,userId);
        rn.push_back(JSONNode(TAG_RES_CODE_STR, ret));
    }

    /// Convert to send reply
    std::string response;
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Provider remove candidate from the service
 * Req: {"requestId":"xxxxxxxx","msg":"removeCandidate","service":"serviceId","email":"userEmail"}
 * Res: {"requestId" : "xxxxxxxx", }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processRemoveCandidateFromService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n) {
    cout << "removeCandidate" << endl;
    ++i;
    std::string serviceId = "";
    std::string userId = "";
    if (i != n.end() && i -> name() == TAG_SERVICE_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        serviceId = i -> as_string();
    ++i;
    if (i != n.end() && i -> name() == TAG_USER_ID_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
        userId = i -> as_string();
    //push tag request id
    ResponseCode ret = DATA_ERROR;
    JSONNode rn;
    rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    if (serviceId.length() == 0 || userId.length() == 0) {
        rn.push_back(JSONNode(TAG_RES_CODE_STR, ret));
    } else {
        //add user to service
        //std::string providerId = cs -> getProviderIDFromEmail(cs -> getUserId());
        std::string providerEmail = cs -> getUserId();

        ret = cs -> removeCandidateFromService(providerEmail,serviceId,userId);
        rn.push_back(JSONNode(TAG_RES_CODE_STR, ret));
    }
    //convert to send reply
    std::string response;
    response = rn.write_formatted();
    std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
    con -> send(response);
}

/*
 * Process remove a service
 * Req: {"requestId":"xxxxxxxx","msg":"deleteService","service":"serviceId",}
 * Res: {"requestId" : "xxxxxxxx", }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processDeleteService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
    cout << "DELETE_SERVICE request" << endl;
    Service service;

    /// Service id
    i = n.find(TAG_SERVICE_STR);
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
        service.setServiceID(i->as_string());
        std::cout << "id = " << service.getServiceID() << std::endl;
    }

    ResponseCode ret = cs -> getService(service);
    if (ret == DATA_SUCCESS) {
        ret = cs -> deleteService(service);
    }
    JSONNode resNode;
    resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    resNode.push_back(JSONNode(TAG_SERVICE_STR, service.getServiceID()));
    resNode.push_back(JSONNode(TAG_RES_CODE_STR, ret));

    /// Send response to client
    std::string response = resNode.write_formatted();
    std::cout<<"Response content:"<< std::endl << response << std::endl;

    con -> send(response);
}


/*
 * Process create a service
 * Req: {"requestId":"xxxxxxxx","msg":"createService",
 *      "name":"serviceName",
 *      "icon":"icon",
 *      "status":"true/false";
 *      "image":"imageName",
 *      "path":"applicationPath"
 *      "stageinDirs":[{"path":"path01"}, {"path":"path02"}]
 *      "stageoutDirs":[{"path":"path03"}, {"path":"path04"}]
 *      "description":"description"
 * }
 * Res: {"requestId" : "xxxxxxxx", "result":"WaitIcon/data_success" }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processCreateService(MessageHeader& header, JSONNode::const_iterator &i, JSONNode& n)
{
    cout << "CREATE_SERVICE request" << endl;
    Service* service = new Service();
    bool isMissField = false;
    /// Service id
    service -> setServiceID(StringUtils::genIdString());

    /// Service name
    i = n.find(TAG_SERVICE_NAME_STR);
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
        service -> setServiceName(i->as_string());
    }

    /// Producer
 //   i = n.find(TAG_PRODUCER_STR);
 //   if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
 //       service -> setProducer(i->as_string());
 //   }

    /// Status
    i = n.find(TAG_STATUS_STR);
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
        std::string status_str = i->as_string();
        service -> setStatus(status_str.compare("true") == 0);
    } else {
        service -> setStatus(false);
    }

    /// Description
    i = n.find(TAG_DESCRIPTION_STR);
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
        service -> setDescription(i->as_string());
    }

    /// Path of excute file
    i = n.find(TAG_PATH_EXCUTE_FILE_STR);
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
        service -> setPathExcuteFile(i->as_string());
    }
	
    /// Path of template job script file
    i = n.find(TAG_PATH_SH_FILE_STR);
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
        service -> setPathShFile(i->as_string());
    }

    /// Path of post-processing sh file
    i = n.find(TAG_PATH_POST_SH_FILE_STR);
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
        service -> setPathPostShFile(i->as_string());
        std::cout << i->as_string().c_str() << service -> getPathPostShFile() << std::endl;;
    }

    /// Path of pre-processing sh file
    i = n.find(TAG_PATH_PRE_SH_FILE_STR);
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
        service -> setPathPreShFile(i->as_string());

        std::cout << i->as_string().c_str() << service -> getPathPreShFile() << std::endl;;
    }

    // Stage-in folders
    std::vector<std::string> dirs;
    i = n.find(TAG_STAGEINDIRS_STR);
    if (i != n.end() && i -> type() == JSON_ARRAY) {
      for (JSONNode::const_iterator j = i->begin(); j != i->end(); j++) {
        JSONNode::const_iterator k = j->find(TAG_PATH_STR);
        if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
            dirs.push_back(k->as_string());
            std::cout <<"stage-in dir " << dirs.size() << " " << k->as_string() <<std::endl;
        }
      }
    }

    service->setStageinDirs(dirs);
    dirs.clear();

    // Stage-out folders
    i = n.find(TAG_STAGEOUTDIRS_STR);
    if (i != n.end() && i -> type() == JSON_ARRAY) {
      for (JSONNode::const_iterator j = i->begin(); j != i->end(); j++) {
        JSONNode::const_iterator k = j->find(TAG_PATH_STR);
        if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
            dirs.push_back(k->as_string());
            std::cout <<"stage-out dir " << dirs.size() << " " << k->as_string() <<std::endl;
        }
      }
    }
    service->setStageoutDirs(dirs);


    /// Image
    i = n.find(TAG_IMAGE_STR);
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
        service -> setImageId(i->as_string());
        //string name = i->as_string();
        //ImageDocker img;
        //ResponseCode ret = cs -> getImageByName(name, img);
        //if(ret == DATA_SUCCESS){
        //	service -> setImageId(img.getImageId());
        //} else {
//			std::cout << "ERR: image not exist!" << std::endl;
//            sendResult(header, ret);
//			delete service;
//			return;
//		}

    } else {
       std::cout << "Missing image field!" << endl;
       isMissField = true;
    }

    /// Number of nodes
    i = n.find(TAG_NUMBER_OF_NODES_STR);
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
      service->setNumberOfNode(i->as_int());
    }else {
      service->setNumberOfNode(0);
    }

    /// Elapse time
    i = n.find(TAG_MAX_ESLAPSED_TIME_STR);
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
      service->setMaxElapseTime(i->as_int());
    } else {
      service->setMaxElapseTime(0);
    }

    /// Icon
    i = n.find(TAG_ICON_STR);
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
        service -> setIconStr(i->as_string());
    } else {
        service -> setIconStr("");
    }

    /// Params
    i = n.find(TAG_PARAMS_STR);
    if (i != n.end() && i -> type() == JSON_NODE) {
        i = i->find(TAG_ITEMS_STR);
        if (i != n.end() && i -> type() == JSON_ARRAY) {
            for (JSONNode::const_iterator j = i->begin(); j != i->end(); j++) {
                Parameter param;
                JSONNode::const_iterator k = j->find(TAG_ID_STR);
                if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                    param.setParamID(k->as_string());
                } else {
                    param.setParamID("");
                }

                k = j->find(TAG_TYPE_STR);
                if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                    param.setType(k->as_string());
                } else {
                    param.setType("");
                }

                k = j->find(TAG_PARAMETER_NAME_STR);
                if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                    param.setName(k->as_string());
                } else {
                    param.setName("");
                }

                k = j->find(TAG_DESCRIPTION_STR);
                if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                    param.setDescription(k->as_string());
                } else {
                    param.setDescription("");
                }

                k = j->find(TAG_OPTION_NAME_STR);
                if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                    param.setOptionName(k->as_string());
                } else {
                    param.setOptionName("");
                }

                k = j->find(TAG_DEFAULT_VALUE_STR);
                if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                    param.setDefaultValue(k->as_string());
                } else {
                    param.setDefaultValue("");
                }

                k = j->find(TAG_IS_OUTPUT_STR);
                if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                    std::string output = k->as_string();
                    param.setIsOutput(output.compare("true")==0);
                } else {
                    param.setIsOutput(false);
                }

                k = j->find(TAG_IS_REQUIRED_STR);
                if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                    std::string require = k->as_string();
                    param.setIsRequired(require.compare("true")==0);
                } else {
                    param.setIsRequired(false);
                }

                param.setServiceID(service -> getServiceID());

                service -> getListParam() -> push_back(param);
            }
        }
    }
    if(isMissField){
        delete service;
        return;
    }

    service -> setProvider(cs -> getUserId());

	ResponseCode ret = cs -> createService(*service);
    JSONNode resNode;
    resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    resNode.push_back(JSONNode(TAG_SERVICE_STR, service -> getServiceID()));

    if (ret == ACTION_WAIT_ICON){
        //resNode.push_back(JSONNode(TAG_UPLOAD_FILE_STR, service -> getIconStr()));
        //resNode.push_back(JSONNode(TAG_RES_CODE_STR, ret));
        /// Get file upload
        cs -> setWorkStatus(ret);
        requestUploadFile(header, service -> getIconStr());
    }
    else if(ret == DATA_SUCCESS) 
    {
      ret = cs -> createServiceContinue();
      responseCreateService(header, service -> getServiceID(),ret);
    } else {
      responseCreateService(header, service -> getServiceID(),ret);
    }
    delete service;
}

/*
 * Process update a service info
 * Req: {"requestId":"xxxxxxxx","msg":"updateService","service":"serviceId",....}
 * Res: {"requestId" : "xxxxxxxx", }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processUpdateService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
	cout << "UPDATE_SERVICE request" << endl;
	Service* service = new Service();

    /// Service id
    i = n.find(TAG_SERVICE_STR);
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
        service -> setServiceID(i->as_string());
    }

    ResponseCode ret = cs->getService(*service);
    unsigned int usf_flags = 0; // all flags/options turned off to start
    if (ret == DATA_SUCCESS) {
        /// Service name
        i = n.find(TAG_SERVICE_NAME_STR);
        if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
            service -> setServiceName(i->as_string());
            usf_flags |= USF_NAME;
        }


        /// Status
        i = n.find(TAG_STATUS_STR);
        if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
            std::string status_str = i->as_string();
            service -> setStatus(status_str.compare("false"));
        }

        /// Description
        i = n.find(TAG_DESCRIPTION_STR);
        if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
            service -> setDescription(i->as_string());
        }

        /// Path of excute file
        i = n.find(TAG_PATH_EXCUTE_FILE_STR);
        if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
            service -> setPathExcuteFile(i->as_string());
            usf_flags |= USF_EXEPATH;
        }
        
        /// Path of sh file
        i = n.find(TAG_PATH_SH_FILE_STR);
        if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
            service -> setPathShFile(i->as_string());
            usf_flags |= USF_SHPATH;
        }

        /// Path of post-proccessing sh file
        i = n.find(TAG_PATH_POST_SH_FILE_STR);
        if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
            service -> setPathPostShFile(i->as_string());
            usf_flags |= USF_POSTSHPATH;
        }

        /// Path of post-proccessing sh file
        i = n.find(TAG_PATH_PRE_SH_FILE_STR);
        if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
            service -> setPathPreShFile(i->as_string());
            usf_flags |= USF_PRESHPATH;
        }


        /// Image id
        i = n.find(TAG_IMAGE_STR);
        if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
            service -> setImageId(i->as_string());
            usf_flags |= USF_IMAGE;
        }


        /// Number of nodes
        i = n.find(TAG_NUMBER_OF_NODES_STR);
        if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
          service->setNumberOfNode(i->as_int());
          usf_flags |= USF_NONODE;
        }

        /// Elapse time
        i = n.find(TAG_MAX_ESLAPSED_TIME_STR);
        if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
          service->setMaxElapseTime(i->as_int());
          usf_flags |= USF_ETIME;
        } 

        // Stage-in folders
        std::vector<std::string> dirs;
        i = n.find(TAG_STAGEINDIRS_STR);
        if (i != n.end() && i -> type() == JSON_ARRAY) {
          for (JSONNode::const_iterator j = i->begin(); j != i->end(); j++) {
            JSONNode::const_iterator k = j->find(TAG_PATH_STR);
            if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                dirs.push_back(k->as_string());
                std::cout <<"stage-in dir " << dirs.size() << " " << k->as_string() <<std::endl;
            }
          }
          service->setStageinDirs(dirs);
          usf_flags |= USF_STGINDIR;
        }
        dirs.clear();

        // Stage-out folders
        i = n.find(TAG_STAGEOUTDIRS_STR);
        if (i != n.end() && i -> type() == JSON_ARRAY) {
          for (JSONNode::const_iterator j = i->begin(); j != i->end(); j++) {
            JSONNode::const_iterator k = j->find(TAG_PATH_STR);
            if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
              dirs.push_back(k->as_string());
              std::cout <<"stage-out dir " << dirs.size() << " " << k->as_string() <<std::endl;
            }
          }
          service->setStageoutDirs(dirs);
          //usf_flags |= USF_STGOUTDIR;
        }


        /// Icon
        i = n.find(TAG_ICON_STR);
        if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
            service -> setIconStr(i->as_string());
        } else {
            service -> setIconStr("");
        }
        /// Params
        i = n.find(TAG_PARAMS_STR);
        if (i != n.end() && i -> type() == JSON_NODE) {
            i = i->find(TAG_ITEMS_STR);
            if (i != n.end() && i -> type() == JSON_ARRAY) {
                for (JSONNode::const_iterator j = i->begin(); j != i->end(); j++) {
                    Parameter param;
                    JSONNode::const_iterator k = j->find(TAG_ID_STR);
                    if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                        param.setParamID(k->as_string());
                    } else {
                        param.setParamID("");
                    }

                    k = j->find(TAG_TYPE_STR);
                    if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                        param.setType(k->as_string());
                    } else {
                        param.setType("");
                    }

                    k = j->find(TAG_PARAMETER_NAME_STR);
                    if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                        param.setName(k->as_string());
                    } else {
                        param.setName("");
                    }

                    k = j->find(TAG_DESCRIPTION_STR);
                    if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                        param.setDescription(k->as_string());
                    } else {
                        param.setDescription("");
                    }

                    k = j->find(TAG_OPTION_NAME_STR);
                    if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                        param.setOptionName(k->as_string());
                    } else {
                        param.setOptionName("");
                    }

                    k = j->find(TAG_DEFAULT_VALUE_STR);
                    if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                        param.setDefaultValue(k->as_string());
                    } else {
                        param.setDefaultValue("");
                    }

                    k = j->find(TAG_ACTION_STR);
                    if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                        param.setAct(convertStrToAct(k->as_string()));
                    } else {
                        param.setAct(convertStrToAct(""));
                    }

                    k = j->find(TAG_IS_OUTPUT_STR);
                    if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                        std::string output = k->as_string();
                        param.setIsOutput(output.compare("true")==0);
                    } else {
                        param.setIsOutput(false);
                    }

                    k = j->find(TAG_IS_REQUIRED_STR);
                    if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
                        std::string require = k->as_string();
                        param.setIsRequired(require.compare("true")==0);
                    } else {
                        param.setIsRequired(false);
                    }

                    param.setServiceID(service -> getServiceID());
                    service -> getListParam() -> push_back(param);
                }
            }
        }

        // Check valid service info    
        ret = cs -> checkValidInfo(*service, usf_flags);
        
        if (ret == DATA_SUCCESS) {
          if (service -> getIconStr().length() == 0) {
              ret = cs->updateService(*service);
          } else {
              service -> setAct(EDIT);
              ret = ACTION_WAIT_ICON;
              cs -> setWorkStatus(ret);
              cs -> setServiceTmp(service);
              requestUploadFile(header, service -> getIconStr());

              /// Check exist file to remove
              std::string fullPath = ICON_FOLDER_PATH;
              std::string path = service -> getServiceID() + ".png";
              if(fullPath.size() > 0 && fullPath.data()[fullPath.size() - 1] == '/')
              {
                  fullPath += path;
              }
              else
              {
                  fullPath = fullPath + string("/") + path;
              }
              cout << "FullPath:" << fullPath << endl;
              std::ofstream fout(fullPath.c_str(), std::ios::out | std::ios::binary);
              if(fout.is_open()){
                  fout.close();
              }
          }
        } else  {
          std::cout << "Updating the service is invalid:" << ret << std::endl;
        }
          
        //ret = cs -> updateService(service,listParam);
    }

    if(ret != ACTION_WAIT_ICON){
        JSONNode resNode;
        resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
        resNode.push_back(JSONNode(TAG_SERVICE_STR, service -> getServiceID()));
        resNode.push_back(JSONNode(TAG_RES_CODE_STR, ret));

        /// Send response to client
        std::string response = resNode.write_formatted();
        std::cout<<"Response content:"<< std::endl << response << std::endl;

        con -> send(response);
        delete service;
    }
}

/*
 * Process uploadIcon request
 * Req: {"requestId":"xxxxxxxx","msg":"uploadIcon","path":"iconFileName","finish":"0/1"}
 * Res: {"requestId" : "xxxxxxxx", }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processUploadIconStep(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
    std::cout << "uploadIcon request" << std::endl;
    std::string requestId = "", path = "";
    int result = 0;
    bool isHasPath = false, isHasFinish;
    i++;
    if (i != n.end() && i -> name() == TAG_PATH_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
    {
        path = i -> as_string();
        isHasPath = true;
    } else if(i != n.end() && i -> name() == TAG_FINISH_STR && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE)
    {
        result = i -> as_int();
        isHasFinish = true;
    }

    if((!isHasPath) && (!isHasFinish))
        return;
	std::map<std::string,Service>* listServ = cs -> getListServiceWaitInsert();
    requestId = header.getRequestID();

    if (listServ -> find(requestId) == listServ -> end())
    {
        cout << "Error: RequestId: "<< requestId << " not found. " << endl;
        sendResult (header, ERR_INVALID_ACCESS);
        return;
    }
    if(isHasPath)
    {
        string fullPath = ICON_FOLDER_PATH;
        if(path.size() > 0 && path.data()[0] == '/')
        {
            fullPath += path;
        }
        else
        {
            fullPath = fullPath + string("/") + path;
        }
        cout << "FullPath:" << fullPath << endl;
        string dir = fullPath.substr(0,fullPath.find_last_of("/"));
        cout << "Directory: " << dir << endl;
        FileUtils::CreateDirectory(dir);
        ofstream fout2(fullPath.c_str(), ios::trunc | ios::out | ios::binary);

        if(fout2.is_open() && fout2.good())
        {
            cs->enqueueFileReceive(requestId, fullPath);
            fout2.close();
        }
        else
        {
            cout << "Error: Can't create file: " << fullPath << endl;
            sendResult (header, ERR_CREATE_FILE);
        }
        cout << "Request is a path" << endl;

    } else if (isHasFinish) {
        if(result == (int)SUCCESS)
        {
			std::map<string,Service>::iterator it = listServ -> find(requestId);
            if(it != listServ -> end())
            {
				if ((*listServ)[requestId].getAct() == CREAT)
                    insertService(header, (*listServ)[requestId]);
				else if ((*listServ)[requestId].getAct() == EDIT)
                    editService(header, (*listServ)[requestId]);
                listServ -> erase(it);
            }
            else {
                cout << "Error: request id not found on service." << endl;
                sendResult (header, ERR_UNKNOW);
			}
        }
        else
        {
            /// Tranfer file fail --> need remove file recieved
            cout << "Tranfer file fail!\n";
        }
        cout << "Request is finish" << endl;
    }
    else
    {
        cout << "Unknow message: " << endl << n.write_formatted();
    }
}

/*
 * Check app path of service
 * Req: {"requestId":"xxxxxxxx","msg":"checkPathOfService","path":"excutePath"}
 * Res: {"requestId" : "xxxxxxxx", }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processCheckPathOfService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
    cout << "CHECK_PATH_OF_SERVICE request" << endl;
	Service service;
    i = n.find(TAG_PATH_EXCUTE_FILE_STR);
    if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		service.setPathExcuteFile(i->as_string());
    }
    ResponseCode ret = cs -> checkPathOfService(service);
    if (ret == DATA_SUCCESS) {
        ret = cs -> deleteService(service);
    }
    JSONNode resNode;
    resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
	resNode.push_back(JSONNode(TAG_SERVICE_STR, service.getServiceID()));
    resNode.push_back(JSONNode(TAG_RES_CODE_STR, ret));

    /// Send response to client
    std::string response = resNode.write_formatted();
    std::cout<<"Response content:"<< std::endl << response << std::endl;
    con -> send(response);
}

/*
 * Check service name is usable
 * Req:
 * { "requestID" : requestID, "msg": "checkserviceName",
		"name": "nameString"
	}
 * Res: { "requestID" : requestID, "result" : resultId }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processCheckServiceNameUsable(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n){
	cout << "CHECK_SERVICE_NAME_USABLE request" << endl;
	std::string name = "";
	i = n.find(TAG_NAME_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		name = i->as_string();
	}
	if(name == ""){
		std::cout << "ERROR: request missing name feild!" << std::endl;
		return;
	}
	ResponseCode ret = cs -> checkNameService(name);
	sendResult(header, ret);
}

/*
 * Get list of provider
 * Req: { "requestID": requestID, "msg": "getProviders" }
 * Res: {
	"requestID" : requestID,
	"providers" : [
		{ "user" : userEmail01, "phone": "phoneNumber", "address":"", "description" : providerDescription },
		{ "user" : userEmail01, "phone": "phoneNumber", "address":"", "description" : providerDescription },
		...
	]
	"
}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processGetProviders(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
  std::cout << "GET_PROVIDERS" << std::endl;
  std::vector<User>providers;
  ResponseCode ret = cs -> getAllProviders(providers);
  if(ret != DATA_SUCCESS)
  {
        sendResult (header, ret);
    return;
  }
  JSONNode resNode;
  JSONNode providersArr(JSON_ARRAY);
  providersArr.set_name(TAG_PROVIDERS_STR);
  std::cout << "number of provider : " << providers.size() << std::endl;
  for(int j = 0; j < providers.size(); j++){
    std::cout << "provider " << j << ": " << providers[j].getEmail() << std::endl;
    Options opt = OPTS_PROVIDER;
    User usr = providers.at(j);
    std::cout << "before ToJsonNode :\n";
    JSONNode prnode = JsonUtils::toJSONNode(usr, opt);

    std::cout << "after to JsonNode\n";
    providersArr.push_back(prnode);
  }
  resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
  resNode.push_back(providersArr);

  std::string response = resNode.write_formatted();
  std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
  con -> send(response);
}

/*
 * Get list of provider register
 * Req: { "requestID": requestID, "msg": "getProviderRegisters" }
 * Res: {
	"requestID" : requestID,
	"registers" : [
		{ "user" : userEmail01, "phone": "phoneNumber", "address":"", "description" : providerDescription },
		{ "user" : userEmail01, "phone": "phoneNumber", "address":"", "description" : providerDescription },
		...
	]
	"
}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processGetProviderRegisters(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
	std::cout << "GET_PROVIDER_REGISTERS" << std::endl;
	std::vector<User>providers;
	ResponseCode ret = cs -> getAllWaitProviders(providers);
	if(ret != DATA_SUCCESS)
	{
        sendResult(header, ret);
		return;
	}
	JSONNode resNode;
	JSONNode providersArr(JSON_ARRAY);
	providersArr.set_name(TAG_REGISTERS_STR);
	for(int i = 0; i < providers.size(); i++){

		Options opt = OPTS_PROVIDER_REGISTER;
		providersArr.push_back(JsonUtils::toJSONNode(providers[i], opt));
	}
	resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
	resNode.push_back(providersArr);
	std::string response = resNode.write_formatted();
	std::cout<<"JSONNode content:"<<std::endl<<response<<std::endl;
	con -> send(response);
}

/*
 * Get list of provider register
 * Req: { "requestID": requestID, "msg": "acceptProviderRegisters"
		  "registers" : [
				"email" : userEmail01,
				"email" : userEmail01,
				...
		]}
 * Res: {
	"requestID" : requestID,
	"result" : ""}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processAcceptProviderRegisters(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
	std::cout << "ACCEPT_PROVIDER_REGISTERS" << std::endl;
	std::vector<User> providers;
	i = n.find(TAG_REGISTERS_STR);
	if (i != n.end() && i -> type() == JSON_ARRAY) {
		for (JSONNode::const_iterator j = i->begin(); j != i->end(); j++) {
			JSONNode::const_iterator k = j->find(TAG_EMAIL_STR);
			if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
				User usr(k->as_string(), PROVIDER_GROUP);
				providers.push_back(usr);
			}
		}
	}
	if(providers.size() > 0){
    ResponseCode ret = cs -> addProviders(providers);
		sendResult(header, ret);
	}
}

/*
 * Get list of provider register
 * Req: { "requestID": requestID, "msg": "rejectProviderRegisters"
		  "registers" : [
				"email" : userEmail01,
				"email" : userEmail01,
				...
		]}
 * Res: {
	"requestID" : requestID,
	"result" : ""}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processRejectProviderRegisters(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
	std::cout << "REJECTPROVIDERREGISTERS" << std::endl;
	std::vector<User> providers;
	i = n.find(TAG_REGISTERS_STR);
	if (i != n.end() && i -> type() == JSON_ARRAY) {
		for (JSONNode::const_iterator j = i->begin(); j != i->end(); j++) {
			JSONNode::const_iterator k = j->find(TAG_EMAIL_STR);
			if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
				User usr(k->as_string(), WAIT_PROVIDER_GROUP);
				providers.push_back(usr);
			}
		}
	}
	if(providers.size() > 0){
		ResponseCode ret = cs -> deleteWaitProviders(providers);
		sendResult(header, ret);
	}
}


/*
 * Get reject providers
 * Req: { "requestID": requestID, "msg": "rejectProviders"
		  "providers" : [
				"email" : userEmail01,
				"email" : userEmail01,
				...
		]}
 * Res: {
	"requestID" : requestID,
	"result" : ""}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processRejectProviders(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
	std::cout << "REJECT_PROVIDERS" << std::endl;
	std::vector<User> providers;
	i = n.find(TAG_PROVIDERS_STR);
	if (i != n.end() && i -> type() == JSON_ARRAY) {
		for (JSONNode::const_iterator j = i->begin(); j != i->end(); j++) {
			JSONNode::const_iterator k = j->find(TAG_EMAIL_STR);
			if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
				User usr(k->as_string(), PROVIDER_GROUP);
				providers.push_back(usr);
			}
		}
	}
	if(providers.size() > 0){
		/// Same api rejectProviderRegisters
    ResponseCode ret = cs -> deleteWaitProviders(providers);
		sendResult(header, ret);
	}
}

/*
 * Get list of provider register
 * Req: { "requestID": requestID, "msg": "updateProviderTimeUsable"
        "email" : userEmail01, "timeUsage" : timeUsable}
 * Res: {
  "requestID" : requestID,
  "result" : ""}
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processUpdateProviderUsableTime(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n){
  std::cout << "UPDATE_PROVIDER_TIME_USABLE" << std::endl;
  std::string email; double usableTime = 0;
  i = n.find(TAG_EMAIL_STR);
  if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
    email = i -> as_string();
  }
  i = n.find(TAG_TIMEUSABLE_STR);
  if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
    usableTime = i -> as_float();
  }

  ResponseCode ret = cs -> updateProviderTimeUsable(email, usableTime);
  sendResult(header, ret);
}

/*
 * Get list of image of provider
 * Req: { "requestID" : requestID, "msg": "findImages", "isPublic":0/1, "isOwner":0/1, "keyword":fileterKeyword}
 * Res: {
        "requestID" : requestID,
        "images" : [
            { "image" : imageId01, "isPublic" : 0/1, "url" : downloadUrl01, "name" : imageName01, "description" : imageDescription01, "url" : downloadUrl01 },
            { "image" : imageId02, "isPublic" : 0/1, "url" : downloadUrl02, "name" : imageName02, "description" : imageDescription02, "url" : downloadUrl02 },
            ...
        ]
        }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processFindImages(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n)
{
	std::cout << "FIND_IMAGES" << std::endl;
	bool isOwner = false, isPublic = false, isIncludeSample = false;
    std::string keyword = "";
	std::vector<ImageDocker> images;
    JSONNode providersArr(JSON_ARRAY);
    providersArr.set_name(TAG_IMAGES_STR);

    i = n.find(TAG_IS_OWNER_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
        isOwner = i -> as_int() == 1;
    }
    i = n.find(TAG_IS_PUBLIC_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
        isPublic = i -> as_int() == 1;
        if(isPublic)
        {
			i = n.find(TAG_KEYWORD_STR);
			if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
				keyword = i -> as_string();
			}
        }
	}
	i = n.find(TAG_IS_INCLUDE_SAMPLE_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		isIncludeSample = i -> as_int() == 1;
	}

	if(isOwner == false && isIncludeSample == false && isPublic == false){
	   std::cout << "Request missing field!" << std::endl;
	   return;
	}
	if(isOwner){
		cs -> getAllImageDockerOfProvider(images);
        if(isIncludeSample){
            for(std::map<std::string, ImageDocker>::iterator it = SampleImages.begin(); it != SampleImages.end(); ++it) {
                ImageDocker id = it->second;
                images.push_back(id);
            }
        }
    } else if(isPublic){
		cs -> getPublicImageByKey(keyword, images);
        if(isIncludeSample){
            for(std::map<std::string, ImageDocker>::iterator it = SampleImages.begin(); it != SampleImages.end(); ++it) {
                ImageDocker id = it->second;
                int idx = id.getImageName().find(keyword);
                std::cout << "Image name = " << id.getImageName().c_str() << std::endl;
                std::cout << "index find = " << idx << std::endl;
                if(idx >= 0){
                    images.push_back(id);
                }
            }
        }
	}

	for(int i = 0 ; i < images.size() ; i++ ){
		providersArr.push_back(JsonUtils::toJSONNode(images[i]));
	}
    JSONNode resNode;
    resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
    resNode.push_back(providersArr);

	/// Send response to client
	std::string response = resNode.write_formatted();
	std::cout<<"Response content:"<< std::endl << response << std::endl;
	con -> send(response);
}

/*
 * Create a new image
 * Req:
 * { "requestID" : requestID, "msg": "createImage",
		"name", imageName,
		"isPublic" : 0/1,
		"tarfile" : localFileName,
		"description" : imageDescription
	}
 * Res: { "requestID" : requestID, "result" : resultId }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processCreateImage(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n){
	cout << "CREATE_IMAGE request" << endl;

	ImageDocker * image = new ImageDocker();

	/// Image name
	i = n.find(TAG_NAME_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		image -> setImageName(i->as_string());
	}

	/// IsPublic
	i = n.find(TAG_IS_PUBLIC_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		image -> setIsPublic(i->as_int() == 1);
	}

	/// Description
	i = n.find(TAG_DESCRIPTION_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		image -> setComment(i->as_string());
	}

	/// Tar file
	i = n.find(TAG_TAR_FILE_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		image -> setTarStr(i->as_string());
	}

	ResponseCode ret = cs -> createImage(*image);
	if (ret == ACTION_WAIT_TAR){
		//cs -> setHeader(header);
		cs -> setWorkStatus(ret);
		requestUploadFile(header, image -> getTarStr());
	} else {
		std::string id = image -> getImageId();
		responseCreateImage(header, id, ret);
	}

	if (image != NULL) {
		delete image;
	}
}

/*
 * Update a image
 * Req:
 * { "requestID" : requestID, "msg": "updateImage",
		"isPublic" : 0/1,
		"description" : imageDescription
	}
 * Res: { "requestID" : requestID, "result" : resultId }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processUpdateImage(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n){
	cout << "UPDATE_IMAGE request" << endl;

	ImageDocker image;

	/// Image name
	i = n.find(TAG_IMAGE_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		cs -> getImageByName(i->as_string(), image);
	} else {
		std::cout << "ERROR: Request feild is missing!" << std::endl;
		return;
	}

	/// IsPublic
	i = n.find(TAG_IS_PUBLIC_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		image.setIsPublic(i->as_int() == 1);
	}

	/// Description
	i = n.find(TAG_DESCRIPTION_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		image.setComment(i->as_string());
	}
	ResponseCode ret = cs -> updateImage(image);
	sendResult(header, ret);
}

/*
 * Delete a image
 * Req:
 * { "requestID" : requestID, "msg": "deleteImage",
		"id", imageId
	}
 * Res: { "requestID" : requestID, "result" : resultId }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processDeleteImage(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n){
	cout << "DELETE_IMAGE request" << endl;

	ImageDocker image;
	/// Image id
	i = n.find(TAG_IMAGE_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		cs -> getImageByName(i->as_string(), image);
	}
	ResponseCode ret = cs -> deleteImage(image);
	sendResult(header, ret);
}

/*
 * Check image name is usable
 * Req:
 * { "requestID" : requestID, "msg": "checkImageNameUsable",
		"class": 1/2/3 <-> image/job/service,
		"name": "nameString"
	}
 * Res: { "requestID" : requestID, "result" : resultId }
 *
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processCheckImageNameUsable(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n){
	cout << "CHECK_NAME_USABLE request" << endl;
	std::string name = "";
	i = n.find(TAG_NAME_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		name = i->as_string();
	}
	if(name == ""){
		std::cout << "ERROR: request missing name feild!" << std::endl;
		return;
	}
	ResponseCode ret = cs -> checkNameImage(name);
	sendResult(header, ret);
}

/*
 * Process download job output file
 * Req: { "requestID" : requestID, "msg": "getImageFile","image":"imageId"}
 * Res: binary data
 * Req: { "requestID" : requestID, "msg": "getImageFile, "result" : resultId }
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processGetImageFile(MessageHeader& header, JSONNode::const_iterator& i, JSONNode & n)
{
	std::cout << "GET_IMAGE_FILE" << std::endl;
	std::string imgId; int result;

	i = n.find(TAG_IMAGE_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		imgId = i -> as_string();
	}

	i = n.find(TAG_RES_CODE_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		result = i -> as_int();
	}

	/// Process get file request
	if( imgId != ""){
		ImageDocker image;
		ResponseCode ret = cs -> getImage(image);
		if(ret == DATA_SUCCESS){
			if( sendFile(image.getTarStr(), ret)){
				cs -> setWorkStatus(ACTION_WAIT_RESPONSE);
			}else{
				std::cout << "SendFile() return fail!" << std::endl;
			}
		}
	} else if(result == DATA_SUCCESS){
		cs -> setWorkStatus(DATA_SUCCESS);
	} else {
		//sendError(header, ERR_WRITE_FILE);
		std::cout << "Request missing field!" << std::endl;
		return;
	}

	/*	Send response report num of packet data	*/
/*	JSONNode resNode(JSON_NODE);
	resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
	resNode.push_back(JSONNode(TAG_FINISH_STR, (int)result));

	std::string resReport = resNode.write_formatted();
	std::cout<<"JSONNode content:"<<std::endl<<resReport<<std::endl;
	con -> send(resReport);*/
}

/*
 *  Register provider
-> Client request :
  { "requestID" : requestID, "msg": "registerProvider", "phone": "phoneNumer", "address":"address","keskacc":"xxxx","description":"notes" }
<- Server response:
	{ "requestID" : requestID, "result" : resultId }
 *
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processRegisterProvider(MessageHeader& header, JSONNode::const_iterator& i, JSONNode & n)
{
	std::cout << "REGISTER_PROVIDER" << std::endl;
	User* usr = new User();
	i = n.find(TAG_PHONE_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		usr -> setPhone(i -> as_string());
	}
	i = n.find(TAG_ADDRESS_STR);
	if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
		usr -> setAddress(i -> as_string());
  }
  i = n.find(TAG_KDESKACC_STR);
  if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
    usr -> setKdeskAcc(i -> as_string());
  }
  i = n.find(TAG_DESCRIPTION_STR);
  if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
    usr -> setDescription(i -> as_string());
  }
  ResponseCode ret = cs -> registerProvider(usr -> getPhone(), usr -> getAddress(), usr -> getKdeskAcc(), usr -> getDescription());
	std::cout << "ResponeCode = " << ret << std::endl;
	sendResult(header, ret);
	delete usr;
}

/*
 *  Remove register provider
-> Client request :
  { "requestID" : requestID, "msg": "rejectProvider"}
<- Server response:
  { "requestID" : requestID, "result" : resultId }
 *
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processRejectProvider(MessageHeader& header, JSONNode::const_iterator& i, JSONNode & n)
{
	std::cout << "REJECT_PROVIDER" << std::endl;
  ResponseCode ret = cs -> outProviderGroup();
	sendResult(header, ret);
}

/*
 * Get resource usage & usable time
 * -> Client request :
 *	{ "requestID" : requestID, "msg": "getProviderTimeResource" }
 * <- Server response:
 *	{ "requestID" : requestID, "unit":{Ms, s, h},"limit" : total_time, "usage" : usage_time, "avaiable":remain_time }
 *
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processGetProviderTimeResource(MessageHeader& header, JSONNode::const_iterator& i, JSONNode & n)
{
  std::cout << "REQ_GET_PROVIDER_TIME_RESOURCE" << std::endl;
  std::string unit = "Ms", public_key;
  double total = 0, remain = 0, usage = 0, totalkp = 0, remainkp = 0, usagekp = 0;
  ResponseCode ret = cs -> getProviderTimeResource(cs -> getUserId(), unit, total, remain, usage, totalkp, remainkp, usagekp, public_key);

  JSONNode rn;
  rn.push_back(JSONNode(TAG_REQUEST_ID_STR, header.getRequestID()));
  rn.push_back(JSONNode(TAG_UNIT_STR, unit));
  rn.push_back(JSONNode(TAG_TOTAL_KD_STR, total));
  rn.push_back(JSONNode(TAG_REMAIN_KD_STR, remain));
  rn.push_back(JSONNode(TAG_USAGE_KD_STR, usage));
  rn.push_back(JSONNode(TAG_TOTAL_KP_STR, totalkp));
  rn.push_back(JSONNode(TAG_REMAIN_KP_STR, remainkp));
  rn.push_back(JSONNode(TAG_USAGE_KP_STR, usagekp));
  rn.push_back(JSONNode(TAG_PUBLICKEY_STR, public_key));

  // Send response to client
  std::string response = rn.write_formatted();
  std::cout<<"Response content:"<< std::endl << response << std::endl;
  con -> send(response);
}

/*
 * Process binary data is recieved
 */
template <typename request_endpoint_type>
void request<request_endpoint_type>::processBinaryData(){	
    ResponseCode ret;
	std::ofstream fout;
	//std::ifstream fin;
	std::string requestId, fullPath, dirPath, path, paramId;//, sentByte, totalByte;
	char sentByte[NOB_SENT_BYTE], totalByte[NOB_TOTAL_BYTE];//, paramId[NOB_PARAM_ID_BYTE];

	const char *data;
	long sentByteNo, totalByteNo;
	int headerSize = 0, ip;
	JSONNode resNode;
	MessageHeader hdr;

	Job* job = NULL;
	switch(cs -> getWorkStatus())
    {
    case ACTION_WAIT_ICON:
        //ret = cs -> saveFileIcon(msg->get_payload().data());
        ret = DATA_SUCCESS;
        std::cout << "Save file: " << cs -> getServiceTmp() -> getIconStr() << ", service = " << cs -> getServiceTmp() -> getServiceID() << std::endl;
        path = cs -> getServiceTmp() -> getServiceID() + ".png";
        fullPath = ICON_FOLDER_PATH;
        if(fullPath.size() > 0 && fullPath.data()[fullPath.size() - 1] == '/')
        {
            fullPath += path;
        }
        else
        {
            fullPath = fullPath + string("/") + path;
        }
        cout << "FullPath:" << fullPath << endl;
        dirPath = fullPath.substr(0,fullPath.find_last_of("/"));
        cout << "Directory: " << dirPath << endl;
        //FileUtils::CreateDirectory(dirPath);

        for( int i = 0; i < msg->get_payload().size(); i ++ )
        {
          if(i < NOB_REQUEST_ID){
            requestId += msg->get_payload().data()[i];
          }
        }

        fout.open(fullPath.c_str(), std::ios::app | std::ios::out | std::ios::binary);
        if(fout.is_open()){
          fout.write(msg->get_payload().data() + NOB_REQUEST_ID, msg->get_payload().size() - NOB_REQUEST_ID);
          fout.close();
        } else {
          std::cout << "[ERR] Can not create file to write.";
          ret = FILE_ACTION_ERROR;
        }
        hdr.setRequestId(requestId);

        if( ret == DATA_SUCCESS)
        {
            if(cs -> getServiceTmp() -> getAct() == EDIT){
                Service* srv = cs -> getServiceTmp();
                editService(hdr, *srv);
                //sendResult(hdr, ret);
                if(srv != NULL){
                    delete srv;
                    srv = NULL;
                    cs -> setServiceTmp(NULL);
                }
            } else {
                std::string id = cs -> getServiceTmp() -> getServiceID();
                ret = cs -> createServiceContinue();
				responseCreateService(hdr, id, ret);
            }
        }else{
            sendResult(hdr, ret);
		}
        break;
	case ACTION_WAIT_TAR:
		//ret = cs -> saveTarFile(msg->get_payload().data() + NOB_REQUEST_ID, cs -> getImageTmp() -> getTarStr());
		std::cout << "Save file: " << cs -> getImageTmp() -> getTarStr() << std::endl;
		/*
		fin.open( cs -> getImageTmp() -> getTarStr().c_str(), ios::binary | ios::in );
		if(fin.is_open()){
			fin.seekg( 0, ios::end );
			std::cout << "File size before write: " << fin.tellg()<< std::endl;
			fin.close();
		}*/

		fout.open(cs -> getImageTmp() -> getTarStr().c_str(), std::ios::app | std::ios::out | std::ios::binary | std::ios::ate);

		if(fout.is_open()){
      for(unsigned int i = 0; i < msg->get_payload().size(); i ++ )
			{
				if(i < NOB_REQUEST_ID){
					requestId += msg->get_payload().data()[i];
				} else if( i < NOB_REQUEST_ID + NOB_SENT_BYTE){
					sentByte[i - NOB_REQUEST_ID] = msg->get_payload().data()[i];
				} else if(i < NOB_REQUEST_ID + NOB_SENT_BYTE + NOB_TOTAL_BYTE){
					totalByte[i - NOB_REQUEST_ID - NOB_SENT_BYTE] = msg->get_payload().data()[i];
				} else {
					/// Write data to file
					headerSize = NOB_REQUEST_ID + NOB_SENT_BYTE + NOB_TOTAL_BYTE;
					std::cout << "Write data size = " << msg->get_payload().size() - headerSize << std::endl;
					fout.write(msg->get_payload().data() + headerSize , msg->get_payload().size() - headerSize);
					break;
					//fout << msg->get_payload().data()[i];
				}
			}
			fout.close();
			if( requestId.size() == NOB_REQUEST_ID){// && sentByte.size() == NOB_SENT_BYTE && totalByte.size() == NOB_TOTAL_BYTE){
				try {
					sentByteNo = *((unsigned int*)sentByte);
					totalByteNo = *((unsigned int*)totalByte);
				}
				catch(char* ex) {
					cout << "EXCEPTION: " << ex << endl;
					break;
				}
				std::cout << "sentByteNo= " << sentByteNo << ", totalByteNo = " << totalByteNo << std::endl;
				if(sentByteNo < totalByteNo){
					resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, requestId));
					resNode.push_back(JSONNode(TAG_FROM_STR, sentByteNo));
					resNode.push_back(JSONNode(TAG_RES_CODE_STR, DATA_SUCCESS));
					std::string resReport = resNode.write_formatted();
					//std::cout<<"JSONNode content:"<<std::endl<<resReport<<std::endl;
					con -> send(resReport);
				}  else if(sentByteNo == totalByteNo && sentByteNo != 0) {
					std::string id = cs -> getImageTmp() -> getImageId();
//					std::string name = cs -> getImageTmp() -> getImageName();
//					std::string email = cs -> getImageTmp() -> getProviderEmail();
					ret = cs -> createImageContinue(*cs -> getImageTmp());
					hdr.setRequestId(requestId);
					if( ret == DATA_SUCCESS) {
//						std::string url = APPMARKET_DOMAIN + "/images/" + name;
//						if(!GmailUtils::SendEmail(email, name, url)){
//							std::cout << "Send email fail!\n";
//						}
						responseCreateImage(hdr, id, ret);

					}else{
						sendResult(hdr, ret);
					}
				}else {
					std::cout << "ERROR: Header fields is missing !" << std::endl;
				}
			} else {
				std::cout << "ERROR: Header is missing !" << std::endl;
			}
        }
		break;

	case ACTION_WAIT_INPUT:
	{
    for(unsigned int i = 0; i < msg->get_payload().size(); i ++ )
		{
			if(i < NOB_REQUEST_ID){
				requestId += msg->get_payload().data()[i];
			} else if( i < NOB_REQUEST_ID + NOB_SENT_BYTE){
				sentByte[i - NOB_REQUEST_ID] = msg->get_payload().data()[i];
			} else if(i < NOB_REQUEST_ID + NOB_SENT_BYTE + NOB_TOTAL_BYTE){
				totalByte[i - NOB_REQUEST_ID - NOB_SENT_BYTE] = msg->get_payload().data()[i];
			} else if(i < NOB_REQUEST_ID + NOB_SENT_BYTE + NOB_TOTAL_BYTE + NOB_PARAM_ID_BYTE){
				paramId += msg->get_payload().data()[i];
			} else {
				break;
			}
		}
		std::cout << "RequestID:" << requestId.c_str() << ", ParamId:" << paramId.c_str() << std::endl;
		job = cs -> getJobTmp();

		ip = -1;
    for(unsigned int i = 0; job != NULL && i < job -> getParamJsonStr() -> size() ; i ++){
			cout << "param id: " << job -> getParamJsonStr()-> at(i).getParamId() << ", is local: " << job -> getParamJsonStr()->at(i).getType() << std::endl;
			if(job -> getParamJsonStr()->at(i).getType() == ARG_LOCAL
					&& job -> getParamJsonStr()-> at(i).getParamId().compare(paramId) == 0){
				ip = i;
				break;
			}
		}
		if(ip >= 0){
			fullPath = job -> getPathJob() + PATH_SEPARATOR + job -> getParamJsonStr()->at(ip).getValue();
			headerSize = NOB_REQUEST_ID + NOB_SENT_BYTE + NOB_TOTAL_BYTE + NOB_PARAM_ID_BYTE;
			fout.open(fullPath.c_str(), std::ios::app | std::ios::out | std::ios::binary | std::ios::ate);
			std::cout << "Write file = " << fullPath << std::endl;
			std::cout << "data size = " << msg->get_payload().size() - headerSize << std::endl;
			if(fout.is_open()){
				fout.write(msg->get_payload().data() + headerSize , msg->get_payload().size() - headerSize);
				fout.close();
			} else {
				std::cout << "Can not open file to write\n";
			}
			try {
				sentByteNo = *((unsigned int*)sentByte);
				totalByteNo = *((unsigned int*)totalByte);
			}
			catch(char* ex) {
				cout << "EXCEPTION: " << ex << endl;
				break;
			}
			hdr.setRequestId(requestId);
			/// NExt param
			if(sentByteNo == totalByteNo && totalByteNo > 0){
				for( int j = ip + 1; j < job -> getParamJsonStr() -> size() ; j ++){
					if(job -> getParamJsonStr()->at(j).getType() == ARG_LOCAL){
						requestUploadFileParam(hdr, job -> getParamJsonStr()->at(j));
						return;
					}
				}
				/// Continue submit job
				Job jb(job -> getId());
				ret = cs-> submitJob(jb);
				if(ret == DATA_SUCCESS){
					responseSubmitJob(hdr, job -> getId(), ret);
				} else {
					sendResult(hdr, ret);
				}
			} else if(sentByteNo < totalByteNo) {
				resNode.push_back(JSONNode(TAG_REQUEST_ID_STR, requestId));
				resNode.push_back(JSONNode(TAG_FROM_STR, sentByteNo));
				resNode.push_back(JSONNode(TAG_PARAMETER_STR, paramId));
				resNode.push_back(JSONNode(TAG_RES_CODE_STR, DATA_SUCCESS));
				std::string resReport = resNode.write_formatted();
				con -> send(resReport);
			}else {
				std::cout << "Size sent & total size invalid \n";
			}
		} else {
			std::cout << "Not found param\n";
		}
		break;
	}
	default:
		std::cout << "ERROR: Don\'t wait file!" << std::endl;
		break;
	}

	/*
    /// Get path to save
    FileReceive fr;
    if( cs -> dequeueFileReceive(fr) != 0 )
    {
        std::cout<<"Error: dequeueFileReceive fail" << std::endl;
        return;
    }

    /// Save data to file
    std::string outFileName = fr.fileName;
    std::cout << "FileName: " << fr.fileName << std::endl;
    std::ofstream fout(outFileName.c_str(), std::ios::app | std::ios::out | std::ios::binary);

    if(fout.is_open() && fout.good())
    {
        std::cout << "Bp: write to file: size: " << msg->get_payload().size()<< std::endl;

        fout.write(msg->get_payload().data(), msg->get_payload().size());

        fout.close();
    }
    else
    {
        cout << "Error: Can't write file: " << outFileName << endl;
        sendError(fr.requestId, ERR_WRITE_FILE);
    }
    */
}

/*
// {"type":"msg","result":"<result>","value":"<msg>" }
template <typename endpoint_type>
std::string request<endpoint_type>::encode_message(std::string result,std::string msg,bool escape) {
        std::stringstream s;

        // escape JSON characters
        //boost::algorithm::replace_all(msg,"\\","\\\\");
        //boost::algorithm::replace_all(msg,"\"","\\\"");

        // escape HTML characters
        if (escape) {
            boost::algorithm::replace_all(msg,"&","&amp;");
            boost::algorithm::replace_all(msg,"<","&lt;");
            boost::algorithm::replace_all(msg,">","&gt;");
        }

        s << "{\"type\":\"msg\",\"result\":\"" << result
          << "\",\"value\":" << msg << "}";
        return s.str();
}

*/

/*
template <typename endpoint_type>
long request<endpoint_type>::get_current_time()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    long ret = 1000L *  time.tv_sec + time.tv_usec / 1000;//milisecond

    return ret;
}

template <typename endpoint_type>
long request<endpoint_type>::caculate_time()
{
    long diff = m_end - m_begin;
    return diff;
}
*/

/*
template <typename endpoint_type>
void k_server_handler<endpoint_type>::validate(connection_ptr con) {
    std::stringstream err;

    // We only know about the chat resource
    if (con->get_resource() != "/search" && con->get_resource() != "/kportal") {
        //LOG4CXX_INFO(logger, "Request for unknown resource " << con->get_resource());
        throw(websocketpp::http::exception(err.str(),websocketpp::http::status_code::NOT_FOUND));
    }
}
*/

