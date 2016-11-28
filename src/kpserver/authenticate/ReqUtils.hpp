#ifndef REQ_UTILS_HPP
#define REQ_UTILS_HPP

#include <string>
#include <sstream>
#include <dirent.h>

#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <time.h>

#include "ReqType.hpp"
#include "../datalayer/user.hpp"

class ReqUtils
{
public:
    static RequestType GetID(std::string name);
    static void GenRequestId(std::string & pre, std::string & id);
    static bool CheckPermissionRequest(const RequestType& req, const TypeUser& usr);
};

#endif //REQ_UTILS_HPP
