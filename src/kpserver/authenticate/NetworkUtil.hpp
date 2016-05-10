#ifndef NETWORKUTIL_HPP
#define NETWORKUTIL_HPP
#include <string>
#include <map>
#include "../ResponeInterface.hpp"
class NetworkUtil {
public:
	static ResponseCode requestHttp(std::string hostIp, std::string port, std::string request_str, std::string &respone_str, std::string &headerRespone_str);
	static ResponseCode getIpAddressOfContainer(std::string container_Id, std::string &IpAddress);
	static ResponseCode ProcessDownloadFile (std::string host, std::string port, std::string sourceFile, std::string desFile);
	static ResponseCode ProcessUploadFile(std::string host, std::string port, std::string sourceFile, std::string desFile);
	static ResponseCode ProcessUploadDir(std::string host, std::string port, std::string sourceDir, std::string desDir);
	static ResponseCode ProcessDeleteDir(std::string host, std::string port, std::string pathDir);
	static ResponseCode ProcessGetListEntry(std::string host, std::string port, std::string pathDir, std::map<std::string, bool> &listEntry);

  static void SendMessage(int sock, MessageStruct mes);
  static void ReceiveMessage(int sock, MessageStruct &mes);

  static int Send (int& m_sock, const char* bytes, const unsigned int& size) ;
  static int Recv (int& m_sock, char* buffer, unsigned int size) ;

};
#endif
