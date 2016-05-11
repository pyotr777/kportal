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

#include "../kpforwarder/header.hpp"
#include "../kpforwarder/message.hpp"
#include "../slavedaemon/message_socket.h"
#include "../slavedaemon/socket_exception.h"
#include "../kpforwarder/system_command_utils.hpp"

//#define MAX_NUM_OF_RETRY 15
const char* kHomeJobDIr = "/home/kportal/jobs/";
const int BUFFER_SIZE = 512;
//
// Forwarder socket using for upload file to kdesk
//
Socket* g_kdesk_socket = NULL;
std::string g_kpserver_host;
int g_kpserver_port;

//
// The connection handler thread routin decodes incoming requests.
//
bool ProcessMessage(Message& msg, Socket* socket);
ResultCodes Stagein(std::string& job_id, std::vector<std::string>&argv);
ResultCodes CopyExeFile(std::string& job_id, const std::string&exe_file);
ResultCodes SubmitJob(Socket&, std::string& host, std::string& port, std::string& job_id, std::string& exe_path);
ResultCodes SendSubmitJobCommand(Socket* client_socket, std::string& job_id, std::string& exe_path);
ResultCodes SendInitJobCommand(Socket* client_socket, std::string& job_id);
ResultCodes SendDataToKdeskDaemon(Socket* socket, std::string& job_id);
//ResultCodes WaitJobNotification(Socket& kd_sock, const std::string& job_id);
//ResultCodes WaitJobNotification(Socket& kd_sock, Socket& kp_sock, const std::string& job_id);
ResultCodes WaitKdeskSubmitResult(Socket* kd_sock, const std::string& job_id);
ResultCodes SendMsgToKpServer(const Message& msg);
void * ConnectionHandler(void *thrd_args);

//
// Main function
//
int main (int argc, char **argv) {
  if (argc < 7) {
      std::cout << "Usage: slavedaemon \n argc = " << argc << " is missing arg\n";
      return 0;
  }

  std::string job_id = argv[1];
  std::string kp_host = argv[2];
  int kp_port = atoi(argv[3]);
  int kd_port = atoi(argv[4]);
  int kd_try_max = atoi(argv[5]);
  std::string exe_path = argv[6];
  std::vector<std::string> stgindirs;
  for(int i = 7; i < argc; i ++){
    stgindirs.push_back(argv[i]);
  }

  std::cout << "job id " << job_id << ", kp_host: " << kp_host <<", kp_port: " << kp_port << ", kd_port: "<< kd_port <<  ", kd_retry_max = " << kd_try_max << ", exe_path = " << exe_path << std::endl;
  g_kpserver_host = kp_host; g_kpserver_port = kp_port;

  // Connect kdeskdaemon
  std::cout << "Connect to kdeskdaemon: " << kp_host << ":" << kd_port << std::endl;
  Socket kdesk_socket;
  try {
    if ( ! kdesk_socket.Create() ) {
      throw SocketException ( "Could not create client socket." );
    }
    for(int i = 0 ; i < kd_try_max ; i ++ ){
      if ( ! kdesk_socket.Connect( kp_host, kd_port)) {
        if( i == kd_try_max - 1)
          throw SocketException ( "Could not connect to server." );
        else{
          std::cout << "Sleep 3s, Retry connect " << i << std::endl;
          sleep(3);
        }
      } else {
        std::cout <<"connect success\n";
        break;
      }
    }
  } catch ( SocketException& e ) {
    std::cout << "Exception was caught:" << e.description() << "\n";
    std::cout << "Exiting\n";
    SendMsgToKpServer(Message(Header(MT_COMMAND, 0, 0, 0, CMD_ACK_ERR), NULL));
    return -1;
  }
  // Copy exe service file
  if(CopyExeFile(job_id, exe_path) == OK){
    std::cout << "copy exe file in dirs successfull\n";
  } else {
    std::cout << "copy exe file in dirs fail\n";
  }

  // Copy Stage-in
  std::cout << "CMD_COPY_STAGIN_DIR <dir01> <dir02>...." << std::endl;
  if(stgindirs.size() > 0){
    if(Stagein(job_id, stgindirs) == OK){
      std::cout << "copy stage in dirs successfull\n";
    } else {
      std::cout << "copy stage in dirs fail\n";
    }
  } else {
    std::cout << "Stage-in nothing\n" << std::endl;
  }

  // Connect to kp_server job_handler
  //std::cout << "Connect to kp_server job_handler: " << kp_host << ":" << kp_port << std::endl;
  /*Socket kpserver_socket;
  try {
    if ( ! kpserver_socket.Create() ) {
      throw SocketException ( "Could not create client socket." );
    }
    if ( ! kpserver_socket.Connect(kp_host, kp_port)) {
      throw SocketException ( "Could not connect to server." );
    }
  } catch ( SocketException& e ) {
    std::cout << "Exception was caught:" << e.description() << "\n";
    std::cout << "Exiting\n";
    return -1;
  }*/

  // Submit job
  std::cout << "SubmitJob\n";
  ResultCodes rlt = OK;
  //rlt = SendInitJobCommand(&kdesk_socket, job_id);
  rlt = SendSubmitJobCommand(&kdesk_socket, job_id, exe_path);
  if(rlt == OK){
    rlt = SendDataToKdeskDaemon(&kdesk_socket, job_id);
    if(rlt == OK){
      //rlt = WaitKdeskSubmitResult(&kpserver_socket, &kdesk_socket, job_id);
      rlt = WaitKdeskSubmitResult(&kdesk_socket, job_id);
//      if(rlt == OK){
//        rlt = WaitJobNotification(kdesk_socket, kpserver_socket, job_id);
//      } else {
//        std::cout << "wait kdesk submijob response & send result to kp_server fail.\n";
//      }
    } else {
      std::cout << "send data fail.\n";
    }
  } else {
    std::cout << "send submit job fail\n";
  }
  if(rlt != OK){
    Message err_msg(Header(MT_COMMAND, 0, 0, 0, CMD_ACK_ERR), NULL);
    err_msg.AddArgv(job_id);
    //if(kpserver_socket.Send(err_msg)){
    if(SendMsgToKpServer(err_msg)){
      std::cout << " send error ack success\n";
    } else {
      std::cout << " send error ack fail\n";
    }
    return 1;
  }
  std::cout << "submit job success\n";
  return 0;


  ///////////////////////////////////////////////////////////////////////////////
  /// old code
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
    std::cout << "LISTEN_PORT = " << port << std::endl;
    std::cout << "Home job folder: " << kHomeJobDIr << std::endl;
    if ( ! server.Create() ) {
      throw SocketException ( "Could not create server socket." );
    }

    if ( ! server.Bind ( port ) ) {
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
      //server.accept ( new_sock );
      if ( ! server.Accept(new_sock) ) {
          throw SocketException ( "Could not accept socket." );
      }
      //puts("Connection accepted");
      std::cout << "Connection accepted\n";
      //pthread_t sniffer_thread;
      //thrd_args->socket = new_sock;

      std::cout << "Handler assigned\n";
      ConnectionHandler((void*) &new_sock);
      std::cout << "Exit hanlder connection\n";
      break;

//      if (pthread_create( &sniffer_thread , NULL ,  ConnectionHandler , (void*) &new_sock) < 0) {
//        perror("could not create thread");
//        continue;
//      }

//      //Now join the thread , so that we dont terminate before the thread
//      pthread_join( sniffer_thread , NULL);
//      puts("Handler assigned");
    }
  } catch ( SocketException& e ) {
    std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
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
  Socket* socket = (Socket*) thrd_args;
  //int read_size, miss_size;
  //char buffer[BUFFER_SIZE];

  // Receive a message from client
  try {
    while (true) {
      Message rec_msg;
      int status = socket->Recv(rec_msg);
      if(status < 0){
        std::cout << "socket error";
        throw new SocketException("receieve fail");
      }
      if(status == 0){
        std::cout << "Wait new message.";
        sleep(1);
        continue;
      }
      std::cout << "Received signal from client\n";
      if(ProcessMessage(rec_msg, socket)) {
        break;
      }
    }
  } catch ( SocketException& e) {
    std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
  }
  return NULL;
}


bool ProcessMessage(Message& msg, Socket* socket){
  bool isFinish = true;
  int argc = 0;
  //char ** argv = msg.GetCmdArgs(argc);;
  std::vector<std::string> argv = msg.GetCmdArgs();
  argc = argv.size();
  Header hdr (MT_COMMAND, 0, 0, 0, CMD_ACK_OK);
  Header hdr_err (MT_COMMAND, 0, 0, 0, CMD_ACK_ERR);
  Message res_msg = Message(hdr, NULL);
  switch(msg.GetHeader() -> GetCommand()){
    case CMD_COPY_STAGIN_DIR:
      std::cout << "CMD_COPY_STAGIN_DIR <dir01> <dir02>...." << std::endl;
      if(argc >= 2){
        //if(Stagein(socket, argv) == OK){
          std::cout << "copy stage in dirs successfull\n";
          isFinish = false;
        //} else {
          std::cout << "copy stage in dirs fail\n";
          res_msg.SetHeader(hdr_err);
        //}
      } else {
        std::cout << "Some argument is missing\n" << std::endl;
        //res_msg.AddArgv("Some argument is missing");
      }
      socket->Send(res_msg);
      break;
    case CMD_SUBMITJOB:
      //<host> <port> <job id>
      if(argc >= 4){
        std::cout << "CMD_SUBMITJOB " << argv[0] << ", " << argv[1] << ", " << argv[2] << ", " << argv[3] << std::endl;
        if(SubmitJob(*socket, argv[0], argv[1], argv[2], argv[3]) == OK){
          std::cout << "submit job successfull\n";
        } else {
          hdr.SetCommand(CMD_ACK_ERR);
          res_msg.SetHeader(hdr);
          //res_msg.AddArgv(std::string("Missing argument"));
          std::cout << "Submit job fail\n";
          socket->Send(res_msg);
        }
      } else {
        std::cout << "Some argument is missing\n" << std::endl;
        hdr.SetCommand(CMD_ACK_ERR);
        res_msg.SetHeader(hdr);
        res_msg.AddArgv(SLAVEDAEMON_SUBMITJOB_ERR);
        socket->Send(res_msg);
      }
      break;

    default:
      std::cout << "Unknown message: " << res_msg.GetHeader()->GetCommand() << std::endl;
      break;
  }
  //msg.FreeParsedArgs(argv);

  return isFinish;
}

ResultCodes CopyExeFile(std::string& job_id, const std::string&exe_file){
  std::cout << "CopyExeFile(" << job_id << ", " << exe_file << ")\n";
  ResultCodes rlt = OK;
  std::cout << "job_id = " << job_id << ", ";
  std::stringstream ss;
  ss << kHomeJobDIr << job_id ;
  std::string job_path = ss.str();
  ss.str("");
  ss << "cp -Rf " << exe_file << " " << job_path << ";";
  std::string cmd = ss.str();
  std::cout << "run cmd: " << cmd << std::endl;
  std::string stdout = SystemCommandUtils::Exec(cmd.c_str());
  std::cout << "stdout: " << stdout << std::endl;
  if(stdout.size() == 0){
    std::cout << "copy success\n";
  } else {
    std::cout << "copy fail\n";
    rlt = SLAVEDAEMON_STAGEIN_ERR;
  }
  return rlt;
}

ResultCodes Stagein(std::string& job_id, std::vector<std::string>&argv){
  std::cout << "Stagein: ";
  ResultCodes rlt = OK;
  std::cout << "job_id = " << job_id << ", ";
  std::stringstream ss;
  ss << kHomeJobDIr << job_id ;
  std::string job_path = ss.str();
  ss.str("");

  for(unsigned int i = 0; i < argv.size(); i ++ ){
    ss.str("");
    std::cout << "dir " << i << ": " << argv[i] << std::endl;
    ss << "cp -R " << argv[i] << " " << job_path << ";";
    std::string cmd = ss.str();
    std::cout << "run cmd: " << cmd << std::endl;
    std::string stdout = SystemCommandUtils::Exec(cmd.c_str());
    std::cout << "stdout: " << stdout << std::endl;
    if(stdout.size() == 0){
      std::cout << "copy success\n";
    } else {
      std::cout << "copy fail\n";
      rlt = SLAVEDAEMON_STAGEIN_ERR;
    }
  }
  return rlt;
}

ResultCodes SubmitJob(Socket& kp_sock, std::string& host, std::string& port, std::string& job_id, std::string& exe_path){
  std::cout << "SubmitJob\n";
  ResultCodes rlt = OK;
  try {
    Socket kdesk_socket;
    if ( ! kdesk_socket.Create() ) {
      throw SocketException ( "Could not create client socket." );
    }
    std::string forwarder_host = host;
    int forwarder_port = atoi(port.c_str());
    if ( ! kdesk_socket.Connect( forwarder_host, forwarder_port)) {
      throw SocketException ( "Could not connect to server." );
    }

    //rlt = SendInitJobCommand(&kdesk_socket, job_id);
    rlt = SendSubmitJobCommand(&kdesk_socket, job_id, exe_path);
    if(rlt == OK){
      rlt = SendDataToKdeskDaemon(&kdesk_socket, job_id);
      if(rlt == OK){
        //rlt = WaitKdeskSubmitResult(&kp_sock, &kdesk_socket, job_id);
        rlt = WaitKdeskSubmitResult(&kdesk_socket, job_id);
        if(rlt == OK){
          std::cout << "submit job success\n";
          //rlt = WaitJobNotification(kdesk_socket, kpserver_socket job_id);
        } else {
          std::cout << "wait kdesk submijob response & send result to kp_server fail.\n";
        }
        //rlt = SendSubmitJobCommand(&client_socket, job_id);
        //if(rlt != OK)

      } else {
        std::cout << "send data fail.\n";
      }
    } else {
      std::cout << "send submit job fail\n";
    }
  } catch ( SocketException& e ) {
    std::cout << "Exception was caught:" << e.description() << "\n";
  }
  return rlt;
}

ResultCodes WaitKdeskSubmitResult(Socket* kd_sock, const std::string& job_id){
  std::cout << "WaitKdeskSubmitResult\n";
  ResultCodes rlt = OK;

  Header err_hdr(MT_COMMAND, 9001, 9008, 0, CMD_ACK_ERR);
  std::vector<std::string> argv;
  std::string pjm_jobid, status, start_date, duration_time;
  bool isFinish = false;

  while(rlt == OK && !isFinish){
    std::cout << "Wait new message from kdeskdaemon\n";
    Message recv_msg, err_msg;
    Message send_msg(Header(MT_COMMAND, 9001, 9008, 0, CMD_JOB_STATE), NULL);
    if(kd_sock->Recv(recv_msg) > 0){
      if(recv_msg.GetHeader()->GetCommand() == CMD_JOB_STATE){
        std::cout << "CMD_JOB_STATE <kdesk job id> <state>\n";
        argv = recv_msg.GetCmdArgs();
        if(argv.size() > 4){
          pjm_jobid = argv[1];
          status = argv[2];
          start_date = argv[3];
          duration_time = argv[4];
          send_msg.AddArgv(job_id);
          send_msg.AddArgv(pjm_jobid);
          send_msg.AddArgv(status);
          send_msg.AddArgv(start_date);
          send_msg.AddArgv(duration_time);
          std::cout <<" job_id = " << job_id << " " << pjm_jobid << " " << status << " " <<  start_date << " " << duration_time << std::endl;

          if(status == "RJT" || status == "EXT" || status == "CCL" || status == "ERR"){
            isFinish = true;
          }
        }
      } else {
        std::cout << "kdesk response submitjob fail\n";
        err_msg.SetHeader(err_hdr);
        err_msg.AddArgv(job_id);
        rlt = KDESK_SUBMITJOB_ERR;
      }
    } else {
      std::cout << "wait response submit job status from kdesk fail\n";
      rlt = KDESK_SUBMITJOB_ERR;
      err_msg.SetHeader(err_hdr);
      err_msg.AddArgv(job_id);
    }
    if(SendMsgToKpServer( rlt == OK ? send_msg : err_msg) == OK){
    //if(kp_sock->Send(*send_msg)){
      std::cout << " send response to kp_server sucesss\n";
    } else {
      std::cout << " send response to kp_server fail\n";
      rlt = SLAVEDAEMON_SUBMITJOB_ERR;
    }
  }

  // wait file
  std::cout << "Wait output file\n";
  std::string job_path = std::string(kHomeJobDIr) + job_id;
  std::cout <<"job dir use to save file: " << job_path << std::endl;
  if(rlt == OK){
    int status = kd_sock->RecvListFile(job_path);
    std::cout << "status = " << status << std::endl;
    if(status > 0){
      std::cout << "Receieve new files success;\n";
    } else {
      std::cout << "Receieve new files fail";
      rlt = KDESK_SUBMITJOB_ERR;
    }
  }
  return rlt;
}


ResultCodes WaitJobNotification(Socket& kd_sock, Socket& kp_sock, const std::string& job_id){
  std::cout << "WaitJobNotification\n";
  ResultCodes rlt = OK;

  std::stringstream ss;
  ss << kHomeJobDIr << job_id ;
  std::string job_path = ss.str();
  std::cout <<"job dir use to save file: " << job_path << std::endl;

  Message notify_msg_begin, notify_msg_end, change_state_msg(Header(MT_COMMAND, 0, 9999, 0, CMD_JOB_STATE), NULL),
      send_msg(Header(MT_COMMAND, 0, 9999, 0, CMD_JOB_STATE), NULL);
  std::vector<std::string> argv;
  std::string pjm_jobid, status, start_date, duration_time;

  // wait new status start running
  if(kd_sock.Recv(notify_msg_begin)){
    if(notify_msg_begin.GetHeader()->GetCommand() == CMD_JOB_STATE){
      argv = notify_msg_begin.GetCmdArgs();
      if(argv.size() > 4){
        pjm_jobid = argv[1];
        status = argv[2];
        start_date = argv[3];
        duration_time = argv[4];
        std::cout << pjm_jobid << " " << status << " " <<  start_date << " " << duration_time << std::endl;
        change_state_msg.AddArgv(job_id);
        change_state_msg.AddArgv(pjm_jobid);
        change_state_msg.AddArgv(status);
        change_state_msg.AddArgv(start_date);
        change_state_msg.AddArgv(duration_time);
      }
      std::cout << "submit job success\n";
    } else {
      rlt = SLAVEDAEMON_SUBMITJOB_ERR;
      std::cout << "wait notification job\n";
      return rlt;
    }
  }
  if(kp_sock.Send(change_state_msg)){
    std::cout << " send event change status to kp_server sucesss\n";
  } else {
    std::cout << " send event change status response to kp_server fail\n";
    rlt = SLAVEDAEMON_SUBMITJOB_ERR;
    return rlt;
  }

  // wait file
  while(true){
    int status = kd_sock.RecvListFile(job_path);
    std::cout << "status = " << status << std::endl;
    if(status > 0){
      std::cout << "Receieve new files success;\n";
      //Message req(Header(MT_COMMAND, 0, 0, 0, CMD_INITJOB), NULL), res;
//      /notify_msg.AddArgv(job_id);
      break;
    }
    sleep(3);
  }

  // wait last notify
  if(kd_sock.Recv(notify_msg_end)){
    if(notify_msg_end.GetHeader()->GetCommand() == CMD_JOB_STATE){
      argv = notify_msg_end.GetCmdArgs();
      if(argv.size() > 3){
        pjm_jobid = argv[1];
        status = argv[2];
        start_date = argv[3];
        duration_time = argv[4];
        std::cout << pjm_jobid << " " << status << " " <<  start_date << " " << duration_time << std::endl;
        send_msg.AddArgv(job_id);
        send_msg.AddArgv(pjm_jobid);
        send_msg.AddArgv(status);
        send_msg.AddArgv(start_date);
        send_msg.AddArgv(duration_time);
      }
      std::cout << "submit job success\n";
    } else {
      rlt = SLAVEDAEMON_SUBMITJOB_ERR;
      std::cout << "wait notification job\n";
    }
  }
  if(kp_sock.Send(send_msg)){
    std::cout << " send response to kp_server sucesss\n";
  } else {
    std::cout << " send response to kp_server fail\n";
    rlt = SLAVEDAEMON_SUBMITJOB_ERR;
  }
  return rlt;
}

ResultCodes SendInitJobCommand(Socket* client_socket, std::string& job_id){
  std::cout << "SendInitJobCommand" << std::endl;
  Message req(Header(MT_COMMAND, 0, 0, 0, CMD_INITJOB), NULL), res;
  req.AddArgv(job_id);
  ResultCodes rlt = OK;
  try {
    // Send request
    std::cout << "send request\n";
    if(client_socket->Send(req)){
      // Get response
      int size = client_socket->Recv(res);
      std::cout << "receieve response: size = " << size << std::endl;
      if(res.GetHeader()->GetCommand() == CMD_ACK_OK){
        rlt = OK;
      } else {
        rlt = KDESK_SUBMITJOB_ERR;
      }
    } else {
      rlt = SLAVEDAEMON_SUBMITJOB_ERR;
    }
  } catch ( SocketException& e) {
    rlt = SLAVEDAEMON_FORWARDER_SOCKET_ERR;
  }
  return rlt;
}

ResultCodes SendSubmitJobCommand(Socket* client_socket, std::string& job_id, std::string& exe_path){
  std::cout << "SendSubmitJobCommand" << std::endl;
  Message req(Header(MT_COMMAND, 0, 0, 0, CMD_SUBMITJOB), NULL), res;
  ResultCodes rlt = OK;
  try {
    // Send request
    std::cout << "send request\n";
    req.AddArgv(job_id);
    req.AddArgv(exe_path);
    if(client_socket->Send(req)){
      // Get response
      int size = client_socket->Recv(res);
      if(size < 0){
        throw SocketException("socket receieve is error\n");
      }
      std::cout << "receieve response\n";
      if(res.GetHeader()->GetCommand() == CMD_ACK_OK){
        rlt = OK;
      } else {
        rlt = KDESK_SUBMITJOB_ERR;
      }
    } else {
      rlt = SLAVEDAEMON_SUBMITJOB_ERR;
    }
  } catch ( SocketException& e) {
    std::cout << "Exception was caught:" << e.description() << "\n";
    rlt = SLAVEDAEMON_FORWARDER_SOCKET_ERR;
  }
  return rlt;
}

ResultCodes SendDataToKdeskDaemon(Socket* client_socket, std::string& job_id){
  std::cout << "SendDataToKdeskDaemon\n";
  std::stringstream ss;
  ss << kHomeJobDIr << job_id;
  std::string job_path = ss.str();
  std::cout << "job folder: " << job_path << std::endl;
  ResultCodes rlt = OK;
  try {
    if(client_socket -> SendFolder(std::string(job_path))){
      std::cout << "send data success" << std::endl;
    } else {
      std::cout << "send job folder fail\n";
      rlt = SLAVEDAEMON_SUBMITJOB_ERR;
    }
  } catch ( SocketException& e) {
    std::cout << "Exception was caught:" << e.description() << "\n";
    rlt = SLAVEDAEMON_FORWARDER_SOCKET_ERR;
  }

  return rlt;
}

ResultCodes SendMsgToKpServer(const Message &msg){
  ResultCodes rlt = OK;
  // Connect kdeskdaemon
  Socket kp_sock;
  try {
    if ( ! kp_sock.Create() ) {
      throw SocketException ( "Could not create client socket." );
    }
    std::cout << "SendMsgToKpServer message to " << g_kpserver_host << ":" << g_kpserver_port  <<std::endl;
    if ( ! kp_sock.Connect( g_kpserver_host, g_kpserver_port)) {
      throw SocketException ( "Could not connect to server." );
    }
  } catch ( SocketException& e ) {
    std::cout << "Create connection exception was caught:" << e.description() << "\n";
    rlt = SLAVEDAEMON_FORWARDER_SOCKET_ERR;
  }

  if(kp_sock.Send(msg)){
    std::cout << " send response to kp_server sucesss\n";
  } else {
    std::cout << " send response to kp_server fail\n";
    rlt = SLAVEDAEMON_FORWARDER_SOCKET_ERR;
  }
  return rlt;
}
