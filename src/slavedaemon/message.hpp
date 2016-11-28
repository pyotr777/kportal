#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <cctype>
#include "header.hpp"

enum ResultCodes {
  OK = 0,
  HDR_MISSING,
  HDR_MESSAGETYPE_INVALID,
  HDR_FROMPORT_INVALID,
  HDR_TOPORT_INVALID,
  HDR_DATASIZE_INVALID,
  HDR_COMMAND_INVALID,
  DAT_INVALID,

  TUNNEL_START_ERR,
  FORWARDER_CONNECTTO_SLAVEDAEMON_ERR,
  FORWARDER_CONNECTTO_KDESKDAEMON_ERR,
  SLAVEDAEMON_FORWARDER_SOCKET_ERR,
  KDESK_SUBMITJOB_ERR,
  SLAVEDAEMON_SUBMITJOB_ERR,
  SLAVEDAEMON_STAGEIN_ERR
};

#define CMD_SEPERATOR_CHAR '#'
//const char CMD_SEPERATOR_CHAR = '#';

class Message {
private:
  Header m_header;
  char* m_data;
public:
  Message();
  Message(Header hdr, char* data);
  ~Message();
  const Header* GetHeader() const;
  void SetHeader(const Header &hdr);
  char* GetData() const;
  void SetData(char*buf);
  void ToBytes(char **bytes, unsigned int &msg_size) const;
  ResultCodes FromBytes(char const * bytes, unsigned int read_size);
  void AddArgv(const std::string &cmd_arg);
  void AddArgv(int cmd_arg);
  void AddArgv(const char* cmd_arg, int cmd_arg_size);
  std::vector<std::string> GetCmdArgs();
private:
  char** GetCmdArgs(int& argc);
  void FreeParsedArgs(char **argv);
  int SetArgs(char *args, char **argv);
  //
  // Get a unsigned int value (4 byte) from byte array
  //
  bool ReadInt32(const char * bytes, const int start_offset, int * value);
  //
  // Get a sub byte array from byte array
  //
  bool ReadBytes(const char * bytes, int start_offset, unsigned int data_size, char ** data);

  char ** ParsedArgs(char *args, int *argc);

  inline void Split(const std::string &str, char del,
                    std::vector<std::string> *out) {
    std::istringstream ss(str);
    std::string s;
    while (std::getline(ss, s, del)) {
        out->push_back(s);
    }
  }

};

#endif
