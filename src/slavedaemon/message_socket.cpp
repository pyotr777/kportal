// Implementation of the Socket class.
#include "message_socket.h"

Socket::Socket() :
  m_sock ( -1 )
{
  //m_incomplete_data = NULL;
  //m_incomplete_size = 0;
  memset ( &m_addr,
	   0,
	   sizeof ( m_addr ) );

}

Socket::~Socket(){
  if ( IsValid() )
    ::close ( m_sock );
  std::cout << "Socket::~Socket()\n";
//  if(m_incomplete_data != NULL && m_incomplete_size > 0){
//    delete m_incomplete_data;
//    m_incomplete_data = NULL;
//  }
}

bool Socket::Create()
{
  m_sock = socket ( AF_INET,
		    SOCK_STREAM,
		    0 );

  if ( ! IsValid() )
    return false;


  // TIME_WAIT - argh
  int on = 1;
  if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
    return false;


  return true;

}

bool Socket::Bind ( const int port )
{

  if ( ! IsValid() )
    {
      return false;
    }



  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons ( port );

  int bind_return = ::bind ( m_sock,
			     ( struct sockaddr * ) &m_addr,
			     sizeof ( m_addr ) );


  if ( bind_return == -1 )
    {
      return false;
    }

  return true;
}

bool Socket::Listen() const
{
  if ( ! IsValid() )
    {
      return false;
    }

  int listen_return = ::listen ( m_sock, MAXCONNECTIONS );


  if ( listen_return == -1 )
    {
      return false;
    }

  return true;
}

bool Socket::Accept ( Socket& new_socket ) const {
  int addr_length = sizeof ( m_addr );
  new_socket.m_sock = ::accept ( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );

  if ( new_socket.m_sock <= 0 )
    return false;
  else
    return true;
}

bool Socket::Send ( const std::string s ) const {
  std::cout << "Socket::Send string = " << s << ";";
  unsigned int size = s.size();
  int status = Send((char*)&size, sizeof(unsigned int));

  status = Send(s.c_str(), s.size());
  //std::cout << status << std::endl;
  return status == (int)s.size();
}

int Socket::Send (const char* bytes, const unsigned int& size) const {
  std::cout << "Socket::Send bsize=" << size << ",last byte=" << bytes[size - 1] << std::endl;
  unsigned int out_bytes = 0;

  while (out_bytes < size) {
   // std::cout << "before write to socket: <<  " << size - out_bytes << "\n";
   int n = ::write(m_sock, bytes + out_bytes, size - out_bytes);
   if (n < 0){
     //printf("[ERROR] Unable to send reponse.\n");
     std::cout << "[ERROR] Unable to send reponse.\n";
     close(m_sock);
     return -1;
   } else
   if (n == 0) { // connection closed!
     //printf("[ERROR] Connection has been unexpectedly closed by remote side!\n");
     std::cout <<"[ERROR] Connection has been unexpectedly closed by remote side!\n";
     close(m_sock);
     return -2;
   } else {
     out_bytes += n;
     //printf("Socket sent (%d bytes).\n", n);
     std::cout << "Socket sent (" << n << " bytes).\n";
   }
  }
  return out_bytes;
}

bool Socket::Send (const Message& msg ) const {
  //std::cout << "Socket::Send message\n";
  // Send header
  char* hdr; unsigned int hdr_size;

  const Header * header = msg.GetHeader();
  Header::GetBytes(*header, &hdr, &hdr_size);
  //std::cout << "header size = " << hdr_size << std::endl;

  if(Send(hdr, hdr_size) != (int)hdr_size){
    //printf("send header fail\n");
    std::cout << "[ERR]Socket::Send send message header fail\n";
    delete[] hdr;
    return false;
  }
  delete[] hdr;

  // Send data
  //std::cout << "begin send message data\n";
  const char *data = msg.GetData();
  unsigned int data_size = msg.GetHeader()->GetDataSize();
  if(data != NULL && data_size > 0){
    if(Send(data, data_size) != (int)data_size){
      //printf("send message data fail.");
      std::cout << "[ERR]Socket::Send message data fail.\n";
      return false;
    } else {
      //printf("send message data success.");
      //std::cout << "send message data success.";
    }
  } else {
    //printf("Message data is emtpy\n");
    //std::cout << "Message data is emtpy\n";
  }
  return true;
}




bool Socket::GetFileNameInFolder(const std::string &path_folder,
                         std::vector<std::string> &path_filename,
                         std::vector<std::string> &path_sub_foldername)
                         const {
  DIR *dir;
  dirent *pdir;
  dir = opendir(path_folder.c_str());
  if (dir == NULL) {
    std::cout << "folder is no exist\n";
    return false;
  }

  /*actach '/' at end of path*/
  std::string _path_folder;
  if (path_folder.at(path_folder.length() - 1) != '/') {
    _path_folder = path_folder;
    _path_folder = _path_folder + '/';
  } else { 
    _path_folder = path_folder;
  }

  /*get folder to send name*/
  std::string foldername;
//  foldername = _path_folder.at(_path_folder.length() - 1) + foldername;
  int i = _path_folder.length() - 1;
  do {
    foldername = _path_folder.at(i) + foldername;
    if (i == 0) break;
    i--;
  } while (_path_folder.at(i) != '/');

  path_filename.push_back(foldername);
  path_sub_foldername.push_back(foldername);

  /* get path filename and path subfolder*/
  while ((pdir = readdir(dir))) {
    struct stat st;

    if(strcmp(pdir->d_name, ".") == 0 || strcmp(pdir->d_name, "..") == 0) {
      continue;
    }

    if (fstatat(dirfd(dir), pdir->d_name, &st, 0) < 0) {
      std::cout << "Some error in fstatat\n";
      continue;
    }

    if (S_ISREG(st.st_mode)) {
      std::string dname(pdir->d_name);
      path_filename.push_back(_path_folder + pdir->d_name);
    } else if (S_ISDIR(st.st_mode)) {
      std::string dname(pdir->d_name);
      path_sub_foldername.push_back(_path_folder + pdir->d_name);
    }
  }

  closedir(dir);
  return true;
}

bool Socket::SendFile(const std::string &r_path_filename) const {
  std::cout << "Socket::SendFile " << r_path_filename.c_str() << std::endl;
  std::string response; //get respponse from other side

  std::string filename;
  filename = r_path_filename.at(r_path_filename.length() - 1) + filename;
  int i = r_path_filename.length() - 2;
  while (r_path_filename.at(i) != '/') {
    filename = r_path_filename.at(i) + filename;
    if (i == 0) break;
    i--;
  }
  std::cout <<"file name = " << filename << std::endl;

  char *data = new char[filename.length() + 1];
  memcpy(data, filename.c_str(), filename.length());
  data[filename.length()] = '\0';
  //std::cout << data << std::endl;
  /* send file name and folder to save file*/
  Header w_filename_hdr = Header(MT_DATA, 8888, 8888,
                               filename.length() + 1, CMD_SENDFILE_INFO);
  Message w_filename_msg = Message(w_filename_hdr, data);
  if (!Send(w_filename_msg)) {
    std::cout << "Send filename failure\n";
    return false;
  }
  Recv(response);
  if (response.length() == 0) {
    std::cout << "No response from client\n";
    return false;
  }
  std::cout <<"recv response: " << response << std::endl;
  response = "";

  Reader *x = new Reader(r_path_filename);
  if (!x->IsOpened()) {
    std::cout << "File is invalid\n";
    delete x;
    return false;
  }
  char* temp;
  int32_t data_size;
  while (!x->IsEOF()) {
    data_size = x->Read(&temp);
    if (data_size == -1) {
      std::cout << "Error when reading file";
      delete x; 
      return false;
    }
    Header filedata_hdr = Header(MT_DATA, 8888, 8888,
                                 data_size, CMD_SENDFILE_PACKET);
    Message filedata_msg = Message(filedata_hdr, temp);

    if (!Send(filedata_msg)) {
      std::cout << " send data failure\n";
      delete x;
      return false;
    }
    Recv(response);
    std::cout << "recv response: " << response << std::endl;
    response= "";
  }
  std::string finish = "*";
  char* endfile = new char;
  memcpy(endfile, finish.c_str(), finish.length());
  Header finish_hdr = Header(MT_COMMAND, 8888, 8888,
                               1, CMD_SENDFILE_FINISH);
  Message finish_msg = Message(finish_hdr, endfile);
  if (!Send(finish_msg)) {
    std::cout << "Send finish signed file failure\n";
    delete x;
    return false;
  }
  Recv(response);
  std::cout << response <<std::endl;
  delete x;
  return true;
}

bool Socket::SendFolder(const std::string &r_pathfolder) const {
  std::string info;
  std::vector<std::string> path_filename;
  std::vector<std::string> path_sub_foldername;
  if (!GetFileNameInFolder(r_pathfolder, path_filename, path_sub_foldername)) {
    std::cout << "Some errors in folder\n";
    return false;
  }

  if (!Send(path_filename.at(0))) {
    std::cout << "Cannot send foldername\n";
    return false;
  }
  Recv(info);
  if (info != "Directory is valid\n") {
    std::cout << " Directory is exits, please change your folder name\n";
    return false;
  }
  info.clear();

  /* check folder empty */
  if (path_filename.size() <= 1 && path_sub_foldername.size() <= 1) {
    Send("folder is empty");
    Recv(info);
    std::cout << info << std::endl;
  } else if (path_filename.size() > 1 && path_sub_foldername.size() > 1 ) {
    Send("Sending file and subfolder");
    Recv(info);
    std::cout << info << std::endl;
    info.clear();

    for (unsigned int i = 1; i < path_filename.size(); i++) {
      SendFile(path_filename.at(i));
      if (i < path_filename.size() - 1) {
        Send("Next file");
      } else {
        Send("End file");
      }
    }
    Recv(info);
    std::cout << info << std::endl;

    for (unsigned int i = 1; i < path_sub_foldername.size(); i++) {
      std::string xxx;
      if (i != path_sub_foldername.size() - 1) {
        Send("Next folder");
        Recv(xxx);
      } else {
        Send("End folder");
        Recv(xxx);
      }
      std::cout << path_sub_foldername.at(i) << std::endl;
      SendFolder(path_sub_foldername.at(i));
    }
  } else if (path_filename.size() > 1 && path_sub_foldername.size() <= 1 ) {
    Send("Sending only file");
    Recv(info);
    std::cout << info << std::endl;
    info.clear();

    for (unsigned int i = 1; i < path_filename.size(); i++) {
      SendFile(path_filename.at(i));
      if (i < path_filename.size() - 1) {
        Send("Next file");
      } else {
        Send("End file");
      }
    }
    Recv(info);
    std::cout << info << std::endl;
  } else if (path_filename.size() <= 1 && path_sub_foldername.size() > 1 ) {
      Send("Sending only subfolder");
      Recv(info);
      std::cout << info << std::endl;
      info.clear();
      for (unsigned int i = 1; i < path_sub_foldername.size(); i++) {
      std::string xxx;
      if (i != path_sub_foldername.size() - 1) {
        Send("Next folder");
        Recv(xxx);
      } else {
        Send("End folder");
        Recv(xxx);
      }
      std::cout << path_sub_foldername.at(i) << std::endl;
      SendFolder(path_sub_foldername.at(i));
    }


  }
  return true;
}


bool Socket::RecvFolder(const std::string &w_pathfolder, std::vector<std::string>& path_filename) {
  std::cout << "start racv folder\n";
  std::string foldername;

  Recv(foldername);

  if (w_pathfolder.at(w_pathfolder.length() - 1) == '/') {
    foldername = w_pathfolder + foldername;
  } else {
    foldername = w_pathfolder + '/' + foldername;
  }

  struct stat existing = {0}; // check if directory is exist
  if (stat(foldername.c_str(), &existing) == -1) {
    Send("Directory is valid\n");

    std::string cmd = std::string("mkdir -p ") + foldername;
    std::cout << "mkidr cmd: " << cmd << std::endl;
    system(cmd.c_str());
  } else {
    Send("Directory is exist\n");
    return false;
  }

  std::string r_folderstatus;
  Recv(r_folderstatus);
  if (r_folderstatus == "folder is empty") {
    Send("Receiver finish receiving...");
    return true;
  } else if (r_folderstatus == "Sending file and subfolder") {
    Send("Receiver start receiving files and subfolders");
//    path_filename.push_back(foldername);
    while (1) {
      std::string path_file;
      RecvFile(foldername, path_file);
      path_filename.push_back(path_file);
      std::string continue_recv;
      Recv(continue_recv);
      std::cout << continue_recv << std::endl;
      if (continue_recv == "End file") break;
    }
    Send ("Receiver start receiving subfolder");
    std::string continue_recv;
    do {
      continue_recv.clear();
      Recv(continue_recv);
      Send("OK");
      //std::cout << continue_recv;
      std::cout << continue_recv << std::endl;
      RecvFolder(foldername, path_filename);
    } while (continue_recv != "End folder");
  } else if (r_folderstatus == "Sending only file") {
    Send("Receiver start receiving files");
//    path_filename.push_back(foldername);
    while (1) {
      std::string path_file;
      RecvFile(foldername, path_file);
      path_filename.push_back(path_file);
      std::string continue_recv;
      Recv(continue_recv);
      std::cout << continue_recv << std::endl;
      if (continue_recv == "End file") break;
    }
    Send ("recive Finish");
  } else if (r_folderstatus == "Sending only subfolder") {
    Send("Receiver start receiving subfolder");
    std::string continue_recv;
    do {
      continue_recv.clear();
      Recv(continue_recv);
      Send("OK");
      //std::cout << continue_recv;
      std::cout << continue_recv << std::endl;
      RecvFolder(foldername, path_filename);
   } while (continue_recv != "End folder");
  }
  return true;
}

bool Socket::RecvFile(const std::string &path_file, std::string &file_name) {
  std::cout << "Socket::RecvFile save to " << path_file.c_str()<<"\n";
  Message filename_msg;

  std::string _path_file = path_file;
  if (_path_file.at(_path_file.length() - 1) != '/') {
    _path_file += '/';
  }

  Recv(filename_msg);
  if (filename_msg.GetData() == NULL) {
    std::cout << "[ERR] Cannot receive file name\n";
    return false;
  } else {
  }

  if(Send("Client received filename") <= 0){
    std::cout << "send ack: \"client received filename fail\"\n";
  }

  char* buf = new char[filename_msg.GetHeader()->GetDataSize() + 1];
  buf[filename_msg.GetHeader()->GetDataSize()] = '\0';
  memcpy(buf, filename_msg.GetData(), filename_msg.GetHeader()->GetDataSize());
  std::string w_filename(buf);
  //std::cout << "file name:" << w_filename << std::endl;
  delete[]buf;

  //std::cout << filename_msg.GetData() << std::endl;
  w_filename = _path_file + w_filename;
  file_name = w_filename;

  std::cout <<"filename will be receieve: " << w_filename << std::endl;

  Writer *x = new Writer(w_filename);
  if (!x->IsOpened()) {
    std::cout << "[ERR] RecvFile  " << w_filename << " is invalid\n";
    delete x;
    return false;
  }
  while (1) {
    Message msg;
    //std::cout << "wait recv new file message; ";
    if (Recv(msg) == 0) {
      std::cout << "[ERR] Receive Message failure\n";
      delete x;
      return false;
    }
    //std::cout << "after receive message; ";
    //std::string test = std::string(msg.GetData());
    if (msg.GetHeader()->GetCommand() == CMD_SENDFILE_FINISH) {
      Send("Tranfer done");
      //std::cout << "receieve success\n";
      break;
    }
    //std::cout << "is data;";
    if (x->Write(msg.GetData(), (int32_t)msg.GetHeader()->GetDataSize()) == -1) {
      std::cout << "[ERR] Write file failure";
      delete x;
      return false;
    }
    //std::cout << "send recived block notify msg;"<< std::endl;
    Send("Recived block");
    //std::cout << "send success;" << std::endl;
  }
  delete x;
  return true;
}

int Socket::Recv ( std::string& s ) const {
  //std::cout << "Socket::Recv ";
  char buf [ MAXRECV + 1 ];
  s = "";
  memset ( buf, 0, MAXRECV + 1 );

  unsigned int sot = 0;
  int status = Recv ((char*)&sot, sizeof(unsigned int));

  if(sot > 0){
    status = Recv (buf, sot);
    //std::cout << status << std::endl;
    if ( status > 0 ) {
      s = buf;
    }
  }
  return status;
}

//int Socket::Recv ( std::string& s ) const {
//  std::cout << "Socket::Recv ";
//  char buf [ MAXRECV + 1 ];
//  s = "";
//  memset ( buf, 0, MAXRECV + 1 );
//  int status = ::recv ( m_sock, buf, MAXRECV, 0 );
//  std::cout << status << std::endl;
//  if ( status == -1 ) {
//    std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
//    return 0;
//  } else if ( status == 0 ) {
//    return 0;
//  } else {
//    s = buf;
//    return status;
//  }
//}

int Socket::Recv (char* buffer, unsigned int size) const {
  unsigned int in_bytes = 0;
  while(in_bytes < size){
    int n = ::read(m_sock, buffer + in_bytes, size - in_bytes);
    if (n < 0){
      //printf("[ERROR] Unable to recv.\n");
      std::cout << "[ERROR] Unable to recv.\n" ;
      close(m_sock);
      return -1;
   } else
     if (n == 0) { // connection closed!
       //printf("[ERROR] Connection has been unexpectedly closed by remote side!\n");
       std::cout << "[ERROR] Connection has been unexpectedly closed by remote side!\n";
       close(m_sock);
       return -2;
   } else {
     in_bytes += n;
     //printf("Socket receieve (%d bytes).\n", n);
     std::cout << "Socket receieve ("<< n << " bytes).\n";
   }
  }
  return in_bytes;
}

int Socket::Recv ( Message& msg ) const {
  std::cout << "Socket::Recv ";

  // Read header
  int hdr_size = sizeof(Header);
  char* buf = new char[hdr_size];
  int in_size = Recv(buf,hdr_size);
  if(in_size != hdr_size) {
    std::cout << "error receieve header\n";
    delete []buf;
    return in_size;
  }
  int msg_type, command;
  unsigned int from_port, to_port, data_size;
  int offset = 0;
  memcpy(&msg_type, buf + offset, sizeof(int));
  offset += sizeof(int);
  memcpy(&from_port, buf + offset, sizeof(unsigned int));
  offset += sizeof(unsigned int);
  memcpy(&to_port, buf + offset, sizeof(unsigned int));
  offset += sizeof(unsigned int);
  memcpy(&data_size, buf + offset, sizeof(unsigned int));
  offset += sizeof(unsigned int);
  memcpy(&command, buf + offset, sizeof(int));

  delete []buf;
  buf = NULL;

  if(data_size > 0){
    buf = new char[data_size];
    int status = Recv(buf, data_size);
    if(status!= (int)data_size){
      std::cout << "error receieve data\n";
      delete []buf;
      return status;
    }
  }

  //std::cout <<"create message from byte.\n";
  std::cout << "type=" << msg_type << ",from port=" << from_port << ",to port=" << to_port << ",data size="<< data_size << ",command = " << command << std::endl;
  Header hdr((MessageTypes)msg_type, from_port, to_port, data_size,(Commands)command);
  msg.SetHeader(hdr);
  msg.SetData(buf);

  //std::cout << "rev message success\n";
  return data_size + hdr_size;
}

void Socket::StatusFolder(const std::string &path_folder, const std::vector<std::string> &path_file,
                          std::vector<std::string> &newfile) {

  DIR *dir;
  dirent *pdir;
  dir = opendir(path_folder.c_str());
  if (dir == NULL) {
    std::cout << "folder is exist\n";
    return;
  }

  std::string _path_folder;
  if (path_folder.at(path_folder.length() - 1) != '/') {
    _path_folder = path_folder;
    _path_folder = _path_folder + '/';
  } else {
    _path_folder = path_folder;
  }

  std::vector<std::string> filename;
  while (pdir = readdir(dir)) {
    struct stat st;

    if(strcmp(pdir->d_name, ".") == 0 || strcmp(pdir->d_name, "..") == 0) {
      continue;
    }

    if (fstatat(dirfd(dir), pdir->d_name, &st, 0) < 0) {
      std::cout << "Some error in fstatat\n";
      continue;
    }

    if (S_ISREG(st.st_mode)) {
      std::string dname(pdir->d_name);
      filename.push_back(_path_folder + pdir->d_name);
    } 
  }

  for (unsigned int i = 0; i < filename.size(); i++) {
    bool foo = false;
    for (unsigned int j = 1; j < path_file.size(); j++) {
      if (filename.at(i) == path_file.at(j)) {
        foo = true;
        break;
      }       
    }
    if (!foo) newfile.push_back(filename.at(i));
  }
  closedir(dir);
}

bool Socket::SendListFile(const std::vector<std::string> &listfile) {
  for (unsigned int i = 0; i < listfile.size(); i++) {
    if (i < listfile.size() - 1) {
      Send("Is not endfile");
      std::string receiver_said;
      Recv(receiver_said);
      std::cout << receiver_said << std::endl;
    } else {
      Send("Is endfile");
      std::string receiver_said;
      Recv(receiver_said);
    }
    if (SendFile(listfile.at(i))) {

    } else {
      return false;
    }
  }
  return true;
}

bool Socket::RecvListFile(const std::string &path_folder) {
  std::string sender_said;
  do {
    std::string path_file;
    Recv(sender_said);
    std::cout << sender_said;
    Send("Receiver already\n");
    if(!RecvFile(path_folder, path_file)) return false;
  } while (sender_said != "Is endfile");
  return true;
}

bool Socket::Connect ( const std::string host, const int port ){
  if ( ! IsValid() ){
    std::cout << "Socket::Connect Invalid\n";
    return false;
  }

  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons ( port );

  int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

  if ( errno == EAFNOSUPPORT ){
    std::cout << "Socket::Connect EAFNOSUPPORT\n";
    return false;
  }

  status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );
  std::cout << "Socket::Connect status = " << status << std::endl;
  if ( status == 0 )
    return true;
  else
    return false;
}

void Socket::SetNonBlocking ( const bool b )
{

  int opts;

  opts = fcntl ( m_sock,
		 F_GETFL );

  if ( opts < 0 )
    {
      return;
    }

  if ( b )
    opts = ( opts | O_NONBLOCK );
  else
    opts = ( opts & ~O_NONBLOCK );

  fcntl ( m_sock,
	  F_SETFL,opts );

}
