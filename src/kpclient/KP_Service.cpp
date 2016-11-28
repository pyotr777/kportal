#include "KP_Service.hpp"
#include "KP_Manager.hpp"
#include <iostream>

KP_Service::KP_Service(std::string providerId, std::string serviceId)
{
    this -> _serviceId = serviceId;
    this -> _providerId = providerId;
}

KP_Service::KP_Service(const KP_Service & copy)
{
    this -> _serviceId = copy._serviceId;
    this -> _providerId = copy._providerId;
}

KP_Service::~KP_Service()
{}

KP_Job * KP_Service::submitJob(std::string methodId, KP_ParametersList * params)
{
    KP_Job * job = NULL;

    string *jobId = NULL;
    if(_man != NULL)
    {
        cout << "SubmitJob: serviceId:" << this-> _serviceId << ", methodId:" << methodId << endl;
        if(params != NULL)
        {
            jobId = ((KP_Manager*) _man) -> submitJob(this -> _serviceId, methodId, params);
        }
    }

    if(jobId != NULL && *jobId != "")
    {
        cout << "Bp: jobid:" << jobId << endl;
        KP_JobStatus status = JOB_START;
        job = new KP_Job(*jobId, status);
        job -> setService(this);
    }
    return job;
}


bool KP_Service::getOutputFile(std::string& jobId, std::string& remoteFile, std::string& localPath) throw (KP_Exception*)
{
    bool result;
    if(_man != NULL)
    {
        result = ((KP_Manager*) _man) -> getOutputFile(jobId, remoteFile, localPath);
    }
    return result;
}
