#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cctype>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <map>

#include "header.hpp"
#include "message.hpp"
#include "tunnel.hpp"
#include "../slavedaemon/message_socket.h"


struct ConnectionThreadArg
{
  Tunnel* tunnel;
  Socket* socket;
};

const int BUFFER_SIZE = 512;
const int KP_FORWARD_PORT = 4000;

// A map cotains key-value (port, tunnel) pairs with unikey keys.
std::map<int, Tunnel*> g_tunnels;

//
// The connection handler thread routin decodes incoming requests.
//
void * ConnectionHandler(void *thrd_args);

bool ProcessMessage(Message& msg, Socket& socket);
bool ContainKey(int port);
//
// Main function
//
int main (int argc, char **argv) {
  if (argc < 2) {
      std::cout << "kpforwarder <port no>\n";
      return 0;
  }

  // Create a shared tunnel object
  //ConnectionThreadArg* thrd_args = new ConnectionThreadArg();
  //thrd_args->tunnel = new Tunnel();
  //
  // Server socket
  //
  try {
    // Create the socket
    //ServerSocket server ( 30000 );
    Socket server;
    int port = atoi(argv[1]);
    if ( ! server.Create() )
    {
      throw SocketException ( "Could not create server socket." );
    }

    if ( ! server.Bind ( port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }
    puts("bind done");

    if ( ! server.Listen() )
    {
      throw SocketException ( "Could not listen to socket." );
    }
    puts("Waiting for incoming connections...");

    Socket new_sock;
    while ( true ) {
      if ( ! server.Accept ( new_sock ) ) {
          throw SocketException ( "Could not accept socket." );
      }
      std::cout << "socket_id outside thread:" << new_sock.GetSock() << std::endl;
      puts("Connection accepted");
      pthread_t sniffer_thread;
      //thrd_args->socket = &new_sock;

      if (pthread_create( &sniffer_thread , NULL ,  ConnectionHandler , (void*)&new_sock) < 0) {
        perror("could not create thread");
        continue;
      }

      //Now join the thread , so that we dont terminate before the thread
      //pthread_join( sniffer_thread , NULL);
      puts("Handler assigned");
    }

  } catch ( SocketException& e ) {
    std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
  }

  // Release pointers
  std::map<int, Tunnel*>::iterator itr = g_tunnels.begin();
  while (itr != g_tunnels.end()) {
     std::map<int, Tunnel*>::iterator toErase = itr;
     delete toErase->second;
     ++itr;
     g_tunnels.erase(toErase);
  }
  return 0;
}

//
// This will handle connection for each command
// Format message: (Type message) - 1byte, (size of message) - sizeof(BYTE), (content) - ...
//

void * ConnectionHandler(void *thrd_args) {
  //ConnectionThreadArg* args = (ConnectionThreadArg*) thrd_args;

  //Get the socket descriptor
  //Socket* socket = args -> socket;
  Socket* socket = reinterpret_cast<Socket*>(thrd_args);
  //int read_size, miss_size;
  //char buffer[BUFFER_SIZE];

  // Receive a message from client
  try {
    while (true) {
      Message rec_msg;
      int recv_size = socket->Recv(rec_msg);
      if(recv_size < 0)
        continue;
      std::cout << "Received signal from client\n";
      if(ProcessMessage(rec_msg, *socket)){
        break;
      }
    }
  } catch ( SocketException& e) {
    std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
  }
  return NULL;
}

bool ProcessMessage(Message& msg, Socket& socket){
  std::cout << "ProcessMessage" << std::endl;;
  bool isFinish = true;
  //if(msg.GetHeader()->GetToPort() == 0){
    int argc = 0;
    //char ** argv = msg.GetCmdArgs(argc);
    std::vector<std::string> argv = msg.GetCmdArgs();
    int port;
    Tunnel* tunnel = new Tunnel();
    Message res_msg = Message(Header(MT_COMMAND, KP_FORWARD_PORT, 0, 0, CMD_ACK_OK), NULL);
    switch(msg.GetHeader()->GetCommand()){
      case CMD_CREATETUNNEL:
        // Args: <kdesk account> <port>
        // Get tunnel port, kdesk account
        std::cout << "CMD_CREATETUNNEL: \n";
        if(argc >= 2){
          port = atoi(argv[1].c_str());
          if(!ContainKey(port)){
            if(tunnel->Start(argv[0], argv[1]) == OK){

              std::cout << "tunnel was created\n";
              g_tunnels.insert(std::pair<int, Tunnel*>(port, tunnel));
              res_msg.GetHeader()->SetCommand(CMD_ACK_OK);
            } else {
              res_msg.GetHeader()->SetCommand(CMD_ACK_ERR);
              res_msg.AddArgv(TUNNEL_START_ERR);
            }
          } else {
            std::cout << "tunnel existed\n";
            res_msg.GetHeader()->SetCommand(CMD_ACK_OK);
          }
        } else {
          std::cout << "Miss a command argument\n";
          res_msg.GetHeader()->SetCommand(CMD_ACK_ERR);
          res_msg.AddArgv(TUNNEL_START_ERR);
        }
        socket.Send(res_msg);
        while(true){
          sleep(1);
        }
        break;
        /*
      case CMD_SLAVEDAEMON_CONNECT:
        // Args: <tunnel port> <slave daemon ip> <slave daemon port>
        // Get tunnel port, kdesk account
        std::cout << "CMD_SLAVEDAEMON_CONNECT:\n";
        port = atoi(argv[0]);
        if(!ContainKey(port)){
          if(g_tunnels[port] -> ConnectSlaveDaemon(argv[1], argv[2])){
            res_msg.GetHeader()->SetCommand(CMD_ACK_OK);
          } else {
            std::cout << "connect fail\n";
            res_msg.GetHeader()->SetCommand(CMD_ACK_ERR);
            res_msg.AddArgv(FORWARDER_CONNECTTO_SLAVEDAEMON_ERR);
          }
        } else {
          std::cout << "connection is existed\n";
          res_msg.GetHeader()->SetCommand(CMD_ACK_ERR);
          res_msg.AddArgv(FORWARDER_CONNECTTO_SLAVEDAEMON_ERR);
        }
        socket.Send(res_msg);
        if(res_msg.GetHeader()->GetCommand() == CMD_ACK_OK){
          g_tunnels[port] -> DoForwardingMessageFromSlaveDaemon(argv[2]);
        }
        break;*/
      default:
        std::cout << "Unknown message: " << res_msg.GetHeader()->GetCommand() << std::endl;
        break;
    }
    //msg.FreeParsedArgs(argv);
  /*} else {
    int to_tunnel_port = msg.GetHeader()->GetToPort();
    if(ContainKey(to_tunnel_port)){
      g_tunnels[to_tunnel_port] -> EnqueueMsg(msg);
      g_tunnels[to_tunnel_port] -> ProcessMessage();
    }
  }*/
  return isFinish;
}

bool ContainKey(int port){
  if ( g_tunnels.find(port) == g_tunnels.end() ) {
    // not found
    return false;
  } else {
    // found
    return true;
  }
}
