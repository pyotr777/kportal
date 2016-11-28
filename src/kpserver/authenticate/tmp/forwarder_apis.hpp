#ifndef FORWARDER_APIS_HPP
#define FORWARDER_APIS_HPP
#include <stdio.h>
#include<stdlib.h>	//strlen
#include <sys/types.h>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr

//
// List of Forwarder Process communicator apis
//
class ForwarderApis
{
private:
    static ForwarderApis * m_instance;
    int m_listen_port = 8001;
    pid_t m_process_id;
    int m_forwarder_sock;
    int m_control_sock;
private:
    ForwarderApis();
    //void * ForwarderApis::ConnectionHandler(void *socket_desc);
public:
    ~ForwarderApis();
    int StartListen();
    static ForwarderApis *GetInstance();
    bool IsStarted();
};
#endif
