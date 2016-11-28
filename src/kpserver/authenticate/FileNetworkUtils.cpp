#include "FileNetworkUtils.hpp"
#include "../stdafx.h"

extern std::string GETFILESIZE_REQUEST;
extern std::string READFILE_REQUEST;
extern std::string WRITEFILE_REQUEST;

FileNetworkUtils::FileNetworkUtils() {

}

FileNetworkUtils::FileNetworkUtils(std::string _host, std::string _port, std::string path_file) {
	host = _host;
	port = _port;
	pathFile = path_file;
	offset = -1;
}

FileNetworkUtils::~FileNetworkUtils() {
	Close();
}

void FileNetworkUtils::setOffset(size_t offset){
	this -> offset = offset;
}

ResponseCode FileNetworkUtils::Open() {
	offset = 0;
	return FILE_ACTION_SUCCESS;
}

ResponseCode FileNetworkUtils::Close() {
	if (offset >= 0) {
		offset = -1;
	}
	return FILE_ACTION_SUCCESS;
}

ResponseCode FileNetworkUtils::GetSize(size_t &size){
	std::cout << "GetSize start \n";
	ResponseCode ret = FILE_ACTION_SUCCESS;
	struct sockaddr_in server;
	int sock;
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
		ret = REQUEST_SOCKET_ERROR;
		return ret;
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr(host.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons( atoi(port.c_str()) );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		ret = REQUEST_CONNECT_ERROR;
		return ret;
	}

  MessageStruct mesToSend, mesToReceive;

	mesToSend.typeMes = Text;
	mesToSend.content = GETFILESIZE_REQUEST + pathFile;
	mesToSend.sizeMes = mesToSend.content.size();

	std::cout << "send - " << mesToSend.content << std::endl;
	NetworkUtil::SendMessage(sock, mesToSend);
	/* Receiving result get size file */
	NetworkUtil::ReceiveMessage(sock, mesToReceive);
	if (mesToReceive.typeMes != Text) {
		std::cout << "Server invalid \n";
		return REQUEST_RECEIVE_REQUEST_ERROR;
	}
	std::cout << "File size : " << mesToReceive.content << std::endl;
	size = atoi(mesToReceive.content.c_str());
	close(sock);
	return ret;
}

ResponseCode FileNetworkUtils::Read(size_t size_buf, size_t &read_length, std::string &buffer) {
	std::cout << "Read buffer start \n";
	ResponseCode ret = FILE_ACTION_SUCCESS;
	struct sockaddr_in server;
	int sock;
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
		ret = REQUEST_SOCKET_ERROR;
		return ret;
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr(host.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons( atoi(port.c_str()) );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		ret = REQUEST_CONNECT_ERROR;
		return ret;
	}

	std::stringstream ss;
  ss << READFILE_REQUEST << pathFile << "," << offset << "," << size_buf;
  MessageStruct mesToSend, mesToReceive;

	mesToSend.typeMes = Text;
	mesToSend.content = ss.str();
	mesToSend.sizeMes = mesToSend.content.size();

	std::cout << "send - " << mesToSend.content << std::endl;
	NetworkUtil::SendMessage(sock, mesToSend);
	/* Receiving result read file */
	NetworkUtil::ReceiveMessage(sock, mesToReceive);
	if (mesToReceive.typeMes != Binary) {
		std::cout << "Server invalid \n";
		return REQUEST_RECEIVE_REQUEST_ERROR;
	}
	std::cout << "Size buffer read : " << mesToReceive.sizeMes << std::endl;
	buffer = mesToReceive.content;
	read_length = mesToReceive.sizeMes;
	offset += read_length;

	close(sock);
	return ret;
}

ResponseCode FileNetworkUtils::Write(std::string buffer) {
	std::cout << "Read buffer start \n";
	ResponseCode ret = FILE_ACTION_SUCCESS;
	struct sockaddr_in server;
	int sock;
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
		ret = REQUEST_SOCKET_ERROR;
		return ret;
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr(host.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons( atoi(port.c_str()) );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		ret = REQUEST_CONNECT_ERROR;
		return ret;
	}

	std::stringstream ss;
	ss << WRITEFILE_REQUEST << pathFile << "," << offset << "," << buffer;
  MessageStruct mesToSend, mesToReceive;

	mesToSend.typeMes = Text;
	mesToSend.content = ss.str();
	mesToSend.sizeMes = mesToSend.content.size();

	std::cout << "send - " << mesToSend.content << std::endl;
	NetworkUtil::SendMessage(sock, mesToSend);
	/* Receiving result read file */
	NetworkUtil::ReceiveMessage(sock, mesToReceive);
	if (mesToReceive.typeMes != Binary) {
		std::cout << "Server invalid \n";
		ret = REQUEST_RECEIVE_REQUEST_ERROR;
	}
	else
	{
		offset += buffer.size();
	}
	close(sock);
	return ret;
}
