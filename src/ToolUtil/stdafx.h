#pragma once
#include <iostream>
#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<pthread.h> //for threading , link with lpthread
#include <string>
#include <errno.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <vector>
#include <sstream>

typedef unsigned char BYTE;

const std::string UPLOAD_FILE_REQUEST = "UploadFile:";
const std::string DOWNLOAD_FILE_REQUEST = "DownloadFile:";
const std::string GETLISTENTRY_REQUEST = "GetListEntry:";
const std::string DELETEDIRECTORY_REQUEST = "DeleteDirectory:";
const std::string GETFILESIZE_REQUEST = "GetFileSize:";
const std::string READFILE_REQUEST = "ReadFile:";
const std::string WRITEFILE_REQUEST = "WriteFile:";

enum TypeMessage {
    Text = 1,
    Binary,
    None,
};

enum ResultCode {
    Success = 1,
    Fail,
    UnknowFormat,
    ReadyReceiveFile,
};

typedef struct {
    TypeMessage typeMes;
    unsigned long sizeMes;
    std::string content;
} Message;

void SendMessage(int sock, Message mes) {
    BYTE type = mes.typeMes;
    std::string msg((char*)&type,sizeof(BYTE));
    msg += std::string((char*)&mes.sizeMes, sizeof(unsigned long));
    msg += mes.content;
    write(sock , msg.data() , msg.size());
}

void ReceiveMessage(int sock, Message &mes) {
    int sizeHeader = sizeof(BYTE) + sizeof(unsigned long);
    char buffer[BUFSIZ];
    std::string msg = "";

    int read_size = 0;
    while (msg.size() <= sizeHeader && ((read_size = recv(sock, buffer, BUFSIZ, 0)) > 0)) {
        msg += std::string(buffer,read_size);
    }
    if (msg.size() <= sizeHeader) {
        std::cout << "message received error\n";
        return;
    }

    mes.typeMes = (TypeMessage)(*(BYTE*)buffer);
    mes.sizeMes = *(unsigned long*)(buffer+1);
    msg = msg.substr(sizeof(unsigned long) + sizeof(BYTE));

    while (((mes.sizeMes - msg.size()) > 0) && ((read_size = recv(sock, buffer, BUFSIZ, 0)) > 0)) {
        msg += std::string(buffer, read_size);
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

int Split(const std::string &work, std::vector<std::string> &array_str, std::string delimiter)
{
	std::string str = work;
	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		token = str.substr(0, pos);
		if (token.length() > 0)
			array_str.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	if (str.length() > 0) {
		array_str.push_back(str);
	}
	return array_str.size();
}
