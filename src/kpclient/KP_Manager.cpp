#include "KP_Manager.hpp"
#include <boost/algorithm/string/replace.hpp>


//using websocketkclient::KP_Manager;
using websocketpp::client;

/*
KP_Manager* KP_Manager::getInstance()
{
	static KP_Manager* _instance;
	if(_instance == NULL)
	{
		_instance = new KP_Manager();
	}
	return _instance;
}
*/
///KP_Manager(std::string userId, std::string pwd);
KP_Manager::KP_Manager(std::string userId, std::string pwd)
{
//	this -> _con		= NULL;
    this -> _userId 	= userId;
	this -> _pwd		= pwd;
}

void KP_Manager::setUserId(string userId)
{
    this -> _userId = userId;
}

void KP_Manager::setPwd(string pwd)
{
    this -> _pwd = pwd;
}

void KP_Manager::setUri(string uri)
{
    this -> _uri = uri;
}

KP_Manager::~KP_Manager()
{
	//if(this -> _con != NULL)
		//delete _con;
}

void KP_Manager::on_fail(connection_ptr con) {
    std::cout << "Connection failed" << std::endl;
    close();
}

void KP_Manager::on_open(connection_ptr con) {
    m_con = con;

    std::cout << "Successfully connected" << std::endl;
    
    sem_post(&sem_conn);
}

void KP_Manager::on_close(connection_ptr con) {
    m_con = connection_ptr();

    std::cout << "client was disconnected" << std::endl;
    
    sem_post(&sem_conn);
}

bool KP_Manager::writeFile(std::string * fileName, IdRes* idRes)
{
    bool result = false;
    if(idRes == NULL || fileName == NULL || fileName == NULL || (!FileUtils::IsFile(*fileName)))
    {
        cout << "Bp: writeFile:" << idRes << ", fileName:" << (fileName != NULL ? (*fileName) : "NULL") << endl;
        return result;
    }
    ofstream fout(fileName -> c_str(), ios::app | ios::binary);
    if(fout.is_open() && fout.good())
    {
        string data = idRes->getData();
        cout <<"Bp: writeFile: size:" << data.size() << endl;
        fout.write(data.data(), data.size());
        result = true;
        fout.close();

    }
    else
    {
        cout << "Bp: Error: cannot open file:" << *fileName << endl;
    }
    return result;
}

void KP_Manager::processMessage(JSONNode & n)
{
	JSONNode::const_iterator i = n.begin();
	
    /// Parse header
    string owner = "";
    if(i != n.end() && i -> name() == TAG_OWNER_STR)
    {
        owner = i -> as_string();
        if(_threadDatas.find(owner) == _threadDatas.end())
        {
            cout << "Error: threadId:" << owner << " not found." << endl;
            return;
        }
    }
    else
    {
        cout << "Error: Message miss owner tag!\n" << n.write_formatted() << endl;
    }

    i++;
    if(i == n.end())
        return;

    std::string node_name = i -> name();

	if(node_name == TAG_VALID)
	{
		int node_value = i -> as_int();
		if(node_value == VALID)
			_isLogined = true;
		else if (node_value == INVALID)
			_isLogined = false;

        std::map<string,sem_t>::iterator it = sems.find(owner);
        if(it != sems.end())
            sem_post(&sems.at(owner));
    }

	else if (node_name == TAG_SERVICES_STR)
	{
        _threadDatas[owner].services = ResUtils::ParseServicesResponse(n);
        semPost(owner);
    }
	/* { “service”: serviceId, “provider”: providerId} */
	else if (node_name == TAG_SERVICE_STR)
    {
        _threadDatas[owner].service = ResUtils::ParseServiceResponse(i, n);
        semPost(owner);
	}
	
    else if(node_name == TAG_JOB_STR)
    {
        KP_Job * _job;
        _job = ResUtils::ParseJobResponse(n);
        //cout << "Bp: processMessage: jobId: " <<_job->getJobId() << endl;
        if(_job != NULL)
            setJob(owner, _job);
        semPost(owner);
    }

    else if(node_name == TAG_ACK_STR ||
            node_name == TAG_PATH_STR ||
            node_name == TAG_DATA_STR ||
            node_name == TAG_FINISH_STR)
    {
        IdRes* idRes = NULL;
        switch((int)getApi(owner))
        {
        case (int)SUBMIT_JOB:
            idRes = ResUtils::ParseIdResponse(n);
            if(idRes != NULL)
            {
                _threadDatas[owner].ack = idRes->getAck();
                delete idRes;
            }
            semPost(owner);
            break;
        case (int)GET_OUTPUT_FILE:
            idRes = ResUtils::ParseIdResponse(n);
            if(idRes != NULL)
            {
                processID(*idRes);
            }
            else
                cout << "Error: idRes after parse: is NULL\n";
            break;

        case (int)GET_RUN_METHOD_FILE:
            //cout << "Bp: GET_RUN_METHOD_FILE \n" << n.write_formatted() << endl;
            idRes = ResUtils::ParseIdResponse(n);
            if(idRes != NULL)
            {
                processID(*idRes);
            }
            else
                cout << "Error: idRes after parse: is NULL\n";
            break;
        }
    }

    /*
    { “jobs”: {
            “items”: [
                { “id”: jobId1 },
                …,
                { “id”: jobIdN },
            ]
    } }
    */
    else if(node_name == TAG_JOBS_STR)
    {
        list<KP_Job> _jobs;
        ResUtils::ParseJobsResponse(_jobs, n);
        _threadDatas[owner].jobs = _jobs;
        semPost(owner);
        //sem_post(&sem_jobList);
    }
    else if(node_name == TAG_STATUS_STR)
    {
        int status = 0;
        bool result = ResUtils::ParseStatusResponse(n, i, status);

        if(status == 0)
            return;

        setError(owner, (Errors) status);
        if((int)getError(owner) == ERR_NO_AUTHENTICATION)
            _isLogined = false;
        semPost(owner);
    }
}

void KP_Manager::processID(IdRes& idRes)
{
    string fileName = "", localFile = "";
    string owner = "";
    FileRecieve * fr = NULL;
    if(_files.find(idRes.getOwner()) != _files.end())
    {
        fr = &_files[idRes.getOwner()];
    }
    else
    {
        cout << "Error: FileRecieve not found." << endl;
        return;
    }

    switch ((int) idRes.getType())
    {
    case (int)IdRes::TYPE_PATH:
        localFile = fr->getLocalPath();
        if(FileUtils::IsFile(localFile))
        {
            fileName = fr->getLocalPath();
            fr->setRealPath(fileName);
        }
        else if(FileUtils::IsDirectory(localFile))
        {
            fileName = fr->getLocalPath() + string(PATH_SEPARATOR) + FileUtils::GetFileName(idRes.getPath());
            ofstream outbin(fileName.data(), ios::out | ios::binary | ios::trunc);
            if(outbin.is_open() && outbin.good())
            {
                fr->setRealPath(fileName);
                outbin.close();
            }
        }
        else
        {
            cout << "Error: path not avaiable:" <<localFile <<endl;
        }
        break;
    case (int)IdRes::TYPE_DATA:
        if(fr->getRealPath() != "")
        {
            ofstream outbin1(fr->getRealPath().data(), ios::out | ios::binary | ios::app);
            if(outbin1.is_open() && outbin1.good())
            {
                outbin1.write(idRes.getData().data(), idRes.getData().size());
                outbin1.close();
            }
        }
        else
        {
            cout << "Error: Can't open file to write \n";
        }
        break;
    case (int)IdRes::TYPE_FINISH:
        fr -> setFinish(idRes.getFinish());
        owner = idRes.getOwner();
        semPost(owner);
        break;
    }
}
void KP_Manager::on_message(connection_ptr con,message_ptr msg) {
    if (msg->get_opcode() != websocketpp::frame::opcode::TEXT) {
        cout << "Message type from server is not TEXT"  << endl;
        return;
    }
    
    unsigned int pos1 = msg->get_payload().find(TAG_DATA_STR);
    if(pos1 == UINT_MAX)
    {
        std::cout << "message from server " << con << ": " << msg->get_payload() << std::endl;
    }
    std::string json = msg->get_payload();
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

    //std::string jc = n.write_formatted();
    //std::cout<<"JSONNode content:"<<std::endl<<jc<<std::endl;
    
    processMessage(n);

}

void KP_Manager::send(const std::string &msg) {


    if (!m_con)
    {
        std::cerr << "Error: no connected session" << std::endl;
        return;
    }

    if (msg == "/close")
    {
        close();
    }
    else
    {
        m_con -> send(msg);
    }
}

void KP_Manager::sendBinary(const string& binary)
{
	if (!m_con) {
        std::cerr << "Error: no connected session" << std::endl;
        return;
    }
	m_con->send(binary, websocketpp::frame::opcode::BINARY);
}

void KP_Manager::close() {
    if (!m_con) {
        std::cerr << "Error: no connected session" << std::endl;
        return;
    }
    m_con->close(websocketpp::close::status::GOING_AWAY,"");
}


int KP_Manager::semTimedWait(sem_t & sem)
{
    struct timespec tm;
    clock_gettime(CLOCK_REALTIME, &tm);
    tm.tv_sec += REQUEST_TIMEOUT_SECONDS;
    tm.tv_nsec += REQUEST_TIMEOUT_NSECONDS;
    return sem_timedwait(&sem, &tm);
}

int KP_Manager::semTimedWait(string & threadId)
{
    sem_t sem;
    sem_init(&sem, 0, 0);
    struct timespec tm;
    clock_gettime(CLOCK_REALTIME, &tm);
    tm.tv_sec += REQUEST_TIMEOUT_SECONDS;
    tm.tv_nsec += REQUEST_TIMEOUT_NSECONDS;

    sems.insert(std::pair<string, sem_t>(threadId, sem));

    int result = sem_timedwait(&sems[threadId], &tm);

    sems.erase(threadId);

    cout << "Bp: semTimedWait:result:" << result << endl;
    return result;
}

void KP_Manager::semPost(string &owner)
{
    //cout << "Bp: semPost: owner:" << owner << endl;
    std::map<string,sem_t>::iterator it = sems.find(owner);
    if(it != sems.end())
    {
        sem_post(&sems.at(owner));
    }
    else
    {
        cout << "Bp: semPost: owner not found" << endl;
    }
}

void KP_Manager::getThreadId(string &threadId)
{
    threadId = "";
    unsigned int threadIdUint = pthread_self();
    threadId = StringUtils::UintToStr(threadIdUint);
}


/*	{ "msg": "connect", "user": userID, "pwd": pwd }	*/
bool KP_Manager::authorize()	throw (KP_Exception* )
{
    string threadId;
    getThreadId(threadId);

    if(_threadDatas.find(threadId) == _threadDatas.end())
    {
        //cout << "Bp: authorize: threadId:" << threadId << endl;
        _threadDatas.insert(pair<string,ThreadData>(threadId,ThreadData()));
    }
    else
        _threadDatas[threadId].services.clear();

    if(_isLogined)
    {
        throw new KP_AuthorizationException(EXCEPTION_HAVE_ACCOUNT_ALREADY_LOGINED_MSG);
    }

	bool isValid = false;

    if(threadId == "")
    {
        cout << "Error: getCurrentThreadId fail." << endl;
        return false;
    }

	try {
        JSONNode n(JSON_NODE);
        n.push_back(JSONNode(TAG_OWNER_STR, threadId));
        n.push_back(JSONNode(TAG_MSG_STR, REQ_CONNECT_STR));
        n.push_back(JSONNode(TAG_USER_ID_STR, this-> _userId));
        n.push_back(JSONNode(TAG_PWD_STR, this -> _pwd));

        std::string msg = n.write_formatted();

        /* Send authorize message */
        //std::cout << "Msg: " << msg << std::endl;
        this -> send(msg);

        if(semTimedWait(threadId))
        {
            throw new KP_NetworkException(EXCEPTION_REQUEST_TIME_OUT_MSG);
        }
		
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        throw new KP_NetworkException(e.what());
    }
    
	isValid = _isLogined;
    return isValid;
}

/*	{ "msg": "logout"}	*/
bool KP_Manager::logout() throw (KP_Exception* )
{
	bool result = false;
    if(!_isLogined)
    {
        result = true;
        return result;
    }
    else
        _isLogined = false;
	try
	{	
        string threadId;
        getThreadId(threadId);
        if(threadId == "")
        {
            cout << "Error: getCurrentThreadId fail." << endl;
            return false;
        }

        JSONNode n(JSON_NODE);
        n.push_back(JSONNode(TAG_OWNER_STR, threadId));
        n.push_back(JSONNode(TAG_MSG_STR, REQ_LOGOUT_STR));
        std::string msg = n.write_formatted();
        this -> send(msg);

//        //sem_wait(&sem_conn);
//        if(semTimedWait(threadId))
//        {
//            throw new KP_NetworkException(EXCEPTION_REQUEST_TIME_OUT_MSG);
//        }
        _isLogined = false;
        result = true;
	}
	catch(std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
        throw new KP_NetworkException(e.what());
	}
    return result;
}

/*	{ "msg": "findServices", ["provider": providerID] }	*/
std::list<KP_Service> KP_Manager::findServices(std::string providerID) throw (KP_Exception*)
{
    string threadId;
    getThreadId(threadId);

    if(_threadDatas.find(threadId) == _threadDatas.end())
    {
        cout << "Bp: findServices: threadId:" << threadId << endl;
        _threadDatas.insert(pair<string,ThreadData>(threadId,ThreadData()));
    }
    else
        _threadDatas[threadId].services.clear();

    if(!_isLogined)
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);

    /// Clean list result
    //_services.clear();

    setApi(FIND_SERVICES);
    setError(ERR_UNKNOW);

	std::list<KP_Service > services;

	try
    {
        if(threadId == "")
        {
            cout << "Error: getCurrentThreadId fail." << threadId << endl;
        }

        JSONNode n(JSON_NODE);
        n.push_back(JSONNode(TAG_OWNER_STR, threadId));
		n.push_back(JSONNode(TAG_MSG_STR, TAG_FIND_SERVICES_STR));
		if(providerID != EMPTY_STR)
		{
			n.push_back(JSONNode(TAG_PROVIDER_STR, providerID));
		}
		
		std::string msg = n.write_formatted();
		std::cout << "Msg: " << msg << std::endl;
		this -> send(msg);
        if(semTimedWait(threadId) < 0)
        //if(semTimedWait(sem_findServices) < 0)
            throw new KP_NetworkException(EXCEPTION_REQUEST_TIME_OUT_MSG);

        services = _threadDatas[threadId].services;
	}
	catch(std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
        throw new KP_NetworkException(e.what());
	}

    //switch((int)_error)
    switch((int)getError(threadId))
    {
    case (int)ERR_INVALID_ACCESS:
        throw new KP_InvalidAccessException(EXCEPTION_INVALID_ACCESS_MSG);
        break;
    case (int)ERR_NO_AUTHENTICATION:
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);
        break;
    }

	return services;
}

/*	{ "msg": "getService”, “service”: serviceID }	*/
KP_Service* KP_Manager::getService(std::string serviceId)throw (KP_Exception*)
{
    string threadId;
    getThreadId(threadId);

    if(_threadDatas.find(threadId) == _threadDatas.end())
    {
        _threadDatas.insert(pair<string,ThreadData>(threadId,ThreadData()));
    }
    else
    {
        if(_threadDatas[threadId].service != NULL)
        {
            delete _threadDatas[threadId].service;
            _threadDatas[threadId].service = NULL;
        }
    }

    if(!_isLogined)
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);

    setApi(GET_SERVICE);
    setError(ERR_UNKNOW);

    KP_Service* _service = NULL;

	try
    {
        JSONNode n(JSON_NODE);
        n.push_back(JSONNode(TAG_OWNER_STR, threadId));
		n.push_back(JSONNode(TAG_MSG_STR, REQ_GET_SERVICE_STR));
		n.push_back(JSONNode(TAG_SERVICE_STR, serviceId));
		std::string msg = n.write_formatted();
		std::cout << "Msg: " << msg << std::endl;

        this -> send(msg);

         if(semTimedWait(threadId) < 0)
            throw new KP_NetworkException(EXCEPTION_REQUEST_TIME_OUT_MSG);

        _service = _threadDatas[threadId].service;
        if(_service != NULL && _service -> getProviderId() != DEFAULT_SERVICE_ID)
        {
            _service -> setMan(this);
        }

	}
	catch(std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
        throw new KP_NetworkException(e.what());
    }

    switch((int)getError(threadId))
    {
    case (int)ERR_INVALID_ACCESS:
        throw new KP_InvalidAccessException(EXCEPTION_INVALID_ACCESS_MSG);
        break;
    case (int)ERR_NO_AUTHENTICATION:
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);
        break;
    }
    return _service;
}

/* 	{ “msg”: “getRunMethodsFile”, “service”: serviceID }	*/
bool KP_Manager::getRunMethodsFile(std::string serviceId, std::string path) throw (KP_Exception*)
{
    if(!_isLogined)
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);

    //api = GET_RUN_METHOD_FILE;
    setApi(GET_RUN_METHOD_FILE);
    //_error = ERR_UNKNOW;
    setError(ERR_UNKNOW);

    bool result = false;
    std::string fileName;
//    try
//    {

    if(path == "")
    {
        throw new KP_IOException(EXCEPTION_DIRECTORY_NOT_EXIST_MSG);
    }
    bool isFile = false, isDir = false;
    if(!FileUtils::IsDirectory(path))
    {
        string dir = path.substr(0,path.find_last_of("/"));
        if(FileUtils::IsDirectory(dir))
        {
            fileName = FileUtils::GetFileName(path);
            if(fileName != "")
            {
                ofstream outbin0(path.data(), ios::out | ios::binary | ios::trunc);
                if(outbin0.is_open() && outbin0.good())
                {
                    isFile = true;
                    outbin0.close();
                }
            }
        }
        else
        {
            cout << "Error: path not exist:dir:" << dir << endl;
            throw new KP_IOException(EXCEPTION_DIRECTORY_NOT_EXIST_MSG);
        }
    }
    else
    {
        isDir = true;
    }
    if(isFile || isDir)
    {
        string threadId;
        getThreadId(threadId);

        JSONNode n(JSON_NODE);
        n.push_back(JSONNode(TAG_OWNER_STR, threadId));
        n.push_back(JSONNode(TAG_MSG_STR, REQ_GET_RUN_METHODS_FILE_STR));
        n.push_back(JSONNode(TAG_SERVICE_STR, serviceId));

        FileRecieve fc;
        fc.setLocalPath(path);

        _files.insert(pair<string,FileRecieve>(threadId, fc));

        /// Send and wait
        std::string msg = n.write_formatted();
        std::cout << "Msg: getRunMethodsFile\n" << msg << std::endl;
        this -> send(msg);
        semTimedWait(threadId);

        /// Continue
        IdRes::FinishStates state = _files[threadId].getFinish();
        if(state == IdRes::FINISH_SUCCESS)
            result = true;

        _files.erase(threadId);
    }
    else
    {
        throw new KP_IOException(EXCEPTION_DIRECTORY_NOT_EXIST_MSG);
    }

//    }
//    catch(std::exception& e) {
//        std::cerr << "Exception: " << e.what() << std::endl;
//        throw new KP_NetworkException(e.what());
//    }

    switch((int)getError())
    {
    case (int)ERR_INVALID_ACCESS:
        throw new KP_InvalidAccessException(EXCEPTION_INVALID_ACCESS_MSG);
        break;
    case (int)ERR_NO_AUTHENTICATION:
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);
        break;
    }

    return result;
}

/*
{ “msg”: “submitJob”,
         “service”: serviceID,
         “runMethod”: runMethodID,
         “params”: {
            “1”: “out.txt”,
            “2”: 4.5,
            “3”: 5,
        }
}
*/
string* KP_Manager::submitJob(std::string serviceId, std::string runMethodId, KP_ParametersList* pars) throw (KP_Exception*)
{
    string threadId;
    getThreadId(threadId);

    if(_threadDatas.find(threadId) == _threadDatas.end())
    {
        cout << "Bp: findServices: threadId:" << threadId << endl;
        _threadDatas.insert(pair<string,ThreadData>(threadId,ThreadData()));
    }
    else
    {
        _threadDatas[threadId].ack = EMPTY_STR;
        if(_threadDatas[threadId].job != NULL)
        {
            delete _threadDatas[threadId].job;
            _threadDatas[threadId].job = NULL;
        }

    }

    if(!_isLogined)
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);

    setJob(NULL);

    string empty = EMPTY_STR;
    setRequestId(empty);

    string* jobID = NULL;

    //api = SUBMIT_JOB;
    setApi(SUBMIT_JOB);

    //cout << "Bp: submitJob 01\n";
    //_error = ERR_UNKNOW;
    setError(ERR_UNKNOW);
    try
    {
        JSONNode n(JSON_NODE);
        n.push_back(JSONNode(TAG_OWNER_STR, threadId));
        n.push_back(JSONNode(TAG_MSG_STR, REQ_SUBMIT_JOB_STR));
        n.push_back(JSONNode(TAG_SERVICE_STR, serviceId));
        n.push_back(JSONNode(TAG_RUN_METHOD_STR, runMethodId));

        JSONNode np(JSON_NODE);
        np.set_name(TAG_PARAMS_STR);

        JSONNode nps(JSON_NODE);
        string jobIdInput, pathStr;
        KP_EntryType type;
        bool isHasSendFile = false;

        std::list<KP_Parameter> *list = pars -> getParameters();
        for (std::list<KP_Parameter>::iterator it = list->begin(); it != list->end(); it++)
        {
            KP_Parameter par = (KP_Parameter)*it;
            switch((int)par.type)
            {
            case (int)KP_Parameter::Int:
                np.push_back(JSONNode(par.name, par.value.intValue));
                break;
            case (int)KP_Parameter::Long:
                np.push_back(JSONNode(par.name, par.value.longValue));
                break;
            case (int)KP_Parameter::Float:
                np.push_back(JSONNode(par.name, par.value.floatValue));
                break;
            case (int)KP_Parameter::Double:
                np.push_back(JSONNode(par.name, par.value.doubleValue));
                break;
            case (int)KP_Parameter::String:
                if(par.value.stringValue != NULL)
                {
                    np.push_back(JSONNode(par.name, * par.value.stringValue));
                }
                else
                    cout << "Par.value.stringvalue is NULL\n";
                break;
            case (int)KP_Parameter::Entry:
                if(par.value.entryValue != NULL)
                {
                    /// Path node
                    nps.set_name(par.name);
                    pathStr = par.value.entryValue -> getPath();

                    nps.push_back(JSONNode(TAG_PATH_STR, FileUtils::GetFileName(pathStr)));

                    /// Type node
                    type = par.value.entryValue -> getType();
                    if(type == LOCAL)
                    {
                        nps.push_back(JSONNode(TAG_TYPE_STR, LOCAL_STR));
                    }
                    else
                    {
                        jobIdInput = par.value.entryValue -> getJobId();
                        nps.push_back(JSONNode(TAG_JOB_STR, jobIdInput.c_str()));
                    }

                    np.push_back(nps);

                    nps.clear();
                }
                else
                    cout << "Par.value.entryValue is NULL\n";
                break;

            case (int) KP_Parameter::Option:
                np.push_back(JSONNode(par.name,""));
                break;

            }
        }

        n.push_back(np);
        std::string msg = n.write_formatted();
        std::cout << "Msg: " << msg << std::endl;
        this -> send(msg);

        if(semTimedWait(threadId) < 0)
        {
            throw new KP_NetworkException(EXCEPTION_REQUEST_TIME_OUT_MSG);
        }

        //string * _requestId = getRequestId();
        string ack = _threadDatas[threadId].ack;
        //if(_requestId != NULL && (*_requestId) != "")
        cout << "Bp: ack:" << ack << endl;
        if(strcmp(ack.c_str(), ACK_OK) == 0)
        {
            Results result = RESULT_FAIL;
            for (std::list<KP_Parameter>::iterator it = list -> begin() ; it != list -> end() ; it++)
            {
                KP_Parameter par = (KP_Parameter)*it;
                if(par.type == KP_Parameter::Entry)
                {
                    if(par.value.entryValue -> getType() == LOCAL)
                    {
                        string localPath = par.value.entryValue -> getPath();
                        cout << "Bp: before send file:" << localPath<< endl;
                        //sendFile(localPath);
                        if(sendLocalFiles(localPath, threadId))
                            result = RESULT_SUCCESS;
                        else
                            result = RESULT_FAIL;
                        isHasSendFile = true;
                    }
                }
            }

            if(isHasSendFile)
            {
                /// Send finish message
                JSONNode n2;
                n2.push_back(JSONNode(TAG_OWNER_STR, threadId));
                //n2.push_back(JSONNode(TAG_ID_STR, *_requestId));
                n2.push_back(JSONNode(TAG_FINISH_STR, (int)result));

                std::string msg2 = n2.write_formatted();
                cout << "Msg2:" << msg2 << endl;

                //api = SUBMIT_JOB_S2;
                setApi(SUBMIT_JOB_S2);
                this -> send(msg2);

                if(result == RESULT_FAIL)
                {
                    setError(ERR_CAN_NOT_OPEN_FILE);
                }
                else if(semTimedWait(threadId) < 0)
                {
                    throw new KP_NetworkException(EXCEPTION_REQUEST_TIME_OUT_MSG);
                }
            }
        }

        //KP_Job * _job = getJob();
        KP_Job * _job = _threadDatas[threadId].job;
        if(_job != NULL && (_job -> getJobId()) != DEFAULT_JOB_ID && (_job -> getJobId()) != EMPTY_STR)
        {
            jobID = new string(_job -> getJobId());
        }

    }
    catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        throw new KP_NetworkException(e.what());
    }

    switch((int)getError())
    {
    case (int)ERR_INVALID_ACCESS:
        throw new KP_InvalidAccessException(EXCEPTION_INVALID_ACCESS_MSG);
        break;
    case (int)ERR_NO_AUTHENTICATION:
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);

    case (int)ERR_CAN_NOT_OPEN_FILE:
        throw new KP_IOException(EXCEPTION_CAN_NOT_OPEN_FILE_OR_DIRECTORY_MSG);

    }

    setApi(IDLE);
    return jobID;
}

bool KP_Manager::sendLocalFiles(std::string& localPath, string& owner)
{
    bool result = false;

    cout <<"LocalPath:" << localPath << endl;
    vector<string> files;
    int count = 0;
    FileUtils::GetAllFileEntryPath(localPath, files, count);

    cout << "Num of file: " << files.size() << endl;
    if(files.size() == 0)
        return false;
    for (vector<string>::iterator it = files.begin() ; it != files.end(); it++)
    {
        cout << "file[i]:" << *it << endl;
        if(!sendFile(localPath, *it, owner))
        {
            /// Send file error
            cout << "Send file: " << *it << " fail !\n";
            return false;
        }
    }
    result = true;
    return result;
}

bool KP_Manager::sendFile(std::string& localPath, std::string& fileName, std::string& owner)
{
    //cout << "Bp: sendFile():" << localPath << ", " << fileName << endl;
    bool result = false;
    ifstream file (fileName.c_str(), ios::in | ios::binary);
    long size = 0;

    /// Create path string to create message
    std::string localPathStr;
    //cout << "From: " << fileName.find_last_of(localPath) + 1 << ", to:" << fileName.size() << endl;

    std::string post = fileName.substr(localPath.length(), fileName.length());

    std::string pre = FileUtils::GetFileName(localPath);
    //cout << "Post:" << post << ", pre:" << pre << endl;

    localPathStr = pre + post;

    if (file.is_open() && file.good() && FileUtils::IsFile(fileName))
    {
        JSONNode nf(JSON_NODE);
        nf.push_back(JSONNode(TAG_OWNER_STR, owner));
        //nf.push_back(JSONNode(TAG_ID_STR, requestId));
        nf.push_back(JSONNode(TAG_PATH_STR, localPathStr));
        string msgw = nf.write();
        cout << "msgw:" << msgw << endl;
        std::string msgf = nf.write_formatted();

        //std::cout << "Msgf: " << msgf << std::endl;
        this -> send(msgf);

        file.seekg(0,ifstream::end);
        size = file.tellg();
        file.seekg(0);

        cout<< "Bp: File size: " << size << endl;
        int total = size / BLOCK_SIZE;

        if(total * BLOCK_SIZE < size)
            total ++;
        cout << "total " << total << endl;

        int index = 0;

        char *data;
        while(!file.eof())
        {
            int rsize = BLOCK_SIZE;
            //cout << "Bp: total: " << total <<", index: "<< index << endl;
            if(index == total)
                break;

            if(index == total - 1)
                rsize = size - index * BLOCK_SIZE;
            if(rsize <= 0 )
                break;

            //cout<< "Bp: Read size: " << rsize << endl;

            data = new char[rsize];

            file.read(data, rsize);

            /* Convert byte array to string */
            std::string str(reinterpret_cast< char const* >(data), rsize);

            /*	Send packets to client */
            //sendBinary(str);
            //cout << "Bp: data send: size = " << str.size() << endl;
            JSONNode n(JSON_NODE);
            n.push_back(JSONNode(TAG_OWNER_STR, owner));
            //n.push_back(JSONNode(TAG_ID_STR, requestId));
            n.push_back(JSONNode(TAG_DATA_STR, str));

            std::string msg = n.write_formatted();

            //std::string msg = n.write();

            //std::cout << "Msg: " << msg << std::endl;

            this -> send(msg);

            delete[] data;
            data = NULL;
            index ++;
        }

        file.close();
        result = true;
    }
    else
    {
        cout << "Can not open file to read: " << fileName << endl;
    }
    return result;
}

/*  { “msg”: “queryStatus”, “job”: jobID }    */
KP_JobStatus KP_Manager::queryStatus(std::string jobId) throw (KP_Exception*)
{
    if(!_isLogined)
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);

    KP_Job * _job = getJob();

    if(_job != NULL)
    {
        delete _job;
        _job = NULL;
    }

    //api = QUERY_STATUS;
    setApi(QUERY_STATUS);
    //_error = ERR_UNKNOW;
    setError(ERR_UNKNOW);

    KP_JobStatus status = JOB_UNKNOW;
    try
    {
        string threadId;
        getThreadId(threadId);
        JSONNode n(JSON_NODE);
        n.push_back(JSONNode(TAG_OWNER_STR, threadId));
        n.push_back(JSONNode(TAG_MSG_STR, REQ_QUERY_STATUS_STR));
        n.push_back(JSONNode(TAG_JOB_STR, jobId));

        std::string msg = n.write_formatted();
        std::cout << "Msg: " << msg << std::endl;

        this -> send(msg);
        if(semTimedWait(threadId) < 0)
        //if(semTimedWait(sem_queryStatus) < 0)
        {
            throw new KP_NetworkException(EXCEPTION_REQUEST_TIME_OUT_MSG);
        }

        if(_job != NULL)
        {
            status = _job -> getStatus();
            if(status == JOB_UNKNOW)
                throw new KP_JobIdNotFoundException(EXCEPTION_JOB_ID_NOT_FOUND_MSG);
        }
    }
    catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        throw new KP_NetworkException(e.what());
    }

    switch((int)getError())
    {
    case (int)ERR_INVALID_ACCESS:
        throw new KP_InvalidAccessException(EXCEPTION_INVALID_ACCESS_MSG);
        break;
    case (int)ERR_NO_AUTHENTICATION:
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);
    }

    return status;
}

/*  { “msg”: “getOutputFile”, “job”: jobID, “path”: entryPath } */
bool KP_Manager::getOutputFile(string jobId, string entryPath, string path) throw (KP_Exception*)
{
    if(!_isLogined)
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);
    //api = GET_OUTPUT_FILE;
    setApi(GET_OUTPUT_FILE);
    //_error = ERR_UNKNOW;
    setError(ERR_UNKNOW);

    bool result = false;
    std::string fileName;
//    try
//    {

    if(path == "")
    {
        throw new KP_IOException(EXCEPTION_DIRECTORY_NOT_EXIST_MSG);
    }
    bool isFile = false, isDir = false;
    if(!FileUtils::IsDirectory(path))
    {
        string dir = path.substr(0,path.find_last_of("/"));
        if(FileUtils::IsDirectory(dir))
        {
            fileName = FileUtils::GetFileName(path);
            if(fileName != "")
            {
                ofstream outbin0(path.data(), ios::out | ios::binary | ios::trunc);
                if(outbin0.is_open() && outbin0.good())
                {
                    isFile = true;
                    outbin0.close();
                }
            }
        }
        else
        {
            cout << "Error: path not exist:dir:" << dir << endl;
            throw new KP_IOException(EXCEPTION_DIRECTORY_NOT_EXIST_MSG);
        }
    }
    else
    {
        isDir = true;
    }
    if(isFile || isDir)
    {
        string threadId;
        getThreadId(threadId);

        JSONNode n(JSON_NODE);
        n.push_back(JSONNode(TAG_OWNER_STR, threadId));
        n.push_back(JSONNode(TAG_MSG_STR, REQ_GET_OUTPUT_FILE_STR));
        n.push_back(JSONNode(TAG_JOB_STR, jobId));
        n.push_back(JSONNode(TAG_PATH_STR, entryPath));


        FileRecieve fc;
        fc.setLocalPath(path);

        _files.insert(pair<string,FileRecieve>(threadId, fc));

        /// Send and wait
        std::string msg = n.write_formatted();
        std::cout << "Msg: getOutputFile\n" << msg << std::endl;
        this -> send(msg);
        semTimedWait(threadId);

        /// Continue
        IdRes::FinishStates state = _files[threadId].getFinish();
        if(state == IdRes::FINISH_SUCCESS)
            result = true;

        _files.erase(threadId);
    }
    else
    {
        throw new KP_IOException(EXCEPTION_DIRECTORY_NOT_EXIST_MSG);
    }

    switch((int)getError())
    {
    case (int)ERR_INVALID_ACCESS:
        throw new KP_InvalidAccessException(EXCEPTION_INVALID_ACCESS_MSG);
        break;
    case (int)ERR_NO_AUTHENTICATION:
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);
    }

    return result;
}

/*   { “msg”: “jobList” }   */
std::list<KP_Job> KP_Manager::jobList() throw (KP_Exception*)
{
    string threadId;
    getThreadId(threadId);

    if(_threadDatas.find(threadId) == _threadDatas.end())
    {
        _threadDatas.insert(pair<string,ThreadData>(threadId,ThreadData()));
    }
    else
        _threadDatas[threadId].jobs.clear();

    if(!_isLogined)
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);

    setApi(JOB_LIST);
    setError(ERR_UNKNOW);

    try
    {
        JSONNode n(JSON_NODE);
        n.push_back(JSONNode(TAG_OWNER_STR, threadId));
        n.push_back(JSONNode(TAG_MSG_STR, REQ_JOB_LIST_STR));
        std::string msg = n.write_formatted();
        std::cout << "Msg: " << msg << std::endl;
        this -> send(msg);

        if(semTimedWait(threadId) < 0)
        //if(semTimedWait(sem_jobList) < 0)
        {
            throw new KP_NetworkException(EXCEPTION_REQUEST_TIME_OUT_MSG);
        }
    }
    catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        throw new KP_NetworkException(e.what());
    }

    switch((int)getError(threadId))
    {
    case (int)ERR_INVALID_ACCESS:
        throw new KP_InvalidAccessException(EXCEPTION_INVALID_ACCESS_MSG);
        break;
    case (int)ERR_NO_AUTHENTICATION:
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);
    }

    return _threadDatas[threadId].jobs;
}

/* { “msg”: “deleteJob”, “job”: jobID } */
KP_JobStatus KP_Manager::deleteJob(string jobId) throw (KP_Exception*)
{
    KP_JobStatus status = JOB_UNKNOW;
    if(!_isLogined)
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);
    //api = DELETE_JOB;
    setApi(DELETE_JOB);

    KP_Job * _job = getJob();
    if(_job != NULL)
    {
        delete _job;
        _job = NULL;
    }

    try
    {
        string threadId;
        getThreadId(threadId);
        JSONNode n(JSON_NODE);
        n.push_back(JSONNode(TAG_OWNER_STR, threadId));
        n.push_back(JSONNode(TAG_MSG_STR, REQ_DELETE_JOB_STR));
        n.push_back(JSONNode(TAG_JOB_STR, jobId));

        std::string msg = n.write_formatted();
        std::cout << "Msg: " << msg << std::endl;

        this -> send(msg);
        if(semTimedWait(threadId) < 0)
        //if(semTimedWait(sem_deleteJob) < 0)
        {
            throw new KP_NetworkException(EXCEPTION_REQUEST_TIME_OUT_MSG);
        }

        if(_job != NULL)
        {
            status = _job -> getStatus();
        }

    }
    catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        throw new KP_NetworkException(e.what());
    }
    return status;
}

/*   { “msg”: “restartJob”, “job”: jobID }  */
KP_JobStatus KP_Manager::restartJob(string jobId) throw (KP_Exception*)
{
    if(!_isLogined)
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);
    setApi(RESTART_JOB);
    KP_JobStatus status = JOB_UNKNOW;

    KP_Job * _job = getJob();
    if(_job != NULL)
    {
        delete _job;
        _job = NULL;
    }

    try
    {
        string threadId;
        getThreadId(threadId);
        JSONNode n(JSON_NODE);
        n.push_back(JSONNode(TAG_OWNER_STR, threadId));
        n.push_back(JSONNode(TAG_MSG_STR, REQ_RESTART_JOB_STR));
        n.push_back(JSONNode(TAG_JOB_STR, jobId));

        std::string msg = n.write_formatted();
        std::cout << "Msg: " << msg << std::endl;

        this -> send(msg);

        if(semTimedWait(threadId) < 0)
        {
            throw new KP_NetworkException(EXCEPTION_REQUEST_TIME_OUT_MSG);
        }

        if(_job != NULL)
        {
            status = _job -> getStatus();
        }

    }
    catch(std::exception& e) {
        setApi(IDLE);
        std::cerr << "Exception: " << e.what() << std::endl;
        throw new KP_NetworkException(e.what());
    }
    return status;
}

/*  { “msg”: “cancelJob”, “job”: jobID }  */
KP_JobStatus KP_Manager::cancelJob(string jobId) throw (KP_Exception*)
{
    if(!_isLogined)
        throw new KP_AuthorizationException(EXCEPTION_NO_ATHORIZATION_MSG);
    setApi(CANCEL_JOB);

    KP_JobStatus status = JOB_UNKNOW;

    KP_Job * _job = getJob();

    if(_job != NULL)
    {
        delete _job;
        _job = NULL;
    }

    try
    {
        string threadId;
        getThreadId(threadId);
        JSONNode n(JSON_NODE);
        n.push_back(JSONNode(TAG_OWNER_STR, threadId));
        n.push_back(JSONNode(TAG_MSG_STR, REQ_CANCEL_JOB_STR));
        n.push_back(JSONNode(TAG_JOB_STR, jobId));

        std::string msg = n.write_formatted();
        std::cout << "Msg: " << msg << std::endl;

        this -> send(msg);
        if(semTimedWait(threadId) < 0)
        {
            throw new KP_NetworkException(EXCEPTION_REQUEST_TIME_OUT_MSG);
        }

        if(_job != NULL)
        {
            status = _job -> getStatus();
        }

    }
    catch(std::exception& e) {
        setApi(IDLE);
        std::cerr << "Exception: " << e.what() << std::endl;
        throw new KP_NetworkException(e.what());
    }
    return status;
}

Apis KP_Manager::getApi()
{
    string threadId;
    getThreadId(threadId);
    if(_threadDatas.find(threadId) != _threadDatas.end())
    {
        return _threadDatas[threadId].api;
    }
    cout << "Error: getApi: by: " << threadId << " is not found" << endl;
    return API_UNKNOW;
}

Apis KP_Manager::getApi(string threadId)
{
    if(_threadDatas.find(threadId) != _threadDatas.end())
    {
        return _threadDatas[threadId].api;
    }
    cout << "Error: getApi: by: " << threadId << " is not found" << endl;
    return API_UNKNOW;
}

void KP_Manager::setApi(Apis api)
{
    string threadId;
    getThreadId(threadId);
    if(_threadDatas.find(threadId) == _threadDatas.end())
    {
        _threadDatas.insert(pair<string,ThreadData>(threadId,ThreadData()));
    }
    _threadDatas[threadId].api = api;
}

void KP_Manager::setError(Errors error)
{
    string threadId;
    getThreadId(threadId);
    setError(threadId, error);
}

void KP_Manager::setError(string threadId, Errors error)
{
    if(_threadDatas.find(threadId) == _threadDatas.end())
    {
        _threadDatas.insert(pair<string,ThreadData>(threadId,ThreadData()));
    }
    _threadDatas[threadId].error = error;
}

Errors KP_Manager::getError()
{
    string threadId;
    getThreadId(threadId);
    return getError(threadId);
}

Errors KP_Manager::getError(string& threadId)
{
    if(_threadDatas.find(threadId) != _threadDatas.end())
    {
        return _threadDatas[threadId].error;
    }
    return ERR_UNKNOW;
}

KP_Job* KP_Manager::getJob()
{
    string threadId;
    getThreadId(threadId);
    return getJob(threadId);
}

KP_Job* KP_Manager::getJob(string& threadId)
{
    if(_threadDatas.find(threadId) != _threadDatas.end())
    {
        return _threadDatas[threadId].job;
    }
    return NULL;
}

void KP_Manager::setJob(KP_Job* job)
{
    string threadId;
    getThreadId(threadId);
    setJob(threadId, job);
}

void KP_Manager::setJob(string& threadId, KP_Job* job)
{
    if(_threadDatas.find(threadId) == _threadDatas.end())
    {
        _threadDatas.insert(pair<string,ThreadData>(threadId,ThreadData()));
    }
    if(job != NULL)
    {
        _threadDatas[threadId].job = new KP_Job(*job);
        //cout << "Bp: setJob: jobid:" <<  _threadDatas[threadId].job -> getJobId() << endl;
    }
    else
        _threadDatas[threadId].job = NULL;
}

string* KP_Manager::getRequestId()
{
    string threadId;
    getThreadId(threadId);
    return getRequestId(threadId);
}

string* KP_Manager::getRequestId(string & threadId)
{
    if(_threadDatas.find(threadId) != _threadDatas.end())
    {
        return &_threadDatas[threadId].requestId;
    }
    return NULL;
}

void KP_Manager::setRequestId(string & requestId)
{
    string threadId;
    getThreadId(threadId);
    setRequestId(threadId, requestId);
}

void KP_Manager::setRequestId(string & threadId, string & requestId)
{
    if(_threadDatas.find(threadId) == _threadDatas.end())
    {
        _threadDatas.insert(pair<string,ThreadData>(threadId,ThreadData()));
    }
    _threadDatas[threadId].requestId = requestId;
}
