#ifndef KP_JOB_HPP
#define KP_JOB_HPP

#include "Exceptions/KP_Exception.hpp"

enum KP_JobStatus {
    JOB_UNKNOW,
    JOB_FINISH,
    JOB_RUNNING,
    JOB_START,
    JOB_SUSPEND,
    JOB_ERROR
};

class KP_Job
{
public:
    static std::string StatusToString(KP_JobStatus & status)
    {
        std::string statusStr = "";
        switch((int)status)
        {
        case JOB_FINISH:
            statusStr = "FINISH";
            break;

        case JOB_RUNNING:
            statusStr = "RUNNING";
            break;

        case JOB_SUSPEND:
            statusStr = "SUSPEND";
            break;
        case JOB_ERROR:
            statusStr = "ERROR";
            break;
        }
        return statusStr;
    }
public:
    KP_Job(): _jobId(""),
        _status(JOB_UNKNOW),
        _service(NULL)
    {}
    ~KP_Job();

    KP_Job(std::string & jobId, KP_JobStatus &status)
    {
        this -> _jobId = jobId;
        this -> _status = status;
        this -> _service = NULL;
    }

    KP_Job(std::string & jobId)
    {
        this -> _jobId = jobId;
        this -> _status = JOB_UNKNOW;
        this -> _service = NULL;
    }

    KP_Job(const KP_Job & copy);

    KP_Job & operator=(const KP_Job & copy);

    std::string getJobId(){ return _jobId; }

    KP_JobStatus getStatus(){ return _status; }

    bool getOutputFile(std::string& remoteFile, std::string& localPath) throw (KP_Exception*);

    void setService(void * service) { _service = service; }
private:
    std::string _jobId;
    KP_JobStatus _status;
    void * _service;
};

#endif // KP_JOB_HPP
