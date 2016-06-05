/*
 * Copyright (c) 2011, Peter Thorson. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the WebSocket++ Project nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL PETER THORSON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef K_SERVER_HANDLER_HPP
#define K_SERVER_HANDLER_HPP

#include <semaphore.h>
#include <signal.h>
#include <map>
#include <string>
#include <vector>
#include <sys/time.h>
#include <pthread.h>
#include <time.h>

#include "../../include/websocketpp-0.2.3-dev/src/endpoint.hpp"
#include "../../include/websocketpp-0.2.3-dev/src/roles/client.hpp"
#include "../../include/websocketpp-0.2.3-dev/src/md5/md5.hpp"
#include "../../include/websocketpp-0.2.3-dev/src/sockets/tls.hpp"
#include "../../include/websocketpp-0.2.3-dev/src/websocketpp.hpp"
#include "../../include/libjson/libjson.h"

#include "Constants.hpp"
#include "TagKeys.hpp"
#include "ReqType.hpp"
#include "KP_Service.hpp"
#include "KP_Provider.hpp"
#include "../datalayer/service.hpp"
#include "../datalayer/user.hpp"
#include "ReqUtils.hpp"
#include "FileUtils.hpp"
#include "ConnectReq.hpp"
#include "GmailUtils.hpp"
#include "ClientSession.hpp"
#include "MessageHeader.hpp"
#include "LoginManager.hpp"
#include "UserInfoObject.hpp"
#include "JsonUtils.hpp"

#define TLS_CERT_CHAIN_FILE "/etc/kportal/ssl/server.crt"
#define TLS_PRIVATE_KEY_FILE "/etc/kportal/ssl/server.key"
#define TLS_TMP_DH_FILE "/etc/kportal/ssl/dh.pem"

extern pthread_mutex_t g_connections_lock;

using websocketpp::server;
using websocketpp::server_tls;

namespace websocketkserver {

enum Results
{
    FAIL = 0,
    SUCCESS
};

enum Errors
{
    ERR_UNKNOW,
    ERR_NO_AUTHENTICATION,
    ERR_INVALID_ACCESS
};

template <typename request_endpoint_type>
class request {
public:
    typedef typename request_endpoint_type::handler::connection_ptr connection_ptr;
    typedef typename request_endpoint_type::handler::message_ptr message_ptr;

    //sem_t *mutex;
    connection_ptr con;
    message_ptr msg;
    ClientSession* cs;
    LoginManager* lm;

//	string request_id;
//	long m_begin;
//	long m_end;
	//request();
	~request(){
		//std::cout << "Request destructor!" << std::endl;
	}
	void process();

private:
    void convertStandardStr(std::string &string_src);
    //JSONNode toJSONNode(Service& service);
    //JSONNode toJSONNode(Parameter param);
    //JSONNode toJSONNode(Job job);
    //JSONNode toJSONNode(KP_Provider );
    bool sendFile(std::string& fileName,/* std::string& requestId,*/ MessageHeader& header);
    void processId(MessageHeader& header, JSONNode::const_iterator& i, JSONNode & n);

    //void sendError(std::string requestId, int errorCode);
    //void sendError(MessageHeader& header, int errorCode);
	void sendResult(MessageHeader& header, int resultCode);
	bool checkResourceJob(Job new_job, Job old_job);

	void insertService(MessageHeader& header,Service &service);
	void editService(MessageHeader& header,Service &service);
    void responseCreateService(MessageHeader &header, std::string serviceId, ResponseCode &ret);

	void requestUploadFile(MessageHeader& header, std::string fileName);
	void requestUploadFileParam(MessageHeader& header, param_job param);
    void responseCreateImage(MessageHeader &header, std::string& imageId, ResponseCode &ret);
	void responseSubmitJob(MessageHeader &header, std::string jobId, ResponseCode &ret);
    bool authorizeUserService(MessageHeader& header, JSONNode::const_iterator i, JSONNode& n);
    //std::string encode_message(std::string sender, std::string msg, bool escape = true);
    //long caculate_time();
    //long get_current_time();

    void processBinaryData();
    void processConnectRequest(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processLogoutRequest(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processFindServices(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processGetAllServices(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processGetService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processGetParametersOfService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);

    void processJobList(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processGetInfomationJobsOfUserByService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processGetDetailInfomationJob(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processSubmitJob(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processResubmitJob(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processQueryStatus(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processDeleteJob(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processCancelJob(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processGetEntriesOfJob(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processGetOutputFile(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);

    void processGetRegisterServicesOfUser(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processCancelRegisterServiceByUser(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processUserRegisterService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processUserRemoveService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);

    void processRegisterUsersOfService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processGetServicesOfProvider(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processGetUsersOfService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processGetServicesHaveCandidates(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processAddUserToService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processRemoveUserFromService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processRemoveCandidateFromService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);

	void processCreateService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processUpdateService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processDeleteService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processUploadIconStep(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processCheckPathOfService(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
	void processCheckServiceNameUsable(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);

	void processGetProviders(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
	void processGetProviderRegisters(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
    void processAcceptProviderRegisters(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
	void processRejectProviderRegisters(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);

	void processRejectProviders(MessageHeader& header, JSONNode::const_iterator& i, JSONNode & n);

  void processUpdateProviderTimeUsable(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
	void processRegisterProvider(MessageHeader& header, JSONNode::const_iterator& i, JSONNode & n);
	void processRejectProvider(MessageHeader& header, JSONNode::const_iterator& i, JSONNode & n);

	void processFindImages(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
	void processCreateImage(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
	void processUpdateImage(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
	void processDeleteImage(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
	void processGetImageFile(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);
	void processCheckImageNameUsable(MessageHeader& header, JSONNode::const_iterator& i, JSONNode& n);

  void processGetProviderTimeResource(MessageHeader& header, JSONNode::const_iterator& i, JSONNode & n);
  void processUpdateProviderUsableTime(MessageHeader& header, JSONNode::const_iterator& i, JSONNode & n);

};

// The coordinator is a simple wrapper around an STL queue. add_request inserts
// a new request. get_request returns the next available request and blocks
// (using condition variables) in the case that the queue is empty.
template <typename endpoint_type>
class request_coordinator {
public:
    void add_request(const request<endpoint_type>& r) {
      boost::unique_lock<boost::mutex> lock(m_lock);
      m_requests.push(r);
      lock.unlock();
      m_cond.notify_one();
    }
    
    void get_request(request<endpoint_type>& value) {
      std::cout << " get_request start lock\n";
      boost::unique_lock<boost::mutex> lock(m_lock);
      while (m_requests.empty()) {
        m_cond.wait(lock);
      }

      std::cout << " get_request unknock lock\n";
      value = m_requests.front();
      m_requests.pop();

      std::cout << " get_request finish\n";
    }
private:
    std::queue<request<endpoint_type> > m_requests;
    boost::mutex                m_lock;
    boost::condition_variable   m_cond;
};

template <typename endpoint_type>
class k_server_handler : public endpoint_type::handler {
/*
 * Fields
 */
public:
    typedef k_server_handler<endpoint_type> type;
    typedef typename endpoint_type::handler::connection_ptr connection_ptr;
    typedef typename endpoint_type::handler::message_ptr message_ptr;

private:
    /// Coordinator client request
    request_coordinator<endpoint_type>& m_coordinator;

    /// List of client connections
    std::map<connection_ptr, ClientSession> m_connections;

/*
 * Methods
 */
public:
    std::string get_password() const {
        return "test";
    }
    
    boost::shared_ptr<boost::asio::ssl::context> on_tls_init() {
        std::cout << "Bp: on_tls_init start\n";
        // create a tls context, init, and return.
        boost::shared_ptr<boost::asio::ssl::context> context(new boost::asio::ssl::context(boost::asio::ssl::context::tlsv1));
        try {
          context->set_options(boost::asio::ssl::context::default_workarounds |
                               boost::asio::ssl::context::no_sslv2 |
                               boost::asio::ssl::context::single_dh_use);
//          context->set_options(boost::asio::ssl::context::default_workarounds);
            context->set_password_callback(boost::bind(&type::get_password, this));
            context->use_certificate_chain_file(TLS_CERT_CHAIN_FILE);
            context->use_private_key_file(TLS_PRIVATE_KEY_FILE, boost::asio::ssl::context::pem);
            context->use_tmp_dh_file(TLS_TMP_DH_FILE);

        } catch (std::exception& e) {
          std::cout << "Exception caught: ";
          std::cout << e.what() << std::endl;
        }
        std::cout << "Bp: on_tls_init end\n";
        return context;
    }

    void http(connection_ptr con) {
        con->set_body("<!DOCTYPE html><html><head><title>WebSocket++ TLS certificate test</title></head><body><h1>WebSocket++ TLS certificate test</h1><p>This is an HTTP(S) page served by a WebSocket++ server for the purposes of confirming that certificates are working since browsers normally silently ignore certificate issues.</p></body></html>");
    }

    k_server_handler(request_coordinator<endpoint_type>& c) : m_coordinator(c) {};

    void load_data();

    ///void validate(connection_ptr con);
    
    /*
     *  Add new connection to the lobby
     */
    void on_open(connection_ptr con){
        std::cout << "client " << con << " joined the lobby." << std::endl;
        //m_connections.insert(std::pair<connection_ptr,std::string>(con,get_con_id(con)));
		ClientSession cs;
		std::string str = get_con_id(con);
		cs.setConId(str);
		pthread_mutex_lock(&g_connections_lock);
		m_connections.insert(std::pair<connection_ptr, ClientSession>(con, cs));
		pthread_mutex_unlock(&g_connections_lock);

    };

    /*
     *  Someone disconnected from the lobby, remove them
     */
    void on_close(connection_ptr con){
        //typename std::map<connection_ptr,std::string>::iterator it = m_connections.find(con);
	pthread_mutex_lock(&g_connections_lock);
        typename std::map<connection_ptr,ClientSession>::iterator it = m_connections.find(con);
	pthread_mutex_unlock(&g_connections_lock);
        if (it == m_connections.end()) {
            // this client has already disconnected, we can ignore this.
            // this happens during certain types of disconnect where there is a
            // deliberate "soft" disconnection preceeding the "hard" socket read
            // fail or disconnect ack message.
            return;
        }

        std::cout << "client " << con << " left the kpserver." << std::endl;
		/// Clear temp session dir

	//sleep(5);
		std::cout << "Clear connection session: start\n";
	pthread_mutex_lock(&g_connections_lock);
        it = m_connections.find(con);
        if (it == m_connections.end()) {
          m_connections.erase(it);
        }
	pthread_mutex_unlock(&g_connections_lock);
	std::cout << "Clear connection session: end\n";
    };
    
    /*
     *  Process when request message is receieved. Add new message to the queue
     */
    void on_message(connection_ptr con, message_ptr msg){
		std::cout << "ON_MESSAGGE" << std::endl;
		//std::string json = msg->get_payload();
		//std::cout<<"Message clien content:"<<std::endl<<json<<std::endl;;
        request<endpoint_type> r;
        r.con = con;
        r.msg = msg;
pthread_mutex_lock(&g_connections_lock);
        r.cs = &m_connections[con];
pthread_mutex_unlock(&g_connections_lock);
        //generate_id32(r.request_id);
        // LOG4CXX_INFO(logger, "Request: " << r.request_id << " queued");
        std::cout << "Request: " << r.con << " queued" << std::endl;
        m_coordinator.add_request(r);
    };

private:
    std::string serialize_state() {
    	std::stringstream s;

    	s << "{\"type\":\"participants\",\"value\":[";

        typename std::map<connection_ptr,std::string>::iterator it;
	pthread_mutex_lock(&g_connections_lock);
    	for (it = m_connections.begin(); it != m_connections.end(); it++) {
        	s << "\"" << (*it).second << "\"";
        	if (++it != m_connections.end()) {
            		s << ",";
        	}
        	it--;
    	}

    	s << "]}";
	pthread_mutex_unlock(&g_connections_lock);
    	return s.str();
    };

    std::string get_con_id(connection_ptr con){
    	std::stringstream endpoint;
    	//endpoint << con->get_endpoint();
    	endpoint << con;
    	return endpoint.str();
    };

    void send_to_all(std::string data){
        typename std::map<connection_ptr,std::string>::iterator it;
    	for (it = m_connections.begin(); it != m_connections.end(); it++) {
        	(*it).first->send(data);
        }
    };

    /*
    void collect_time(std::string time){
    // LOG4CXX_INFO(logger, "Time to reveive data and send result: " << time);

	std::string outFileName = "network.csv";
	std::ofstream fout(outFileName.c_str(), std::ios::app | std::ios::out );
	if(fout.is_open() && fout.good())
	{
		fout << get_current_time_as_string() << "," << time << endl;
		fout.close();
	}
    };

    std::string get_current_time_as_string(){
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	std::strftime (buffer,80,"%Y-%m-%d-%H-%M-%S", timeinfo);
	puts (buffer);
	std::string timebuf(buffer);	
	return timebuf;
    };
    */
};

}

#include "k_server_handler.cpp"

#endif // K_SERVER_HANDLER_HPP
