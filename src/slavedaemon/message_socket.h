// Definition of the Socket class

#ifndef MESSAGE_SOCKET_H
#define MESSAGE_SOCKET_H

#include <iostream>
#include <set>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <vector>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "reader.h"
#include "writer.h"
#include "header.hpp"
#include "message.hpp"

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = BLOCK_SIZE * 1024 + 100;

//class MessageSocket : public Socket {
class Socket {
 public:
  Socket();
  virtual ~Socket();

  // Server initialization
  bool Create();
  bool Bind ( const int port );
  bool Listen() const;
  bool Accept ( Socket& ) const;

  // Client initialization
  bool Connect ( const std::string host, const int port );

  // Data Transimission
  int Send (const char* bytes, const unsigned int& size) const ;
  bool Send ( const std::string ) const;
  bool Send ( const Message& ) const;
  int Recv ( std::string& ) const;
  int Recv (char* buffer, unsigned int size)const ;
  int Recv ( Message& )const;

  void StatusFolder(const std::string &, const std::vector<std::string> &,
                          std::vector<std::string> &);

  bool SendFile(const std::string &) const;
  bool SendFolder(const std::string &) const;
  bool SendListFile(const std::vector<std::string> &, const std::string except_path);

  bool RecvFile(const std::string &, std::string &);
  bool RecvFolder(const std::string &, std::vector<std::string>&);
  bool RecvListFile(const std::string &);


  bool GetFileNameInFolder(const std::string &,
                           std::vector<std::string> &,
                           std::vector<std::string> &) const;


  void SetNonBlocking ( const bool );
  bool IsValid() const { return m_sock != -1; }
  int GetSock() const {return m_sock;}
  
  std::vector<std::string> SplitPath(const std::string& str, const std::set<char> delimiters);
  std::string SubDirPath(const std::string except_path, const std::string file_path);

 private:
  int m_sock;
  sockaddr_in m_addr;
  //char* m_incomplete_data;
  //int m_incomplete_size;
};


#endif
