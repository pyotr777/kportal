#ifndef KP_MANAGER_HPP
#define KP_MANAGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

#include "../include/websocketpp/src/endpoint.hpp"
#include "../include/websocketpp/src/roles/client.hpp"
#include "../include/websocketpp/src/md5/md5.hpp"
#include "../include/websocketpp/src/sockets/tls.hpp"
#include "../include/websocketpp/src/websocketpp.hpp"

#include "../include/libjson/libjson.h"

#include "IdRes.hpp"
#include "Constants.hpp"

#include "Utils/ReqType.hpp"
#include "Utils/TagKeys.hpp"
#include "KP_Service.hpp"
#include "KP_ParametersList.hpp"

#include "Exceptions/KP_Exception.hpp"
#include "Exceptions/KP_AuthorizationException.hpp"
#include "Exceptions/KP_NetworkException.hpp"
#include "Exceptions/KP_IOException.hpp"
#include "Exceptions/KP_InvalidAccessException.hpp"
#include "Exceptions/KP_InvalidParameterException.hpp"
#include "Exceptions/KP_JobIdNotFoundException.hpp"

#include "FileRecieve.hpp"
#include "ResUtils.hpp"
#include "ReqUtils.hpp"
#include "FileUtils.hpp"


using boost::asio::ip::tcp;
using namespace std;
using websocketpp::client;
using websocketpp::client_tls;
/* Semaphores */
extern sem_t sem_conn;
extern map<string, sem_t> sems;

#define KP_ManagerMC(handler, userId, pwd) \
        KP_Manager_ptr handler (new KP_Manager(userID, pwd));	\
        tls_endpoint_type endpoint(handler);	\
        tls_endpoint_type::connection_ptr _con;	\
        endpoint.alog().unset_level(websocketpp::log::alevel::ALL); \
        endpoint.elog().unset_level(websocketpp::log::elevel::ALL);		\
	    endpoint.elog().set_level(websocketpp::log::elevel::RERROR);	\
	    endpoint.elog().set_level(websocketpp::log::elevel::FATAL);		\
	    _con = endpoint.get_connection(uri);	\
        _con->add_request_header("User-Agent","WebSocket++/0.2.0 WebSocket++/0.2.0");	\
	    _con->set_origin("http://anlab.vn");	\
	    endpoint.connect(_con);	\
        boost::thread t(boost::bind(&tls_endpoint_type::run, &endpoint, false));	\
        sem_init(&sem_conn, 0, 0);          \
        sem_wait(&sem_conn);                \


#define EMPTY_STR  	""
		
typedef boost::thread *PThread;

enum Errors
{
    ERR_UNKNOW,
    ERR_NO_AUTHENTICATION,
    ERR_INVALID_ACCESS,
    ERR_CAN_NOT_OPEN_FILE
};

enum Results
{
    RESULT_FAIL,
    RESULT_SUCCESS
};

enum Apis
{
    API_UNKNOW,
    IDLE,
    GET_SERVICE,
    FIND_SERVICES,
    GET_RUN_METHOD_FILE,
    GET_RUN_METHOD_FILE2,
    GET_OUTPUT_FILE,
    SUBMIT_JOB,
    SUBMIT_JOB_S2,
    JOB_LIST,
    QUERY_STATUS,
    DELETE_JOB,
    RESTART_JOB,
    CANCEL_JOB
};


class ThreadData
{
public:
    ThreadData():error(ERR_UNKNOW), api(API_UNKNOW), job(NULL), requestId(""), ack("")
    {
        services.clear();
        jobs.clear();
        service = NULL;
    }
    ~ThreadData()
    {
        if(job != NULL)
        {
            delete job;
            job = NULL;
        }
        services.clear();
        jobs.clear();

        if(service != NULL)
        {
            delete service;
            service = NULL;
        }
    }
    ThreadData(const ThreadData & copy)
    {
        this -> error = copy.error;
        this -> api = copy.api;
        if(copy.job != NULL)
            this -> job = new KP_Job(*copy.job);
        else
            job = NULL;
        this -> requestId = copy.requestId;
        this -> services = copy.services;
        this -> jobs = copy.jobs;

        if(copy.service != NULL)
            this -> service = new KP_Service(*copy.service);
        else
            this -> service = NULL;

        this -> ack = copy.ack;
    }

    Errors error;
    Apis api;
    KP_Job *job;
    string requestId;
    string ack;

    std::list<KP_Service > services;
    std::list<KP_Job> jobs;
    KP_Service * service;

private:
};


//class KP_Manager : public client::handler {
typedef websocketpp::endpoint<websocketpp::role::client,websocketpp::socket::tls> tls_endpoint_type;
typedef tls_endpoint_type::handler_ptr tls_handler_ptr;
typedef tls_endpoint_type::connection_ptr connection_ptr;

class KP_Manager : public tls_endpoint_type::handler
{
public:

    typedef tls_endpoint_type type;
    typedef tls_endpoint_type::connection_ptr connection_ptr;

    boost::shared_ptr<boost::asio::ssl::context> on_tls_init()
    {
        // create a tls context, init, and return.
        boost::shared_ptr<boost::asio::ssl::context> context(new boost::asio::ssl::context(boost::asio::ssl::context::tlsv1));
        try {
            context->set_verify_mode(boost::asio::ssl::context::verify_peer);

            context->load_verify_file("../include/ssl/server.crt");

        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
        return context;
    }

public:
	//static KP_Manager* getInstance();
	
	KP_Manager() {}
	
	KP_Manager(std::string userId, std::string pwd);

    void setUserId(string userId);

    void setPwd(string pwd);

    void setUri(string uri);

	virtual ~KP_Manager();
    
    void on_fail(connection_ptr con);
    
    // connection to kp server complete
    void on_open(connection_ptr con);

    // connection to kp server closed
    void on_close(connection_ptr con);

    bool writeFile(std::string *fileName, IdRes* idRes);

    // got a new message from server
    void on_message(connection_ptr con, message_ptr msg);
    
    //// CLIENT API ==>
    void send(const std::string &msg);

	void sendBinary(const string& binary);
	
    //bool sendFile(std::string fileName, std::string requestId);
    bool sendFile(std::string& localPath, std::string& fileName, string& owner);

    bool sendLocalFiles(std::string& localPath, string& owner);

    void close();
	
	void processMessage(JSONNode & n);

    void processID(IdRes& idRes);

	bool authorize()	throw (KP_Exception*);
	
	bool logout()	throw (KP_Exception*);
	
	std::list<KP_Service> findServices(std::string providerID = EMPTY_STR) throw (KP_Exception*);
		
	KP_Service* getService(std::string serviceId)throw (KP_Exception*);
	
	bool getRunMethodsFile(std::string serviceId, std::string path) throw (KP_Exception*);
	
    string * submitJob(std::string serviceId, std::string methodId, KP_ParametersList *parameter) throw (KP_Exception*);

    bool sendFile(std::string& localPath, std::string& fileName);

    KP_JobStatus queryStatus(std::string jobId) throw (KP_Exception*);

    bool getOutputFile(std::string jobId, std::string entryPath, std::string localPath) throw (KP_Exception*);

    std::list<KP_Job> jobList() throw (KP_Exception*);

    //bool deleteJob(string jobId, KP_JobStatus & status) throw (KP_Exception*);
    KP_JobStatus deleteJob(string jobId) throw (KP_Exception*);

    KP_JobStatus restartJob(string jobId) throw (KP_Exception*);

    KP_JobStatus cancelJob(string jobId) throw (KP_Exception*);

    /// ===>  CLIENT API


private:
	std::string _recMsg;
	std::string _userId;
	std::string _pwd;
    std::string _uri;

private:
	bool _isLogined;

    std::map<std::string,FileRecieve> _files;

    std::map<std::string, ThreadData> _threadDatas;

    connection_ptr m_con;
private:

    int semTimedWait(sem_t & sem);

    int semTimedWait(string & threadId);

    void semPost(string & owner);

    void getThreadId(string & threadId);

    //void decode_server_msg(const std::string &msg);

    Apis getApi();

    Apis getApi(string threadId);

    void setApi(Apis api);

    void setError(Errors error);

    void setError(string threadId, Errors error);

    Errors getError();

    Errors getError(string& threadId);

    KP_Job* getJob();

    KP_Job* getJob(string& threadId);

    void setJob(KP_Job* job);

    void setJob(string& threadId, KP_Job* job);

    string* getRequestId();

    string* getRequestId(string & threadId);

    void setRequestId(string & requestId);

    void setRequestId(string & threadId, string & requestId);

};
typedef boost::shared_ptr<KP_Manager> KP_Manager_ptr;
#endif //KP_MANAGER_HPP
