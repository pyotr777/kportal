#include "job.hpp"
#include "../stdafx.h"
#include <math.h>       /* floor */
extern std::map<std::string,Job> listJob;
extern std::string g_docker_bridge_host;
extern unsigned int g_job_handler_port;
Job::Job() {
  numberOfNode = 0;
  maxElapseTime = 0;
  tunnel_port = 0;
  timerStarted = false;
}

Job::Job(std::string id) {
  job_id = id;
  numberOfNode = 0;
  maxElapseTime = 0;
  tunnel_port = 0;
  timerStarted = false;
}

void Job::setId(std::string id) {
  job_id = id;
}

std::string Job::getId() {
  return job_id;
}

void Job::setServiceId(std::string id) {
  service_id = id;
}

std::string Job::getServiceId() {
  return service_id;
}

void Job::setUserEmail(std::string email) {
  user_email = email;
}

std::string Job::getUserEmail() {
  return user_email;
}

void Job::setProviderEmail(std::string email) {
  provider_email = email;
}

std::string Job::getProviderEmail() {
  return provider_email;
}

void Job::setJobName(std::string name) {
  job_name = name;
}

std::string Job::getJobName() {
  return job_name;
}

void Job::setNumberOfNode(int n) {
  numberOfNode = n;
}

int Job::getNumberOfNode() {
  return numberOfNode;
}

void Job::setMaxElapseTime(unsigned int t) {
  maxElapseTime = t;
}

unsigned int Job::getMaxElapseTime() {
  return maxElapseTime;
}
void Job::setPathJob(std::string path) {
  pathJob = path;
}

std::string Job::getPathJob() {
  return pathJob;
}

void Job::setTimeStartJob(std::string time) {
  timeStartJob = time;
}

std::string Job::getTimeStartJob() {
  return timeStartJob;
}

void Job::setTimeEndJob(std::string time) {
  timeEndJob = time;
}

std::string Job::getTimeEndJob() {
  return timeEndJob;
}

void Job::setDuration(std::string time) {
  duration = time;
}

std::string Job::getDuration() {
  if(duration.size() == 0){
    duration = "0000:00:00";
  }
  return duration;
}

void Job::setStatus(KP_JobStatus st) {
  status = st;
}

KP_JobStatus Job::getStatus() {
  return status;
}

void Job::setStrJsonParam(std::string str) {
  strJson_param = str;
}

std::string Job::getStrJsonParam() {
  return strJson_param;
}

void Job::setStrJsonOfJob(std::string str) {
  strJsonOfJob = str;
}

std::string Job::getStrJsonOfJob() {
  return strJsonOfJob;
}

void Job::addParam(param_job &pa) {
  listParam.push_back(pa);
}

std::vector<param_job>* Job::getParamJsonStr() {
  return &listParam;
}

void Job::setContainerId(std::string id) {
  container_id = id;
}

std::string Job::getContainerId() {
  return container_id;
}

MessageHeader Job::getHeader() {
  return _header;
}

void Job::setHeader(MessageHeader& header) {
  _header = header;
}

void Job::setTunnelPort(int port){
  tunnel_port = port;
}

int Job::getTunnelPort(){
  return tunnel_port;
}

/*
ResponseCode Job::
hFile() {
  std::cout << "generateShFile \n";
  ResponseCode ret = DATA_ERROR;
  Service sv;
  sv.setServiceID(service_id);
  DataManager data_manager(PATH_OF_DATABASE);
  ret = data_manager.connectDB();
  if ( ret == DATA_SUCCESS) {
    ret = data_manager.getInfoOfService(sv);
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when get info service\n";
    }
  }
  if (ret != DATA_SUCCESS) {
    return ret;
  }

  std::string host, sourceDir;
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    User user(user_email, USER_GROUP);
    ret = data_manager.getUser(user);
    if (ret != DATA_SUCCESS) {
      return ret;
    }
    std::string containerId = user.getContainerId();
    ret = NetworkUtil::getIpAddressOfContainer(containerId, host);
    if (ret != REQUEST_SUCCESS) {
      return ret;
    }
    data_manager.disConnectDB();
  }

  sourceDir = pathJob;

  //Get info list param
  std::vector<Parameter> listParamOfService;
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    Service sv;
    sv.setServiceID(service_id);
    ret = data_manager.getParamOfService(sv, listParamOfService);
    if (ret != DATA_SUCCESS) {
      return ret;
    }
    data_manager.disConnectDB();
  }

  std::stringstream ss;
  ss << sv.getPathExcuteFile();
  for (int j = 0; j < listParamOfService.size(); j++) {
    Parameter* param = &listParamOfService[j];
    for (unsigned int i = 0; i < listParam.size(); i++) {
      param_job* pa_job = &listParam[i];
      if (pa_job->getParamId().compare(param->getParamID()) == 0) {
        if (param->getOptionName().size() != 0) {
          ss << " " << param->getOptionName();
        }
        if (param->getType().compare("file") == 0) {
          if (pa_job->getType() == ARG_LOCAL) {
            ss << " \"" << sourceDir << PATH_SEPARATOR << pa_job->getValue() << "\"";
          } else if (pa_job->getType() == ARG_REMOTE) {
            ss << " \"" << sourceDir << PATH_SEPARATOR << pa_job->getReferJobId() << "_" << pa_job->getValue() << "\"";
          } else {
            ss << " \"" << sourceDir << PATH_SEPARATOR << pa_job->getValue() << "\"";
          }
        } else if (pa_job->getType() == VALUE_ONLY) {
          ss << " " << pa_job->getValue();
        }
      }
    }
  }
  ss << " 1> " << sourceDir << PATH_SEPARATOR << "stdout.txt 2> " << sourceDir << PATH_SEPARATOR << "stderr.txt";
  ss << "\n";
  std::string content = ss.str();
  std::cout << "content of start script : " << content << std::endl;
  FILE * pFile;
  std::cout << (pathJob + PATH_SEPARATOR + job_id + ".sh") << std::endl;
  pFile = fopen ((pathJob + PATH_SEPARATOR + job_id + ".sh").c_str(), "wb");
  if (pFile) {
    fwrite (content.c_str() , sizeof(char), content.length(), pFile);
    fclose (pFile);
    ret = DATA_SUCCESS;
  }
  if (ret != DATA_SUCCESS) {
    return ret;
  }

  //generate script to stop job
  ss.str("");
  ss << "kill -9 $(pidof " << sv.getPathExcuteFile() << ")";
  content = ss.str();
  std::cout << "content of stop script : " << content << std::endl;
  pFile = fopen ((pathJob + PATH_SEPARATOR + "stop_" + job_id + ".sh").c_str(), "wb");
  if (pFile) {
    fwrite (content.c_str() , sizeof(char), content.length(), pFile);
    fclose (pFile);
    ret = DATA_SUCCESS;
  }

  return ret;
}
*/


ResponseCode Job::generateShFile(Service& sv) {
  std::cout << "generateShFile \n";
  ResponseCode ret = DATA_ERROR;

  std::stringstream elapsedTime_ss;
  elapsedTime_ss << maxElapseTime/3600 << ":" << (maxElapseTime%3600)/60 << ":" << (maxElapseTime%3600)%60;

  std::stringstream ss, argss, stagging_ss;
  std::cout << "params size = " << listParam.size() << std::endl;
  //for (int i = (int)listParam.size() - 1; i >= 0; i--) {

  //Get info list param
  DataManager data_manager(PATH_OF_DATABASE);
  std::vector<Parameter> listParamOfService;
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    Service sv;
    sv.setServiceID(service_id);
    ret = data_manager.getParamOfService(sv, listParamOfService);
    if (ret != DATA_SUCCESS) {
      return ret;
    }
    data_manager.disConnectDB();
  }

  // Matching parameter vs value
  for (int j = 0; j < listParamOfService.size(); j++) {
    Parameter* param = &listParamOfService[j];

    for (int i = 0; i < (int)listParam.size(); i++) {
      param_job* pa_job = &listParam[i];
      if(pa_job->getParamId() != param->getParamID()){
        continue;
      }
      if (pa_job->getType() == VALUE_ONLY) {
        argss << " " << pa_job->getValue();
      } else if (pa_job->getType() == ARG_LOCAL) {
        argss << " \"" << pa_job->getValue() << "\"";
        stagging_ss << "#PJM --stgin \"./" << pa_job->getValue() << " ./\"\n";
      }else if( pa_job->getType() == ARG_REMOTE) {
        argss << " \"" << pa_job -> getReferJobId() << "_" << pa_job->getValue() << "\"";
        stagging_ss << "#PJM --stgin \"./" <<  pa_job -> getReferJobId() << "_" << pa_job->getValue() << " ./\"\n";
      } else if (pa_job->getType() == ARG_OUTPUT) {
        argss << " \"" << pa_job->getValue() << "\"";
        stagging_ss << "#PJM --stgout \"./" << pa_job->getValue() << " ./\"\n";
      } else {
        std::cout << "unknown parameter " << pa_job->getParamId() << std::endl;
      }
    }
  }

  // stage-in dirs
  for(unsigned int i = 0; i < sv.getStageinDirs()->size(); i++){
    std::string stgindirs = sv.getStageinDirs()->at(i), dirname;
    std::vector<std::string>names;
    StringUtils::Split(stgindirs, names, '/');
    for(int j = (int)names.size() - 1; j >= 0; j--){
      if(names[j].size()> 0){
        dirname = names[j];
        break;
      }
    }
    stagging_ss << "#PJM --stgin-dir \"./" << dirname << " ./\"\n";
  }

  // stage-out dirs
  for(unsigned int i = 0; i < sv.getStageoutDirs()->size(); i++){
    std::string stgoutdir = sv.getStageoutDirs()->at(i), dirname;
    std::vector<std::string>names;
    std::cout << "split " << stgoutdir;
    StringUtils::Split(stgoutdir, names, "/");
    std::cout << " size = " << names.size() << std::endl;
    for(int j = (int)names.size() - 1; j >= 0; j--){
      if(names[j].size()> 0){
        dirname = names[j];
        break;
      }
    }
    stagging_ss << "#PJM --stgout-dir \"./" << dirname << " ./\"\n";
  }

  std::string stg = stagging_ss.str();
  std::string filename = FileUtils::GetFileName(sv.getPathExcuteFile());
  std::string args = argss.str();
  ss << "#!/bin/sh -x\n"
     << "#PJM --rsc-list  \"node=" << numberOfNode <<"\"\n"
     << "#PJM --rsc-list  \"elapse=" << elapsedTime_ss.str() << "\"\n"
     << "#PJM --stg-transfiles all\n"
     //<< "#PJM --stgin \"/home/ra000007/a03320/services/" << sv.getPathExcuteFile()  << " ./\"\n"
     << "#PJM --stgin \"./" << filename << " ./\"\n"
     << stg
     //<< "#PJM --stgin \"./ ./\"\n"
     << "#PJM -s\n"
     << "#\n"
     << ". /work/system/Env_base\n"
     << "#\n"
     << "./" << FileUtils::GetFileName(sv.getPathExcuteFile()) << " " << args << "\n";

  std::string content = ss.str();
  std::cout << "sh content: \n";
  std::cout << content << std::endl;
  FILE * pFile;
  pFile = fopen ((pathJob + PATH_SEPARATOR + job_id + ".sh").c_str(), "wb");
  if (pFile) {
    fwrite (content.c_str() , sizeof(char), content.length(), pFile);
    fclose (pFile);
    ret = DATA_SUCCESS;
  }
  return ret;
}

ResponseCode Job::GenerateSlaveDaemonShFile() {
  ResponseCode ret = DATA_ERROR;
  // Copy daemon file
  std::cout << "Copy daemon file to job directory\n";
  std::string daemon_file_name = "slavedaemon";
  //std::string daemon_file_path = "../slavedaemon/";
  std::string daemon_dest_file = pathJob + PATH_SEPARATOR + daemon_file_name;
  //FileUtils::copy_file(daemon_file_path + daemon_file_name , daemon_dest_file);


  // Gen ssh: <programe file> <port> <stdout> <stderr>
  std::cout << "Generate start SlaveDaemon sh file\n";

  std::stringstream ss;
  ss << daemon_dest_file << " " << container_port;
  ss << " 1> " << pathJob << PATH_SEPARATOR << "slavedaemon_stdout.txt 2> " << pathJob << PATH_SEPARATOR << "slavedaemon_stderr.txt";
  ss << "\n";
  std::string content = ss.str();
  std::cout << "content of start script : " << content << std::endl;
  FILE * pFile;
  std::cout << (pathJob + PATH_SEPARATOR + daemon_file_name + ".sh") << std::endl;
  pFile = fopen ((pathJob + PATH_SEPARATOR + daemon_file_name + ".sh").c_str(), "wb");
  if (pFile) {
    fwrite (content.c_str() , sizeof(char), content.length(), pFile);
    fclose (pFile);
    ret = DATA_SUCCESS;
  }
  if (ret != DATA_SUCCESS) {
    return ret;
  }
  return ret;
}

extern std::string DEFAULT_JOB_FOLDER;

ResponseCode Job::getRemoteArg() {
  std::cout << "getRemoteArg \n";
  ResponseCode ret = FILE_ACTION_SUCCESS;
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    for (unsigned int i = 0; i < listParam.size(); i++) {
      param_job* pa_job = &listParam[i];
      if (pa_job->getType() == ARG_REMOTE) {
        std::stringstream ss;
        ss << DEFAULT_JOB_FOLDER << PATH_SEPARATOR << pa_job->getReferJobId() << PATH_SEPARATOR << pa_job->getValue();
        std::string srcFile = ss.str();
        ss.str("");
        ss << DEFAULT_JOB_FOLDER << PATH_SEPARATOR << job_id << PATH_SEPARATOR << pa_job->getReferJobId() << "_" << pa_job->getValue();
        std::string desFile = ss.str();
        std::string userEmail = listJob[pa_job->getReferJobId()].getUserEmail();
        std::cout << "get job " << pa_job->getReferJobId() << " owner " << userEmail << std::endl;
        User user(userEmail, USER_GROUP);
        ret = data_manager.getUser(user);
        if (ret != DATA_SUCCESS) {
          break;
        }
        std::string containerId = user.getContainerId();
        std::string host;
        ret = NetworkUtil::getIpAddressOfContainer(containerId, host);
        if (ret != REQUEST_SUCCESS) {
          break;
        }
        ret = NetworkUtil::ProcessDownloadFile(host, Container_Com_Port, srcFile, desFile);
        if (ret != FILE_ACTION_SUCCESS) {
          break;
        }
      }
    }
    data_manager.disConnectDB();
  }
  return ret;
}

ResponseCode Job::Init() {
  std::cout << "Init job \n";
  ResponseCode ret = DATA_ERROR;
  job_id = std::string("J") + StringUtils::genIdString();
  extern std::string DEFAULT_JOB_FOLDER;
  //std::string pathJob;
  if (DEFAULT_JOB_FOLDER[DEFAULT_JOB_FOLDER.length() - 1] == '/') {
    pathJob = DEFAULT_JOB_FOLDER + job_id;
  } else {
    pathJob = DEFAULT_JOB_FOLDER + PATH_SEPARATOR + job_id;
  }
  FileUtils::CreateDirectory(pathJob);
  if (FileUtils::IsDirectory(pathJob)) {
    //get input
    ret = getRemoteArg();
    if (ret != FILE_ACTION_SUCCESS) {
      return ret;
    }

    // get service
    Service sv;
    sv.setServiceID(service_id);
    DataManager data_manager(PATH_OF_DATABASE);
    ret = data_manager.connectDB();
    if ( ret == DATA_SUCCESS) {
      ret = data_manager.getInfoOfService(sv);
      if (data_manager.disConnectDB() != DATA_SUCCESS) {
        std::cout << "Error close db when get info service\n";
      }
    } else {
      return ret;
    }

    // Save provider id
    provider_email = sv.getProvider();

    //gen sh file
    ret = generateShFile(sv);

    // Set slave_daemon port listen
    container_port = "9008";

    //gen sh for slave_daemon
    //ret = GenerateSlaveDaemonShFile();

  } else {
    ret = ERROR_CREATE_JOB_FOLDER;
  }

  if (ret == DATA_SUCCESS) {
    status = JOB_START;
  }
  return ret;
}

ResponseCode Job::CreateJobContainer() {
  ResponseCode ret = DATA_ERROR;
  return ret;
}

ResponseCode Job::Submit() {
  std::cout << "Job::Submit job \n";
  if (status != JOB_START) {
    return ERROR_JOB_DONT_INIT;
  }

  //
  // Create job's container
  //
  std::cout << "create job's container \n";
  //std::string pathShFile;// = pathJob + PATH_SEPARATOR + job_id + ".sh";
  std::string imageId, host, containerIdOfMasterInstance, job_containner_ip;
  ResponseCode ret = DATA_ERROR;
  Service sv;
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    //std::string userEmail = listJob[pa_job->getReferJobId()].getUserEmail();
    User user(user_email, USER_GROUP);
    ret = data_manager.getUser(user);
    if (ret != DATA_SUCCESS) {
      return ret;
    }
    containerIdOfMasterInstance = user.getContainerId();
    ret = NetworkUtil::getIpAddressOfContainer(containerIdOfMasterInstance, host);
    if (ret != REQUEST_SUCCESS) {
      return ret;
    }

    ret = NetworkUtil::ProcessUploadDir(host, Container_Com_Port, pathJob, pathJob);
    if (ret != FILE_ACTION_SUCCESS) {
      return ret;
    }

    sv.setServiceID(service_id);
    ret = data_manager.getInfoOfService(sv);
    if (ret == DATA_SUCCESS) {
      imageId = sv.getImageId();
    }
    data_manager.disConnectDB();
  }

  if (ret != DATA_SUCCESS) {
    return ret;
  }

  //pathShFile = pathJob + PATH_SEPARATOR + job_id + ".sh";
  //std::string daemon_file_name = "slavedaemon";
  //pathShFile = pathJob + PATH_SEPARATOR + daemon_file_name + ".sh";
  std::stringstream stginss;
  std::string stgindirs;
  for(unsigned int i = 0; i < sv.getStageinDirs()->size(); i++){
    stginss << ",\"" << sv.getStageinDirs()->at(i) <<"\"";
  }
  stgindirs = stginss.str();
//  for(unsigned int i = 0; i < sv.getStageoutDirs()->size(); i++){
//    stgoutss << sv.getStageoutDirs()->at(i) << " ";
//  }
//  stgoutstr = stgoutss.str();

  std::stringstream ss;
  //ss  << "{\"Cmd\":[\"sh\",\"/startslavedaemon.sh\"]"
  std::string home_job_str = DEFAULT_JOB_FOLDER[DEFAULT_JOB_FOLDER.size() - 1] == '/' ? DEFAULT_JOB_FOLDER : DEFAULT_JOB_FOLDER + "/" ;
  ss  << "{\"Cmd\":[\"/bin/sh\",\"/bin/startslavedaemon.sh\",\"" << home_job_str << "\",\"" << job_id << "\",\"" << g_docker_bridge_host << "\",\"" << tunnel_port <<"\",\"" << g_job_handler_port <<"\",\"" << sv.getPathExcuteFile() <<"\"" << stgindirs << "]"
      << ",\"Image\":\"" << imageId << "\""
      << ",\"HostConfig\": {\"Binds\":[\"/etc/localtime:/etc/localtime\"],\"VolumesFrom\":[\"" << containerIdOfMasterInstance << "\"]}"
      << "}\r\n";
  std::string content = ss.str();
  ss.str("");
  ss << "POST /containers/create HTTP/1.1\r\n";
  ss << "Content-Type:application/json\r\n";
  ss << "Content-Length: " << content.size() << "\r\n";
  ss << "Host: " << DockerTcp_IP << "\r\n\r\n";

  ss << content;

  std::string request = ss.str();
  std::cout << request << std::endl;
  std::string result, header_result;

  std::size_t found;
  ///if(false) /// just test
  //{
  NetworkUtil::requestHttp(DockerTcp_IP, DockerTcp_Port, request, result, header_result);
  found = header_result.find(DOCKER_RESPONE_HTTP_201);
  if (found == std::string::npos) {
    std::cout << "Request docker create error. \n";
    std::cout << "Content error: \n" << result << std::endl;
    return REQUEST_DOCKER_ERROR;
  }

  JSONNode n;
  try {
    n = libjson::parse(result);
  } catch(const std::invalid_argument& e) {
    std::cerr << "Lib json error. Please check : " << e.what() << '\n';
    return DOCKER_CREATE_CON_ERROR;
  }

  JSONNode::const_iterator i = n.begin();
  if (i == n.end()) {
    std::cout << "Respone error.\n";
    return DOCKER_CREATE_CON_ERROR;
  }

  std::string containerId = i->as_string();
  std::cout << "container id = " << containerId << std::endl;

  //
  // Start job's container
  //
  ss.str("");
  ss << "POST /containers/" << containerId << "/start HTTP/1.1\r\n";
  ss << "Host: " << DockerTcp_IP << "\r\n\r\n";
  request = ss.str();

  std::cout << request << std::endl;

  result = "";
  header_result = "";
  NetworkUtil::requestHttp(DockerTcp_IP, DockerTcp_Port, request, result, header_result);
  found = header_result.find(DOCKER_RESPONE_HTTP_204);
  if (found == std::string::npos) {
    std::cout << "Request docker start error. \n";
    std::cout << "Content error: \n" << result << std::endl;
    return REQUEST_DOCKER_ERROR;
  }
  // Remove temp job folder
  if (FileUtils::delete_folder_tree(pathJob.c_str())) {
    std::cout << "delete " << pathJob << " fail. \n";
  }

  //
  // Send command create job container vs forwarder (slavedaemon vs kpforwarder) connection
  //
  ret = NetworkUtil::getIpAddressOfContainer(containerId, job_containner_ip);
  if (ret != REQUEST_SUCCESS) {
    std::cout << "get job container ip is failed.\n";
    return ret;
  }
  //}

  // Save container id & Save status
  status = JOB_START;
  container_id = containerId;
  
  // Save job info to master container
  std::cout << "job container ip:" << job_containner_ip.c_str() << std::endl;
  std::string json_str;
  generateJsonStr(json_str);
  FileNetworkUtils fileNetworkUtil(host, Container_Com_Port, pathJob + PATH_SEPARATOR + job_id + ".info");
  fileNetworkUtil.Open();
  ret = FILE_ACTION_ERROR;

  int tryi = 0;
  while (ret != FILE_ACTION_SUCCESS && tryi++ <= 3) {
    ret = fileNetworkUtil.Write(json_str);
    std::cout << "try write file info job \n";
    sleep(5);
  }
  fileNetworkUtil.Close();

//  // Save status
//  status = JOB_START;
//  container_id = containerId;
  //
  // Connect to job container (slavedaemon) & send submit job command
  //

  //ret = SendCreateForwarderConnectionCommand(tunnel_port, job_containner_ip, container_port);
  //if (ret != REQUEST_SUCCESS) {
  //  return ret;
  //}


//  std::cout<< "sleep some seconds to wait job container start\n";
//  sleep(10);
//  ret = SendSubmitJobCommand(job_containner_ip, container_port, *sv.getStageinDirs());
//  if (ret != REQUEST_SUCCESS) {
//    std::cout << "send submit job request to job container is failed.\n";
//    status = JOB_ERROR;
//    return ret;
//  }

//  // Send request get status job to test
//  ret = UpdateStatusJobKcom();
//  if (ret != REQUEST_SUCCESS) {
//    std::cout << "send get status request to kdesk daemon is failed.\n";
//    status = JOB_ERROR;
//    return ret;
//  }
//  // Save start date & start duration timer & update job status
//  container_id = containerId;
//  status = JOB_RUNNING;
//  timer.Start();
//  SaveStartDate();

  return DATA_SUCCESS;
}
/*
ResponseCode Job::UpdateStatusJobKcom(){
  std::cout << "Job::UpdateStatusJobKcom() tunnel_port " << tunnel_port << std::endl;
  std::string _host = "127.0.0.1";
  int _port = tunnel_port;
  ResponseCode res_code = DATA_SUCCESS;
  try {
    Socket client_socket;
    // ClientSocket client_socket ( _host, _port );
    if ( ! client_socket.Create() ) {
      throw SocketException ( "Could not create client socket." );
    }
    if ( ! client_socket.Connect( _host, _port)) {
      throw SocketException ( "Could not connect to host." );
    }

    // Send query status
    Message req(Header(MT_COMMAND, 0, _port, 0, CMD_GET_JOB_STATE), NULL), res;
    req.AddArgv(kdesk_jid);
    if(client_socket.Send(req)){
      if(client_socket.Recv(res)){
        std::cout << "command " << res.GetHeader()->GetCommand() << std::endl;
        if(res.GetHeader()->GetCommand() == CMD_JOB_STATE){
          res_code = REQUEST_SUCCESS;
          std::vector<std::string> argv = res.GetCmdArgs();
          if(argv.size() > 4){
            status = ConvertKdeskJobState(kdesk_jstate);
          } else {
            std::cout << "CMD_JOB_STATE args is missing.\n";
            res_code = ERROR_GET_JOB_STATUS;
          }
        } else {
          std::cout << "slave daemon submit job has failed \n";
          res_code = ERROR_GET_JOB_STATUS;
        }
      } else {
        res_code = ERROR_GET_JOB_STATUS;
        std::cout << "recv response getJobState fail\n";
      }
    } else {
      res_code = ERROR_GET_JOB_STATUS;
      std::cout << "send request getJobState fail\n";
    }

  } catch ( SocketException& e ) {
    std::cout << "Exception was caught:" << e.description() << "\n";
    res_code = ERROR_GET_JOB_STATUS;
  }

  return res_code;
}
*/
void Job::SaveEndDate(long duration){
  struct tm * timeinfo;
  char buffer[80] = {'\0'};
  time_t end_time = startTime + duration;
  timeinfo = localtime(&end_time);
  strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeinfo);
  timeEndJob = buffer;
}

void Job::SaveStartDate(){
  struct tm * timeinfo;
  char buffer[80];

  time (&startTime);
  timeinfo = localtime(&startTime);

  strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
  std::string str(buffer);

  std::cout <<"Start date:" << str << std::endl;
  timeStartJob = str;
}

//
//
//
ResponseCode Job::SendCreateForwarderConnectionCommand(int tunnel_port, std::string container_host, std::string container_port){
  std::cout << "Job::SendCreateForwarderConnectionCommand\n";
  ResponseCode res_code = ERROR_CREATE_FORWARCER_CONNECTION;
  try {
    Socket client_socket;
    // ClientSocket client_socket ( _host, _port );
    if ( ! client_socket.Create() ) {
      throw SocketException ( "Could not create client socket." );
    }
    std::string forwarder_host = "127.0.0.1";
    int forwarder_port = 8999;
    if ( ! client_socket.Connect( forwarder_host, forwarder_port)) {
      throw SocketException ( "Could not bind to port." );
    }

    // Check ssh tunnel
    Message req(Header(MT_COMMAND, 0, 0, 0, CMD_SLAVEDAEMON_CONNECT), NULL), res;
    req.AddArgv(tunnel_port);
    req.AddArgv(container_host);
    req.AddArgv(container_port);
    try {
      // send request
      client_socket.Send(req);

      // get response
      client_socket.Recv(res);

      if(res.GetHeader()->GetCommand() == CMD_ACK_OK){
        // Get port
        res_code = DATA_SUCCESS;
      }
    } catch ( SocketException& e) {}
  } catch ( SocketException& e ) {
    std::cout << "Exception was caught:" << e.description() << "\n";
  }
  return res_code;
}

//
// Connect to slave_daemon server inside job container and send submit job request
//
ResponseCode Job::SendSubmitJobCommand(const std::string& _host, const std::string& _port, std::vector<std::string>& stgins){
  std::cout << "Job::SendSubmitJobCommand(std::string _host, std::string _port) " << _host.c_str() << ":" << _port.c_str() << std::endl;
  ResponseCode res_code = REQUEST_SUCCESS;
  try {
    Socket client_socket;
    // ClientSocket client_socket ( _host, _port );
    if ( ! client_socket.Create() ) {
      throw SocketException ( "Could not create client socket." );
    }
    if ( ! client_socket.Connect( _host, atoi(_port.c_str()))) {
      throw SocketException ( "Could not connect to host." );
    }

    //
    // Send stagin request
    //
    std::cout << "Stage-in request to slave daemon\n";
    Message stgreq(Header(MT_COMMAND, 0, 0, 0, CMD_COPY_STAGIN_DIR), NULL), stgres;
    stgreq.AddArgv(job_id);
    for(unsigned int i = 0; i < stgins.size() ; i++){
      stgreq.AddArgv(stgins[i]);
    }

    // Send request
    if(client_socket.Send(stgreq)){
      // Get response
      if(client_socket.Recv(stgres) > 0){
        if(stgres.GetHeader()->GetCommand() == CMD_ACK_OK){
          std::cout << "stage-in dir successfull\n";
        } else {
          std::cout << "stage-in dir fail\n";
          res_code = ERROR_SUBMIT_JOB;
        }

      } else {
        std::cout << "can not recv response from slave daemon \n";
        res_code = ERROR_SUBMIT_JOB;
      }
    } else {
      std::cout << "can not send stagin command to slave daemon \n";
      res_code = ERROR_SUBMIT_JOB;
    }

    if(res_code != REQUEST_SUCCESS){
      return res_code;
    }

    // Get provider of service
//    std::cout << "Get service owner: " << provider_email << std::endl;

//    DataManager data_manager(PATH_OF_DATABASE);
//    User service_owner(provider_email, USER_GROUP);
//    if ( data_manager.connectDB() == DATA_SUCCESS) {
//      res_code = data_manager.getUser(service_owner);
//      if (res_code != DATA_SUCCESS) {
//        std::cout << "get provider from db fail\n";
//        return res_code;
//      }
//      std::cout << "tunnel port: "  << service_owner.getKdeskPort() << std::endl;
//      tunnel_port = service_owner.getKdeskPort();
//      data_manager.disConnectDB();
//    }

//    ret = data_manager.connectDB();
//    if ( ret == DATA_SUCCESS) {
//      ret = data_manager.getInfoOfService(sv);
//      if (data_manager.disConnectDB() != DATA_SUCCESS) {
//        std::cout << "Error close db when get info service\n";
//      }
//    }

    //
    // Send submit job command
    //
    std::cout << "Send submit job command \n";
    Message req(Header(MT_COMMAND, 0, 0, 0, CMD_SUBMITJOB), NULL), res;
    req.AddArgv(g_docker_bridge_host);
    std::stringstream ss;
    ss << tunnel_port;
    std::string tunnel_port_str = ss.str();
    req.AddArgv(tunnel_port_str);
    req.AddArgv(job_id);
    // Send request
    if(client_socket.Send(req)){
      // Get response
      if(client_socket.Recv(res) > 0){
        std::cout << "command " << res.GetHeader()->GetCommand() << std::endl;
        if(res.GetHeader()->GetCommand() == CMD_JOB_STATE){
          res_code = REQUEST_SUCCESS;
          std::vector<std::string> argv = res.GetCmdArgs();
          if(argv.size() > 5){
            this->kdesk_jid = argv[1];
            this->kdesk_jstate = argv[2];
            this->kdesk_start_date = argv[3];
            this->kdesk_duration_time = argv[4];
            status = ConvertKdeskJobState(kdesk_jstate);
          } else {
            std::cout << "CMD_JOB_STATE args is missing.\n";
          }
        } else {
          std::cout << "slave daemon submit job has failed \n";
          res_code = ERROR_SUBMIT_JOB;
        }

      } else {
        std::cout << "can not recv response from slave daemon \n";
        res_code = ERROR_SUBMIT_JOB;
      }
    } else {
      std::cout << "can not send command to slave daemon \n";
      res_code = ERROR_SUBMIT_JOB;
    }
  } catch ( SocketException& e ) {
    std::cout << "Exception was caught:" << e.description() << "\n";
  }
  return res_code;
}

KP_JobStatus Job::ConvertKdeskJobState(const std::string& kd_jstate){
//  Current processing state of the job
//  ACC: Accepted job submission
//  RJT: Rejected job submission
//  QUE: Waiting for job execution
//  SIN: In the processing of the stage-in
//  RDY: Ready to execute the job
//  RNA: Acquiring resources required for job execution
//  RUN: Executing job
//  RNO: Waiting for completion of job termination processing
//  SOT: In the processing of the stage-out
  //EXT: Completed of the job termination processing
  //CCL: Exit by job cancelation
  //HLD: Holding status by user operation
  //ERR: In fixed state due to an error
  std::cout << "Job::ConvertKdeskJobState(kd_jstate = << " << kd_jstate << ")\n";
  KP_JobStatus kp_state = JOB_UNKNOW;
  if(kd_jstate == "RJT" || kd_jstate == "EXT" || kd_jstate == "CCL" || kd_jstate == "RJT"){
    kp_state = JOB_FINISH;
  //}else if(kd_jstate == "RNO" || kd_jstate == "SOT"){
  //  kp_state = JOB_FINISH;
  }else if(kd_jstate == "ERR"){
    kp_state = JOB_ERROR;
  }else if(kd_jstate == "QUE"){
    kp_state = JOB_QUEUE;
  }else {
    kp_state = JOB_RUNNING;
  }
  std::cout << " kp status = " << kp_state << std::endl;
  return kp_state;
}

  ResponseCode Job::Stop() {
    std::cout << "stop job \n";
    //return DATA_SUCCESS; //justtest
    //Get info of service
    ResponseCode ret = DATA_ERROR;
    Service sv;
    sv.setServiceID(service_id);
    DataManager data_manager(PATH_OF_DATABASE);
    ret = data_manager.connectDB();
    if ( ret == DATA_SUCCESS) {
      ret = data_manager.getInfoOfService(sv);
      if (data_manager.disConnectDB() != DATA_SUCCESS) {
        std::cout << "Error close db when get info service\n";
      }
    }
    if (ret != DATA_SUCCESS) {
      return ret;
    }
    //Create request stop job
    std::stringstream ss;

    ss  << "{\"Cmd\":[\"sh\",\"" << pathJob + PATH_SEPARATOR + "stop_" + job_id + ".sh" << "\"]"<< "}\r\n";
    std::string content = ss.str();
    ss.str("");
    ss << "POST /containers/" << container_id << "/exec HTTP/1.1\r\n";
    ss << "Content-Type:application/json\r\n";
    ss << "Content-Length: " << content.size() << "\r\n";
    ss << "Host: " << DockerTcp_IP << "\r\n\r\n";
    ss << content;

    std::string request = ss.str();
    std::cout << request << std::endl;
    std::string result, header_result;
    NetworkUtil::requestHttp(DockerTcp_IP, DockerTcp_Port, request, result, header_result);
    std::size_t found = header_result.find(DOCKER_RESPONE_HTTP_201);
    if (found == std::string::npos) {
      std::cout << "Request docker create exec error. \n";
      std::cout << "Content error: \n" << result << std::endl;
      return REQUEST_DOCKER_ERROR;
    }

    JSONNode n;
    try {
      n = libjson::parse(result);
    } catch(const std::invalid_argument& e) {
      std::cerr << "Lib json error. Please check : " << e.what() << '\n';
      return DATA_ERROR;
    }

    JSONNode::const_iterator i = n.find("Id");
    if (i == n.end()) {
      std::cout << "Respone error.\n";
      return DATA_ERROR;
    }
    std::string execId = i->as_string();
    std::cout << "execId = " << execId << std::endl;

    //Start request stop job
    content = "{\"Detach\":true}\r\n";
    ss.str("");
    ss << "POST /exec/" << execId << "/start HTTP/1.1\r\n";
    ss << "Content-Type:application/json\r\n";
    ss << "Content-Length: " << content.size() << "\r\n";
    ss << "Host: " << DockerTcp_IP << "\r\n\r\n";
    ss << content;

    request = ss.str();
    std::cout << request << std::endl;
    result = "";
    header_result = "";
    NetworkUtil::requestHttp(DockerTcp_IP, DockerTcp_Port, request, result, header_result);
    found = header_result.find(DOCKER_RESPONE_HTTP_204);
    if (found == std::string::npos) {
      std::cout << "Request docker start exec error. \n";
      std::cout << "Content error: \n" << result << std::endl;
      return REQUEST_DOCKER_ERROR;
    }

    return ret;
  }
/*
  ResponseCode Job::updateDuration() {
    std::cout << "updateDuration\n";

    //return DATA_SUCCESS; //just test

  //  struct tm startTime;
  //  int y,m,d,hh,mm;
  //  double mss;
    long duration_second = 0;
    long duration_msecond = 0;
    if (status == JOB_RUNNING) {
      struct timeval now_timev;
      gettimeofday(&now_timev, NULL);

      struct tm * now_tm;
      now_tm = gmtime (&(now_timev.tv_sec));
      now_timev.tv_sec = mktime ( now_tm );
      double now_mss = now_timev.tv_sec + now_timev.tv_usec/1000000.0;
      std::cout << "now_mss = " << (long)now_mss << std::endl;

      std::cout << now_tm->tm_year + 1900 << "/" << now_tm->tm_mon + 1 << "/" << now_tm->tm_mday << " " << now_tm->tm_hour << ":" << now_tm->tm_min << ":" << now_tm->tm_sec << "." << now_timev.tv_usec << std::endl;

      struct tm startTime;
      int y,m,d,hh,mm;
      double mss;
      sscanf(timeStartJob.c_str(), "%d-%d-%dT%d:%d:%lf", &y, &m, &d, &hh, &mm, &mss);
      fprintf(stdout, "%d-%d-%dT%d:%d:%lf\n", y, m, d, hh, mm, mss);
      startTime.tm_hour = hh;   startTime.tm_min = mm; startTime.tm_sec = 0;
      startTime.tm_year = y - 1900; startTime.tm_mon = m - 1; startTime.tm_mday = d;
      mss += mktime (&startTime);
      std::cout << "mss = " << (long)mss << std::endl;

      double diff = now_mss - mss;
      std::cout << "diff = " << diff << std::endl;
      duration_second = (long)diff;
      duration_msecond = (long)((diff - duration_second) * 1000000);

      std::cout << "duration_second = " << duration_second << std::endl;
      std::cout << "duration_msecond = " << duration_msecond << std::endl;
    } else if (status == JOB_FINISH){
      if(timeStartJob.c_str()[0] == '0'){
        duration_second  = 0;
      } else {
        struct tm startTime;
        int y,m,d,hh,mm;
        double mss;
        sscanf(timeStartJob.c_str(), "%d-%d-%dT%d:%d:%lf", &y, &m, &d, &hh, &mm, &mss);
        fprintf(stdout, "%d-%d-%dT%d:%d:%lf\n", y, m, d, hh, mm, mss);
        startTime.tm_hour = hh;   startTime.tm_min = mm; startTime.tm_sec = 0;
        startTime.tm_year = y - 1900; startTime.tm_mon = m - 1; startTime.tm_mday = d;
        mss += mktime (&startTime);
        std::cout << "mss = " << (long)mss << std::endl;

        int e_y,e_m,e_d,e_hh,e_mm;
        double e_mss;
        struct tm endTime;
        sscanf(timeEndJob.c_str(), "%d-%d-%dT%d:%d:%lf", &e_y, &e_m, &e_d, &e_hh, &e_mm, &e_mss);
        fprintf(stdout, "%d-%d-%dT%d:%d:%lf\n", y, m, d, hh, mm, mss);
        endTime.tm_hour = e_hh;   endTime.tm_min = e_mm; endTime.tm_sec = 0;
        endTime.tm_year = e_y - 1900; endTime.tm_mon = e_m - 1; endTime.tm_mday = e_d;
        e_mss += mktime (&endTime);
        std::cout << "e_mss = " << (long)e_mss << std::endl;

        double diff = e_mss - mss;
        std::cout << "diff = " << diff << std::endl;
        duration_second = (long)diff;
        duration_msecond = (long)((diff - duration_second) * 1000000);
        std::cout << "duration_second = " << duration_second << std::endl;
        std::cout << "duration_msecond = " << duration_msecond << std::endl;
      }
    } else {
      return DATA_ERROR;
    }
    std::cout << "duration_second = " << duration_second << std::endl;
    int hour = 0,minute =0, second = 0;
    second = (int)(duration_second % 60);
    minute = (int)((duration_second / 60) % 60);
    hour = (int)((duration_second / 60) / 60);
    std::stringstream strstr;
    strstr << hour << ":" << minute << ":" << second << "." << duration_msecond;
    duration = strstr.str();
    return DATA_SUCCESS;
  }
*/

  void Job::TimerEnd(){
    timer.Stop();
    timer.Reset();
    timerStarted = false;
  }

  void Job::TimerStart(){
    timer.Reset();
    timer.Start();
    timerStarted = true;
  }

  ResponseCode Job::updateDuration() {
    if(!timerStarted)
      return DATA_SUCCESS;

    std::cout << "updateDuration\n";    
  //  struct tm startTime;
  //  int y,m,d,hh,mm;
  //  double mss;
    double duration_second = 0;
    long duration_msecond = 0;
    if (status == JOB_RUNNING) {
      timer.Stop();
      duration_second = timer.Seconds();
      timer.Start();
    } else {//if (status == JOB_FINISH){
      timer.Stop();
      timerStarted = false;
      return DATA_SUCCESS;
    }
//    else {
//      return DATA_ERROR;
//    }

    std::cout << "duration_second = " << duration_second << std::endl;
    long duration_second_l = floor(duration_second);
    duration_msecond = (int)((duration_second - duration_second_l) * 1000);
    int hour = 0,minute =0, second = 0;
    second = (int)(duration_second_l % 60);
    minute = (int)((long)floor(duration_second_l / 60) % 60);
    hour = (int)floor((duration_second_l / 60) / 60);
    std::stringstream strstr;
    strstr << hour << ":" << minute << ":" << second << "." << duration_msecond;
    duration = strstr.str();
    std::cout << "duration_str: " << duration << std::endl;

    //std::ostringstream strs;
    //strs << duration_second;
    //duration = strs.str();
    return DATA_SUCCESS;
  }

  ResponseCode Job::getStat() {
    updateDuration();
    return DATA_SUCCESS;
  }
/*
  ResponseCode Job::getStat() {
    std::cout << "getStat job start\n";
    //Send request get status job
    std::stringstream ss;
    ss << "GET /containers/" << container_id << "/json HTTP/1.1\r\n";
    ss << "Host: " << DockerTcp_IP << "\r\n\r\n";

    std::string request = ss.str();
    std::cout << request << std::endl;
    std::string result, header_result;
    NetworkUtil::requestHttp(DockerTcp_IP, DockerTcp_Port, request, result, header_result);
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

    JSONNode::const_iterator i = n.find("State");
    if (i == n.end() || i->type() != JSON_NODE) {
      std::cout << "Respone error.\n";
      return DATA_ERROR;
    }
    JSONNode stateNode = i->as_node();
    i = stateNode.find("Running");
    bool isRunning = i->as_bool();

    if (isRunning) {
      std::cout << "job is running" << std::endl;
      status = JOB_RUNNING;
    } else {
      std::cout << "job finished" << std::endl;
      if (status == JOB_RUNNING) {
        status = JOB_FINISH;
      }
    }

    i = stateNode.find("StartedAt");
    //timeStartJob = std::string(i->as_string().data());

    i = stateNode.find("FinishedAt");
    //timeEndJob = std::string(i->as_string().data());

    updateDuration();

      return DATA_SUCCESS;
  }
  */
/*
  ResponseCode Job::getStat() {
    //return DATA_SUCCESS; // justest
    std::cout << "getStat job start\n";
    //Send request get status job
    std::stringstream ss;
    ss << "GET /containers/" << container_id << "/json HTTP/1.1\r\n";
    ss << "Host: " << DockerTcp_IP << "\r\n\r\n";

    std::string request = ss.str();
    std::cout << request << std::endl;
    std::string result, header_result;
    NetworkUtil::requestHttp(DockerTcp_IP, DockerTcp_Port, request, result, header_result);
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

    JSONNode::const_iterator i = n.find("State");
    if (i == n.end() || i->type() != JSON_NODE) {
      std::cout << "Respone error.\n";
      return DATA_ERROR;
    }
    JSONNode stateNode = i->as_node();
    i = stateNode.find("Running");
    bool isRunning = i->as_bool();

    if (isRunning) {
      std::cout << "job is running" << std::endl;
      status = JOB_RUNNING;
    } else {
      std::cout << "job finished" << std::endl;
      if (status == JOB_RUNNING) {
        status = JOB_FINISH;
      }
    }

    i = stateNode.find("StartedAt");
    timeStartJob = std::string(i->as_string().data());

    i = stateNode.find("FinishedAt");
    timeEndJob = std::string(i->as_string().data());

    updateDuration();

      return DATA_SUCCESS;
  }
*/
  ResponseCode Job::Clear() {
    std::cout << "delete job container \n";
    ResponseCode ret = DATA_ERROR;
    while ((ret = getStat()) == DATA_SUCCESS && status == JOB_RUNNING) {
      ret = Stop();
      if (ret != DATA_SUCCESS) {
      //    return ret;
      }
    }
    //ret = DATA_SUCCESS;
    if (ret == DATA_SUCCESS) {
      //delete from docker server
      std::stringstream ss;
      ss << "DELETE /containers/" << container_id << "?v=1&force=1 HTTP/1.1\r\n";
      ss << "Host: " << DockerTcp_IP << "\r\n\r\n";

      std::string request = ss.str();
      std::cout << request << std::endl;
      std::string result = "";
      std::string header_result = "";
      NetworkUtil::requestHttp(DockerTcp_IP, DockerTcp_Port, request, result, header_result);
      std::size_t found = header_result.find(DOCKER_RESPONE_HTTP_204);
      if (found == std::string::npos) {
        std::cout << "Request docker delete container error. \n";
        std::cout << "Content error: \n" << result << std::endl;
      //    return REQUEST_DOCKER_ERROR;
      }

      //delete resource
      std::cout << "delete job folder in master instance \n";
      std::string host;
      DataManager data_manager(PATH_OF_DATABASE);
      if ( data_manager.connectDB() == DATA_SUCCESS) {
        User user(user_email, USER_GROUP);
        ret = data_manager.getUser(user);
        if (ret != DATA_SUCCESS) {
          return ret;
        }
        std::string containerId = user.getContainerId();
        ret = NetworkUtil::getIpAddressOfContainer(containerId, host);
        if (ret != REQUEST_SUCCESS) {
          return ret;
        }
        data_manager.disConnectDB();
      }

      ret = NetworkUtil::ProcessDeleteDir(host, Container_Com_Port, pathJob);
    }

    return ret;
  }

  ResponseCode Job::Delete() {
    std::cout << "delete job \n";
    ResponseCode ret = DATA_ERROR;
    
    while ((ret = getStat()) == DATA_SUCCESS && status == JOB_RUNNING) {
      ret = Stop();
      if (ret != DATA_SUCCESS) {
      //    return ret;
      }
    }
    //ret = DATA_SUCCESS;
    if (ret == DATA_SUCCESS) {
      //delete from docker server
      std::stringstream ss;
      ss << "DELETE /containers/" << container_id << "?v=1&force=1 HTTP/1.1\r\n";
      ss << "Host: " << DockerTcp_IP << "\r\n\r\n";

      std::string request = ss.str();
      std::cout << request << std::endl;
      std::string result = "";
      std::string header_result = "";
      NetworkUtil::requestHttp(DockerTcp_IP, DockerTcp_Port, request, result, header_result);
      std::size_t found = header_result.find(DOCKER_RESPONE_HTTP_204);
      if (found == std::string::npos) {
        std::cout << "Request docker delete container error. \n";
        std::cout << "Content error: \n" << result << std::endl;
      //    return REQUEST_DOCKER_ERROR;
      }

      //delete resource
      std::cout << "delete job folder in master instance \n";
      std::string host;
      DataManager data_manager(PATH_OF_DATABASE);
      if ( data_manager.connectDB() == DATA_SUCCESS) {
        User user(user_email, USER_GROUP);
        ret = data_manager.getUser(user);
        if (ret != DATA_SUCCESS) {
          return ret;
        }
        std::string containerId = user.getContainerId();
        ret = NetworkUtil::getIpAddressOfContainer(containerId, host);
        if (ret != REQUEST_SUCCESS) {
          return ret;
        }
        data_manager.disConnectDB();
      }

      ret = NetworkUtil::ProcessDeleteDir(host, Container_Com_Port, pathJob);

      if (ret != REQUEST_SUCCESS) {
    //    return ret;
      }

      //delete in listjob
      std::cout << "remove job in listJob \n";
            extern std::map<std::string,Job> listJob;
      std::map<std::string,Job>::iterator it=listJob.find(job_id);
      if (it != listJob.end()) {
        std::cout << "remove job ok \n";
                listJob.erase(it);
      } else {
        std::cout << "remove job in listJob \n";
        ret = DATA_SELECT_EMPTY;
      }
    }

    return ret;
  }

  void Job::generateJsonStr(std::string &json_str) {
    JSONNode item(JSON_NODE);

    item.push_back(JSONNode("job_id", job_id));
    item.push_back(JSONNode("service_id", service_id));
    item.push_back(JSONNode("user_email", user_email));
    item.push_back(JSONNode("job_name", job_name));
    item.push_back(JSONNode("container_id", container_id));
    item.push_back(JSONNode("pathJob", pathJob));
    item.push_back(JSONNode("timeStartJob", timeStartJob));
    item.push_back(JSONNode("timeEndJob", timeEndJob));
    item.push_back(JSONNode("duration", duration));
    item.push_back(JSONNode("status", (int)status));
    item.push_back(JSONNode("strJson_param", strJson_param));
    item.push_back(JSONNode("strJsonOfJob", strJsonOfJob));

    JSONNode listParam_Node(JSON_ARRAY);
    listParam_Node.set_name("listParam");
    for (int i = 0; i < listParam.size(); i++) {
      JSONNode param(JSON_NODE);
      param.push_back(JSONNode("param_id", listParam[i].getParamId()));
      param.push_back(JSONNode("type", (int)listParam[i].getType()));
      param.push_back(JSONNode("value", listParam[i].getValue()));
      param.push_back(JSONNode("refer_job_id", listParam[i].getReferJobId()));
      listParam_Node.push_back(param);
    }
    item.push_back(listParam_Node);
    json_str = item.write();
  }
