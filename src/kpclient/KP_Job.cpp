#include "KP_Job.hpp"
#include "KP_Service.hpp"

KP_Job::~KP_Job()
{
    _status = JOB_UNKNOW;
    if(_service != NULL)
    {
        KP_Service* service = (KP_Service*)_service;
        _service = NULL;
        delete service;
        service = NULL;
    }
}

bool KP_Job::getOutputFile(std::string& remoteFile, std::string& localPath) throw (KP_Exception*)
{
    bool result = false;
    if(_service != NULL)
    {
        result = ((KP_Service*) _service)->getOutputFile(this -> _jobId, remoteFile, localPath);
    }
    return result;
}

KP_Job::KP_Job(const KP_Job & copy)
{
    this -> _jobId = copy._jobId;
    this -> _status = copy._status;
    if(copy._service != NULL)
    {
        this -> _service = new KP_Service(*((KP_Service *)copy._service));
    }
    else
        this -> _service = NULL;
}

KP_Job & KP_Job::operator=(const KP_Job & copy)
{
    this -> _jobId = copy._jobId;
    this -> _status = copy._status;
    if(copy._service != NULL)
    {
        KP_Service * service = (KP_Service *)copy._service;
        this -> _service = new KP_Service(*service);
    }
    else
        this -> _service = NULL;
}
