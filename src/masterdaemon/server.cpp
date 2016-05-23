#include "stdafx.h"

//the thread function
void *connection_handler(void *);
ResultCode ProcessMessage(Message mes, int sock);

int Recv (int& m_sock, char* buffer, unsigned int size);
int Send (int& m_sock, const char* bytes, const unsigned int& size);

int main(int argc , char *argv[])
{
    if (argc < 2) {
        std::cout << "Format invalid\n";
        return 0;
    }
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        std::cout << "[ERR] Could not create socket\n";
        return 1;
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( atoi(argv[1]) );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        //perror("bind failed. Error");
        std::cout << "[ERR] bind failed.\n";
        return 1;
    }
    puts("bind done");

    //Listen
	listen(socket_desc , 5);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
    bool isAcceptFailLast = false;
    while( true )
    {
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (client_sock < 0)
        {
            //perror("accept failed");
            if(!isAcceptFailLast) {
              std::cout << "[ERR] accept failed\n";
              isAcceptFailLast = true; 
	    }
            continue;
        }
        isAcceptFailLast = false;
        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = (int*)malloc(1);
        *new_sock = client_sock;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            //perror("could not create thread");
            std::cout << "[ERR] could not create thread\n";
            continue;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
    close(socket_desc);
    return 0;
}



/*
 * This will handle connection for each client
 * Format message: (Type message) - 1byte, (size of message) - sizeof(BYTE), (content) - ...
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char buffer[BUFSIZ];


    //Receive a message from client
    while( (read_size = recv(sock , buffer , BUFSIZ , 0)) > 0 )
    {
//        write(sock , buffer , read_size);
        std::cout << "Received signal from client\n";
        Message mes;
//        unsigned long remain_message = 0;
        std::string message = "";
        mes.typeMes = (TypeMessage)buffer[0];

        if (read_size > (int)(sizeof(unsigned long) + sizeof(BYTE))) {
            mes.sizeMes = *(unsigned long*)(buffer + sizeof(BYTE));
            message += std::string(buffer + sizeof(unsigned long) + sizeof(BYTE), read_size - sizeof(unsigned long) - sizeof(BYTE));
        }

        ResultCode res = Success;
        if (message.size() >= mes.sizeMes) {
            mes.content = message.substr(0, mes.sizeMes);
            res = ProcessMessage(mes, sock);
        } else {
            while ( (read_size = recv(sock , buffer , BUFSIZ , 0)) > 0 ) {
                message += std::string (buffer, read_size);
                if (message.size() >= mes.sizeMes) {
                    mes.content = message.substr(0, mes.sizeMes);
                    res = ProcessMessage(mes, sock);
                    break;
                }
            }
        }

        Message mesToSend;
        mesToSend.typeMes = Binary;
        mesToSend.sizeMes = sizeof(BYTE);
        BYTE temp = res;
        mesToSend.content = std::string((char*)&temp, sizeof(BYTE));
        SendMessage(sock, mesToSend);
//        BYTE temp = res;
//        write(sock , &temp , sizeof(BYTE));
//        close(sock);
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    
    // Close the socket and exit this thread
    close(sock);

    //Free the socket pointer
    free(socket_desc);

    return 0;
}

ResultCode ProcessUploadRequest(Message mes, int sock);
ResultCode ProcessDownloadRequest(Message mes, int sock);
ResultCode ProcessGetListEntryRequest(Message mes, int sock);
ResultCode ProcessDeleteDirectoryRequest(Message mes);
ResultCode ProcessGetFileSizeRequest(Message mes, int sock);
ResultCode ProcessReadFileRequest(Message mes, int sock);
ResultCode ProcessWriteFileRequest(Message mes);

ResultCode ProcessMessage(Message mes,int sock) {
    if (mes.typeMes == Text) {
        ResultCode rc = Success;
        if (mes.content.substr(0, UPLOAD_FILE_REQUEST.size()) == UPLOAD_FILE_REQUEST) {
            rc = ProcessUploadRequest(mes,sock);
        } else if (mes.content.substr(0, DOWNLOAD_FILE_REQUEST.size()) == DOWNLOAD_FILE_REQUEST) {
            rc = ProcessDownloadRequest(mes,sock);
        } else if (mes.content.substr(0, GETLISTENTRY_REQUEST.size()) == GETLISTENTRY_REQUEST) {
            rc = ProcessGetListEntryRequest(mes,sock);
		} else if (mes.content.substr(0, DELETEDIRECTORY_REQUEST.size()) == DELETEDIRECTORY_REQUEST) {
			rc = ProcessDeleteDirectoryRequest(mes);
		} else if (mes.content.substr(0, GETFILESIZE_REQUEST.size()) == GETFILESIZE_REQUEST) {
			rc = ProcessGetFileSizeRequest(mes,sock);
		} else if (mes.content.substr(0, READFILE_REQUEST.size()) == READFILE_REQUEST) {
			rc = ProcessReadFileRequest(mes,sock);
		} else if (mes.content.substr(0, WRITEFILE_REQUEST.size()) == WRITEFILE_REQUEST) {
			rc = ProcessWriteFileRequest(mes);
		}
        return rc;
    } else if (mes.typeMes == Binary) {

        return Success;
    } else {

        return UnknowFormat;
    }
}

ResultCode ProcessUploadRequest(Message mes, int sock) {
    std::cout << "Start receive file from client\n";
    std::string pathFile = mes.content.substr(UPLOAD_FILE_REQUEST.size());

    Message mesToSend;
    mesToSend.typeMes = Binary;
    mesToSend.sizeMes = sizeof(BYTE);
    BYTE temp = Success;
    mesToSend.content = std::string((char*)&temp, sizeof(BYTE));
    std::cout << "send - success\n";
    SendMessage(sock, mesToSend);

    int read_size;
    char buffer[BUFSIZ];
    /* Receiving file size */
    read_size = recv(sock, buffer, BUFSIZ, 0);
    if (read_size <= 0) {
        return Fail;
    }
    unsigned long file_size = atoi(buffer);
    std::cout << "file_size = " << file_size << std::endl;

	int ret = mkpath(pathFile.substr(0, pathFile.find_last_of("\\/")).c_str(), 0700);
    if (ret != 0) {
        return Fail;
    }
    FILE *received_file = fopen(pathFile.c_str(), "w");
    if (received_file == NULL)
    {
        fprintf(stderr, "Failed to open file.");
        return Fail;
    }
    unsigned long remain_data = file_size;

    std::cout << "send - success\n";
    SendMessage(sock, mesToSend);

    while (((read_size = recv(sock, buffer, BUFSIZ, 0)) > 0) && (remain_data > 0))
    {
        fwrite(buffer, sizeof(char), read_size, received_file);
        remain_data -= read_size;
        fprintf(stdout, "Receive %i bytes and we hope :- %li bytes\n", read_size, remain_data);
    }
    
    //Free the socket pointer
    fclose(received_file);
    return Success;
}

ResultCode ProcessDownloadRequest(Message mes, int sock) {
    Message mesToReceive;
    char buffer[BUFSIZ];
    std::cout << "Start send file to client\n";
    std::string sourceFile = mes.content.substr(DOWNLOAD_FILE_REQUEST.size());
    std::cout << "source file: " << sourceFile << std::endl;
    int fd = open(sourceFile.c_str(), O_RDONLY);
    if (fd == -1)
    {
            fprintf(stderr, "Error opening file --> %s", strerror(errno));
            return Fail;
    }

    /* Get file stats */
    struct stat file_stat;
    if (fstat(fd, &file_stat) < 0)
    {
            fprintf(stderr, "Error fstat --> %s", strerror(errno));
	    close(fd);
            return Fail;
    }

    fprintf(stdout, "File Size: %li bytes\n", file_stat.st_size);

    /* Sending file size */

  sprintf(buffer, "%li", file_stat.st_size);

  std::cout << "buffer to send: " << buffer << std::endl;

  unsigned int size = strlen(buffer);
  // Send size:
  std::cout << "send size of buf: " << size <<std::endl;
  int len = Send(sock, (char*) &size, sizeof(int));
  if(len < 0){
    std::cout << "send length fail\n";
  }
  len = Send(sock, buffer, size);
  //int len = send(sock, buffer, strlen(buffer), 0);
    if (len < 0)
    {
          fprintf(stderr, "Error on sending greetings --> %s", strerror(errno));
	  close(fd);
          return Fail;
    }

    ReceiveMessage(sock,mesToReceive);
    if (mesToReceive.typeMes != Binary || mesToReceive.sizeMes != sizeof(BYTE)) {
        std::cout << "Client invalid (1).\n";
        close(fd);
        return Fail;
    }

    ResultCode rc = (ResultCode)(*(BYTE*)mesToReceive.content.data());
    if (rc != Success) {
        std::cout << "Receive file fail (1).\n";
	close(fd);
        return rc;
    }

    unsigned long offset = 0;
    unsigned long remain_data = file_stat.st_size;
    /* Sending file data */
    int sent_bytes = 0;
    while (((sent_bytes = sendfile(sock, fd, (off_t*)&offset, BUFSIZ)) > 0) && (remain_data > 0))
    {
        remain_data -= sent_bytes;
        fprintf(stdout, "Sent %i bytes from file's data, offset is now : %li and remaining data = %li\n", sent_bytes, offset, remain_data);
	}
	ReceiveMessage(sock, mesToReceive);
    // Close file
    close(fd);
    return Success;
}

ResultCode ProcessGetListEntryRequest(Message mes, int sock) {
    std::string pathDir = mes.content.substr(GETLISTENTRY_REQUEST.size());
//    std::map<string, bool> listEntry;
    std::cout << "Start get list entry of job\n";
    std::cout << "path dir - " << pathDir << std::endl;
    std::string msg = "";
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (pathDir.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_type == DT_DIR) {
				if(ent->d_name[0] == '.') {
                    continue;
				}
				msg += std::string(ent->d_name) + "=" + "true;";
			} else {
				msg += std::string(ent->d_name) + "=" + "false;";
            }
        }
        closedir (dir);
        Message mesToSend, mesToReceive;
        mesToSend.typeMes = Text;
        mesToSend.sizeMes = msg.size();
        mesToSend.content = msg;
        SendMessage(sock, mesToSend);
		ReceiveMessage(sock, mesToReceive);
        return Success;
    } else {
        return Fail;
    }
}

int delete_folder_tree (const char* directory_name) {
	DIR*            dp;
	struct dirent*  ep;

	dp = opendir(directory_name);

	if (dp == NULL) {
		printf ("Cannot open directory '%s'\n", directory_name);
		return -1;
	}

	while ((ep = readdir(dp)) != NULL) {
	if (strcmp(ep->d_name,".") != 0 && strcmp(ep->d_name,"..") != 0) {
		if (ep->d_type == DT_DIR) {
			printf("dir: %s\n",  ep->d_name);
			std::string path = std::string(directory_name) + "/" + ep->d_name;
				delete_folder_tree(path.c_str());
		} else {
			printf("file: %s\n",  ep->d_name);
			std::string path = std::string(directory_name) + "/" + ep->d_name;
			remove(path.c_str());
		}
	}
	}
	//std::cout << "close dir after read\n";
	closedir(dp);
	//std::cout << "close dir finished\n";
	return rmdir(directory_name);
}

ResultCode ProcessDeleteDirectoryRequest(Message mes) {
	std::cout << "Start delete directory\n";
	std::string pathDir = mes.content.substr(DELETEDIRECTORY_REQUEST.size());
	std::cout << "path - " << pathDir.c_str() << std::endl;
	if (!delete_folder_tree(pathDir.c_str())) {
		return Success;
	} else {
		return Fail;
	}
}

ResultCode ProcessGetFileSizeRequest(Message mes, int sock) {
	Message mesToReceive;
	std::cout << "Start send file size to client\n";

	std::string sourceFile = mes.content.substr(GETFILESIZE_REQUEST.size());
        std::cout << "path file: " << sourceFile.c_str() << std::endl;
	int fd = open(sourceFile.c_str(), O_RDONLY);
	if (fd == -1)
	{
			fprintf(stderr, "Error opening file --> %s\n", strerror(errno));
			return Fail;
	}

	/* Get file stats */
	struct stat file_stat;
	if (fstat(fd, &file_stat) < 0)
	{
			fprintf(stderr, "Error fstat --> %s", strerror(errno));
			close(fd);
			return Fail;
	}

	fprintf(stdout, "File Size: %li bytes\n", file_stat.st_size);

	/* Sending file size */
	std::stringstream ss;
	ss << file_stat.st_size;
	std::string size_file = ss.str();

	Message mesToSend;
	mesToSend.typeMes = Text;
	mesToSend.content = size_file;
	mesToSend.sizeMes = size_file.size();
	std::cout << "send - file_size\n";
	SendMessage(sock, mesToSend);
	ReceiveMessage(sock,mesToReceive);
        // Close the file 
        close(fd);
	return Success;
}

ResultCode ProcessReadFileRequest(Message mes, int sock) {
	Message mesToReceive;
	std::cout << "Start read file, send to client\n";
	std::string content = mes.content.substr(READFILE_REQUEST.size());
	std::string pathFile, offset_str, size_buf_str;
	std::vector<std::string> array_str;
	int numberEntry = Split(content, array_str, ",");
	if (numberEntry == 3) {
		pathFile = array_str[0];
		offset_str = array_str[1];
		size_buf_str = array_str[2];
	} else {
		return Fail;
	}
	size_t offset = atoi(offset_str.c_str());
	size_t size_buf = atoi(size_buf_str.c_str());

	FILE * pFile;
	char* buffer = new char[size_buf];
	size_t result;

	pFile = fopen ( pathFile.c_str() , "rb" );
	if (pFile==NULL)
	{
		fprintf(stderr,"Open file error:%s\n", pathFile.c_str());
		return Fail;
	}
	fseek ( pFile , offset , SEEK_SET );
	result = fread (buffer,1,size_buf,pFile);
	if (result < 0) {
		fclose (pFile);
		delete[] buffer;
		return Fail;
	}

	std::string content_send(buffer, result);


	Message mesToSend;
	mesToSend.typeMes = Binary;
	mesToSend.content = content_send;
	mesToSend.sizeMes = result;
	std::cout << "send - buffer read file\n";
	SendMessage(sock, mesToSend);
	ReceiveMessage(sock,mesToReceive);
	// close the file
	fclose(pFile);
	// relese buffer
	delete[] buffer;
	return Success;
}

ResultCode ProcessWriteFileRequest(Message mes) {
	std::cout << "Start write file\n";
	std::string content = mes.content.substr(WRITEFILE_REQUEST.size());
	std::cout << "content : " << content << std::endl;
	std::string pathFile, offset_str, buffer;
	std::vector<std::string> array_str;
	int numberEntry = Split(content, array_str, ",");
	if (numberEntry >= 3) {

		pathFile = array_str[0];
		offset_str = array_str[1];
		buffer = content.substr(pathFile.size() + offset_str.size() + 2);
		std::cout << buffer << std::endl;
	} else {
		return Fail;
	}
	size_t offset = atoi(offset_str.c_str());

	FILE * pFile;
	size_t result;
	if (offset == 0) {
		std::cout << "wb" << std::endl;
		pFile = fopen ( pathFile.c_str() , "wb" );
	} else {
		std::cout << "ab" << std::endl;
		pFile = fopen ( pathFile.c_str() , "ab" );
	}
	if (pFile==NULL)
	{
		fprintf(stderr,"Open file error:%s\n", pathFile.c_str());
		return Fail;
	}
	result = fwrite (buffer.data() , 1, buffer.size(), pFile);
	std::cout << result << std::endl;
	if (result != buffer.size()) {
		fclose (pFile);
		return Fail;
	}
	fclose (pFile);
	return Success;
}


int Recv (int& m_sock, char* buffer, unsigned int size) {
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

int Send (int& m_sock, const char* bytes, const unsigned int& size) {
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
