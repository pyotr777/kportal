#include "param_job.hpp"
param_job::param_job() {

}

param_job::param_job(std::string id, TYPE_PARAM_JOB t, std::string v) {
    param_id = id;
    type = t;
    value = v;
}

void param_job::setParamId(std::string id) {
    param_id = id;
}

std::string param_job::getParamId() {
    return param_id;
}

void param_job::setType(TYPE_PARAM_JOB t) {
    type = t;
}

TYPE_PARAM_JOB param_job::getType() {
    return type;
}

void param_job::setValue(std::string v) {
    value = v;
}

std::string param_job::getValue() {
    return value;
}

void param_job::setReferJobId(std::string r) {
    refer_job_id = r;
}

std::string param_job::getReferJobId() {
    return refer_job_id;
}
