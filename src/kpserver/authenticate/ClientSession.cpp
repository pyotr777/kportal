//HISTORY
//2013/09/06  EH0001  longnm  Add api for client
//2014/02/19  EH0011  longnm  UpdateFX10

#include "ClientSession.hpp"
#include "../job_manager/job_manager.hpp"
#include <time.h>
#include <sys/types.h>
#include "../stdafx.h"

#define MAX_NUM_OF_RETRY 10
using namespace std;
extern std::map<std::string,Job> listJob;
//extern pthread_mutex_t g_listjobs_lock;
extern std::map<std::string, ImageDocker> SampleImages;
extern std::map<std::string, int> g_tunnels;
extern std::string DEFAULT_JOB_FOLDER;
extern std::string APPMARKET_DOMAIN;
extern int g_kpserver_port;
extern std::string g_docker_bridge_host;
extern std::string DEFAULT_TEMPLATE_JOB_SCRIPT_FILE;

ClientSession::ClientSession()
{
  _localFile = NULL;
  _header = NULL;
  _service = NULL;
  _image = NULL;
  _isLogined = false;
  _job = NULL;
}

ClientSession::~ClientSession()
{
  std::cout << "~ClientSession() start" << std::endl;
  if(_localFile != NULL) {
    if(_localFile -> is_open())
      _localFile -> close();
    delete _localFile;
    _localFile = NULL;
  }
  std::cout << "Free _header" << std::endl;
  if(_header != NULL){
    delete _header;
    _header = NULL;
  }
    std::cout << "Free _service" << std::endl;
  if(_service != NULL){
    delete _service;
    _service = NULL;
  }

    std::cout << "Free _image" << std::endl;
  if(_image != NULL){
    delete _image;
    _image = NULL;
  }

  if(_job != NULL){
    _job = NULL;
    /// Not delete _job because point to item of map
  }
  std::cout << "~ClientSession() end" << std::endl;
}

//get status of a job
KP_JobStatus ClientSession::queryStatus(string msg)
{
  KP_JobStatus status = JOB_UNKNOW;
  /*
  JOB_STATUS js;
  try
  {
    js = JobManager::GetInstance() -> queryStatus(msg, _userId);
  }
  catch(std::exception& e)
  {
    std::cerr << "Exception: KP_JobStatus ClientSession::queryStatus(string msg)\n" << e.what() << endl;
    return status;
  }
  status = toKPJobStatus(js);
  */
  return status;
}
/*
std::list<KP_Service > ClientSession::getServices(std::string providerId = "")
{
  std::list<KP_Service > services;
  ServiceManager* svManager = ServiceManager::GetInstance();
  //svManager->setPwd("test");
  //svManager->BindToLDAP(HOSTNAME, PORT_NUMBER);
  svManager->InitUserAndService();

  vector<string> serviceIds = ServiceManager::GetInstance() -> getListServicesOfUser(_userId);
  vector<string> servicesByProvider;
  if(providerId != "")
  {
      servicesByProvider = ServiceManager::GetInstance() -> GetAllServiceOfProvider(providerId);
      for(int i = 0 ; i < serviceIds.size() ; i ++)
      {
          if(providerId != "")
          {
              if(std::find(servicesByProvider.begin(), servicesByProvider.end(), serviceIds[i]) == servicesByProvider.end())
                  continue;
          }

          KP_Service service = KP_Service(providerId, serviceIds[i]);
          services.push_back(service);
      }
  }
  else
  {
      for(int i = 0 ; i < serviceIds.size() ; i ++)
      {
          if(serviceIds[i] == "")
              continue;

          string proId = ServiceManager::GetInstance() -> GetProviderOfService(serviceIds[i]);
          KP_Service service = KP_Service(proId, serviceIds[i]);
          services.push_back(service);
      }
  }


    //svManager->UnBindLDAP();

    return services;
}
*/

std::list<Job> ClientSession::getJobs(std::string msg)
{
  std::list<Job > jobs;
  /*
  JobManager* jm = JobManager::GetInstance();

  vector<std::string> jobIds = jm -> getJobList(msg, _userId);
  vector<std::string>::iterator i =  jobIds.begin();


  while(i != jobIds.end())
  {
      jobs.push_back(KP_Job (*i));
      i++;
  }
  */
   return jobs;
}

ResponseCode ClientSession::getService(Service &service) {
  DataManager data_manager(PATH_OF_DATABASE);
  ResponseCode ret = DATA_ERROR;
  if ( data_manager.connectDB() == DATA_SUCCESS) {
  ret = data_manager.getInfoOfService(service);
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when get all service\n";
    }
  }
  return ret;
}

std::string ClientSession::getRunMethodsFile(std::string msg) {
  //std::string fileName = "/home/tuannp/k-portal-tuannp/K-Portal/src/K-Server/RunMethodsFile.xml";
  //std::string fileName = "";
  //fileName = JobManager::GetInstance()->getRunMethodsFile(msg);
  //return fileName;
  return "";
}

void ClientSession::removeRunMethodsFileTemp(std::string& path)
{
  //JobManager::GetInstance()->cleanAfterGetRMFile(path);
}

ResponseCode ClientSession::InitJob(JSONNode& n, Job &newJob) {
  ResponseCode ret = DATA_ERROR;
  bool needLocalInput = false;
  std::vector<Parameter> params;
  //Job newJob;
  //job name
  JSONNode::const_iterator i = n.find(TAG_JOB_NAME_STR);
  if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
    newJob.setJobName(i->as_string());
  } else {
    newJob.setJobName("");
  }

  //service id
  i = n.find(TAG_SERVICE_STR);
  if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
    newJob.setServiceId(i->as_string());
    ret = getAllParametersOfService(i->as_string(), params);
    if(ret != DATA_SUCCESS){
      std::cout << "get list defined parameter of service " << i->as_string() << " fail. code: " << ret << std::endl;
      return ret;
    }
  } else {
    newJob.setServiceId("");
  }

  //user email
  newJob.setUserEmail(getUserId());

  //number of node
  i = n.find(TAG_NUMBER_OF_NODES_STR);
  if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
      newJob.setNumberOfNode(i->as_int());
  } else {
      newJob.setNumberOfNode(0);
  }

  //max elapsed time
  i = n.find(TAG_MAX_ESLAPSED_TIME_STR);
  if (i != n.end() && i -> type() != JSON_ARRAY && i -> type() != JSON_NODE) {
      newJob.setMaxElapseTime(i->as_int());
  } else {
      newJob.setMaxElapseTime(0);
  }

  //params: Check existence of input paramter in a message
  i = n.find(TAG_PARAMS_STR);
  std::vector<int> param_existed_idxs;

  std::cout << "parse parameter\n";
  if (i != n.end() && i -> type() == JSON_NODE) {
    std::cout << " json node; ";
    for (JSONNode::const_iterator j = i->begin(); j != i->end(); j++) {
      param_job pa_job;
      unsigned int pi = 0;
      for ( ;pi < params.size(); pi++){
        if(params[pi].getParamID() == j->name()){
          break;
        }
      }
      if(pi == params.size()){
        // parameter not exist
        std::cout << "unknown parameter name " << j->name() << "\n";
      } else {
        // id
        pa_job.setParamId(j->name());
        //std::cout << "param: " << j->name() << ", ";

        // type
        if (j->type() != JSON_NODE && j->type() != JSON_ARRAY) {
          if(params[pi].getIsOutput())
            pa_job.setType(ARG_OUTPUT);
          else
            pa_job.setType(VALUE_ONLY);
          //std::cout << "type: " << pa_job.getType() << ", ";

          pa_job.setValue(j->as_string());
          //std::cout << "value: " << pa_job.getValue() << "\n";
        } else if (j->type() == JSON_NODE){
          //type
          JSONNode::const_iterator k = j->find(TAG_TYPE_STR);
          TYPE_PARAM_JOB type = ARG_UNKNOW;
          if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE)
            if (k->as_string().compare("local") == 0) {
              type = ARG_LOCAL;
              needLocalInput = true;
            }
          //job
          if (type != ARG_LOCAL) {
            k = j->find(TAG_JOB_STR);
            if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
              type = ARG_REMOTE;
              pa_job.setReferJobId(k->as_string());
            }
          }
          //std::cout << "type: " << type << ", ";
          pa_job.setType(type);
          //path
          k = j->find(TAG_PATH_STR);
          if (k != j->end() && k -> type() != JSON_ARRAY && k -> type() != JSON_NODE) {
             pa_job.setValue(k->as_string());
          } else {
             pa_job.setValue("");
          }

          //std::cout << "value: " << pa_job.getValue() << "\n";
        } else {
          std::cout << "unknown json node type\n";
          continue;
        }
        // Add param to listparam
        std::cout << "add param to list\n";
        newJob.addParam(pa_job);
        param_existed_idxs.push_back(pi);
      }
    }
    newJob.setStrJsonParam(i->write_formatted());
  } else {
    std::cout << " json is not node, parameter is emtpy.\n";
  }

  // Check required parameter
  std::cout << "check required parameter\n";
  for(unsigned int k = 0; k < params.size() ; k ++){
    if(params[k].getIsRequired()){
      unsigned int ik = 0;
      for(;ik < param_existed_idxs.size(); ik++){
        if((int)k == param_existed_idxs[ik]){
          break;
        }
      }
      if(ik == param_existed_idxs.size()){
        std::cout <<"[ERR]" << params[k].getParamID() << " required parameter is missing" << std::endl;
        return ret;
      }
    }
  }

  //newJob.setStrJsonOfJob((n.write_formatted()));

  //init
  ret = newJob.Init();
  if (ret == DATA_SUCCESS) {
    //add to list
    std::cout << "add to list a new job: " << newJob.getId() << std::endl;

    //pthread_mutex_lock(&g_listjobs_lock);
    listJob[newJob.getId()] = newJob;
    //pthread_mutex_unlock(&g_listjobs_lock);

    //add kp_job
    //job = newJob;
    if (needLocalInput) {
      ret = ACTION_WAIT_INPUT;
      /// Save temp _job wait input file
      //pthread_mutex_lock(&g_listjobs_lock);
      _job = &listJob[newJob.getId()];
      //pthread_mutex_unlock(&g_listjobs_lock);
    }
  }
  return ret;
}

ResponseCode ClientSession::submitJob(Job &job) {
  std::cout << "ClientSession::submitJob\n";
  ResponseCode ret = DATA_ERROR;
  JobManager job_manager;
 // pthread_mutex_lock(&g_listjobs_lock);
  Job* p_job = &listJob[job.getId()];

  // Get provider info
  std::cout << "configure tunnel  " << p_job->getProviderEmail() << std::endl;
  User owner(p_job->getProviderEmail());
  ret = getInfoUser(owner);
  if(ret != DATA_SUCCESS){
    std::cout << "get info of provider fail\n";
    //pthread_mutex_unlock(&g_listjobs_lock);
    return ret;
  }

  // If time usable == 0, auto get kcomputer time quota and set usable time for provider = 1/10
  if(owner.getTimeUsable() == 0){
    std::cout << "The first time connect to kacc, so need get timequota to set time usable for provider\n";
    std::string unit = "Ms", public_key;
    double total = 0, remain = 0, usage = 0, totalkp = 0, remainkp = 0, usagekp = 0;
    ret = getProviderTimeResource(p_job->getProviderEmail(), unit, total, remain, usage, totalkp, remainkp, usagekp, public_key);

//    if(ret == REQUEST_SUCCESS){
//      double usableTime = remain / 10;
//      std::cout << "usable time = " << usableTime << std::endl;
//      ret = updateProviderTimeUsable(p_job->getProviderEmail(), usableTime);
//      if(ret == DATA_SUCCESS){
//        owner.setTimeUsable(usableTime);
//      } else {
//        std::cout << "update time usable fail\n";
//      }
//    } else {
//      std::cout << "get time resource fail\n";
//    }
  } else {
    // Check if ssh tunnel (a provider / a tunnel) has already started
    TunnelManager tunnel_mng;
    unsigned int kport = 0;
    tunnel_mng.ConfigureTunnel(owner.getKdeskAcc(), kport);
    std::cout << "tunnel port: " << kport << std::endl;
    p_job->setTunnelPort(kport);
  }

  // Check quota time
  if(owner.getTimeUsable() - owner.getTimeUsage() < (job.getMaxElapseTime() / 1000000)){
    std::cout << "max elapse time invalid or time quota over.\n";
    ret = ERROR_TIMEQUOTA_OVER;
    //pthread_mutex_unlock(&g_listjobs_lock);
    return ret;
  }


  std::cout << "after job id: " ;
  std::cout << p_job->getId() << std::endl;

  // Submit job
  std::cout << "before submit\n" ;
  ret = p_job->Submit();

  if (ret != DATA_SUCCESS) {
    //p_job->Delete();
    p_job->Clear();
    job = *p_job;
    //pthread_mutex_unlock(&g_listjobs_lock);
    job_manager.Remove(p_job->getId());
  } else {
    job = *p_job;
    //pthread_mutex_unlock(&g_listjobs_lock);
  }
  return ret;
}

std::string ClientSession::getOutputFile(std::string jobId, std::string pathFile) {
  std::string fileName;
  //pthread_mutex_lock(&g_listjobs_lock);
  Job *job = &listJob[jobId];
  fileName = job->getPathJob() + PATH_SEPARATOR + pathFile;
  //pthread_mutex_unlock(&g_listjobs_lock);
  return fileName;
}

bool ClientSession::checkRequestTimeout() {
  time_t current;
  time(&current);
  double seconds = difftime(current, _lastAction);
  _lastAction = current;
  if(seconds > REQUEST_TIMEOUT_ACCOUNT_SECONDS && _isLogined) {
    _isLogined = false;
    return false;
  } else {
    return true;
  }
}

ResponseCode ClientSession::saveTarFile(const char* data, std::string fileName) {
  ResponseCode ret = FILE_ACTION_SUCCESS;
  FILE * pFile;
  pFile = fopen(fileName.c_str(), "wb");
  if (pFile == NULL) {
    perror ("Error opening file");
    ret = FILE_ACTION_ERROR;
  } else {
    fwrite (data , sizeof(char), sizeof(*data), pFile);
    fclose (pFile);
  }
  return ret;
}

ResponseCode ClientSession::addNewUser(DataManager data_manager, std::string userEmail) {
  ResponseCode ret = DATA_ERROR;
  User user(userEmail, USER_GROUP);
  std::cout << "user email: " << user.getEmail() << std::endl;

  ret = createResourceForUser(user);
  if (ret == DOCKER_CREATE_CON_SUCCESS) {
    ret = data_manager.insertUser(user);
  }
  return ret;
}

ResponseCode ClientSession::createResourceForUser(User &user) {
  std::cout << "createResourceForUser \n";
  ResponseCode ret = DOCKER_CREATE_CON_SUCCESS;

  //create master-instance for user
  std::stringstream ss;
  ss << "{\"Cmd\":[\"sh\",\"/start_server.sh\"]"
     << ",\"Image\":\"" << BASE_IMAGE <<  ":" << TAG_IMAGE_DOCKER_STR "\""
     << ",\"Volumes\":{\"" << DEFAULT_JOB_FOLDER << "\": {}}"
       << ",\"HostConfig\":{\"Binds\":[\"/etc/localtime:/etc/localtime:ro\"]}"
     << "}\r\n";
    std::string content = ss.str();
    ss.str("");
  ss << "POST /containers/create HTTP/1.1\r\n";
  ss << "Content-Type:application/json\r\n";
    ss << "Content-Length: " << content.size() << "\r\n";
  ss << "Host: " << DockerTcp_IP << "\r\n\r\n";

    ss << content;

  std::string request = ss.str();
  std::cout <<"request to docker:" << request.c_str() << std::endl;
  std::string result, header_result;
  NetworkUtil::requestHttp(DockerTcp_IP, DockerTcp_Port, request, result, header_result);
  std::size_t found = header_result.find(DOCKER_RESPONE_HTTP_201);
  if (found == std::string::npos) {
    std::cout << "Request docker create error. \n";
    std::cout << "Content error: \n" << result << std::endl;
    return REQUEST_DOCKER_ERROR;
  }
  std::cout << "Docker response: " << result.c_str() << std::endl;

  JSONNode n;
  try
  {
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

  //start master-instance of user
    ss.str("");
  ss << "POST /containers/" << containerId << "/start HTTP/1.1\r\n";

  ss << "Host: " << DockerTcp_IP << "\r\n\r\n";

  request = ss.str();

    std::cout << request << std::endl;

  NetworkUtil::requestHttp(DockerTcp_IP, DockerTcp_Port, request, result, header_result);
  found = header_result.find(DOCKER_RESPONE_HTTP_204);
  if (found == std::string::npos) {
    std::cout << "Request docker start error. \n";
    std::cout << "Content error: \n" << result << std::endl;
    return REQUEST_DOCKER_ERROR;
  }

  user.setContainerId(containerId);

  return ret;
}

ResponseCode ClientSession::deleteJob(Job job)
{
  ResponseCode ret = DATA_ERROR;
  //pthread_mutex_lock(&g_listjobs_lock);
  Job *p_job = &listJob[job.getId()];
  if (p_job != NULL) {
    ret = p_job->Delete();
  }
  //pthread_mutex_unlock(&g_listjobs_lock);
  return ret;
}

//Job *ClientSession::restartJob(std::string jobId, std::string json)
//{
//  Job * job = NULL;
////  JOB_STATUS status = JobManager::GetInstance() -> RestartJob(json, _userId);
////  job = new KP_Job(jobId, toKPJobStatus(status));
//  job = new Job(jobId);
//  return job;
//}

ResponseCode ClientSession::cancelJob(Job &job) {
  std::cout << "ClientSession::cancelJob\n";
  return DATA_SUCCESS;
  ResponseCode ret = DATA_ERROR;
  //pthread_mutex_lock(&g_listjobs_lock);
  Job *p_job = &listJob[job.getId()];
  ret = p_job->Stop();
  job = *p_job;
  //pthread_mutex_unlock(&g_listjobs_lock);
  return ret;
}

//EH0001
ResponseCode ClientSession::getAllServices(std::vector<Service> &services) {
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
  ret = data_manager.getAllService(services);
      if (data_manager.disConnectDB() != DATA_SUCCESS) {
          std::cout << "Error close db when get all service\n";
      }
  }
  return ret;
}

ResponseCode ClientSession::getAllServicesOfProvider(std::string providerEmail, std::vector<Service> &services) {
  ResponseCode ret = DATA_ERROR;
  User prov(providerEmail, 2);
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
  ret = data_manager.getAllServiceOfProvider(prov,services);
      if (data_manager.disConnectDB() != DATA_SUCCESS) {
          std::cout << "Error close db when get all service of provider\n";
      }
  }
  return ret;
}

/*
// API for user normal register a service.
// username : email of user
// serviceID : id of service
*/
ResponseCode ClientSession::registerService(std::string username, std::string serviceID) {
  ResponseCode ret = DATA_ERROR;
  User_Service us(username, serviceID, WAITING);
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    if (data_manager.beginTransaction() == DATA_SUCCESS) {
      ret = data_manager.insertUserService(us);
      if (ret == DATA_SUCCESS)
        ret = data_manager.endTransaction();
      else
        data_manager.rollback();
    }

    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when user registry service\n";
    }
  }
  return ret;
}

ResponseCode ClientSession::registerProvider(std::string phone, std::string address, std::string kdeskacc, std::string description) {
  ResponseCode ret = DATA_ERROR;

  User user(_userId, WAIT_PROVIDER_GROUP);
  user.setPhone(phone);
  user.setAddress(address);
  user.setKdeskAcc(kdeskacc);
  user.setDescription(description);
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    if (data_manager.beginTransaction() == DATA_SUCCESS) {
      std::cout << "user email: " << user.getEmail() << std::endl;
      ret = data_manager.insertUser(user);
      if (ret == DATA_SUCCESS)
        ret = data_manager.endTransaction();
      else
        data_manager.rollback();
    }

    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when user registry service\n";
    }
  }
  return ret;
}

/*
//
*/
ResponseCode ClientSession::getAllCandidateUsersOfService(string serviceId,string providerId, vector<string> &listCandidate){
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    ret = data_manager.getAllWaitUserOfService(serviceId,listCandidate);
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when get all wait user of service\n";
    }
  }
  return ret;
}

/*
//
*/
vector<string> ClientSession::getAllUserOfService(string serviceId,string providerId) {
  vector<string> ret;
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    data_manager.getAllReadyUserOfService(serviceId,ret);
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
        std::cout << "Error close db when get all wait user of service\n";
    }
  }
  return ret;
}

/*
//
*/
ResponseCode ClientSession::getServicesHaveCandidates(string providerEmail, std::vector<Service> &list_Service) {
  ResponseCode ret = DATA_ERROR;
  User prov(providerEmail, 2);
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
  ret = data_manager.getAllServiceOfProviderHaveCandidate(prov,list_Service);
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when get all service of provider have candidate\n";
    }
  }
  return ret;
}

/*
// API for Provider add user to service.
*/
ResponseCode ClientSession::addUserToService(std::string serviceId, std::string userEmail) {
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {

    User_Service us(userEmail,serviceId,NO_STATUS);
    ret = data_manager.getInfoOfUserService(us);
    if (data_manager.beginTransaction() == DATA_SUCCESS) {
      if (ret == DATA_SELECT_EMPTY) {
        us.setStatus(READY);
        ret = data_manager.insertUserService(us);
        if (ret == DATA_SUCCESS) {
          User user(userEmail, USER_GROUP);
          ResponseCode respone = data_manager.getUser(user);
          if (respone == DATA_SELECT_EMPTY) {
            std::cout << "user is new user.\n";
            ret = addNewUser(data_manager, userEmail);
            std::cout << "finish add new user.\n";
          }
        }
      } else if ( ret == DATA_SUCCESS) {
        if (us.getStatus() == WAITING) {
          us.setStatus(READY);
          ret = data_manager.updateUserService(us);
          if (ret == DATA_SUCCESS) {
            User user(userEmail, USER_GROUP);
            ResponseCode respone = data_manager.getUser(user);
            if (respone == DATA_SELECT_EMPTY) {
              std::cout << "user is new user.\n";
              ret = addNewUser(data_manager, userEmail);
              std::cout << "finish add new user.\n";
            }
          }
        } else if (us.getStatus() == READY) {
          ret = DATA_IS_EXISTED;
        }
      }
      if (ret == DATA_SUCCESS)
        ret = data_manager.endTransaction();
      else
        data_manager.rollback();
    }

    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when provider add user to service\n";
    }
  }
  return ret;
}

/*
//
*/
ResponseCode ClientSession::removeUserToService(string providerEmail,string serviceId,string userEmail) {
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    User_Service us(userEmail,serviceId,NO_STATUS);
    ret = data_manager.getInfoOfUserService(us);
    if (data_manager.beginTransaction() == DATA_SUCCESS) {
      if (us.getStatus() != READY) {
        ret = DATA_SELECT_EMPTY;
      } else {
        ret = data_manager.deleteUserService(us);
      }
      if (ret == DATA_SUCCESS) {
        std::vector<std::string> listJobId;
        //pthread_mutex_lock(&g_listjobs_lock);
        for(std::map<std::string, Job>::iterator it = listJob.begin(); it != listJob.end(); ++it) {
          Job *job = & it->second;
          if (job->getServiceId().compare(serviceId) == 0 && job->getUserEmail().compare(userEmail) == 0) {
//                        job->Delete();
            listJobId.push_back(job->getId());
          }
        }

        for (unsigned int i = 0; i < listJobId.size(); i++) {
          std::map<std::string, Job>::iterator it = listJob.find(listJobId[i]);
          if (it != listJob.end()) {
            it->second.Delete();
          }
        }
       // pthread_mutex_unlock(&g_listjobs_lock);
        ret = data_manager.endTransaction();
      }
      else
        data_manager.rollback();
    }

    if (data_manager.disConnectDB() != DATA_SUCCESS) {
        std::cout << "Error close db when provider remove user from service\n";
    }
  }
  return ret;
}

/*
//
*/
ResponseCode ClientSession::removeCandidateFromService(string providerEmail,string serviceId,string userEmail) {
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    if (data_manager.beginTransaction() == DATA_SUCCESS) {
      User_Service us(userEmail,serviceId,NO_STATUS);
      ret = data_manager.getInfoOfUserService(us);
      if (us.getStatus() != WAITING) {
        ret = DATA_SELECT_EMPTY;
      } else {
        ret = data_manager.deleteUserService(us);
      }
      if (ret == DATA_SUCCESS)
        ret = data_manager.endTransaction();
      else
        data_manager.rollback();
    }

    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when provider remove candidate from service\n";
    }
  }
  return ret;
}

/*
//
*/
ResponseCode ClientSession::getServicesOfUser(string userEmail, std::vector<Service> &list_Service) {
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    ret = data_manager.getAllReadyServiceOfUser(userEmail,list_Service);
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when get all service of provider have candidate\n";
    }
  }
  return ret;
}

/*
//
*/
ResponseCode ClientSession::getServicesWaitToUseOfUser(string userEmail, std::vector<Service> &list_Service) {
    //vector<string> listService;
/*
    ServiceManager* svManager = ServiceManager::GetInstance();
    //svManager->setPwd(DEFAULT_PASS);
    //svManager->BindToLDAP(HOSTNAME, PORT_NUMBER);
    svManager->InitUserAndService();

    listService = svManager->getServicesWaitToUseOfUser(userID);
    //svManager->UnBindLDAP();
*/
  //return listService;
  ResponseCode ret = DATA_ERROR;
  std::vector<Service> listService;
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    ret = data_manager.getAllWaitServiceOfUser(userEmail,list_Service);
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
        std::cout << "Error close db when get all service of provider have candidate\n";
    }
  }
  return ret;
}

/*
//
*/
ResponseCode ClientSession::removeRegisterServiceOfUser(string serviceId,string userEmail) {
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    if (data_manager.beginTransaction() == DATA_SUCCESS) {
      User_Service us(userEmail,serviceId,NO_STATUS);
      ret = data_manager.getInfoOfUserService(us);
      if (us.getStatus() != WAITING && us.getStatus() != READY) {
          ret = DATA_SELECT_EMPTY;
      } else {
          ret = data_manager.deleteUserService(us);
      }
      if (ret == DATA_SUCCESS) {
        std::vector<std::string> listJobId;
        //pthread_mutex_lock(&g_listjobs_lock);
        for(std::map<std::string, Job>::iterator it = listJob.begin(); it != listJob.end(); ++it) {
          Job *job = & it->second;
          if (job->getServiceId().compare(serviceId) == 0 && job->getUserEmail().compare(userEmail) == 0) {
            //job->Delete();
            listJobId.push_back(job->getId());
          }
        }

        for (unsigned int i = 0; i < listJobId.size(); i++) {
          std::map<std::string, Job>::iterator it = listJob.find(listJobId[i]);
          if (it != listJob.end()) {
            it->second.Delete();
          }
        }
        //pthread_mutex_unlock(&g_listjobs_lock);
        ret = data_manager.endTransaction();
      } else
        data_manager.rollback();
    }

    if (data_manager.disConnectDB() != DATA_SUCCESS) {
        std::cout << "Error close db when provider remove candidate from service\n";
    }
  }
  return ret;
}

/*
//
*/
bool ClientSession::checkTypeUserIsProvider(string userEmail) {
  ResponseCode ret = DATA_ERROR;
  User prov(userEmail, 2);
  DataManager data_manager(PATH_OF_DATABASE);
  ret = data_manager.connectDB();
  if ( ret == DATA_SUCCESS) {
    ret = data_manager.getProvider(prov);
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when get provider\n";
    }
  }
  if (ret == DATA_SUCCESS) {
    return true;
  } else {
    return false;
  }
}

/*
//
*/
int timespec2str(char *buf, uint len, struct timespec *ts) {
  int ret;
  struct tm t;

  tzset();
  if (localtime_r(&(ts->tv_sec), &t) == NULL)
    return 1;

  ret = strftime(buf, len, "%F %T", &t);
  if (ret == 0)
    return 2;
  len -= ret - 1;

  ret = snprintf(&buf[strlen(buf)], len, ".%09ld", ts->tv_nsec);
  if (ret >= (int)len)
    return 3;

  return 0;
}

ResponseCode ClientSession::getJobsOfUsersByService(string serviceId, string userEmail, vector<Job> &listJob) {
  ResponseCode ret = DATA_ERROR;
    JobManager job_manager;
  ret = job_manager.getJobsOfUserByService(userEmail, serviceId, listJob);
  return ret;
}

ResponseCode ClientSession::getInfoOfJob(Job &job) {
  ResponseCode ret = DATA_ERROR;
  //pthread_mutex_lock(&g_listjobs_lock);
  std::map<std::string,Job>::iterator it=listJob.find(job.getId());
  if (it != listJob.end()) {
    Job* p_job = &listJob[job.getId()];
    if (p_job->getStatus() == JOB_RUNNING || p_job->getStatus() == JOB_QUEUE) {
      ret = p_job->getStat();
      if (ret != DATA_SUCCESS) {
        //pthread_mutex_unlock(&g_listjobs_lock);
        return ret;
      }
    }
    job = *p_job;
    ret = DATA_SUCCESS;
  } else {
    ret = ERROR_JOB_NOT_EXIST;
  }
  //pthread_mutex_unlock(&g_listjobs_lock);
  return ret;
}

ResponseCode ClientSession::getListEntryOfJob(string jobId, string curDir,map<string,bool> &listEntry) {
  JobManager job_manager;
  return job_manager.getListEntryOfJob(jobId,curDir,listEntry);
}

//EH0001 end

//User's api
ResponseCode ClientSession::getInfoUser(User &user) {
  ResponseCode ret = DATA_SUCCESS;

  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    ret = data_manager.getUserDb(user);
    std::cout << "Bp:getInfoUser:data_manager.disConnectDB start\n";
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "[ERR] close db when get user info \n";
    }
    std::cout << "Bp:getInfoUser:data_manager.disConnectDB end\n";
  }
  return ret;
}

//Admin's api
ResponseCode ClientSession::getAllWaitProviders(std::vector<User> &listWaitProvides) {
  ResponseCode ret = DATA_SUCCESS;
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    ret = data_manager.getAllWaitProviders(listWaitProvides);
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when getAllWaitProviders\n";
    }
  }
  return ret;
}

extern std::string kSshKeyDir;
extern std::string kKdeskHost;
extern std::string kDockerImagesUrl;
ResponseCode ClientSession::addProviders(std::vector<User> &listProvides) {
  ResponseCode ret = DATA_SUCCESS;

  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    //insert provider
    if ((ret = data_manager.beginTransaction()) == DATA_SUCCESS) {
      for (int i = 0; i < (int)listProvides.size(); i++) {
        // Check kdesk account 
        User tmp(listProvides[i].getEmail());
        data_manager.getUserDb(tmp);
        bool isExists = false;
        if(data_manager.checkKdeskAccExists( tmp.getEmail(), tmp.getKdeskAcc(), isExists) != DATA_SUCCESS || isExists)
        {
          std::cout << "[ERR] The kdesk account already exists by other provider.\n";
          ret = ERROR_KDESK_ACC_EXISTS;
          break;
        }
        // Add ssh config
        std::stringstream ss;
        ss << kSshKeyDir << tmp.getKdeskAcc() << "." << kKdeskHost;
        std::string ssh_key_file = ss.str();
        std::string ssh_public_key_str;
        std::string remote_user = tmp.getKdeskAcc();
        std::cout << "ssh key file: " << ssh_key_file << ", kdesk user: " << remote_user << std::endl;
        if(SshUtils::GenerateSshKey(ssh_key_file,ssh_public_key_str)){
          if(SshUtils::AddHost(kKdeskHost, remote_user, ssh_key_file)){
            std::cout << "Insert users to DB\n";
            listProvides[i].setPublicKey(ssh_public_key_str);
            //listProvides[i].setKdeskPort(providerKdeskPort++);
            ret = data_manager.insertUser(listProvides[i]);
          } else {
            std::cout << "add host fail\n";
            ret = SSH_ADDKEY_ERR;
          }
        } else {
          std::cout << "generate ssh key fail\n";
          ret = SSH_ADDKEY_ERR;
        }

        if (ret == DATA_SUCCESS) {
          std::string url = APPMARKET_DOMAIN + kDockerImagesUrl;
          // Send email to provider user
          std::string email = listProvides[i].getEmail();
          if( ! GmailUtils::SendEmail(email, "image_base", url, ssh_public_key_str)){
              std::cout << "Send email fail!\n";
              //ret = SSH_SENDEMAIL_ERR;
              ret = DATA_SUCCESS; //justtest
          }
        } else {
          std::cout << "ignore send email\n";
        }
      }

      if (ret == DATA_SUCCESS)
        ret = data_manager.endTransaction();
      else
        data_manager.rollback();
    }
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when addProviders\n";
    }
  }
  return ret;
}

ResponseCode ClientSession::updateProviderTimeUsable(std::string email, double timeUsable) {
  std::cout << "ClientSession::updateProviderTimeUsable " << email << " , " << timeUsable << std::endl;
  ResponseCode ret = DATA_SUCCESS;
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    //insert provider
    if ((ret = data_manager.beginTransaction()) == DATA_SUCCESS) {
      User tmp(email, PROVIDER_GROUP);
      data_manager.getUserDb(tmp);
      std::cout << "current time usale " << tmp.getTimeUsable() << ", new value " << timeUsable << std::endl;
      if(tmp.getTimeUsable() != timeUsable){
        tmp.setTimeUsable(timeUsable);
        ret = data_manager.updateUser(tmp);
        if(ret == DATA_SUCCESS){
          std::cout << "update success\n";
        } else {
          std::cout << "update fail\n";
        }
      } else {
        std::cout << "Time usable up-to-date\n";
        ret = DATA_SUCCESS;
      }
    }
    if (ret == DATA_SUCCESS)
      ret = data_manager.endTransaction();
    else
      data_manager.rollback();
  }
  if (data_manager.disConnectDB() != DATA_SUCCESS) {
    std::cout << "Error close db when updateUser";
  }
return ret;
}

ResponseCode ClientSession::deleteWaitProviders(std::vector<User> &listProvides) {
  ResponseCode ret = DATA_SUCCESS;

  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    //remove wait provider
    //if ((ret = data_manager.beginTransaction()) == DATA_SUCCESS) {
    for (int i = 0; i < (int)listProvides.size(); i++) {
      if ((listProvides[i].getType() & PROVIDER_GROUP) > 0) {
        // Remove list docker image
        std::vector<ImageDocker> listImages;
        data_manager.getAllImageDockerOfProvider(listProvides[i].getEmail(),listImages);
        for (int j = 0; j < (int)listImages.size(); j++) {
          ret = deleteImage(listImages[j]);
        }

        // Remove list service
        std::vector<Service> listService;
        getAllServicesOfProvider(listProvides[i].getEmail(), listService);
        for (unsigned int j = 0; j < listService.size(); j++) {
          ret = deleteService(listService[j]);
        }

        // Remove tunnel
        if(ret == DATA_SUCCESS){
          std::string provider_email = listProvides[i].getEmail();
          SendExitReq(provider_email);
        }
      }
      ret = data_manager.deleteUser(listProvides[i]);
      if (ret != DATA_SUCCESS) {
        break;
      }
  //            }

  //            if (ret == DATA_SUCCESS)
  //                ret = data_manager.endTransaction();
  //            else
  //                data_manager.rollback();

    }

    if (data_manager.disConnectDB() != DATA_SUCCESS) {
        std::cout << "Error close db when deleteWaitProviders. \n";
    }
  }
  return ret;
}

ResponseCode ClientSession::getAllProviders(std::vector<User> &listProvides) {
  ResponseCode ret = DATA_SUCCESS;

  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    ret = data_manager.getAllProvider(listProvides);
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
        std::cout << "Error close db when getAllProviders\n";
    }
  }
  return ret;
}
//End admins's api

//Provider's api

/*
 * Get all image docker of provider
 */
ResponseCode ClientSession::getAllImageDockerOfProvider(std::vector<ImageDocker> &listImages) {
  ResponseCode ret = DATA_SUCCESS;

  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    ret = data_manager.getAllImageDockerOfProvider(_userId, listImages);
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when getAllImageDockerOfProvider \n";
    }
  }
  return ret;
}

/*
 * Create a new image
 */
ResponseCode ClientSession::createImage(ImageDocker &images) {
  ResponseCode ret = ACTION_WAIT_TAR;
  _image = new ImageDocker();
  _image->setImageName(images.getImageName());
  _image->setIsPublic(images.getIsPublic());
  _image->setProviderEmail(_userId);
  _image->setComment(images.getComment());

  //check home dir of session connect
  FileUtils::CreateDirectory(FileUtils::GenPathDirOfSession(_conId));

  //gen path to upload image
  char* tmp_imagepath = tempnam (FileUtils::GenPathDirOfSession(_conId).c_str(), "imageFileUpload_");
  std::string pathImageTmp = tmp_imagepath;
  free(tmp_imagepath);

  _image->setTarStr(pathImageTmp);
  return ret;
}

/*
 * Continue to create a ImageDocker
 */

  // Update tag name
ResponseCode ClientSession::createImageContinue(ImageDocker &image) {
  std::cout << "createImageContinue "<< std::endl;
  std::string pathTarFile = image.getTarStr();
  std::cout << "path tmp dir: " << pathTarFile.c_str() << std::endl;
 
  ResponseCode ret = DATA_SUCCESS; 
  std::stringstream ss;
  std::string stdout, cmd;
  std::string delimiter = "\n";
  std::vector<std::string> after_images;
  std::vector<std::string> before_images;
  
  // Run command get list images
  ss.str("");
  ss << "docker -H " << DockerTcp_IP << ":" << DockerTcp_Port << " images -q ";
  cmd = ss.str();
  std::cout << "cmd: " << cmd.c_str() << std::endl;
  stdout = Exec(cmd.c_str());
  std::cout << "stdout: " << stdout.c_str() << std::endl;
  StringUtils::Split(stdout, before_images, delimiter);
  
  // Run command load image
  ss.str("");
  ss << "docker -H " << DockerTcp_IP << ":" << DockerTcp_Port << " load -i " << pathTarFile;
  cmd = ss.str();
  std::cout << "cmd: " << cmd.c_str() << std::endl;
  stdout = Exec(cmd.c_str());
  std::cout << "stdout: " << stdout.c_str() << std::endl;
  
  // Get new image id
  ss.str("");
  ss << "docker -H " << DockerTcp_IP << ":" << DockerTcp_Port << " images -q ";
  cmd = ss.str();
  std::cout << "cmd: " << cmd.c_str() << std::endl;
  stdout = Exec(cmd.c_str());
  std::cout << "stdout: " << stdout.c_str() << std::endl;
  StringUtils::Split(stdout, after_images, delimiter);
  std::string image_id = "";
  for (size_t a = 0; a < after_images.size(); a++){
    for(size_t b = 0; b < before_images.size(); b++){
      if(after_images[a] == before_images[b])
      {
        break;
      } 
      else
      {
        if(b == before_images.size() - 1){
         image_id = after_images[a];
         break;
        }
      }
    }
    if(image_id.size() != 0)
      break;
  }
  std::cout << "image id new: " << image_id.c_str() << std::endl;
  if(image_id.size() == 0)
  {
	std::cout << "try to load image fail.\n";
	return REQUEST_DOCKER_ERROR; 
  }  
  image.setImageId(image_id);
  
  // Update tag name
  ss.str("");
  ss << "docker -H " << DockerTcp_IP << ":" << DockerTcp_Port << " tag " << image_id << " " << image.getImageName();
  cmd = ss.str();
  std::cout << "cmd: " << cmd.c_str() << std::endl;
  stdout = Exec(cmd.c_str());
  std::cout << "stdout: " << stdout.c_str() << std::endl;
  
  //backup Tarball Image
  extern std::string ICON_FOLDER_PATH;
  std::string pathBackupImageDocker = ICON_FOLDER_PATH + PATH_SEPARATOR + image.getImageName() + ".tar";
  ss.str("");
  ss << "docker -H " << DockerTcp_IP << ":" << DockerTcp_Port << " save -o "<< pathBackupImageDocker << " " << image_id << " " ;
  cmd = ss.str();
  std::cout << "cmd: " << cmd.c_str() << std::endl;
  stdout = Exec(cmd.c_str());
  std::cout << "stdout: " << stdout.c_str() << std::endl;
  
  //delete Tarball Image
  if (remove(image.getTarStr().c_str()) != 0) {
    std::cout << "remove error: " << image.getTarStr() << std::endl;
  }
  image.setTarStr(pathBackupImageDocker);

  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
  //add image to db
    if ((ret = data_manager.beginTransaction()) == DATA_SUCCESS) {
      ret = data_manager.insertImageDocker(image);
      if (ret == DATA_SUCCESS)
        ret = data_manager.endTransaction();
      else
        data_manager.rollback();
    }
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when createImageContinue. \n";
    }
    if (_image != NULL) {
      delete _image;
      _image = NULL;
    }
  }
  
  return ret;
}


ResponseCode ClientSession::createImageContinue1(ImageDocker &image) {
  std::cout << "createImageContinue \n";
  ResponseCode ret = DATA_SUCCESS;

  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    //Check Tarball image
    std::string id_image;
    ret = UpdateRepoTagOfTarball (image.getTarStr(), image.getImageName(), TAG_IMAGE_DOCKER_STR, id_image);

    if (ret != FILE_ACTION_SUCCESS) {
      std::cout << "Process image upload error. \n";
            //delete Tarball Image
      if (remove(image.getTarStr().c_str()) != 0) {
        std::cout << "remove error: " << image.getTarStr() << std::endl;
      }
      return ret;
    }
    ret = data_manager.checkIdImageDocker(id_image);
    if (ret != DATA_SELECT_EMPTY) {
      std::cout << "Check image upload error. \n";
      //delete Tarball Image
      if (remove(image.getTarStr().c_str()) != 0) {
          std::cout << "remove error: " << image.getTarStr() << std::endl;
      }
      return ret;
    }
    image.setImageId(id_image);

    std::ifstream in(image.getTarStr().c_str(), std::ifstream::ate | std::ifstream::binary);
    unsigned int file_size = in.tellg();

    //load image to docker
    std::stringstream ss;
    ss << "POST /images/load HTTP/1.1\r\n";
    ss << "Content-Type: application/x-tar\r\n";
    ss << "Content-Length: " << file_size << "\r\n";
    ss << "Host: " << DockerTcp_IP << "\r\n\r\n";
    std::string request = ss.str();
    request = request + FileUtils::ReadAllFile(image.getTarStr());
    std::string result, header_result;
    NetworkUtil::requestHttp(DockerTcp_IP, DockerTcp_Port, request, result, header_result);
    std::size_t found = header_result.find(DOCKER_RESPONE_HTTP_200);
    if (found == std::string::npos) {
      std::cout << "Request docker load image error. \n";
      std::cout << "Content error: \n" << result << std::endl;
      return REQUEST_DOCKER_ERROR;
    }

    //backup Tarball Image
    extern std::string ICON_FOLDER_PATH;
    std::string pathBackupImageDocker = ICON_FOLDER_PATH + PATH_SEPARATOR + image.getImageName() + ".tar";
    FileUtils::copy_file(image.getTarStr(), pathBackupImageDocker);

    //delete Tarball Image
    if (remove(image.getTarStr().c_str()) != 0) {
      std::cout << "remove error: " << image.getTarStr() << std::endl;
    }

    image.setTarStr(pathBackupImageDocker);

    //add image to db
    if ((ret = data_manager.beginTransaction()) == DATA_SUCCESS) {
      ret = data_manager.insertImageDocker(image);

      if (ret == DATA_SUCCESS)
        ret = data_manager.endTransaction();
      else
        data_manager.rollback();
    }
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when createImageContinue. \n";
    }

    if (_image != NULL) {
      delete _image;
            _image = NULL;
    }
  }
  return ret;
}

ResponseCode ClientSession::updateImage(ImageDocker &image) {
  std::cout << "updateImage \n";
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  ret = data_manager.connectDB();
  if ( ret == DATA_SUCCESS) {
    if (data_manager.beginTransaction() == DATA_SUCCESS) {
      ret = data_manager.updateImageDocker(image);
      if (ret == DATA_SUCCESS)
        ret = data_manager.endTransaction();
      else
        data_manager.rollback();
    }
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when update service\n";
    }
  }
  return ret;
}

ResponseCode ClientSession::deleteImage(ImageDocker &image) {
  std::cout << "deleteImage \n";
  ResponseCode ret = DATA_SUCCESS;

  //remove all service

  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    std::vector<Service> listService;
    ret = data_manager.getAllServiceByImage(image.getImageId(), listService);
    if (ret == DATA_SUCCESS) {
      for (int  i=0; i< listService.size(); i++) {
          ret = deleteService(listService[i]);
      }
    }

    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when deleteWaitProviders. \n";
    }
  }


    //delete image to docker
    std::stringstream ss;
    ss << "DELETE /images/" << image.getImageId() << "?force=1 HTTP/1.1\r\n";
    ss << "Host: " << DockerTcp_IP << "\r\n\r\n";

    std::string request = ss.str();
    std::cout << "request: " << request << std::endl;
//    request = request + FileUtils::ReadAllFile(image.getTarStr());

    std::string result, header_result;
    NetworkUtil::requestHttp(DockerTcp_IP, DockerTcp_Port, request, result, header_result);
    std::size_t found = header_result.find(DOCKER_RESPONE_HTTP_200);
    if (found == std::string::npos) {
        std::cout << "Request docker delete image error. \n";
        std::cout << "Content error: \n" << result << std::endl;
//      return REQUEST_DOCKER_ERROR;
    }

  if ( data_manager.connectDB() == DATA_SUCCESS) {
    //remove image from db
//    if ((ret = data_manager.beginTransaction()) == DATA_SUCCESS) {
      ret = data_manager.deleteImageDocker(image);

//            if (ret == DATA_SUCCESS)
//        ret = data_manager.endTransaction();
//      else
//        data_manager.rollback();
//    }
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when deleteWaitProviders. \n";
    }
  }
  return ret;
}

/*
 * Get image by image id
 */
ResponseCode ClientSession::getImage(ImageDocker &image){
  std::cout << "get imageDocker by id \n";
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  ret = data_manager.connectDB();
  if ( ret == DATA_SUCCESS) {
    ret = data_manager.getImageDockerById(image.getImageId(), image);
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when update service\n";
        }
    }
  return ret;
}

ResponseCode ClientSession::getImageByName(std::string name,ImageDocker &image) {
  std::cout << "get imageDocker by Name \n";
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  ret = data_manager.connectDB();
  if ( ret == DATA_SUCCESS) {
    ret = data_manager.getImageDockerByName(name, image);
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when update service\n";
    }
    if ( ret == DATA_SUCCESS){
      return ret;
    }
/*
    /// Find image in sample images
    for(std::map<std::string, ImageDocker>::iterator it = SampleImages.begin(); it != SampleImages.end(); ++it) {
      string key = it->first;
      if( key.compare(name) == 0){
        ImageDocker id = it->second;
        image.setImageId(id.getImageId());
        image.setImageName(id.getImageName());
        image.setComment(id.getComment());
        image.setIsPublic(id.getIsPublic());
        image.setProviderEmail(id.getProviderEmail());
        image.setTarStr(id.getTarStr());
        ret = DATA_SUCCESS;
        return ret;
      }
    }
        */
  }

  return ret;
}

ResponseCode ClientSession::checkNameImage(std::string name) {
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  ret = data_manager.connectDB();
  if ( ret == DATA_SUCCESS) {
    ret = data_manager.checkNameImageDocker(name);
    if (ret != DATA_SELECT_EMPTY) {
      std::cout << "checkNameImage is false \n";
      return ret;
    }
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when get all service\n";
    }
  }
    /*
  /// Find image in sample images
  for(std::map<std::string, ImageDocker>::iterator it = SampleImages.begin(); it != SampleImages.end(); ++it) {
    ImageDocker id = it->second;
    if( id.getImageName().compare(name) == 0){
      ret = DATA_IS_EXISTED;
      break;
    }
    }*/
  return ret;
}

ResponseCode ClientSession::outProviderGroup() {
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  ret = data_manager.connectDB();
  if ( ret == DATA_SUCCESS) {
    User user(_userId, PROVIDER_GROUP);
        std::vector<ImageDocker> listImages;
        getAllImageDockerOfProvider(listImages);
        for (int i = 0; i < (int)listImages.size(); i++) {
            ret = deleteImage(listImages[i]);
        }
    ret = data_manager.deleteUser(user);
    if (ret != DATA_SUCCESS) {
      std::cout << "outProviderGroup is false \n";
      //return ret;
    }
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when outProviderGroup\n";
    }
  }

  if(ret == DATA_SUCCESS){
    // Remove tunnel;
    std::cout << "Remove tunnel of provider\n";
    SendExitReq(_userId);
  }
  return ret;
}

ResponseCode ClientSession::getPublicImageByKey(std::string key, std::vector<ImageDocker> &listImages) {
  ResponseCode ret = DATA_SUCCESS;
  std::vector<ImageDocker> listImages_temp;
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    if (key.size() > 0) {
      ret = data_manager.getAllPublicImageDocker(listImages_temp);
    } else {
      ret = data_manager.getAllPublicImageDocker(listImages);
    }
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when getPublicImageByKey \n";
    }
  }

  if (key.size() > 0) {
    std::locale loc;
    std::string tmp_key;
    for (std::string::size_type i=0; i<key.length(); ++i) {
      tmp_key += std::toupper(key[i],loc);
    }
    for (int i = 0; i < (int)listImages_temp.size(); i++) {
      std::string name = listImages_temp[i].getImageName();
      std::string tmp_name;
      for (std::string::size_type j=0; j<name.length(); ++j) {
        tmp_name += std::toupper(name[j],loc);
      }

      if (tmp_name.find(tmp_key) != std::string::npos) {
        listImages.push_back(listImages_temp[i]);
      }
    }

  }
  return ret;
}

/*
 * Process continue create a service
 */
ResponseCode ClientSession::createService(Service &service) {
  _service = new Service(service);
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  ret = data_manager.connectDB();
  
  // check service name
  ret = data_manager.checkNameService(service.getServiceName());
  if (ret != DATA_SELECT_EMPTY) {  
    data_manager.disConnectDB();
    ret = ERROR_SERVICE_INVALID_NAME;
    return ret;
  }
  
  //check path excute file & check template sh file
  ret = browsePathInsideImage(*_service, USF_ISNEW);
  if ( ret == DATA_SUCCESS) {
    if (service.getIconStr().size() > 0) {
      ret = ACTION_WAIT_ICON;
    }
  }
  return ret;
}

ResponseCode ClientSession::checkValidInfo(Service &service, const unsigned char usf_flags)
{
  std::cout << "checkValidInfo\n";
  ResponseCode ret = DATA_SUCCESS;
  if(usf_flags & USF_NAME)
  {
    DataManager data_manager(PATH_OF_DATABASE);
    ret = data_manager.connectDB();
    // check service name 
    ret = data_manager.checkNameService(service.getServiceName());
    if (ret != DATA_SELECT_EMPTY) {  
      data_manager.disConnectDB();
      ret = ERROR_SERVICE_INVALID_NAME;
      return ret;
    }
    else 
    {
      data_manager.disConnectDB();
      ret = DATA_SUCCESS;
    }
  }
  // Check path exists
  if(usf_flags & USF_IMAGE || usf_flags & USF_EXEPATH || usf_flags & USF_SHPATH || usf_flags & USF_STGINDIR)
    ret = browsePathInsideImage(service, usf_flags);
  return ret;
}

ResponseCode ClientSession::browsePathInsideImage(Service &service, const unsigned char usf_flags)
{
  std::cout << "browsePathInsideImage: usf_flags = " << usf_flags << ", image = " << service.getImageId() << std::endl;
  
  ResponseCode ret = DATA_SUCCESS;
  std::stringstream ss;
  std::string stdout, cmd;
  std::string container_id;
  std::string image_id = service.getImageId();
  std::string flag_str = "exists";
  
  // create temp container
  std::cout << "start a daemon container\n";
  ss.str(""); ss << "docker -H " << DockerTcp_IP << ":" << DockerTcp_Port << " create " << image_id << " /bin/sh -c \"while true; do sleep 1; done\"";
  cmd = ss.str(); std::cout << "cmd: " << cmd.c_str() << std::endl;
  stdout = Exec(cmd.c_str());
  std::cout << "stdout: " << stdout.c_str() << std::endl;
  container_id = stdout.substr(0, stdout.size() - 1);
  
  // start temp deamon 
  ss.str(""); ss << "docker -H " << DockerTcp_IP << ":" << DockerTcp_Port << " start " << container_id;
  cmd = ss.str(); std::cout << "cmd: " << cmd.c_str() << std::endl;
  stdout = Exec(cmd.c_str());
  std::cout << "stdout: " << stdout.c_str() << std::endl;
  
  // check deamon 
  //docker -H 127.0.0.1:9555 inspect  82ba5e4a2aa9b81fcd61965c3a9684089e288b3660cbdf458f32b3c4b9ba4e9c  | grep '"Running": true'
  ss.str(""); ss << "docker -H " << DockerTcp_IP << ":" << DockerTcp_Port << " inspect " << container_id << " | grep Running ";
  cmd = ss.str(); std::cout << "cmd: " << cmd.c_str() << std::endl;
  stdout = Exec(cmd.c_str());
  std::cout << "stdout: " << stdout.c_str() << std::endl;
  if(stdout.find("\"Running\": true") == std::string::npos){
    ret = ERROR_SERVICE_SLAVEDAEMON_NOTFOUND;
    std::cout << "[ERR] Can not create a container from this image.\n";
  }

  // check slavedaemon exists
  if(ret == DATA_SUCCESS) {
    // docker -H 127.0.0.1:9555 exec 82ba5e4a2aa9b81fcd61965c3a9684089e288b3660cbdf458f32b3c4b9ba4e9c test -f bin/startslavedaemon.sh && echo 1
    ss.str(""); ss << "docker -H " << DockerTcp_IP << ":" << DockerTcp_Port << " exec  " << container_id << " test -f /bin/startslavedaemon.sh && echo " << flag_str << " ";
    cmd = ss.str(); std::cout << "cmd: " << cmd.c_str() << std::endl;
    stdout = Exec(cmd.c_str());
    std::cout << "stdout: " << stdout.c_str() << std::endl;
    if(stdout.find(flag_str) == std::string::npos){
      std::cout << "[ERR] The slavedaemon not found. Image is invalid\n";
      ret = ERROR_SERVICE_SLAVEDAEMON_NOTFOUND;
    }
  }
  //check the execute file exists
  if(ret == DATA_SUCCESS && usf_flags & USF_EXEPATH) {
    std::string exe_path = service.getPathExcuteFile();
    if(exe_path == ""){
      std::cout << "[ERR] The exe file \"" << exe_path << "\" not found. Path is invalid\n";
      ret = ERROR_SERVICE_EXEPATH_NOTFOUND;
    }
    //exe_path = exe_path.find_first_of(PATH_SEPARATOR) == 0 ? exe_path.substr(1) : exe_path;
    exe_path = exe_path.find_first_of(PATH_SEPARATOR) == 0 ? exe_path : std::string(PATH_SEPARATOR + exe_path);
    ss.str(""); ss << "docker -H " << DockerTcp_IP << ":" << DockerTcp_Port << " exec  " << container_id << " test -f \"" << exe_path << "\" && echo " << flag_str << " ";
    cmd = ss.str(); std::cout << "cmd: " << cmd.c_str() << std::endl;
    stdout = Exec(cmd.c_str());
    std::cout << "stdout: " << stdout.c_str() << std::endl;
    if(stdout.find(flag_str) == std::string::npos){
      std::cout << "[ERR] The exe file \"" << exe_path << "\" not found. Path is invalid\n";
      ret = ERROR_SERVICE_EXEPATH_NOTFOUND;
    }
  }
  
  //check the stgin_dir_path file exists
  if(ret == DATA_SUCCESS && usf_flags & USF_STGINDIR) {
    std::string stgin_dir_path = service.getStageinDirs() -> size() > 0 ? service.getStageinDirs() -> at(0) : "";
    if(stgin_dir_path != ""){
      //stgin_dir_path = stgin_dir_path.find_first_of(PATH_SEPARATOR) == 0 ? stgin_dir_path.substr(1) : stgin_dir_path;
      stgin_dir_path = stgin_dir_path.find_first_of(PATH_SEPARATOR) == 0 ? stgin_dir_path : std::string(PATH_SEPARATOR + stgin_dir_path);
      ss.str(""); ss << "docker -H " << DockerTcp_IP << ":" << DockerTcp_Port << " exec  " << container_id << " test -d \"" << stgin_dir_path << "\" && echo " << flag_str << " ";
      cmd = ss.str(); std::cout << "cmd: " << cmd.c_str() << std::endl;
      stdout = Exec(cmd.c_str());
      std::cout << "stdout: " << stdout.c_str() << std::endl;
      if(stdout.find(flag_str) == std::string::npos){
        std::cout << "[ERR] The stage-in directory \"" << stgin_dir_path << "\" not found.\n";
        ret = ERROR_SERVICE_STAGEINDIR_NOTFOUND;
        return ret;
      }    
    } else {
      std::cout << "Have not stage-in dir\n";
    }
  }

  // get the template job script file.
  if(ret == DATA_SUCCESS && usf_flags & USF_SHPATH) {
    std::string sh_content; 
    // check template sh file
    if(service.getPathShFile() == ""){
      // get default job script file
      std::ifstream ifs(DEFAULT_TEMPLATE_JOB_SCRIPT_FILE.c_str());
      if(ifs.is_open()){
        sh_content.assign( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>()) );
        ret = DATA_SUCCESS;
      } else {
        std::cout << "[ERR] Read default jobs script file.\n";
        ret = ERROR_SERVICE_SHPATH_NOTFOUND;
      }
    } else{
      //std::string sh_file = service.getPathShFile().find_first_of(PATH_SEPARATOR) == 0 ? service.getPathShFile().substr(1) : service.getPathShFile();
      std::string sh_file = service.getPathShFile().find_first_of(PATH_SEPARATOR) == 0 ? service.getPathShFile() : std::string(PATH_SEPARATOR + service.getPathShFile());
      ss.str(""); ss << "docker -H " << DockerTcp_IP << ":" << DockerTcp_Port << " exec " << container_id << " cat \"" << sh_file << "\" ";
      cmd = ss.str(); std::cout << "cmd: " << cmd.c_str() << std::endl;
      stdout = Exec(cmd.c_str());
      std::cout << "stdout: " << stdout.c_str() << std::endl;
      if(stdout.find("#PJM") == std::string::npos){
        std::cout << "[ERR] The sh file \"" << sh_file << "\" not found.\n";
        ret = ERROR_SERVICE_STAGEINDIR_NOTFOUND;
      }
      sh_content = stdout;
    }
    std::cout << "Ssh content: \n" << sh_content << std::endl;
    service.setShTemplate(sh_content);
  }
  
  // remove temp container
  ss.str(""); ss << "docker -H " << DockerTcp_IP << ":" << DockerTcp_Port << " rm -f " << container_id;
  cmd = ss.str(); std::cout << "cmd: " << cmd.c_str() << std::endl;
  stdout = Exec(cmd.c_str());
  std::cout << "stdout: " << stdout.c_str() << std::endl;
  if(stdout.find(container_id) == std::string::npos){
    std::cout << "[ERR] Can not remove the temp container \"" << container_id << "\". Need manual remove it.\n";
  }
  return ret;
}

/*
 * Process continue create service
 */
ResponseCode ClientSession::createServiceContinue()
{
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  ret = data_manager.connectDB();
  if ( ret == DATA_SUCCESS) {
    //check path excute file
    ret = data_manager.checkIdImageDocker(_service->getImageId());
    if (ret != DATA_IS_EXISTED) {
        if (data_manager.disConnectDB() != DATA_SUCCESS) {
            std::cout << "Error close db when insert service\n";
        }
    } else {
      //insert service
      if (data_manager.beginTransaction() == DATA_SUCCESS) {
        ret = data_manager.insertService(*_service);
        if (ret == DATA_SUCCESS) {
          for (unsigned int i = 0; i < (*_service->getListParam()).size(); i++) {
            ret = data_manager.insertParam((*_service->getListParam())[i]);
            if (ret != DATA_SUCCESS) {
              break;
            }
          }
        }
        if (ret == DATA_SUCCESS)
          ret = data_manager.endTransaction();
        else
          data_manager.rollback();
      }
      if (data_manager.disConnectDB() != DATA_SUCCESS) {
        std::cout << "Error close db when insert service\n";
      }
    }
  }
  if(_service != NULL){
    delete _service;
    _service = NULL;
  }
  return ret;
}

ResponseCode ClientSession::updateService(Service &service) {
  //_service = new Service(service);
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  ret = data_manager.connectDB();
  if ( ret == DATA_SUCCESS) {
    // check docker image
    ret = data_manager.checkIdImageDocker(service.getImageId());
    if (ret != DATA_IS_EXISTED) {
        if (data_manager.disConnectDB() != DATA_SUCCESS) {
            std::cout << "Error close db when insert service\n";
        }
    } else {
      /*
      // check service name
      if(service.getServiceName() != ""){
        ret = data_manager.checkNameService(service.getServiceName());
        if (ret != DATA_SELECT_EMPTY) {  
          data_manager.disConnectDB();
          ret = ERROR_SERVICE_INVALID_NAME;
          return ret;
        }
      }

      // check sh file
      if(service.getPathShFile() != ""){
        ret = browsePathInsideImage(service);
	if(ret != DATA_SUCCESS) {
          data_manager.disConnectDB(); 
          return ret;
        }
      }
      */
      if (data_manager.beginTransaction() == DATA_SUCCESS) {
        ret = data_manager.updateService(service);
        if (ret == DATA_SUCCESS) {
          for (unsigned int i = 0; i < service.getListParam() -> size(); i++) {
            if ((*service.getListParam())[i].getAct() == CREAT) {
              ret = data_manager.insertParam((*service.getListParam())[i]);
              if (ret != DATA_SUCCESS) {
                  break;
              }
            } else if ((*service.getListParam())[i].getAct() == EDIT) {
              ret = data_manager.updateParam((*service.getListParam())[i]);
              if (ret != DATA_SUCCESS) {
                break;
              }
            } else if ((*service.getListParam())[i].getAct() == DELETE) {
              ret = data_manager.deleteParam((*service.getListParam())[i]);
              if (ret != DATA_SUCCESS) {
                  break;
              }
            }
          }
        }
        if (ret == DATA_SUCCESS)
            ret = data_manager.endTransaction();
        else
          data_manager.rollback();
      }
      if (data_manager.disConnectDB() != DATA_SUCCESS) {
        std::cout << "Error close db when update service\n";
      }
    }
  }
  return ret;
}

ResponseCode ClientSession::deleteService(Service &service) {
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  ret = data_manager.connectDB();
  if ( ret == DATA_SUCCESS) {
    if (data_manager.beginTransaction() == DATA_SUCCESS) {
        ret = data_manager.deleteService(service);
        if (ret == DATA_SUCCESS) {
            std::vector<std::string> listJobId;
            //pthread_mutex_lock(&g_listjobs_lock);
            for(std::map<std::string, Job>::iterator it = listJob.begin(); it != listJob.end(); ++it) {
                Job *job = & it->second;
                if (job->getServiceId().compare(service.getServiceID()) == 0) {
//                        job->Delete();
                    listJobId.push_back(job->getId());
                }
            }

            for (int i = 0; i < listJobId.size(); i++) {
                std::map<std::string, Job>::iterator it = listJob.find(listJobId[i]);
                if (it != listJob.end()) {
                    it->second.Delete();
                }
            }
            //pthread_mutex_unlock(&g_listjobs_lock);
            ret = data_manager.endTransaction();
        }
        else
            data_manager.rollback();
    }
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
        std::cout << "Error close db when delete service\n";
    }
  }
  return ret;
}

ResponseCode ClientSession::getAllParametersOfService(std::string serviceId, std::vector<Parameter> &listParam) {
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  ret = data_manager.connectDB();
  if ( ret == DATA_SUCCESS) {
    Service sv;
    sv.setServiceID(serviceId);
    ret = data_manager.getParamOfService(sv, listParam);
    if (ret != DATA_SUCCESS) {
      std::cout << "Error get params of service " << serviceId << std::endl;
    }
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when get all service\n";
    }
  }
  return ret;
}

ResponseCode ClientSession::checkPathOfService(Service &kp_sv) {
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  ret = data_manager.connectDB();
  if ( ret == DATA_SUCCESS) {
//        Service sv;
//        sv.setPathExcuteFile(kp_sv.pathExcuteFile);
//        std::vector<Service> listSv;
//        ret = data_manager.getServicesByPathExcute(sv, listSv);
    bool existPathExe = false;
    ret = data_manager.checkPathExcute(kp_sv.getPathExcuteFile(), existPathExe);
    if (existPathExe) {
        ret = ERROR_DUPLICATE_PATH_EXCUTE;
    }
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
        std::cout << "Error close db when get all service\n";
    }
  }
  return ret;
}

ResponseCode ClientSession::checkNameService(std::string name) {
  ResponseCode ret = DATA_ERROR;
  DataManager data_manager(PATH_OF_DATABASE);
  ret = data_manager.connectDB();
  if ( ret == DATA_SUCCESS) {
    ret = data_manager.checkNameService(name);
    if (ret != DATA_SELECT_EMPTY) {
      std::cout << "checkNameService is false \n";
      return ret;
    }
    if (data_manager.disConnectDB() != DATA_SUCCESS) {
      std::cout << "Error close db when get all service\n";
    }
  }
  return ret;
}

//EH0011 end

/// EH004 end

int ClientSession::enqueueFileReceive(std::string requestId, std::string path){
  struct FileReceive fr;
  fr.fileName = path;
  fr.requestId = requestId;
  _fileReceives.push(fr);
  return 0;
}

int ClientSession::dequeueFileReceive(struct FileReceive & fr){
  if(_fileReceives.size() == 0){
    return -1;
  }
  fr.fileName = _fileReceives.front().fileName;
  fr.requestId = _fileReceives.front().requestId;
  _fileReceives.pop();
  return 0;
}

/// EH004
/*
 * Get state
 */
ResponseCode ClientSession::getWorkStatus()
{
  return _state;
}

/*
 * Set work Status
 */
void ClientSession::setWorkStatus(ResponseCode& res)
{
   _state = res;
}

/*
 * Save file icon for service
 * data: binary data file
 */
ResponseCode ClientSession::saveFileIcon(const char* data) {
  return DATA_SUCCESS;
}

MessageHeader *ClientSession::getHeader() {
  return _header;
}

void ClientSession::setHeader(MessageHeader& hdr) {
  if(_header == NULL){
    _header = new MessageHeader(hdr.getRequestID(), hdr.getType());
  } else {
    *_header = hdr;
  }
}

/*
 * Get new service
 */
std::string ClientSession::getServiceId() {
  return string("new service id");
}

ImageDocker* ClientSession::getImageTmp(){
  return _image;
}

void ClientSession::setServiceTmp(Service * srv){
  /*if(_service != NULL){
  delete _service;
      _service = NULL;
  }*/
  _service = srv;
}

Service * ClientSession::getServiceTmp(){
  return _service;
}

Job * ClientSession::getJobTmp(){
  return _job;
}

/*
 * Query get user type
 */
TypeUser ClientSession::getUserType() {
  User usr(_userId);
  TypeUser type = USER_GROUP;
  ResponseCode ret = getInfoUser(usr);
  if (ret == DATA_SUCCESS && (int)(usr.getType() & (char)ADMIN_GROUP)) {
    type = ADMIN_GROUP;
  } else if (ret == DATA_SUCCESS && (int)(usr.getType() & (char)PROVIDER_GROUP)) {
    type = PROVIDER_GROUP;
  }
  return type;
}

ResponseCode ClientSession::getProviderTimeResource(const std::string& provider_email, std::string& unit, double& limit, double& avaiable, double& usage, double& limitkp, double& avaiablekp, double& usagekp, std::string& public_key){
  std::cout << "ClientSession::getProviderTimeResource\n";
  ResponseCode res_code = DATA_SUCCESS;

  User usr(provider_email);
  res_code = getInfoUser(usr);
  if(res_code != DATA_SUCCESS){
    std::cout << "get infor of provider fail\n";
    return res_code;
  }
  std::string kdeskacc = usr.getKdeskAcc();
  usagekp = usr.getTimeUsage();
  limitkp = usr.getTimeUsable();

  avaiablekp = limitkp - usagekp;

  std::cout << "kp usage time " << usagekp << ", kp limit " << limitkp << ", kp usable = " << avaiablekp << std::endl;
  bool isNeedSetTimeUseable = limitkp == 0 && usagekp == 0;

//  int port = usr.getKdeskPort();
//  if(port == 0)
//    port = 5000;
  std::string host = g_docker_bridge_host;

  // Check tunnel
  TunnelManager tunnel_mng;
  unsigned int kdesk_port;
  tunnel_mng.ConfigureTunnel(kdeskacc, kdesk_port);
  public_key = usr.getPublicKey();

  try {
    Socket client_socket;
    // ClientSocket client_socket ( _host, _port );

    for(int i = 0 ; i < MAX_NUM_OF_RETRY ; i++ ){
      if ( ! client_socket.Create() ) {
        throw SocketException ( "Could not create client socket." );
      }
      if ( ! client_socket.Connect(host, kdesk_port)) {
        client_socket.~Socket();
        std::cout << "Connect error, retry " << i << std::endl;
        sleep(3);
      } else {
        break;
      }
      if(i == MAX_NUM_OF_RETRY - 1){
        throw SocketException ( "Could not connect to host." );
      }
    }

    Message req(Header(MT_COMMAND, g_kpserver_port, kdesk_port, 0, CMD_GET_TIME_RESOURCE), NULL), res;
    req.AddArgv(kdeskacc);
    req.AddArgv(unit);

    // Send request
    if(client_socket.Send(req)){
      // Get response
      client_socket.Recv(res);

      std::cout << "command " << res.GetHeader()->GetCommand() << std::endl;
      if(res.GetHeader()->GetCommand() == CMD_ACK_OK){
        res_code = DATA_SUCCESS;
        std::vector<std::string> argv = res.GetCmdArgs();
        if(argv.size() > 3){
          try{
            unit = argv[0];
            limit = atoi(argv[1].c_str());
            usage = atoi(argv[2].c_str());
            avaiable = atoi(argv[3].c_str());
            if(isNeedSetTimeUseable){
              double usableTime = avaiable / 10;
              std::cout << "usable time = " << usableTime << std::endl;
              res_code = updateProviderTimeUsable(provider_email, usableTime);
              if(res_code == DATA_SUCCESS){
                usr.setTimeUsable(usableTime);
              } else {
                std::cout << "update time usable fail\n";
              }
            } else {
              std::cout << " don't need set provider usable time\n";
            }
          } catch (int &e){
            std::cout << "[ERR] Exception caught during convert string to int.\n";
          }
        } else {
          std::cout << "[ERR] response args is missing.\n";
        }
      } else {
        std::cout << "[ERR] kdesk response message unknown command.\n";
        res_code = ERROR_GET_TIME_RESOURCE;
      }
    } else {
      std::cout << "[ERR] can not send command to kdesk daemon \n";
      res_code = ERROR_GET_TIME_RESOURCE;
    }
  } catch ( SocketException& e ) {
    std::cout << "[ERR]  was caught:" << e.description() << "\n";
  }
  return res_code;
}

ResponseCode ClientSession::SendExitReq(std::string& provider_email){
  ResponseCode ret = DATA_SUCCESS;
  std::cout << "ClientSession::SendExitReq" << provider_email << "\n";
  try {
    // Get provider user
    User provider(provider_email);
    ret = getInfoUser(provider);
    if(ret != DATA_SUCCESS){
      std::cout << " get info of provider fail\n";
      //pthread_mutex_unlock(&g_listjobs_lock);
      return ret;
    }
    std::string kacc = provider.getKdeskAcc();
    std::map<std::string,int>::iterator it = g_tunnels.find(kacc);
    if(it == g_tunnels.end()){
      std::cout << " tunnel not exist. Ingore send exit request.\n";
      ret = DATA_ERROR;
      return ret;
    }
    int kdesk_port = it -> second;
    std::cout << " tunnel port " << kdesk_port << std::endl;

    //
    Socket client_socket;
    if ( ! client_socket.Create() ) {
      throw SocketException ( "Could not create client socket." );
    }

    if ( ! client_socket.Connect(g_docker_bridge_host, kdesk_port)) {
      throw SocketException ( "Could not connect to host." );
    }

    Message req(Header(MT_COMMAND, g_kpserver_port, kdesk_port, 0, CMD_REMOVETUNNEL), NULL);
    // Send request
    if(client_socket.Send(req)){
      g_tunnels.erase(it);
      std::cout << "Send remove tunnel request success\n";
    } else {
      std::cout << "Send remove tunnel request fail\n";
      ret = DATA_ERROR;
    }
  } catch ( SocketException& e ) {
    std::cout << "[ERR] SendExitReq was caught:" << e.description() << "\n";
    ret = DATA_ERROR;
  }
  return ret;
}
