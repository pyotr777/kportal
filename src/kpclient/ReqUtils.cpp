#include "ReqUtils.hpp"

ReqUtils::ReqUtils()
{

}

void ReqUtils::GenRequestId(std::string & id)
{
    id = "";

    time_t rawtime;

    struct tm * timeinfo;

    char buffer [80];

    time ( &rawtime );

    timeinfo = localtime ( &rawtime );

    strftime (buffer,80,"%Y%m%d%H%M%S",timeinfo);

    cout << "RequestId:" << buffer << endl;

    id = string(buffer);
}
