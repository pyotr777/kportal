#include "job_manager.hpp"
#include "../stdafx.h"
#include "../timer_utils.hpp"
extern std::map<std::string,Job> listJob;
//extern pthread_mutex_t g_listjobs_lock;
extern std::string DEFAULT_JOB_FOLDER;
JobManager::JobManager() {

}

ResponseCode JobManager::getJobsOfUserByService(std::string userEmail, std::string serviceId, std::vector<Job> &listJobFind) {
  //pthread_mutex_lock(&g_listjobs_lock);
  for (std::map<std::string,Job>::iterator it=listJob.begin(); it!=listJob.end(); ++it) {
    Job* tempJob = &(it->second);
    if(tempJob->getUserEmail().compare(userEmail) == 0 && tempJob->getServiceId().compare(serviceId) == 0) {
      tempJob->getStat();
      listJobFind.push_back(*tempJob);
    }
  }
  //pthread_mutex_unlock(&g_listjobs_lock);
  return DATA_SUCCESS;
}

ResponseCode JobManager::getListEntryOfJob(std::string jobId,std::string curDir, std::map<string, bool> &listEntry) {
	std::cout << "getListEntryOfJob \n";
	std::stringstream ss;
	std::string containerId, host, userEmail;
	ss << DEFAULT_JOB_FOLDER << PATH_SEPARATOR << jobId << PATH_SEPARATOR << curDir;
	std::string pathDir = ss.str();

	ResponseCode ret = FILE_ACTION_SUCCESS;
	DataManager data_manager(PATH_OF_DATABASE);
	if ( data_manager.connectDB() == DATA_SUCCESS) {

    //pthread_mutex_lock(&g_listjobs_lock);
		Job* jobInfo =  &listJob[jobId];
		userEmail = jobInfo->getUserEmail();
    //pthread_mutex_unlock(&g_listjobs_lock);
		User user(userEmail, USER_GROUP);
		ret = data_manager.getUser(user);

		if ( ret == DATA_SUCCESS) {
			containerId = user.getContainerId();
			ret = NetworkUtil::getIpAddressOfContainer(containerId, host);
		}


		if (ret == REQUEST_SUCCESS) {
      std::cout << "Connect to master get entry: " << containerId << std::endl;
			ret = NetworkUtil::ProcessGetListEntry(host, Container_Com_Port, pathDir, listEntry);
			if (ret == FILE_ACTION_SUCCESS) {
				for (std::map<string, bool>::iterator it=listEntry.begin(); it!=listEntry.end(); ++it) {
					std::string startFile, stopFile, infoFile;
					startFile = jobId + ".sh";
					stopFile = "stop_" + jobId + ".sh";
					infoFile = jobId + ".info";
					if (it->first.compare(startFile) == 0 || it->first.compare(stopFile) == 0 || it->first.compare(infoFile) == 0) {
						listEntry.erase(it);
					}
				}
			}
		}
		data_manager.disConnectDB();
  }
	return ret;
}

ResponseCode JobManager::Remove(const std::string& job_id){
  std::cout << "JobManager::Remove \n";
  ResponseCode ret;
  //pthread_mutex_lock(&g_listjobs_lock);
  std::map<std::string,Job>::iterator it = listJob.find(job_id);
  if (it != listJob.end()) {
    listJob.erase(it);
    std::cout << "remove job ok \n";
  } else {
    std::cout << "remove job in listJob \n";
    ret = DATA_SELECT_EMPTY;
  }
  //pthread_mutex_unlock(&g_listjobs_lock);
}

ResponseCode JobManager::UpdateJobStatusErr(const std::string& job_id){
  std::cout << "JobManager::UpdateJobStatusErr \n";
  //pthread_mutex_lock(&g_listjobs_lock);
  std::map<std::string,Job>::iterator it = listJob.find(job_id);
  if (it != listJob.end()) {
    Job* job_info = &it -> second;
    job_info->setStatus(JOB_ERROR);
    job_info->setDuration("");
    job_info->TimerEnd();
  }
  //pthread_mutex_unlock(&g_listjobs_lock);
}

ResponseCode JobManager::UpdateJobStatus(const std::string& job_id, const std::string& pjm_job_id,
                                         const std::string& pjm_status, const std::string& pjm_start_date,
                                         const std::string& pjm_duration_time, bool& isFinished){
  std::cout << "JobManager::UpdateJobStatus \n";
  ResponseCode ret = DATA_SUCCESS;
  isFinished = false;
  //pthread_mutex_lock(&g_listjobs_lock);
  std::map<std::string,Job>::iterator it = listJob.find(job_id);
  if (it != listJob.end()) {
    Job* job_info =  &it -> second;;
    KP_JobStatus status =  job_info->ConvertKdeskJobState(pjm_status);
    job_info->setStatus(status);
    if(status == JOB_RUNNING){
      job_info->setTimeStartJob(pjm_start_date);
      job_info->TimerStart();
    }
    std::cout << "kp job status " << status << std::endl;
    isFinished = (status == JOB_FINISH || status == JOB_ERROR || status == JOB_UNKNOW);
    if(isFinished){
      job_info->setDuration(pjm_duration_time);
      job_info->TimerEnd();
    } else {
      if(pjm_status != "RUN"){
        job_info->setDuration(pjm_duration_time);
        job_info->TimerEnd();
      }
    }
    std::cout << "udpate status job ok \n";

    if(status == JOB_FINISH){
      std::cout << "calculate enddate from: start " << pjm_start_date << ", duration " << pjm_duration_time;
      std::string end_date = TimeUtils::GetEndDate(pjm_start_date, pjm_duration_time);
      std::cout<< "=> end date = " << std::endl;
      job_info->setTimeEndJob(end_date);

      std::cout << "update usage time of provider. " << job_info->getProviderEmail() << std::endl;
      std::cout << "job_id = " << job_id << ", elapsed time = " << pjm_duration_time << std::endl;
      double dur_secs = TimeUtils::TimeStringToSeconds(pjm_duration_time);
      std::cout << "elapsed time str to int: " << dur_secs << std::endl;
      if(dur_secs >= 0){
        ret = UpdateProviderUsageTime(job_info->getProviderEmail(), dur_secs);
        if(ret == DATA_SUCCESS){
          std::cout << "update time usage success\n";
        } else {
          std::cout << "update time usage fail\n";
        }
      } else {
        std::cout << "time durration invalid\n";
      }
    }
  } else {
    std::cout << "job id " << job_id << " not found in listJob \n";
    ret = DATA_SELECT_EMPTY;
  }
  //pthread_mutex_unlock(&g_listjobs_lock);
  return ret;
}

ResponseCode JobManager::UpdateProviderUsageTime(const std::string provider_email, const double seconds){
  std::cout << "JobManager::UpdateProviderUsageTime " << provider_email << ", " << seconds << std::endl;
  //pthread_mutex_lock(&g_listjobs_lock);
  ResponseCode ret = DATA_SUCCESS;
  DataManager data_manager(PATH_OF_DATABASE);
  if ( data_manager.connectDB() == DATA_SUCCESS) {
    //insert provider
    if ((ret = data_manager.beginTransaction()) == DATA_SUCCESS) {
      User tmp(provider_email, PROVIDER_GROUP);
      data_manager.getUserDb(tmp);

      std::cout << "current usage time" << tmp.getTimeUsage() << ", ";
      tmp.setTimeUsage((tmp.getTimeUsage() * 1000000 + seconds)/ 1000000);
      std::cout << "new = " << tmp.getTimeUsage() << "\n";
      ret = data_manager.updateUser(tmp);
      if(ret == DATA_SUCCESS){
        std::cout << "update success\n";
      } else {
        std::cout << "update fail\n";
        ret = ERROR_UPDATE_PROVIDER;
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
  //pthread_mutex_unlock(&g_listjobs_lock);
  return ret;
}

