#include "stdafx.h"

void ProcessUploadFile(int sock, std::string sourceFile, std::string desFile);
void ProcessDownloadFile (int sock, std::string sourceFile, std::string desFile);
void ProcessUploadDir(std::string host, std::string port, std::string sourceDir, std::string desDir);
void ProcessGetListEntry(int sock, std::string pathDir);
void ProcessDeleteDir(int sock, std::string pathDir);
void ProcessGetFileSize(int sock, std::string pathFile);
void ProcessReadFile(int sock, std::string pathFile,size_t offset,size_t buff_size);
void ProcessWriteFile(int sock, std::string pathFile,size_t offset, std::string buffer);

int main(int argc , char *argv[])
{
    //Parse info
    if (argc < 4) {
        std::cout << "Format command invalid\n";
        std::cout << "./client \"UploadFile\" (host) (port) (source file) (destination file)\n";
        std::cout << "./client \"DownloadFile\" (host) (port) (source file) (destination file)\n";
		std::cout << "./client \"UploadDirectory\" (host) (port) (source dir) (destination dir)\n";
		std::cout << "./client \"GetListEntry\" (host) (port) (path directory)\n";
		std::cout << "./client \"DeleteDirectory\" (host) (port) (path directory)\n";
		std::cout << "./client \"GetFileSize\" (host) (port) (path file)\n";
		std::cout << "./client \"ReadFile\" (host) (port) (path file) (offset) (buff_size)\n";
		std::cout << "./client \"WriteFile\" (host) (port) (path file) (offset) (content)\n";
        return 0;
    }
    std::string command = argv[1];
    std::string host = argv[2];
    std::string port = argv[3];

    //Connect server
    int sock;
    struct sockaddr_in server;

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr(host.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons( atoi(port.c_str()) );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    puts("Connected\n");

    //
    if (command.compare("UploadFile") == 0) {
        if (argc < 6) {
            std::cout << "Format command invalid\n";
            return 0;
        }
        std::string sourceFile = argv[4];
        std::string destinationFile = argv[5];
        ProcessUploadFile(sock, sourceFile, destinationFile);
    } else if (command.compare("DownloadFile") == 0) {
        if (argc < 6) {
            std::cout << "Format command invalid\n";
            return 0;
        }
        std::string sourceFile = argv[4];
        std::string destinationFile = argv[5];
        ProcessDownloadFile(sock, sourceFile, destinationFile);
	} else if (command.compare("UploadDirectory") == 0) {
		if (argc < 6) {
			std::cout << "Format command invalid\n";
			return 0;
		}
		std::string sourceDir = argv[4];
		std::string destinationDir = argv[5];
		ProcessUploadDir(host, port, sourceDir, destinationDir);
	} else if (command.compare("GetListEntry") == 0) {
		if (argc < 5) {
			std::cout << "Format command invalid\n";
			return 0;
		}
		std::string pathDir = argv[4];
		ProcessGetListEntry(sock, pathDir);
	}  else if (command.compare("DeleteDirectory") == 0) {
		if (argc < 5) {
			std::cout << "Format command invalid\n";
			return 0;
		}
		std::string pathDir = argv[4];
		ProcessDeleteDir(sock, pathDir);
	}  else if (command.compare("GetFileSize") == 0) {
		if (argc < 5) {
			std::cout << "Format command invalid\n";
			return 0;
		}
		std::string pathFile = argv[4];
		ProcessGetFileSize(sock, pathFile);
	}  else if (command.compare("ReadFile") == 0) {
		if (argc < 7) {
			std::cout << "Format command invalid\n";
			return 0;
		}
		std::string pathFile = argv[4];
		size_t offset = atoi(argv[5]);
		size_t buff_size = atoi(argv[6]);
		ProcessReadFile(sock, pathFile, offset, buff_size);
	}  else if (command.compare("WriteFile") == 0) {
		if (argc < 7) {
			std::cout << "Format command invalid\n";
			return 0;
		}
		std::string pathFile = argv[4];
		size_t offset = atoi(argv[5]);
		std::string buffer = argv[6];
		ProcessWriteFile(sock, pathFile, offset, buffer);
	}

    close(sock);
    return 0;
}

void ProcessDownloadFile (int sock, std::string sourceFile, std::string desFile) {
    int read_size;
    char buffer[BUFSIZ];
    Message mesToSend, mesToReceive;

    mesToSend.typeMes = Text;
    mesToSend.content = DOWNLOAD_FILE_REQUEST + sourceFile;
    mesToSend.sizeMes = mesToSend.content.size();

    std::cout << "send - " << mesToSend.content << std::endl;
    SendMessage(sock, mesToSend);
    /* Receiving file size */
    read_size = recv(sock, buffer, BUFSIZ, 0);
    if (read_size <= 0) {
        return;
    }
    unsigned long file_size = atoi(buffer);
    std::cout << "file_size = " << file_size << std::endl;

    FILE *received_file = fopen(desFile.c_str(), "w");
    if (received_file == NULL)
    {
        fprintf(stderr, "Failed to open file.");
        return;
    }
    unsigned long remain_data = file_size;

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

void ProcessUploadFile(int sock, std::string sourceFile, std::string desFile) {
    std::cout << "Start upload file\n";
    char buffer[BUFSIZ];
    Message mesToSend, mesToReceive;
    mesToSend.typeMes = Text;
    mesToSend.content = UPLOAD_FILE_REQUEST + desFile;
    mesToSend.sizeMes = mesToSend.content.size();

    std::cout << "send - " << mesToSend.content << std::endl;
    SendMessage(sock, mesToSend);

    ReceiveMessage(sock,mesToReceive);
    if (mesToReceive.typeMes != Binary || mesToReceive.sizeMes != sizeof(BYTE)) {
        std::cout << "Server invalid (1).\n";
        return;
    }

    ResultCode rc = (ResultCode)(*(BYTE*)mesToReceive.content.data());
    if (rc != Success) {
        std::cout << "Upload file fail (1).\n";
        return;
    }
    std::cout << "Receive signal : Success\n";


    int fd = open(sourceFile.c_str(), O_RDONLY);
    if (fd == -1)
    {
            fprintf(stderr, "Error opening file --> %s", strerror(errno));
            return;
    }

    /* Get file stats */
    struct stat file_stat;
    if (fstat(fd, &file_stat) < 0)
    {
            fprintf(stderr, "Error fstat --> %s", strerror(errno));
            return;
    }

    fprintf(stdout, "File Size: %d bytes\n", file_stat.st_size);

    /* Sending file size */

    sprintf(buffer, "%d", file_stat.st_size);
    int len = send(sock, buffer, sizeof(buffer), 0);
    if (len < 0)
    {
          fprintf(stderr, "Error on sending greetings --> %s", strerror(errno));
          return;
    }

    ReceiveMessage(sock,mesToReceive);
    if (mesToReceive.typeMes != Binary || mesToReceive.sizeMes != sizeof(BYTE)) {
        std::cout << "Server invalid (2).\n";
        return;
    }

    rc = (ResultCode)(*(BYTE*)mesToReceive.content.data());
    if (rc != Success) {
        std::cout << "Upload file fail (2).\n";
        return;
    }

    unsigned long offset = 0;
    unsigned long remain_data = file_stat.st_size;
    /* Sending file data */
    int sent_bytes = 0;
    while (((sent_bytes = sendfile(sock, fd, (off_t*)&offset, BUFSIZ)) > 0) && (remain_data > 0))
    {
        remain_data -= sent_bytes;
        fprintf(stdout, "Sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
    }
}

void ProcessUploadDir(std::string host, std::string port, std::string sourceDir, std::string desDir) {
	std::cout << "Start upload dir \n";


	DIR*            dp;
	struct dirent*  ep;
	dp = opendir(sourceDir.c_str());

	if (dp == NULL) {
		printf ("Cannot open directory '%s'\n", sourceDir.c_str());
		return;
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

			//Connect server
			int sock;
			struct sockaddr_in server;

			//Create socket
			sock = socket(AF_INET , SOCK_STREAM , 0);
			if (sock == -1)
			{
				printf("Could not create socket");
			}
			puts("Socket created");

			server.sin_addr.s_addr = inet_addr(host.c_str());
			server.sin_family = AF_INET;
			server.sin_port = htons( atoi(port.c_str()) );

			//Connect to remote server
			if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
			{
				perror("connect failed. Error");
				return;
			}
			puts("Connected\n");

			ProcessUploadFile(sock, sourceFile, desFile);
			close(sock);
		}
	}
	}
	std::cout << "close dir after upload \n";
	closedir(dp);
	std::cout << "close dir finished \n";
}

void ProcessGetListEntry(int sock, std::string pathDir) {
	std::cout << "ProcessGetListEntry \n";
	Message mesToSend, mesToReceive;
	mesToSend.typeMes = Text;
	mesToSend.content = GETLISTENTRY_REQUEST + pathDir;
	mesToSend.sizeMes = mesToSend.content.size();

	std::cout << "send - " << mesToSend.content << std::endl;
	SendMessage(sock, mesToSend);

	ReceiveMessage(sock,mesToReceive);
	if (mesToReceive.typeMes != Text) {
		std::cout << "Server invalid (1).\n";
		return;
	}

	std::cout << mesToReceive.content << std::endl;
	std::vector<std::string> array_str;
	int numberEntry = Split(mesToReceive.content, array_str, ";");
	if (numberEntry > 0) {
		for (int i = 0; i < numberEntry; i++) {
			std::vector<std::string> entry_type;
			int size = Split(array_str[i],entry_type,"=");
			if (size == 2) {
				std::cout << entry_type[0] << " is " << entry_type[1] << std::endl;
			}
		}
	}
}

void ProcessDeleteDir(int sock, std::string pathDir) {
	std::cout << "ProcessDeleteDir \n";

	Message mesToSend, mesToReceive;

	mesToSend.typeMes = Text;
	mesToSend.content = DELETEDIRECTORY_REQUEST + pathDir;
	mesToSend.sizeMes = mesToSend.content.size();

	std::cout << "send - " << mesToSend.content << std::endl;
	SendMessage(sock, mesToSend);
	/* Receiving result delete dir */
	ReceiveMessage(sock, mesToReceive);
	if (mesToReceive.typeMes != Binary || mesToReceive.sizeMes != sizeof(BYTE)) {
		std::cout << "Server invalid \n";
		return;
	}
	ResultCode rc = (ResultCode)(*(BYTE*)mesToReceive.content.data());
	if (rc == Success) {
		std::cout << "delete ok \n";
	} else {
		std::cout << "delete fail \n";
	}
}

void ProcessGetFileSize(int sock, std::string pathFile) {
	std::cout << "ProcessGetFileSize \n";

	Message mesToSend, mesToReceive;

	mesToSend.typeMes = Text;
	mesToSend.content = GETFILESIZE_REQUEST + pathFile;
	mesToSend.sizeMes = mesToSend.content.size();

	std::cout << "send - " << mesToSend.content << std::endl;
	SendMessage(sock, mesToSend);
	/* Receiving result get size file */
	ReceiveMessage(sock, mesToReceive);
	if (mesToReceive.typeMes != Text) {
		std::cout << "Server invalid \n";
		return;
	}
	std::cout << "File size : " << mesToReceive.content << std::endl;
}

void ProcessReadFile(int sock, std::string pathFile,size_t offset,size_t buff_size) {
	std::cout << "ProcessReadFile \n";

	std::stringstream ss;
	ss << READFILE_REQUEST << pathFile << "," << offset << "," << buff_size;
	Message mesToSend, mesToReceive;

	mesToSend.typeMes = Text;
	mesToSend.content = ss.str();
	mesToSend.sizeMes = mesToSend.content.size();

	std::cout << "send - " << mesToSend.content << std::endl;
	SendMessage(sock, mesToSend);
	/* Receiving result read file */
	ReceiveMessage(sock, mesToReceive);
	if (mesToReceive.typeMes != Binary) {
		std::cout << "Server invalid \n";
		return;
	}
	std::cout << "Content buffer : " << mesToReceive.content << std::endl;
}

void ProcessWriteFile(int sock, std::string pathFile,size_t offset,std::string buffer) {
	std::cout << "ProcessWriteFile \n";

	std::stringstream ss;
	ss << WRITEFILE_REQUEST << pathFile << "," << offset << "," << buffer;
	Message mesToSend, mesToReceive;

	mesToSend.typeMes = Text;
	mesToSend.content = ss.str();
	mesToSend.sizeMes = mesToSend.content.size();

	std::cout << "send - " << mesToSend.content << std::endl;
	SendMessage(sock, mesToSend);
	/* Receiving result read file */
	ReceiveMessage(sock, mesToReceive);
	if (mesToReceive.typeMes != Binary) {
		std::cout << "Server invalid \n";
		return;
	}
	ResultCode rc = (ResultCode)(*(BYTE*)mesToReceive.content.data());
	if (rc == Success) {
		std::cout << "Success \n";
	} else {
		std::cout << "Fail \n";
	}
}
