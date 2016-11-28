#ifndef KP_SERVICE_HPP
#define KP_SERVICE_HPP

#include <string>

#include "Exceptions/KP_Exception.hpp"
#include "KP_Job.hpp"
#include "KP_ParametersList.hpp"

class KP_Service
{
private:
    void * _man;
    std::string _serviceId;
    std::string _providerId;

public:
    KP_Service(std::string providerId, std::string serviceId);
    ~KP_Service();
    KP_Service(const KP_Service& copy);
    KP_Job * submitJob(std::string methodId, KP_ParametersList * params);
    bool getOutputFile(std::string& jobId, std::string& remoteFile, std::string& localPath) throw (KP_Exception*);
    void setMan(void* man) {  this -> _man = man;  }
    void setServiceId(std::string serviceId) { this -> _serviceId = serviceId;  }
    void setProviderId(std::string providerId) { this -> _providerId = providerId;  }
    void * getMan() { return _man; }
    std::string getServiceId() { return _serviceId; }
    std::string getProviderId() { return _providerId; }
};


#endif //KP_SERVICE_HPP

