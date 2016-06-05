#include "header.hpp"
Header::Header(){
  m_type = MT_COMMAND;
  m_from_port = 0;
  m_to_port = 0;
  m_data_size = 0;
  m_command = CMD_UNKNOWN;
  //m_argc = 0;
}

Header::Header(MessageTypes type, unsigned int from_port, unsigned int to_port, unsigned int data_size, Commands command){
    m_type = type;
    m_from_port = from_port;
    m_to_port = to_port;
    m_data_size = data_size;
    m_command = command;
    //m_argc = 0;
}

Header::~Header()
{
}

Header::Header(const Header& cp){
  m_type = cp.m_type;
  m_from_port = cp.m_from_port;
  m_to_port = cp.m_to_port;
  m_data_size = cp.m_data_size;
  m_command = cp.m_command;
}

MessageTypes Header::GetType() const {
  return m_type;
}

void Header::SetType(const MessageTypes type) {
  m_type = type;
}

void Header::SetFromPort(const unsigned int from_port){
  m_from_port = from_port;
}

unsigned int Header::GetFromPort() const {
  return m_from_port;
}

void Header::SetToPort(const unsigned int to_port){
  m_to_port = to_port;
}

unsigned int Header::GetToPort() const {
  return m_to_port;
}

void Header::SetDataSize(const unsigned int data_size){
  m_data_size = data_size;
}

unsigned int Header::GetDataSize() const {
  return m_data_size;
}

void Header::SetCommand(const Commands command){
  m_command = command;
}

Commands Header::GetCommand() const {
  return m_command;
}

//unsigned int Header::GetCommandArgc() const{
//  return m_argc;
//}

//void Header::SetCommandArgc(const unsigned int argc) {
//  m_argc = argc;
//}

void Header::GetBytes(const Header& hdr, char ** bytes, unsigned int* size){
  //std::cout << "Header::GetBytes \n";
  *size = sizeof(int) + sizeof(unsigned int) + sizeof(unsigned int) + sizeof(unsigned int) + sizeof(int);
  *bytes = new char[*size];
  unsigned int offset = 0;
  memcpy(*bytes + offset, &hdr.m_type, sizeof(int));offset += sizeof(int);
  memcpy(*bytes + offset, &hdr.m_from_port, sizeof(int));offset += sizeof(unsigned int);
  memcpy(*bytes + offset, &hdr.m_to_port, sizeof(unsigned int));offset += sizeof(unsigned int);
  memcpy(*bytes + offset, &hdr.m_data_size, sizeof(unsigned int));offset += sizeof(unsigned int);
  memcpy(*bytes + offset, &hdr.m_command, sizeof(int));
  //std::cout << "done: size = " << *size << ", offset = " << offset << std::endl;
  //size = sizeof(Header);
//  /return bytes;
}
