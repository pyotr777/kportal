#include "functions.hpp"
#include "stdafx.h"
extern std::string g_admin_email_addr;

extern std::string g_admin_email_pass;

extern std::string DEFAULT_CONFIG_FILE_PATH;

extern std::string DEFAULT_JOB_FOLDER;

extern std::string ICON_FOLDER_PATH;

extern std::string APPMARKET_DOMAIN;

extern std::string READFILE_REQUEST;

extern std::map<std::string, ImageDocker> SampleImages;
/*
 * Load config
 */
void readConfigFile()
{
	using namespace rapidxml;
	xml_document<> doc;
	xml_node<> * root_node;

	std::ifstream theFile (DEFAULT_CONFIG_FILE_PATH.c_str());

	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());

	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	root_node = doc.first_node(DATA_TAG);
	if(root_node == 0)
	{
		return;
	}

	xml_node<> * kpserver_node = root_node->first_node(KPSERVER_TAG);

	if(kpserver_node != 0)
	{
		xml_node<> * s_email_nodes = kpserver_node->first_node(ADMIN_EMAIL_ADDRESS_TAG);
                if(s_email_nodes != 0)
                {
                        g_admin_email_addr = s_email_nodes->value();
                }

                xml_node<> * s_pass_nodes = kpserver_node->first_node(ADMIN_EMAIL_PASSWORD_TAG);
                if(s_pass_nodes != 0)
                {
                        g_admin_email_pass = s_pass_nodes->value();
                }

		
		xml_node<> * s_root_nodes = kpserver_node->first_node(ROOT_DIR_TAG);
		if(s_root_nodes != 0)
		{
			DEFAULT_JOB_FOLDER = s_root_nodes->value();
		}

		xml_node<> * s_amdomain_nodes = kpserver_node->first_node(APPMARKET_DOMAIN_TAG);
		if(s_amdomain_nodes != 0)
		{
			APPMARKET_DOMAIN = s_amdomain_nodes->value();
		}

		xml_node<> * icon_folder_node = kpserver_node->first_node(ICON_FOLDER_TAG);
		if(icon_folder_node != 0)
		{
			ICON_FOLDER_PATH = icon_folder_node->value();
		}

		xml_node<> * sample_images_node = kpserver_node->first_node(SAMPLE_IMAGES_TAG);
		if(sample_images_node != 0)
		{
			for (xml_node<> * image_node = sample_images_node->first_node(IMAGE_TAG); image_node; image_node = image_node->next_sibling())
			{
				std::string id = image_node->first_attribute(ID_ATTR)->value();
				std::string tag = image_node->first_attribute(TAG_ATTR)->value();
                SampleImages[id] = ImageDocker(id, tag, "admin", true, "Sample image public for all provider");
			}
		}
	}
}

/*
 * Check process running
 */
bool checkProcess(std::string nameProcess)
{
	FILE *shellCommand;
	int pid= 0;
	char path[1035], command[20];

	sprintf(command,"pgrep -x %s",nameProcess.c_str());

	shellCommand = popen(command, "r");
	if (shellCommand == NULL) {
		exit(20);
	}
	int count = 0;
	while (fgets(path, sizeof(path)-1, shellCommand) != NULL)
	{
		sscanf(path, "%d" , &pid);
		count ++;
	}
	pclose(shellCommand);
	if (count > 1) {
		return false;
	} else {
		return true;
	}
}

/*
 * Load list job from all master instance
 */
ResponseCode ReloadListJob() {
	ResponseCode ret = DATA_SUCCESS;
	vector<User> listUsers;
	DataManager data_manager(PATH_OF_DATABASE);
	if ( data_manager.connectDB() == DATA_SUCCESS) {
		ret = data_manager.getAllUser(listUsers);
		if (data_manager.disConnectDB() != DATA_SUCCESS) {
			std::cout << "Error close db when get all user\n";
		}
	}

	if (ret == DATA_SUCCESS) {
		for (int i = 0; i < listUsers.size(); i++) {
			std::string containerId = listUsers[i].getContainerId();
			std::string host;
			ret = NetworkUtil::getIpAddressOfContainer(containerId, host);
			if (ret != REQUEST_SUCCESS) {
				break;
			}
			std::map<std::string, bool> listEntry;
			ret = NetworkUtil::ProcessGetListEntry(host, Container_Com_Port, DEFAULT_JOB_FOLDER, listEntry);
			if (ret != FILE_ACTION_SUCCESS) {
				break;
			}
			for (map<string,bool>::iterator it=listEntry.begin(); it != listEntry.end(); it++)
			{
				std::string jobId = (*it).first;
				if ((*it).second) {
					std::string pathFileInfo = DEFAULT_JOB_FOLDER + PATH_SEPARATOR + jobId + PATH_SEPARATOR + jobId + ".info";
					FileNetworkUtils fileNetworkUtil(host, Container_Com_Port, pathFileInfo);
					fileNetworkUtil.Open();
					size_t file_size = -1;
					ret = fileNetworkUtil.GetSize(file_size);
					if (ret != FILE_ACTION_SUCCESS) {
						break;
					}
					size_t read_length;
					std::string infoJob_str = "";
					std::string buffer;
					while (infoJob_str.size() < file_size) {
						ret = fileNetworkUtil.Read(BUFSIZ, read_length, buffer);
						if (read_length > 0) {
							infoJob_str += buffer;
						}
					}
					fileNetworkUtil.Close();
					std::cout << "buffer = " << buffer << std::endl;
					JSONNode jSon_InfoJob;
					try {
						jSon_InfoJob = libjson::parse(buffer);
					} catch(const std::invalid_argument& e) {
						std::cerr << "Invalid argument: " << e.what() << '\n';
						continue;
					}

					Job job(jobId);
					std::cout << "jobId = " << jobId << std::endl;
					JSONNode::const_iterator i = jSon_InfoJob.find("service_id");
					if (i != jSon_InfoJob.end()) {
						job.setServiceId(i->as_string());
					}
					std::cout << "service_id = " << job.getServiceId() << std::endl;

					i = jSon_InfoJob.find("user_email");
					if (i != jSon_InfoJob.end()) {
						job.setUserEmail(i->as_string());
					}
					i = jSon_InfoJob.find("job_name");
					if (i != jSon_InfoJob.end()) {
						job.setJobName(i->as_string());
					}
					i = jSon_InfoJob.find("container_id");
					if (i != jSon_InfoJob.end()) {
						job.setContainerId(i->as_string());
					}
					i = jSon_InfoJob.find("pathJob");
					if (i != jSon_InfoJob.end()) {
						job.setPathJob(i->as_string());
					}
					i = jSon_InfoJob.find("timeStartJob");
					if (i != jSon_InfoJob.end()) {
						job.setTimeStartJob(i->as_string());
					}
					i = jSon_InfoJob.find("timeEndJob");
					if (i != jSon_InfoJob.end()) {
						job.setTimeEndJob(i->as_string());
					}
					i = jSon_InfoJob.find("duration");
					if (i != jSon_InfoJob.end()) {
						job.setDuration(i->as_string());
					}
					i = jSon_InfoJob.find("status");
					if (i != jSon_InfoJob.end()) {
						job.setStatus((KP_JobStatus)i->as_int());
					}
					i = jSon_InfoJob.find("strJson_param");
					if (i != jSon_InfoJob.end()) {
						job.setStrJsonParam(i->as_string());
					}
					i = jSon_InfoJob.find("strJsonOfJob");
					if (i != jSon_InfoJob.end()) {
						job.setStrJsonOfJob(i->as_string());
					}

					i = jSon_InfoJob.find("listParam");
					if (i != jSon_InfoJob.end() && i->type() == JSON_ARRAY) {
						for (JSONNode::const_iterator j = i->begin(); j != i->end(); j++) {
							param_job param;
							JSONNode jSon_InfoListParam = j->as_node();
							JSONNode::const_iterator k = jSon_InfoListParam.find("param_id");
							if (k != jSon_InfoListParam.end()) {
								param.setParamId(i->as_string());
							}
							k = jSon_InfoListParam.find("type");
							if (k != jSon_InfoListParam.end()) {
								param.setType((TYPE_PARAM_JOB)i->as_int());
							}
							k = jSon_InfoListParam.find("value");
							if (k != jSon_InfoListParam.end()) {
								param.setValue(i->as_string());
							}
							k = jSon_InfoListParam.find("refer_job_id");
							if (k != jSon_InfoListParam.end()) {
								param.setReferJobId(i->as_string());
							}
							job.addParam(param);
						}
					}

					extern std::map<std::string,Job> listJob;
					listJob[jobId] = job;
				}
			}

		}
	}

	return ret;
}

ResponseCode StartMasterInstance() {
	ResponseCode ret = DATA_SUCCESS;
	vector<User> listUsers;
	DataManager data_manager(PATH_OF_DATABASE);
	if ( data_manager.connectDB() == DATA_SUCCESS) {
		ret = data_manager.getAllUser(listUsers);
		if (data_manager.disConnectDB() != DATA_SUCCESS) {
			std::cout << "Error close db when get all user\n";
		}
	}

	if (ret == DATA_SUCCESS) {
		for (int i = 0; i < listUsers.size(); i++) {
			std::string containerId = listUsers[i].getContainerId();
			//start master-instance of user
			std::stringstream ss;
			ss.str("");
			ss << "POST /containers/" << containerId << "/start HTTP/1.1\r\n";

			ss << "Host: " << DockerTcp_IP << "\r\n\r\n";

			std::string request = ss.str();

			std::cout << request << std::endl;
			std::string result, header_result;
			NetworkUtil::requestHttp(DockerTcp_IP, DockerTcp_Port, request, result, header_result);
			std::size_t found = header_result.find(DOCKER_RESPONE_HTTP_204);
			if (found == std::string::npos) {
				found = header_result.find(DOCKER_RESPONE_HTTP_304);
				if (found == std::string::npos) {
					std::cout << "Request docker start error. \n";
					std::cout << "Content error: \n" << result << std::endl;
					return REQUEST_DOCKER_ERROR;
				} else {
					return DATA_SUCCESS;
				}
			}
		}
	}
	return ret;
}
