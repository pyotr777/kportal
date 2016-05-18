#ifndef TUNNEL_HPP
#define TUNNEL_HPP

#include <iostream>
#include <queue>          // std::queue
#include <map>            // std::map
#include <mutex>
#include <pthread.h>
#include <sstream>
#include "header.hpp"
#include "message.hpp"
#include "system_command_utils.hpp"
#include "../slavedaemon/message_socket.h"
#include "../slavedaemon/socket_exception.h"


//pthread_mutex_t message_queue_mutex;

class Tunnel
{
private:
    int m_listen_port;
    std::queue<Message> m_messages;
    std::map<int,Socket*> m_slavedaemons;
    Socket* m_kdesk_socket = NULL;

public:
    Tunnel();
    ~Tunnel();
    int GetListenPort();
    void ProcessMessage();
    int EnqueueMsg(Message msg);
    ResultCodes ConnectKdeskDaemon(std::string& kdesk_daemon_host, std::string& kdesk_daemon_port);
    ResultCodes Start(std::string& kdesk_acc, std::string& port);
    bool ContainDaemonSlave(int);
    bool AddDaemonSlaveConn(int, Socket*);
    //ResultCodes ConnectSlaveDaemon(char*, char*);
    void DoForwardingMessageFromSlaveDaemon(char*);
};

#endif
