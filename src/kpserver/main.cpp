#include <iostream>
#include <semaphore.h>
#include <signal.h>
#include <sys/time.h>
#include <boost/asio.hpp>
#include "job_manager/job_manager.hpp"
#include "k_server_handler.hpp"
#include "authenticate/tunnel_manager.hpp"
//#include "forwarder_apis.hpp"
#include "../../include/websocketpp-0.2.3-dev/src/websocketpp.hpp"
//#include "job/job_manager.h"
//#include "service_management/service_management.hpp"
//#include "node_management/master_control.hpp"
//#include "ldap/service_manager.h"
//#include "common.h"
//#include  <stdio.h>
//#include  <sys/types.h>
//#include  <signal.h>
//#include  <sys/ipc.h>
//#include  <sys/shm.h>
//#include    <fstream>
#include "stdafx.h"
#include "authenticate/LoginManager.hpp"

//#define KPSERVER_TAG            "KPServer"
//#define DATA_TAG                "Data"
//#define ICON_FOLDER_TAG         "IconFolder"
//#define ROOT_DIR_TAG            "RootDir"
//#define APPMARKET_DOMAIN_TAG	"AppMarketDomain"
//#define KPSLAVE             "KPSlave"
//#define SERVICE_DESCRIPTION "ServiceDescription"
//#define ROOT_DIR            "RootDir"               // Jobs Folder
//#define SLAVEROOT_DIR       "SlaveRootDir"
//#define HOSTNAME       "127.0.0.1"
//#define PORT_NUMBER    389//LDAP_PORT

using namespace websocketkserver;
using websocketpp::server;
using websocketpp::server_tls;

//string _ROOT_DIR = "/home/kportal/jobs";
//string USER =	"kportal";
//string PASS =	"123456";

std::string UPLOAD_FILE_REQUEST = "UploadFile:";
std::string DOWNLOAD_FILE_REQUEST = "DownloadFile:";
std::string GETLISTENTRY_REQUEST = "GetListEntry:";
std::string DELETEDIRECTORY_REQUEST = "DeleteDirectory:";
std::string GETFILESIZE_REQUEST = "GetFileSize:";
std::string READFILE_REQUEST = "ReadFile:";
std::string WRITEFILE_REQUEST = "WriteFile:";

std::string DEFAULT_CONFIG_FILE_PATH = "/etc/kportal/kportal_conf.xml";

std::string DEFAULT_HOME_FOLDER = "/etc/kportal";

std::string DEFAULT_TEMPLATE_JOB_SCRIPT_FILE = "/etc/kportal/default_job_script.sh";

std::string DEFAULT_JOB_FOLDER = "/etc/kportal/jobs";

std::string ICON_FOLDER_PATH = "";

std::string APPMARKET_DOMAIN = "";

std::string _KPSERVER_ = "kp_server";

std::string g_admin_email_addr;

std::string g_admin_email_pass;

pthread_mutex_t g_tunnels_lock;

//pthread_mutex_t g_listjobs_lock;

pthread_mutex_t g_connections_lock;
std::string kSshKeyDir = "~/.ssh/kportal/";
std::string kKdeskHost = "k.aics.riken.jp";
std::string kDockerImagesUrl = "images/docker_images/base_image.tar";
int g_kpserver_port = 9001;
std::string g_docker_bridge_host = "10.0.0.1";
int g_job_handler_port = 9999;
/// List sample images
std::map<std::string,ImageDocker> SampleImages;

/// List all jobs on kpserver
std::map<std::string,Job> listJob;

///
std::map<std::string,int> g_tunnels;
unsigned int TunnelManager::tunnel_next_port = 5000;
int TunnelManager::exit_code = 0;

/// User login management singleton
LoginManager g_loginMng;


template <typename endpoint_type>
struct request_data
{
    request_coordinator<endpoint_type> *coordinator;
    LoginManager* login_manager;
    //UploadFileManager* upload_manager;
    //SearchTree *search_tree;
    sem_t *mutex;
};


// process_requests is the body function for a processing thread. It loops
// forever reading requests, processing them serially, then reading another
// request.
template <typename endpoint_type>
void process_requests(struct request_data<endpoint_type> *data);

template <typename endpoint_type>
void process_requests(struct request_data<endpoint_type> *data){
    request_coordinator<endpoint_type> *coordinator = data->coordinator;
    request<endpoint_type> r;
    while (1) {
        coordinator->get_request(r);
        r.lm = data -> login_manager;
        //r.ufm = data -> upload_manager;

		// this free function will be used internally as is
    boost::thread(&request<endpoint_type>::process, r);

//        r.process();
    }
}


void * JobHandlerConnectionHandler(void *thrd_args);
void* StartJobHandler(void * thread_args);


int main(int argc, char* argv[]) {
  // Init mutex
  if (pthread_mutex_init(&g_tunnels_lock, NULL) != 0){
    printf("\n tunnels_lock mutex init failed\n");
    return 1;
  }

  //if (pthread_mutex_init(&g_listjobs_lock, NULL) != 0){
  //  printf("\n listjobs_lock mutex init failed\n");
  //  return 1;
  //}

  if (pthread_mutex_init(&g_connections_lock, NULL) != 0){
    printf("\n connections_lock mutex init failed\n");
    return 1;
  }

    /// Define num of threads, pool threads
    size_t worker_threads = 7;
    size_t pool_threads = 2;

    readConfigFile();

	StartMasterInstance();

	ReloadListJob();

    if (!checkProcess(_KPSERVER_.c_str())) {
        std::cout << "kp_server is running." << std::endl;
        return 0;
    }

    bool tls = false;

    if (argc == 2) {
        // TODO: input validation?
        g_kpserver_port = atoi(argv[1]);
    }

    if (argc == 3) {
        // TODO: input validation?
        g_kpserver_port = atoi(argv[1]);
        tls = !strcmp(argv[2],"-tls");
    }

    // Start forwarder process
    //if(!ForwarderApis::GetInstance() -> IsStarted())
    //{
    //    std::cout << "Can not fork kpforwarder process.\n";
    //    return 1;
    //}

    // Start job handler thread
    std::cout << "Start job handler thread\n";
    pthread_t thread_id;
    if (pthread_create(&thread_id , NULL , StartJobHandler , (void*)&g_job_handler_port) < 0) {
      //perror("could not create thread");
      std::cout << "could not create thread\n";
    }

    // init tunnels

    //TunnelManager::tunnel_next_port = 5000;

    // Start websocketpp
    unsigned short port = (unsigned int)g_kpserver_port;
    std::cout << "Port: " << port << std::endl;
    try {
        if (tls) {
            request_coordinator<server_tls> rc;
            server_tls::handler::ptr handler(new k_server_handler<server_tls>(rc));
            //server_tls::handler::ptr handler(new k_server_handler());
            server_tls endpoint(handler);

            endpoint.alog().unset_level(websocketpp::log::alevel::ALL);
            endpoint.elog().unset_level(websocketpp::log::elevel::ALL);
            endpoint.alog().set_level(websocketpp::log::alevel::CONNECT);
            endpoint.alog().set_level(websocketpp::log::alevel::DISCONNECT);
            endpoint.elog().set_level(websocketpp::log::elevel::RERROR);
            endpoint.elog().set_level(websocketpp::log::elevel::FATAL);

            //std::cout << "Starting secure kpserver on port "
            //          << port << std::endl;
            //endpoint.listen(port);

            std::list<boost::shared_ptr<boost::thread> > threads;
            LoginManager login_manager;
            assert(worker_threads > 0);
            for (size_t i = 0; i < worker_threads; i++)
            {
                struct request_data<server_tls> *req = new request_data<server_tls>();
                req->coordinator = &rc;
                req->login_manager = &login_manager;
                //sem_t mutex;
                //sem_init(&mutex, 0, 1);
                //req->mutex = &mutex;
                threads.push_back(
              boost::shared_ptr<boost::thread>(
                                //new boost::thread(boost::bind(&process_requests, &rc))
                                new boost::thread(boost::bind(&process_requests<server_tls>, req))
                                )
                            );
            }

            std::cout << "Starting kpserver secure (tls) on port "
            << port << " with thread pool size " << pool_threads << " and " << worker_threads << " worker threads." << std::endl;
            //unsigned int kp_port = (unsigned int)g_kpserver_port;
            // Listening
            endpoint.listen(port, pool_threads);
        } else {
            /* Old code
            server::handler::ptr handler(new k_server_handler<server>());
            //server::handler::ptr handler(new k_server_handler());

            server endpoint(handler);

            endpoint.alog().unset_level(websocketpp::log::alevel::ALL);
            endpoint.elog().unset_level(websocketpp::log::elevel::ALL);

            std::cout << "Starting kpserver on port "
            << port << std::endl;
            endpoint.listen(port);
            */
            request_coordinator<server> rc;
            server::handler::ptr handler(new k_server_handler<server>(rc));

            // create a server that listens on port `port` and uses our handler
            server endpoint(handler);

            endpoint.alog().unset_level(websocketpp::log::alevel::ALL);
            endpoint.elog().unset_level(websocketpp::log::elevel::ALL);
            endpoint.alog().set_level(websocketpp::log::alevel::CONNECT);
            endpoint.alog().set_level(websocketpp::log::alevel::DISCONNECT);
            endpoint.elog().set_level(websocketpp::log::elevel::RERROR);
            endpoint.elog().set_level(websocketpp::log::elevel::FATAL);

            std::list<boost::shared_ptr<boost::thread> > threads;
            LoginManager login_manager;
            assert(worker_threads > 0);
            for (size_t i = 0; i < worker_threads; i++)
            {
                struct request_data<server> *req = new request_data<server>();
                req->coordinator = &rc;
                req->login_manager = &login_manager;
                //sem_t mutex;
                //sem_init(&mutex, 0, 1);
                //req->mutex = &mutex;
                threads.push_back(
							boost::shared_ptr<boost::thread>(
                                //new boost::thread(boost::bind(&process_requests, &rc))
                                new boost::thread(boost::bind(&process_requests<server>, req))
                                )
                            );
            }

            std::cout << "Starting kpserver on port "
            << port << " with thread pool size " << pool_threads << " and " << worker_threads << " worker threads." << std::endl;
            //unsigned int kp_port = (unsigned int)g_kpserver_port;
            // Listening
            endpoint.listen(port, pool_threads);
        }

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}

/*
 * Start job handler thread
 */
void* StartJobHandler(void* thread_args){
  int listen_port = *(int*) thread_args;
  std::cout << "StartJobHandler(" << listen_port << ")\n";
  //
  // Server socket
  //
  try {
    Socket server;
    if (!server.Create() ) {
      throw SocketException ( "Could not create server socket." );
    }

    if (!server.Bind(listen_port)) {
      throw SocketException ( "Could not bind to port." );
    }
    std::cout << "bind done" << std::endl;

    if ( ! server.Listen() ) {
      throw SocketException ( "Could not listen to socket." );
    }
    std::cout << "Waiting for incoming connections..." << std::endl;

    while ( true ) {
      Socket *client_sock = new Socket();
      std::cout << "before accept\n";
      //server.accept ( new_sock );
      if ( ! server.Accept(*client_sock )) {
          delete client_sock;
          throw SocketException ( "Could not accept socket." );
      }
      std::cout << "Connection accepted" << std::endl;
      pthread_t sniffer_thread;
      //thrd_args->socket = &client_sock;
      if (pthread_create(&sniffer_thread , NULL , JobHandlerConnectionHandler , (void*)client_sock) < 0) {
        //perror("could not create thread");
        std::cout << "could not create thread\n";
        delete client_sock;
        continue;
      }

      //Now join the thread , so that we dont terminate before the thread
      //pthread_join( sniffer_thread , NULL);
      std::cout <<"Handler assigned" << std::endl;
    }
  } catch ( SocketException& e ) {
    std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
  }
  return NULL;
}


//
// This will handle connection for each command
// Format message: (Type message) - 1byte, (size of message) - sizeof(BYTE), (content) - ...
//
void * JobHandlerConnectionHandler(void *thrd_args) {
  std::cout <<"JobHandlerConnectionHandler";
//  ConnectionThreadArg* args = (ConnectionThreadArg*) thrd_args;
  Socket* slavedaemon_sock = (Socket*) thrd_args;
  std::string kp_jobid, pjm_jobid, pjm_status, pjm_start_date, pjm_duration_time;

  Message notify_msg, res(Header(MT_COMMAND, g_job_handler_port, 0, 0, CMD_ACK_OK), NULL);
  std::vector<std::string> argv;
  if(slavedaemon_sock->Recv(notify_msg) >= 0){
    std::cout << "Receieve a event message from slavedaemon\n";
    if(notify_msg.GetHeader()->GetCommand() == CMD_JOB_STATE){
      argv = notify_msg.GetCmdArgs();
      if(argv.size() > 4){
        kp_jobid = argv[0];
        pjm_jobid = argv[1];
        pjm_status = argv[2];
        pjm_start_date = argv[3];
        pjm_duration_time = argv[4];
        std::cout << pjm_jobid << " " << pjm_status << " " <<  pjm_start_date << " " << pjm_duration_time << std::endl;
        JobManager job_mng;
        bool isFinished = false;
        ResponseCode ret = job_mng.UpdateJobStatus(kp_jobid, pjm_jobid, pjm_status, pjm_start_date, pjm_duration_time, isFinished);
        if(ret == DATA_SUCCESS && isFinished)
          std::cout << "update status success\n";
      } else {
        std::cout << "CMD_JOB_STATE missing arg\n";
      }
    } else if(notify_msg.GetHeader()->GetCommand() == CMD_ACK_ERR){
      argv = notify_msg.GetCmdArgs();
      if(argv.size() > 0){
        JobManager job_mng;
        job_mng.UpdateJobStatusErr(argv[0]);
      }
      std::cout << "slavedaemon get status fail handler exiting!\n";
    }
  } else {
    std::cout << "Socket error. Handler exiting!\n";
  }
  delete slavedaemon_sock;
  return NULL;
}
