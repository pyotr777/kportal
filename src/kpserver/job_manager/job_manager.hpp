#ifndef JOB_MANAGER_HPP
#define JOB_MANAGER_HPP
#include <map>
#include "job.hpp"
#include "../ResponeInterface.hpp"

class JobManager {
private:

public:
    JobManager();
    ResponseCode getJobsOfUserByService(std::string userEmail, std::string serviceId, std::vector<Job> &listJobFind);
    ResponseCode getListEntryOfJob(std::string jobId,std::string curDir, std::map<string, bool> &listEntry);
    ResponseCode Remove(const std::string& job_id);
    ResponseCode UpdateJobStatus(const std::string& job_id, const std::string& pjm_job_id, const std::string& pjm_status, const std::string& pjm_start_date, const std::string& pjm_duration_time, bool& isFinished);
    ResponseCode UpdateJobStatusErr(const std::string& job_id);
    ResponseCode UpdateProviderUsageTime(const std::string provider_email, const double seconds);
};

#endif
