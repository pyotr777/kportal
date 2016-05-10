#pragma once
#ifndef STDAFX_H
#define STDAFX_H
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
#include <fstream>
#include <fcntl.h>
#include <dirent.h>
#include <vector>
#include <map>
#include <algorithm>
#include <netdb.h>
#include <sys/time.h>
#include <time.h>

#include "functions.hpp"
#include "ResponeInterface.hpp"
#include "authenticate/NetworkUtil.hpp"
#include "datalayer/service.hpp"
#include "datalayer/parameter.h"
#include "datalayer/image_docker.hpp"
#include "datalayer/DataManager.hpp"
#include "../include/libtar-1.2.11/lib/libtar.h"
#include "../include/libjson/libjson.h"
#include "authenticate/TarUtil.hpp"
#include "authenticate/FileUtils.hpp"
#include "authenticate/StringUtils.hpp"
#include "authenticate/NetworkUtil.hpp"
#include "authenticate/TagKeys.hpp"
#include "job_manager/job.hpp"
#include "authenticate/FileNetworkUtils.hpp"
#include "../include/rapidxml/rapidxml_iterators.hpp"
#include "../include/rapidxml/rapidxml_utils.hpp"
#include "../include/rapidxml/rapidxml_print.hpp"
#include "../include/rapidxml/rapidxml.hpp"


typedef unsigned char BYTE;

#define DockerTcp_IP "127.0.0.1"
#define DockerTcp_Port "9555"
#define Container_Com_Port "9009"

#define DOCKER_RESPONE_HTTP_200 "HTTP/1.1 200 OK"
#define DOCKER_RESPONE_HTTP_201 "HTTP/1.1 201 Created"
#define DOCKER_RESPONE_HTTP_204 "HTTP/1.1 204 No Content"
#define DOCKER_RESPONE_HTTP_304 "HTTP/1.1 304 Not Modified"

#define BASE_IMAGE "base_image"
#define TAG_IMAGE_DOCKER_STR "latest"

#define KPSERVER_TAG            "KPServer"
#define DATA_TAG                "Data"
#define ICON_FOLDER_TAG         "IconFolder"
#define ROOT_DIR_TAG            "RootDir"
#define APPMARKET_DOMAIN_TAG	"AppMarketDomain"
#define SAMPLE_IMAGES_TAG		"SampleImages"
#define IMAGE_TAG				"Image"
#define ID_ATTR					"id"
#define TAG_ATTR				"tag"




#endif //STDAFX_H
