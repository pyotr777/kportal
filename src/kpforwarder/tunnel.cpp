#include "tunnel.hpp"
pthread_mutex_t message_queue_mutex;

Tunnel::Tunnel() {}

Tunnel::~Tunnel() {
  std::map<int, Socket*>::iterator itr = m_slavedaemons.begin();
  while (itr != m_slavedaemons.end()) {
     std::map<int, Socket*>::iterator toErase = itr;
     delete toErase->second;
     ++itr;
     m_slavedaemons.erase(toErase);
  }

  if(m_kdesk_socket != NULL){
    delete m_kdesk_socket;
  }
}

int Tunnel::GetListenPort() {
  return m_listen_port;
}

int Tunnel::EnqueueMsg(Message msg) {
  pthread_mutex_lock(&message_queue_mutex);
  m_messages.push(msg);
  pthread_mutex_unlock(&message_queue_mutex);
  return 0;
}

bool Tunnel::ContainDaemonSlave(int key){
  if ( m_slavedaemons.find(key) == m_slavedaemons.end() ) {
    // not found
    return false;
  } else {
    // found
    return true;
  }
}

bool Tunnel::AddDaemonSlaveConn(int key, Socket* sock){
  if(ContainDaemonSlave(key)){
    return false;
  }
  //std::pair<char,int>('a',100)
  m_slavedaemons.insert(std::pair<int, Socket*>(key, sock));
  return true;
}

void Tunnel::ProcessMessage() {
  pthread_mutex_lock(&message_queue_mutex);
  while(m_messages.size() > 0) {
    try {
      // send request
      Message* req_msg = &m_messages.front();
      m_kdesk_socket -> Send(*req_msg);

      // wait response
      Message res_msg;
      m_kdesk_socket->Recv(res_msg);
      std::cout << "Received response from kdesk daemon\n";

      // forward to slave daemon
      int from_port = req_msg->GetHeader()->GetFromPort();
      std::map<int, Socket*>::iterator it = m_slavedaemons.find(from_port);
      if(it == m_slavedaemons.end()){
        Socket* socket = it->second;
        socket->Send(res_msg);
      } else {
        std::cout << "Unknown destination slave daaemon for message \n";
      }

      // remove message
      m_messages.pop();
    } catch (SocketException& e) {
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }
  }
  pthread_mutex_unlock(&message_queue_mutex);
  return;
}

ResultCodes Tunnel::Start(std::string& kdesk_acc, std::string& port){
  std::cout << "Tunnel::Start " << "kdesk acc = " << kdesk_acc << ", port = " << port << std::endl;
  ResultCodes rlt = OK;
  //
  // Start
  //
  std::stringstream ss;
  std::string std_out;

  char hostname[] = "k.aics.riken.jp";
  // Ssh command example: "ssh -f -L 9002:localhost:9002 -v a03320@k.aics.riken.jp '/path/to/kdesk_daemon 9002 & '"
  //"'/home/ra000007/a03320/kportal/kdeskdaemon.sh " << port << " & '"
  //ss << "ssh -f -L " << port << ":" << hostname << ":" << port << " -v " << kdesk_acc << "@k.aics.riken.jp -c '/home/ra000007/a03320/kportal/kdeskdaemon" << port << " '";
  //ss << "ssh -f -N -L " << port << ":" << hostname << ":" << port << " -v " << kdesk_acc << "@k.aics.riken.jp; "
  //   << "ssh " << kdesk_acc << "@k.aics.riken.jp " << "'/home/ra000007/a03320/kportal/kdeskdaemon.sh " << port << " & '";

  ss << "ssh -L *:" << port << ":localhost:" << port << " -v " << kdesk_acc << "@" << hostname <<" kdeskdaemon " << port;
  std::string cmd = ss.str();
  std::cout << "system run cmd: " << cmd.c_str() << std::endl;
  int pid = SystemCommandUtils::System(cmd.c_str(), NULL, NULL);

  //std_out = SystemCommandUtils::Exec(cmd.c_str());
  //std::cout <<"stdout: " << std_out << std::endl;
  std::cout << "process id: " << pid << std::endl;
  if(pid < 0){
    rlt = TUNNEL_START_ERR;
    return rlt;
  }

  //pin
  // Create kdeskdaemon connection
  //
  //rlt = ConnectKdeskDaemon(hostname, port);
  return rlt;
}

ResultCodes Tunnel::ConnectKdeskDaemon(std::string& kdesk_daemon_host, std::string& kdesk_daemon_port){
  std::cout << "ConnectKdeskDaemon( host = " << kdesk_daemon_host << ", port = " << kdesk_daemon_port << ")\n";
  ResultCodes rlt = FORWARDER_CONNECTTO_KDESKDAEMON_ERR;

  rlt = FORWARDER_CONNECTTO_SLAVEDAEMON_ERR;
  try {
    Socket* client_socket = new Socket();
    // ClientSocket client_socket ( _host, _port );
    if ( ! client_socket->Create() ) {
      throw SocketException ( "Could not create client socket." );
    }

    std::string host = kdesk_daemon_host;
    int port = atoi(kdesk_daemon_port.c_str());

    if ( ! client_socket->Connect( host, port)) {
      throw SocketException ( "Could not bind to port." );
    }

    // Send a
    Message req(Header(MT_COMMAND, 0, 0, 0, CMD_SLAVEDAEMON_CONNECT), NULL), res;
    try {
      // send request
      client_socket->Send(req);

      // get response
      client_socket->Recv(res);

      if(res.GetHeader()->GetCommand() == CMD_ACK_OK){
        // Get port
        rlt = OK;
        // Save kdesk socket
        m_kdesk_socket = client_socket;
      }
    } catch ( SocketException& e) {}
  } catch ( SocketException& e ) {
    std::cout << "Exception was caught:" << e.description() << "\n";
  }

  return OK;
}

/*
ResultCodes Tunnel::ConnectSlaveDaemon(char * slave_daemon_host, char* slave_daemon_port){
  std::cout << "Tunnel::ConnectSlaveDaemon( host = " << slave_daemon_host << ", port = "<< slave_daemon_port << ")\n";
  ResultCodes rlt = OK;

  rlt = FORWARDER_CONNECTTO_SLAVEDAEMON_ERR;
  try {
    Socket* client_socket = new Socket();
    // ClientSocket client_socket ( _host, _port );
    if ( ! client_socket->Create() ) {
      throw SocketException ( "Could not create client socket." );
    }

    std::string slavedaemon_host = slave_daemon_host;
    int slavedaemon_port = atoi(slave_daemon_port);

    if ( ! client_socket->Connect( slavedaemon_host, slavedaemon_port)) {
      throw SocketException ( "Could not bind to port." );
    }

    // Check send/receieve socket
    Message req(Header(MT_COMMAND, 0, 0, 0, CMD_SLAVEDAEMON_CONNECT), NULL), res;
    try {
      // send request
      client_socket->Send(req);

      // get response
      client_socket->Recv(res);

      if(res.GetHeader()->GetCommand() == CMD_ACK_OK){
        // Get port
        rlt = OK;

        AddDaemonSlaveConn(slavedaemon_port, client_socket);
      }
    } catch ( SocketException& e) {}
  } catch ( SocketException& e ) {
    std::cout << "Exception was caught:" << e.description() << "\n";
  }

  return OK;
}*/

void Tunnel::DoForwardingMessageFromSlaveDaemon(char* slave_daemon_port){
  int port = atoi(slave_daemon_port);
  Socket * socket = m_slavedaemons[port];
  try {
    while (true) {
      Message rec_msg;
      socket->Recv(rec_msg);
      rec_msg.GetHeader()->SetFromPort(port);

      std::cout << "Received signal from slave daemon\n";
      if(EnqueueMsg(rec_msg)){
        ProcessMessage();
      }
    }
  } catch (SocketException& e) {
    std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
  }
}
