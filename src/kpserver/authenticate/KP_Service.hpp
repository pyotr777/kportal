#ifndef KP_SERVICE_HPP
#define KP_SERVICE_HPP
#include<string>
#include "../datalayer/parameter.h"
using namespace std;
class KP_Service
{
public:
    KP_Service() {

    }

    KP_Service(std::string serviceId,
               std::string serviceName,
               std::string producer,
               bool        status,
               int         numberOfNode,
               int         numberOfUser,
               int         numberOfWaitUser,
               std::string description,
               std::string pathExcuteFile,
               std::string pathShFile,
               std::string pathPreShFile,
               std::string pathPostShFile,
               std::string providerId,
               unsigned int timeElapse)
	{
        this -> serviceId = serviceId;
        this -> serviceName = serviceName;
        this -> producer = producer;
        this -> status = status;
        this -> numberOfNode = numberOfNode;
        this -> numberOfUser = numberOfUser;
        this -> numberOfWaitUser = numberOfWaitUser;
        this -> description = description;
        this -> pathExcuteFile = pathExcuteFile;
        this -> pathPostShFile = pathPostShFile;
        this -> pathPreShFile = pathPreShFile;
        this -> pathShFile = pathShFile;
        this -> providerId = providerId;
        this -> maxElapseTime = timeElapse;
    }

    KP_Service(const KP_Service& copy)
    {
        this -> serviceId = copy.serviceId;
        this -> serviceName = copy.serviceName;
        this -> producer = copy.producer;
        this -> status = copy.status;
        this -> numberOfNode = copy.numberOfNode;
        this -> numberOfUser = copy.numberOfUser;
        this -> numberOfWaitUser = copy.numberOfWaitUser;
        this -> description = copy.description;
        this -> pathExcuteFile = copy.pathExcuteFile;
        this -> pathShFile = copy.pathShFile;
        this -> pathPostShFile = copy.pathPostShFile;
        this -> pathPreShFile = copy.pathPreShFile;
        this -> providerId = copy.providerId;
        this -> maxElapseTime = copy.maxElapseTime;
        this -> act = copy.act;
        this -> listParam = copy.listParam;
    }
public:
    std::string serviceId;
    std::string serviceName;
    std::string producer;
    bool        status;
    int         numberOfNode;
    int         numberOfUser;
    int         numberOfWaitUser;
    std::string description;
    std::string pathExcuteFile;
    std::string pathShFile;
    std::string pathPostShFile;
    std::string pathPreShFile;
    std::string providerId;
    std::string iconStr;
    unsigned int maxElapseTime;
    std::vector<Parameter> listParam;
    std::vector<Parameter> getListParam(){return listParam;}

    action      act;
};

#endif //KP_SERVICE_HPP

