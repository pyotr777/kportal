#include <iostream>
#include "ResUtils.hpp"

using namespace std;

#define EMPTY_STR ""

void ParseJSON(const JSONNode & n, std::list<KP_Service > & services){
    
    std::string providerId, serviceId;
	bool isHasServiceId = false, isHasProviderId = false;

	JSONNode::const_iterator i = n.begin();
    while (i != n.end()){
        // recursively call ourselves to dig deeper into the tree
        if (i -> type() == JSON_ARRAY || i -> type() == JSON_NODE){
            ParseJSON(*i, services);
        }
 		
        // get the node name and value as a string
        std::string node_name = i -> name();
 		//std::cout<< "Name: "<< node_name << endl;
        // find out where to store the values
        if (node_name == TAG_SERVICE_STR){
	        isHasServiceId = true;
            serviceId = i -> as_string();
        }
        else if (node_name == TAG_PROVIDER_STR){
        	isHasProviderId = true;
            providerId = i -> as_string();
        }
        
        //increment the iterator
        ++i;
    }
    if(isHasServiceId && isHasProviderId)
	    services.push_back(KP_Service (providerId, serviceId));
}

std::list<KP_Service> ResUtils::ParseServicesResponse(JSONNode &n)
{
	std::list<KP_Service > services;
	ParseJSON(n, services);
	for (std::list<KP_Service >::iterator it = services.begin(); it != services.end(); it++)
	{
        std::cout << "ProviderId: "<< ((KP_Service)*it).getProviderId() << ", ServiceId:" << ((KP_Service )*it).getServiceId() << endl;
	}
	
	return services;
}

void ResUtils::ParseJobsResponse(std::list<KP_Job>& jobs, JSONNode &n)
{
    std::string jobId;
    JSONNode::const_iterator i = n.begin();
    if (i != n.end() && i -> name() == TAG_JOBS_STR && i -> type() == JSON_NODE)
    {
        JSONNode::const_iterator ijs = (*i).begin();
        if (ijs != n.end() && ijs -> name() == TAG_ITEMS_STR && ijs -> type() == JSON_ARRAY)
        {
            JSONNode::const_iterator ij = (*ijs).begin();
            while (ij != (*ijs).end())
            {
                if(ij -> type() == JSON_ARRAY || ij -> type() == JSON_NODE)
                {
                    JSONNode::const_iterator ia = (*ij).begin();
                    if (ia != (*ij).end() && ia -> name() == TAG_ID_STR && ia -> type() != JSON_ARRAY && ia -> type() != JSON_NODE)
                    {
                        jobId = ia -> as_string();
                        //cout << "Bp: jobId: " << jobId << endl;
                        jobs.push_back(KP_Job (jobId));
                    }
                }
                ij ++;
            }
        }
    }

}

KP_Service* ResUtils::ParseServiceResponse(JSONNode::const_iterator& i, JSONNode &n)
{
	std::string serviceId, providerId;
	bool isHasSeviceId = false, isHasProviderId = false;
	KP_Service* service = NULL;
	
    //JSONNode::const_iterator i = n.begin();
    //++i;
    if (i != n.end() && i -> name() == TAG_SERVICE_STR && i -> type() != JSON_NODE && i -> type() != JSON_ARRAY)
    {
        isHasSeviceId = true;
        serviceId = i -> as_string();
    }
    ++ i;
    if (i != n.end() && i -> name() == TAG_PROVIDER_STR && i -> type() != JSON_NODE && i -> type() != JSON_ARRAY)
	{
		isHasProviderId = true;
		providerId = i -> as_string();
    }
	//cout<< "Service: serviceId = "<< serviceId << ", providerId = " << providerId << endl;
	
    if(isHasSeviceId && isHasProviderId)
	    service = new KP_Service(providerId, serviceId);
	return service;
}

/*  { “job”: jobID, “status”: status }  */
KP_Job * ResUtils::ParseJobResponse(JSONNode &n)
{
    KP_Job *job = NULL;

    std::string jobId;
    KP_JobStatus status = JOB_UNKNOW;
    bool isHasOwner = false, isHasJobId = false, isHasStatus = false;
    JSONNode::const_iterator i = n.begin();
    if (i != n.end() && i -> name() == TAG_OWNER_STR && i -> type() != JSON_NODE && i -> type() != JSON_ARRAY)
    {
        isHasOwner = true;
    }
    ++ i;
    if (i != n.end() && i -> name() == TAG_JOB_STR && i -> type() != JSON_NODE && i -> type() != JSON_ARRAY)
    {
        isHasJobId = true;
        jobId = i -> as_string();
    }
    ++ i;
    if (i != n.end() && i -> name() == TAG_STATUS_STR && i -> type() != JSON_NODE && i -> type() != JSON_ARRAY)
    {
        isHasStatus = true;
        status = (KP_JobStatus)i -> as_int();
    }
    if(!isHasOwner)
        return job;

    if(isHasJobId && isHasStatus)
        job = new KP_Job(jobId, status);
    else if(isHasJobId)
    {
        status = JOB_UNKNOW;
        job = new KP_Job(jobId, status);
    }
    return job;

}

/* { “id”: requestID, “finish”: 1 } */
/* { “id”: requestID } */
IdRes*  ResUtils::ParseIdResponse(JSONNode &n)//, bool isOnlyId)
{
    string owner = "", data = "", path = "", ack = "";
    int result;
    bool isHasOwner = false, isHasFinish = false, isHasData = false, isHasPath = false, isHasAck;
    IdRes * idRes;

    JSONNode::const_iterator i = n.begin();
    if (i != n.end() && i -> name() == TAG_OWNER_STR && i -> type() != JSON_NODE && i -> type() != JSON_ARRAY)
    {
        isHasOwner = true;
        owner = i -> as_string();
    }

    ++ i;
    if (i != n.end() && i -> name() == TAG_FINISH_STR && i -> type() != JSON_NODE && i -> type() != JSON_ARRAY)
    {
        isHasFinish = true;
        result = i -> as_int();
    }
    else if (i != n.end() && i -> name() == TAG_DATA_STR && i -> type() != JSON_NODE && i -> type() != JSON_ARRAY)
    {
        isHasData = true;
        data = i -> as_string();
    }
    else if (i != n.end() && i -> name() == TAG_PATH_STR && i -> type() != JSON_NODE && i -> type() != JSON_ARRAY)
    {
        isHasPath = true;
        path = i -> as_string();
    }
    else if (i != n.end() && i -> name() == TAG_ACK_STR && i -> type() != JSON_NODE && i -> type() != JSON_ARRAY)
    {
        isHasAck = true;
        ack = i -> as_string();
    }

    if(isHasOwner)
    {
        if(isHasFinish)
            idRes = new IdRes(owner, result);
        else if(isHasData)
            idRes = new IdRes(owner, data, IdRes::TYPE_DATA);
        else if(isHasPath)
            idRes = new IdRes(owner, path, IdRes::TYPE_PATH);
        else if(isHasAck)
            idRes = new IdRes(owner, ack, IdRes::TYPE_ACK);
    }
    return idRes;
}

int ResUtils::ParseStatusResponse(JSONNode &n, JSONNode::const_iterator& i,int &outStatus)
{
    bool result = false;
    //JSONNode::const_iterator i = n.begin();
    if (i != n.end() && i -> name() == TAG_STATUS_STR && i -> type() != JSON_NODE && i -> type() != JSON_ARRAY)
    {
        outStatus = i -> as_int();
        result = true;
    }
    return result;
}

