#include "NetworkUtil.hpp"
#include "../stdafx.h"

extern std::string UPLOAD_FILE_REQUEST;
extern std::string DOWNLOAD_FILE_REQUEST;
extern std::string GETLISTENTRY_REQUEST;
extern std::string DELETEDIRECTORY_REQUEST;

void NetworkUtil::SendMessage(int sock, MessageStruct mes) {
	BYTE type = mes.typeMes;
	std::string msg((char*)&type,sizeof(BYTE));
	msg += std::string((char*)&mes.sizeMes, sizeof(unsigned long));
	msg += mes.content;
  write(sock , msg.data() , msg.size());
}

int NetworkUtil::Recv (int& m_sock, char* buffer, unsigned int size) {
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
     std::cout << "Socket receieved ("<< n << " bytes).\n";
   }
  }
  return in_bytes;
}

int NetworkUtil::Send (int& m_sock, const char* bytes, const unsigned int& size) {
  std::cout << "Socket::Send bytes size =  " << size << std::endl;
  unsigned int out_bytes = 0;

  while (out_bytes < size) {
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

void NetworkUtil::ReceiveMessage(int sock, MessageStruct &mes) {
	int sizeHeader = sizeof(BYTE) + sizeof(unsigned long);
	char buffer[BUFSIZ];
	std::string msg = "";

  int read_size = 0;
  memset(buffer, BUFSIZ, 0);
	while (msg.size() < sizeHeader && ((read_size = recv(sock, buffer, BUFSIZ, 0)) > 0)) {
		msg += std::string(buffer,read_size);
	}
	if (msg.size() < sizeHeader) {
		std::cout << "message received error\n";
		return;
	}

	mes.typeMes = (TypeMessage)(*(BYTE*)buffer);
	mes.sizeMes = *(unsigned long*)(buffer+1);
	msg = msg.substr(sizeof(unsigned long) + sizeof(BYTE));

  //memset(buffer, BUFSIZ, 0);
	while (((mes.sizeMes - msg.size()) > 0) && ((read_size = recv(sock, buffer, BUFSIZ, 0)) > 0)) {
    msg += std::string(buffer, read_size);
    memset(buffer, BUFSIZ, 0);
	}

	mes.content = msg.substr(0, mes.sizeMes);
}


static int do_mkdir(const char *path, mode_t mode)
{
	struct stat     st;
	int             status = 0;
	if (strlen(path) == 0) {
		return status;
	}
	if (stat(path, &st) != 0)
	{
		/* Directory does not exist. EEXIST for race condition */
		if (mkdir(path, mode) != 0 && errno != EEXIST)
			status = -1;
	}
	else if (!S_ISDIR(st.st_mode))
	{
		errno = ENOTDIR;
		status = -1;
	}

	return(status);
}

int mkpath(const char *path, mode_t mode)
{
	char           *pp;
	char           *sp;
	int             status;
	char           *copypath = strdup(path);

	status = 0;
	pp = copypath;
	while (status == 0 && (sp = strchr(pp, '/')) != 0)
	{
		if (sp != pp)
		{
			/* Neither root nor double slash in path */
			*sp = '\0';
			status = do_mkdir(copypath, mode);
			*sp = '/';
		}
		pp = sp + 1;
	}
	if (status == 0)
		status = do_mkdir(path, mode);
	free(copypath);
	return (status);
}

ResponseCode NetworkUtil::requestHttp(std::string hostIp,std::string port, std::string request_str,std::string &respone_str, std::string &headerRespone_str) {
    int sock;
    struct sockaddr_in client;
    struct in_addr ip;
    struct hostent * host;

    if (!inet_aton(hostIp.c_str(), &ip)) {
        host = gethostbyname(hostIp.c_str());
    } else {
        host  = gethostbyaddr((const void *)&ip,sizeof(ip),AF_INET);
    }

    if ( (host == NULL) || (host->h_addr == NULL) ) {
        std::cout << "Error retrieving DNS information. \n";
        return REQUEST_UNKNOW_HOST;
    }

    bzero(&client, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons( atoi(port.c_str()) );
    memcpy(&client.sin_addr, host->h_addr, host->h_length);
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        std::cout << "Error creating socket. \n";
        return REQUEST_SOCKET_ERROR;
    }

    if ( connect(sock, (struct sockaddr *)&client, sizeof(client)) < 0 ) {
        close(sock);
        std::cout << "Could not connect \n";
        return REQUEST_CONNECT_ERROR;
    }


	int send_length = send(sock, request_str.data(), request_str.length(), 0);
	std::cout << "length send = " << send_length << std::endl;
	std::cout << "length request = " << request_str.length() << std::endl;
	if ( send_length != (int)request_str.length()) {
        close(sock);
        std::cout << "Error sending request. \n";
        return REQUEST_SEND_REQUEST_ERROR;
    }
    //
    std::string result = "";
    std::string chunk = "";
    char cur[BUFSIZ];
    long size_data = -1;
    long size_data_chunk = -1;
    int length = 0;
    short formatResponse = -1;
    do {
        length = read(sock, cur, BUFSIZ);
        cur[length] = NULL;
        std::string temp(cur,length);
        if (formatResponse == 2) {
            chunk = chunk + temp;
        } else {
            result = result + temp;
        }
        if (formatResponse == -1) {
            int pos_fin_header = result.find("\r\n\r\n");
            if (pos_fin_header >= 0) {
                headerRespone_str = result.substr(0, pos_fin_header);
                formatResponse = 0;
                int pos = result.find("\r\nContent-Length: ");
                if (pos >= 0) {
                    std::cout << "normal \n";
                    int end_line = result.find("\r\n",pos + strlen("\r\nContent-Length: "));
                    std::string size_data_str = result.substr(pos + strlen("\r\nContent-Length: ")
                                ,end_line - pos - strlen("\r\nContent-Length: "));
                    formatResponse = 1;
                    size_data = atoi (size_data_str.c_str());
                    result = result.substr(pos_fin_header + strlen("\r\n\r\n"));
                    if (result.length() >= size_data) {
                        break;
                    }
				} else if ( (pos = result.find("\r\nTransfer-Encoding: ")) >= 0) {
					int end_line = result.find("\r\n",pos + strlen("\r\nTransfer-Encoding: "));
					std::string transfer_encoding=result.substr(pos + strlen("\r\nTransfer-Encoding: ")
							,end_line - pos - strlen("\r\nTransfer-Encoding: "));
					if (transfer_encoding.compare("chunked") == 0) {
						std::cout << "chunked \n";
						formatResponse = 2;
						chunk = result.substr(pos_fin_header + strlen("\r\n\r\n"));
						result = "";
						while (true) {
							if (size_data_chunk < 0) {
								int pos_fin_header = chunk.find("\r\n");
								if (pos_fin_header >= 0) {

									std::string size_data_str = chunk.substr(0,pos_fin_header);
									char *p;
									size_data_chunk = strtol( size_data_str.c_str(), & p, 16 );
									if ( *p != 0 ) {
										std::cout << "String is not number. \n";
									}
									if (size_data_chunk == 0) {
										break;
									}
									chunk = chunk.substr(pos_fin_header + strlen("\r\n"));
								} else {
									break;
								}
							} else {
								if (chunk.length() >= size_data_chunk) {
									std::cout << "read chunk finish \n";
									result = result + chunk.substr(0,size_data_chunk);
									chunk = chunk.substr(size_data_chunk + 2);
									size_data += size_data_chunk;
									size_data_chunk = -1;
								}
							}
						}
						if (size_data_chunk == 0) {
							break;
						}
                    }
				} else {
					int pos_test = result.find(DOCKER_RESPONE_HTTP_204);
					std::cout << "pos_test = " << pos_test << std::endl;
					if (pos_test >= 0) {
						std::cout << "respone from server is no content : " << result << std::endl;
					}

					pos_test = result.find(DOCKER_RESPONE_HTTP_304);
					std::cout << "pos_test = " << pos_test << std::endl;
					if (pos_test >= 0) {
						std::cout << "respone from server is not Modified : " << result << std::endl;
					}

					headerRespone_str = result;
					result = "";
					break;
				}
            }
        } else {
            if (formatResponse == 2) {
                if (size_data_chunk < 0) {
                    int pos_fin_header = chunk.find("\r\n");
                    if (pos_fin_header >= 0) {

                        std::string size_data_str = chunk.substr(0,pos_fin_header);
                        char *p;
                        size_data_chunk = strtol( size_data_str.c_str(), & p, 16 );
                        if ( *p != 0 ) {
                            std::cout << "String is not number. \n";
                        }
                        if (size_data_chunk == 0) {
                            break;
                        }
                        chunk = chunk.substr(pos_fin_header + strlen("\r\n"));
                    }
                } else {
                    if (chunk.length() >= size_data_chunk) {
                        std::cout << "read chunk finish \n";
                        result = result + chunk.substr(0,size_data_chunk);
                        chunk = chunk.substr(size_data_chunk + 2);
                        size_data += size_data_chunk;
                        size_data_chunk = -1;
                    }
                }
            } else {
                if (result.length() >= size_data) {
                    break;
                }
            }
        }
    }
    while (length > 0 );
    close(sock);
    respone_str = result;
    return REQUEST_SUCCESS;
}

ResponseCode NetworkUtil::getIpAddressOfContainer(std::string container_Id, std::string &IpAddress) {
	std::cout << "getIpAddressOfContainer \n";
	ResponseCode ret = REQUEST_DOCKER_ERROR;

	//create master-instance for user
	std::stringstream ss;
	ss << "GET /containers/" << container_Id << "/json HTTP/1.1\r\n";
	ss << "Host: " << DockerTcp_IP << "\r\n\r\n";

	std::string request = ss.str();
	std::cout << request << std::endl;
	std::string result, header_result;
	NetworkUtil::requestHttp(DockerTcp_IP, DockerTcp_Port, request, result, header_result);
  std::cout << "response: " << result.c_str() << std::endl;
	std::size_t found = header_result.find(DOCKER_RESPONE_HTTP_200);
	if (found == std::string::npos) {
		std::cout << "Request docker get info container error. \n";
		std::cout << "Content error: \n" << result << std::endl;
		return REQUEST_DOCKER_ERROR;
	}

	JSONNode n;
	try
	{
		n = libjson::parse(result);
	} catch(const std::invalid_argument& e) {
		std::cerr << "Lib json error. Please check : " << e.what() << '\n';
		return DATA_ERROR;
	}

	JSONNode::const_iterator i = n.find(DOCKER_TAG_RESPONE_NETWORK_SETTING);
	if (i == n.end() || i -> type() != JSON_NODE ) {
		std::cout << "Respone error.\n";
		return DATA_ERROR;
	}
	JSONNode networkNode = i->as_node();
	i = networkNode.find(DOCKER_TAG_RESPONE_IP_ADDRESS);
	if (i == n.end()) {
		std::cout << "Respone error.\n";
		return DATA_ERROR;
	}

	IpAddress = i->as_string();
	std::cout << "IpAddress = " << IpAddress << std::endl;
	ret = REQUEST_SUCCESS;
	return ret;
}

ResponseCode NetworkUtil::ProcessDownloadFile (std::string host, std::string port, std::string sourceFile, std::string desFile) {
  std::cout << "NetworkUtil::ProcessDownloadFile \n";
	ResponseCode ret = FILE_ACTION_SUCCESS;
	//Connect server
	int sock;
	struct sockaddr_in server;

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
	} else {
		puts("Connected\n");

    int read_size = 0;
    char buffer[BUFSIZ] = { '\0' };
    MessageStruct mesToSend;

		mesToSend.typeMes = Text;
		mesToSend.content = DOWNLOAD_FILE_REQUEST + sourceFile;
		mesToSend.sizeMes = mesToSend.content.size();

		std::cout << "send - " << mesToSend.content << std::endl;

    SendMessage(sock, mesToSend);
    //int sentbytes = Send(sock, )

		/* Receiving file size */
    //read_size = recv(sock, buffer, BUFSIZ, 0); //tuannp comment

    int ms = 0;
    read_size = Recv(sock, (char*)&ms, sizeof(int));
    if(read_size < sizeof(int)){
      std::cout << "size of message is missing \n";
    }
    std::cout << "message size = " << ms << std::endl;
    read_size = Recv(sock, buffer, ms);
    //read_size = Recv(sock, buffer, ms)
    if(read_size != ms){
      std::cout << "message content is missing \n";
    }
    buffer[ms] = '\0';

    std::cout  << "recv buffer: " << buffer << std::endl;
    if (read_size <= 0) {
			ret = REQUEST_RECEIVE_REQUEST_ERROR;
		} else {
			unsigned long file_size = atoi(buffer);
			std::cout << "file_size = " << file_size << std::endl;

			FILE *received_file = fopen(desFile.c_str(), "w");
			if (received_file == NULL)
			{
				fprintf(stderr, "Failed to open file.");
				return FILE_ACTION_ERROR;
			} else {
                long remain_data = file_size;

				mesToSend.typeMes = Binary;
				mesToSend.sizeMes = sizeof(BYTE);
				BYTE temp = Success;
				mesToSend.content = std::string((char*)&temp, sizeof(BYTE));
				std::cout << "send - success\n";
				SendMessage(sock, mesToSend);

                while ((remain_data > 0) && ((read_size = recv(sock, buffer, BUFSIZ, 0)) > 0))
				{
					fwrite(buffer, sizeof(char), read_size, received_file);
					remain_data -= read_size;
					fprintf(stdout, "Receive %d bytes and we hope :- %d bytes\n", read_size, remain_data);
				}
				fclose(received_file);
			}
		}
	}
	close(sock);
	return ret;
}

ResponseCode NetworkUtil::ProcessUploadFile(std::string host, std::string port, std::string sourceFile, std::string desFile) {
	ResponseCode ret = FILE_ACTION_SUCCESS;
	//Variable local
	int sock;
	struct sockaddr_in server;
	char buffer[BUFSIZ];
  MessageStruct mesToSend, mesToReceive;
	struct stat file_stat;
	ResultCode rc;
	int fd;

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
	}
	if (ret == FILE_ACTION_SUCCESS) {
		puts("Connected\n");

		std::cout << "Start upload file\n";

		mesToSend.typeMes = Text;
		mesToSend.content = UPLOAD_FILE_REQUEST + desFile;
		mesToSend.sizeMes = mesToSend.content.size();

		std::cout << "send - " << mesToSend.content << std::endl;
		SendMessage(sock, mesToSend);

		ReceiveMessage(sock,mesToReceive);
		if (mesToReceive.typeMes != Binary || mesToReceive.sizeMes != sizeof(BYTE)) {
			std::cout << "Server invalid (1).\n";
			ret = REQUEST_RECEIVE_REQUEST_ERROR;
		}
	}

	if (ret == FILE_ACTION_SUCCESS) {
		rc = (ResultCode)(*(BYTE*)mesToReceive.content.data());
		if (rc != Success) {
			std::cout << "Upload file fail (1).\n";
			ret = FILE_ACTION_ERROR;
		}
	}

	if (ret == FILE_ACTION_SUCCESS) {
		std::cout << "Receive signal : Success\n";

		fd = open(sourceFile.c_str(), O_RDONLY);
		if (fd == -1)
		{
				fprintf(stderr, "Error opening file --> %s", strerror(errno));
				ret = FILE_ACTION_ERROR;
		}
	}

	if (ret == FILE_ACTION_SUCCESS) {
		/* Get file stats */
		if (fstat(fd, &file_stat) < 0)
		{
				fprintf(stderr, "Error fstat --> %s", strerror(errno));
				ret = FILE_ACTION_ERROR;
		}
	}

	if (ret == FILE_ACTION_SUCCESS) {
		fprintf(stdout, "File Size: %d bytes\n", file_stat.st_size);

		/* Sending file size */

		sprintf(buffer, "%d\0", file_stat.st_size);
		int len = send(sock, buffer, strlen(buffer), 0);
		std::cout << "length of message file size : " << len << std::endl;
		if (len < 0)
		{
			  fprintf(stderr, "Error on sending greetings --> %s", strerror(errno));
			  ret = FILE_ACTION_ERROR;
		}
	}

	if (ret == FILE_ACTION_SUCCESS) {
		ReceiveMessage(sock,mesToReceive);
		if (mesToReceive.typeMes != Binary || mesToReceive.sizeMes != sizeof(BYTE)) {
			std::cout << "Server invalid (2).\n";
			ret = REQUEST_RECEIVE_REQUEST_ERROR;
		}
	}

	if (ret == FILE_ACTION_SUCCESS) {
		rc = (ResultCode)(*(BYTE*)mesToReceive.content.data());
		std::cout << "ResultCode = " << rc << std::endl;
		if (rc != Success) {
			std::cout << "Upload file fail (2).\n";
			ret = FILE_ACTION_ERROR;
		}
	}

	if (ret == FILE_ACTION_SUCCESS) {
        long offset = 0;
        long remain_data = file_stat.st_size;
		/* Sending file data */
		int sent_bytes = 0;
        while ( (remain_data > 0) && ((sent_bytes = sendfile(sock, fd, (off_t*)&offset, BUFSIZ)) > 0))
		{
			remain_data -= sent_bytes;
			fprintf(stdout, "Sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
		}
          // Close descriptor for file that was sent */
          close(fd);
	}
	close(sock);
	return ret;
}

ResponseCode NetworkUtil::ProcessUploadDir(std::string host, std::string port, std::string sourceDir, std::string desDir) {
	std::cout << "Start upload dir \n";
  std::cout <<host.c_str() << port.c_str() << sourceDir.c_str() << desDir.c_str() << std::endl;

	DIR*            dp;
	struct dirent*  ep;
	dp = opendir(sourceDir.c_str());

	if (dp == NULL) {
		printf ("Cannot open directory '%s'\n", sourceDir.c_str());
		return FILE_ACTION_ERROR;
	}

	while ((ep = readdir(dp)) != NULL) {
	if (strcmp(ep->d_name,".") != 0 && strcmp(ep->d_name,"..") != 0) {
		if (ep->d_type == DT_DIR) {
			printf("dir: %s \n",  ep->d_name);
			ProcessUploadDir(host, port, sourceDir + "/" + ep->d_name, desDir + "/" + ep->d_name);
		} else {
			printf("file: %s \n",  ep->d_name);
			std::string sourceFile = sourceDir + "/" + ep->d_name;
			std::string desFile = desDir + "/" + ep->d_name;

//			//Connect server
//			int sock;
//			struct sockaddr_in server;

//			//Create socket
//			sock = socket(AF_INET , SOCK_STREAM , 0);
//			if (sock == -1)
//			{
//				printf("Could not create socket");
//			}
//			puts("Socket created");

//			server.sin_addr.s_addr = inet_addr(host.c_str());
//			server.sin_family = AF_INET;
//			server.sin_port = htons( atoi(port.c_str()) );

//			//Connect to remote server
//			if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
//			{
//				perror("connect failed. Error");
//				return REQUEST_SOCKET_ERROR;
//			}
//			puts("Connected\n");

			ProcessUploadFile(host, port, sourceFile, desFile);
//			close(sock);
		}
	}
	}
	std::cout << "close dir after upload \n";
	closedir(dp);
	std::cout << "close dir finished \n";
	return FILE_ACTION_SUCCESS;
}

ResponseCode NetworkUtil::ProcessDeleteDir(std::string host, std::string port, std::string pathDir) {
	std::cout << "ProcessDeleteDir \n";
	ResponseCode ret = FILE_ACTION_SUCCESS;
	//Connect server
	int sock;
	struct sockaddr_in server;

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
	} else {
		puts("Connected\n");
    MessageStruct mesToSend, mesToReceive;

		mesToSend.typeMes = Text;
		mesToSend.content = DELETEDIRECTORY_REQUEST + pathDir;
		mesToSend.sizeMes = mesToSend.content.size();

		std::cout << "send - " << mesToSend.content << std::endl;
		SendMessage(sock, mesToSend);
		/* Receiving result delete dir */
		ReceiveMessage(sock, mesToReceive);
		if (mesToReceive.typeMes != Binary || mesToReceive.sizeMes != sizeof(BYTE)) {
			std::cout << "Server invalid \n";
			return REQUEST_RECEIVE_REQUEST_ERROR;
		}
		ResultCode rc = (ResultCode)(*(BYTE*)mesToReceive.content.data());
		if (rc == Success) {
			ret = REQUEST_SUCCESS;
		} else {
			ret = FILE_ACTION_ERROR;
		}
	}
	close(sock);
	return ret;
}

ResponseCode NetworkUtil::ProcessGetListEntry(std::string host, std::string port, std::string pathDir, std::map<std::string, bool> &listEntry) {
	std::cout << "ProcessGetListEntry \n";
	ResponseCode ret = FILE_ACTION_SUCCESS;
	//Connect server
	int sock;
	struct sockaddr_in server;

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
	} else {
		puts("Connected\n");
    MessageStruct mesToSend, mesToReceive;
		mesToSend.typeMes = Text;
		mesToSend.content = GETLISTENTRY_REQUEST + pathDir;
		mesToSend.sizeMes = mesToSend.content.size();

		std::cout << "send - " << mesToSend.content << std::endl;
		SendMessage(sock, mesToSend);

		ReceiveMessage(sock,mesToReceive);
		if (mesToReceive.typeMes != Text) {
			std::cout << "Server invalid (1).\n";
			return REQUEST_RECEIVE_REQUEST_ERROR;
		}
		std::cout << mesToReceive.content << std::endl;

		std::vector<std::string> array_str;
		int numberEntry = StringUtils::Split(mesToReceive.content, array_str, ";");
		if (numberEntry > 0) {
			for (int i = 0; i < numberEntry; i++) {
				std::vector<std::string> entry_type;
				int size = StringUtils::Split(array_str[i],entry_type,"=");
				if (size == 2) {
					listEntry[entry_type[0]] = (bool)entry_type[1].compare("false");
				}
			}
		}
	}
	close(sock);
	return ret;


}
