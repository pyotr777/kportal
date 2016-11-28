#ifndef JOB_HPP
#define JOB_HPP
#include <string>
#include <vector>
#include "../ResponeInterface.hpp"
#include "param_job.hpp"
#include "../authenticate/FileUtils.hpp"
#include "../authenticate/KP_Job.hpp"
#include "../../slavedaemon/header.hpp"
#include "../../slavedaemon/message.hpp"
#include "../../slavedaemon/message_socket.h"
#include "../../slavedaemon/socket_exception.h"
#include "../datalayer/service.hpp"
#include "timer.hpp"
class Job {
private:
  std::string job_id;
  std::string service_id;
  std::string user_email;
  std::string provider_email;
  std::string job_name;
  int     numberOfNode;
  unsigned int maxElapseTime;
  std::vector<param_job> listParam;
//	std::string FX10_job_id;
	std::string container_id;
  std::string pathJob;
  std::string timeStartJob;
  std::string timeEndJob;
  std::string duration;
  KP_JobStatus   status;
  std::string strJson_param;
  std::string strJsonOfJob;
	MessageHeader _header;
  std::string container_port;
  int tunnel_port;
  std::string kdesk_jid;
  std::string kdesk_jstate;
  std::string kdesk_start_date;
  std::string kdesk_duration_time;
  Timer timer;
  time_t startTime;
  bool timerStarted;

public:
  Job();
	Job(std::string id);

  void setId(std::string id);
  std::string getId();
  void setServiceId(std::string id);
  std::string getServiceId();
  void setUserEmail(std::string email);
  std::string getUserEmail();
  void setProviderEmail(std::string email);
  std::string getProviderEmail();
  void setJobName(std::string name);
  std::string getJobName();
  void setNumberOfNode(int n);
  int getNumberOfNode();
  void setMaxElapseTime(unsigned int t);
  unsigned int getMaxElapseTime();
	void setPathJob(std::string path);
  std::string getPathJob();
	void setTimeStartJob(std::string time);
  std::string getTimeStartJob();
	void setTimeEndJob(std::string time);
  std::string getTimeEndJob();
	void setDuration(std::string time);
  std::string getDuration();
  void TimerStart();
  void TimerEnd();
  ResponseCode updateDuration();
	void setStatus(KP_JobStatus st);
  KP_JobStatus getStatus();
  void setStrJsonParam(std::string str);
  std::string getStrJsonParam();
  void setStrJsonOfJob(std::string str);
	std::string getStrJsonOfJob();

  void addParam(param_job &pa);
  std::vector<param_job>* getParamJsonStr();

  void setContainerId(std::string id);
  std::string getContainerId();

  void setTunnelPort(int port);
  int getTunnelPort();

	MessageHeader getHeader();
	void setHeader(MessageHeader& header);

	ResponseCode getRemoteArg();

  ResponseCode generateShFile(Service& sv);
  ResponseCode GenerateSlaveDaemonShFile();
  ResponseCode CreateJobContainer();
  ResponseCode Init();
  ResponseCode Submit();
  ResponseCode Stop();
  ResponseCode Restart();
  ResponseCode getStat();
  ResponseCode Delete();
  ResponseCode Clear();
  //ResponseCode UpdateStatusJobKcom();
  void SaveEndDate(long duration);
  void SaveStartDate();

	void generateJsonStr(std::string &json_str);
  ResponseCode SendSubmitJobCommand(const std::string& _host, const std::string& _port, std::vector<std::string>& stgindirs);
  ResponseCode SendCreateForwarderConnectionCommand(int tunnel_port, std::string container_host, std::string container_port);
  KP_JobStatus ConvertKdeskJobState(const std::string& kd_jstate);
};

#endif
