#ifndef RES_UTILS_HPP
#define RES_UTILS_HPP

#include <string>
#include <list>

#include "../include/libjson/libjson.h"

#include "Utils/TagKeys.hpp"
#include "KP_Service.hpp"
#include "KP_Job.hpp"
#include "IdRes.hpp"


class ResUtils
{
public:
	static std::list<KP_Service > ParseServicesResponse(JSONNode &n);
    static void ParseJobsResponse(std::list<KP_Job>& jobs, JSONNode &n);
    static KP_Service* ParseServiceResponse(JSONNode::const_iterator& i,JSONNode &n);
	static int ParseNumOfPacketsResponse(JSONNode &n);
    static KP_Job * ParseJobResponse(JSONNode &n);
    static IdRes* ParseIdResponse(JSONNode &n);//, bool isOnlyId = false);
    static int ParseStatusResponse(JSONNode &n, JSONNode::const_iterator& i, int &outStatus);
};

#endif //RES_UTILS_HPP
