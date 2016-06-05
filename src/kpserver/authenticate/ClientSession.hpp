//HISTORY
//2013/09/06  EH0001  longnm  Add api for client

#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <time.h>

#include "BlockData.hpp"
#include "KP_Job.hpp"
#include "FileUtils.hpp"
#include "ClientSession.hpp"
#include "ssh_utils.hpp"
#include "Constants.hpp"
#include "../../include/libjson/libjson.h"
#include "../authenticate/TagKeys.hpp"
#include "StringUtils.hpp"
#include "GmailUtils.hpp"
#include "Constants.hpp"
#include "../ResponeInterface.hpp"
#include "../datalayer/DataManager.hpp"
#include "../job_manager/job.hpp"
//#include "../job/job_manager.h"
//#include "../../kpserver/ldap/service_manager.h"
#include "../datalayer/image_docker.hpp"
#include "../../slavedaemon/header.hpp"
#include "../../slavedaemon/message.hpp"
#include "../../slavedaemon/message_socket.h"
#include "../../slavedaemon/socket_exception.h"
#include "tunnel_manager.hpp"

using namespace std;

#define REQUEST_TIMEOUT_ACCOUNT_SECONDS 900

struct FileReceive{
  string requestId;
  string fileName;
};

class ClientSession
{
public:
  ClientSession();
  ~ClientSession();

  //std::list<KP_Service> getServices(std::string providerId );

  std::list<Job> getJobs(std::string msg);

  ResponseCode getService(Service &service);

  std::string getRunMethodsFile(std::string msg);

  void removeRunMethodsFileTemp(std::string& path);

  //KP_Job * getJob(string jobId);

  ResponseCode submitJob(Job &job);

  ResponseCode InitJob(JSONNode &n, Job &job);

  std::string getOutputFile(std::string jobId, std::string pathFile);

  ResponseCode deleteJob(Job job);

//  Job * restartJob(std::string jobId, std::string json);

  ResponseCode cancelJob(Job &job);

  //KP_JobStatus toKPJobStatus(KP_JobStatus status);

  //EH0001
  ResponseCode getAllServices(std::vector<Service> &services);
  ResponseCode getAllServicesOfProvider(std::string providerEmail, std::vector<Service> &services);
  ResponseCode registerService(std::string username, std::string serviceID);
  ResponseCode registerProvider(std::string phone, std::string address, std::string kdeskacc, std::string description);
  ResponseCode getAllCandidateUsersOfService(string serviceId,string providerId, vector<string> &listCandidate);
  vector<string> getAllUserOfService(string serviceId,string providerId);
  //vector<string> getServicesHaveCandidates(string providerId);
  ResponseCode getServicesHaveCandidates(std::string providerId, std::vector<Service> &list_Service);
  ResponseCode addUserToService(std::string serviceId, std::string userEmail);
  ResponseCode removeUserToService(string providerEmail,string serviceId,string userEmail);
  ResponseCode getServicesWaitToUseOfUser(string userEmail, std::vector<Service> &list_Service);
  ResponseCode removeRegisterServiceOfUser(string serviceId,string userEmail);
  ResponseCode removeCandidateFromService(string providerEmail,string serviceId,string userEmail);
  bool  checkTypeUserIsProvider(string userEmail);
  string  getProviderIDFromEmail(string email);
  ResponseCode getJobsOfUsersByService(string serviceId, string userEmail, vector<Job> &listJob);
  ResponseCode getListEntryOfJob(string jobId, string curDir,map<string, bool> &listEntry);
  //ResponseCode getDetailOfJob(string serviceId, string userEmail, KP_Job &kp_job);
  //KP_Job getJobFromJobManager(string jobId);
  ResponseCode getInfoOfJob(Job &job);
  bool removeServiceByUser(string serviceId,string userId);
  //EH0001 end

  //User's api
  ResponseCode getInfoUser(User &user);

  //Admins's api
  ResponseCode getAllWaitProviders(std::vector<User> &listWaitProvides);
  ResponseCode addProviders(std::vector<User> &listProvides);
  ResponseCode updateProviderTimeUsable(std::string email, double timeUsable);
  ResponseCode deleteWaitProviders(std::vector<User> &listProvides);
  ResponseCode getAllProviders(std::vector<User> &listProvides);

  //Provider's api
  ResponseCode getAllImageDockerOfProvider(std::vector<ImageDocker> &listImages);
  ResponseCode getImage(ImageDocker &image);
  ResponseCode getImageByName(std::string name, ImageDocker &image);
  ResponseCode createImage(ImageDocker &images);
  ResponseCode createImageContinue(ImageDocker &image);
  ResponseCode createImageContinue1(ImageDocker &image);
  ResponseCode updateImage(ImageDocker &images);
  ResponseCode deleteImage(ImageDocker &image);
  ResponseCode checkNameImage(std::string name);
  ResponseCode outProviderGroup();
  ResponseCode getPublicImageByKey(std::string key, std::vector<ImageDocker> &listImages);
  //ResponseCode getProviderTimeResource(std::string& unit, int& limit, int& usage, int& avaiable);

  ResponseCode getProviderTimeResource(const std::string& provider_email, std::string& unit, double& limit, double& usage, double& avaiable, double& limitkp, double& usagekp, double& avaiablekp, std::string& public_key);

  ResponseCode browsePathInsideImage(Service&, const unsigned char);
  ResponseCode checkValidInfo(Service &service, const unsigned char);
  ResponseCode createService(Service &service);
  ResponseCode updateService(Service &service);
  ResponseCode deleteService(Service &service);
  //ResponseCode deleteService(KP_Service &service);
  //ResponseCode addParameter(vector<Parameter> listParam);
  ResponseCode getAllParametersOfService(std::string serviceId, std::vector<Parameter> &listParam);
  ResponseCode getServicesOfUser(string userEmail, std::vector<Service> &list_Service);
  ResponseCode checkPathOfService(Service &kp_sv);
  ResponseCode checkNameService(std::string name);
  ResponseCode createServiceContinue();
  int enqueueFileReceive(std::string requestId, std::string path);
  int dequeueFileReceive(struct FileReceive & fr);
  bool checkRequestTimeout();
  ResponseCode saveTarFile(const char* data, std::string fileName);
  ResponseCode addNewUser(DataManager data_manager, std::string userEmail);
  ResponseCode createResourceForUser(User &user);

  //
  // SshTunnel manager
  //
  ResponseCode ConfigureSshTunnel(std::string serviceId);
  ResponseCode ConfigureForwarder(std::string serviceId);
  ResponseCode SendExitReq(std::string& provider_email);
public:
  KP_JobStatus queryStatus(string msg);
  void setConId(std::string conId) {  _conId = conId;  }
  std::string getConId()  {  return _conId;  }
  void setUserId(std::string userId) {  _userId = userId;  }
  std::string getUserId() {  return _userId;  }
  void setPwd(std::string pwd) {  _pwd = pwd;  }
  std::string getPwd() { return _pwd; }
  void setLogined(bool isLogined) { _isLogined = isLogined; }
  bool isLogined(){ return _isLogined; }

  bool isNeedRecieveFile() { return _isNeedRecieveFile; }
  void setNeedRecieveFile(bool isNeed) { _isNeedRecieveFile = isNeed; }

  std::string getSubmitJobMsg() { return _submitJobMsg; }
  void setSubmitJobMsg(std::string msg) { this -> _submitJobMsg = msg; }

  std::list<ofstream*>* getInputFiles()  {   return &_inputFiles;  }
  void setLocalFile(ofstream* of) { this -> _localFile = of; }
  ofstream * getLocalFile(){ return _localFile; }
  std::map<std::string,Job>* getRequestJobs(){ return & _requestJobs;  }
  std::map<std::string,Service>* getListServiceWaitInsert(){ return & listServiceWaitInsert;  }

  ResponseCode saveFileIcon(const char* data);

  void setHeader(MessageHeader &hdr);
  MessageHeader* getHeader();
  std::string getServiceId();

  ResponseCode getWorkStatus();
  void setWorkStatus(ResponseCode& ret);

  ImageDocker* getImageTmp();

  void setServiceTmp(Service* srv);
  Service* getServiceTmp();

  Job* getJobTmp();

  TypeUser getUserType();
  ResponseCode ConfigureSshTunnel(Job & job);
private:
  std::map<std::string, Job> _requestJobs;
  std::map<std::string, Service> listServiceWaitInsert;
  std::ofstream * _localFile;
  std::list<ofstream*> _inputFiles;
  /// EH0004
  std::queue<FileReceive> _fileReceives;
  /// EH0004 end
  bool _isNeedRecieveFile;
//  std::list<KP_Job> jobs;
  /// Buffer
  std::string _submitJobMsg;

  time_t _lastAction;
  MessageHeader* _header;
  Service* _service;
  ImageDocker* _image;
  Job* _job;
private:
  std::string _conId;
  std::string _userId;
  std::string _pwd;
  bool _isLogined;
  ResponseCode _state;
};

#endif

