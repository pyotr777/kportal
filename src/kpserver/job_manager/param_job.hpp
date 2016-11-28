#ifndef PARAM_JOB_HPP
#define PARAM_JOB_HPP
#include <string>
enum TYPE_PARAM_JOB {
    VALUE_ONLY,
    ARG_OUTPUT,
    ARG_LOCAL,
    ARG_REMOTE,
    ARG_UNKNOW
};

class param_job {
private:
    std::string param_id;
    TYPE_PARAM_JOB   type;
    std::string value;
    std::string refer_job_id;
public:
    param_job();
    param_job(std::string id, TYPE_PARAM_JOB t, std::string v);

    void setParamId(std::string id);
    std::string getParamId();
    void setType(TYPE_PARAM_JOB t);
    TYPE_PARAM_JOB getType();
    void setValue(std::string v);
    std::string getValue();
    void setReferJobId(std::string r);
    std::string getReferJobId();
};

#endif
