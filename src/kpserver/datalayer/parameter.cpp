#include "parameter.h"

action convertStrToAct(std::string ac_str) {
    if (ac_str.compare("newparam") == 0) {
        return CREAT;
    } else if (ac_str.compare("editparam") == 0) {
        return EDIT;
    } else if (ac_str.compare("deleteparam") == 0) {
        return DELETE;
    } else {
        return NOACTION;
    }
}

Parameter::Parameter() {
    paramId = "";
    isOutPut = false;
    isRequired = false;
    optionName = "";
    description = "";
    type = "";
}

Parameter::Parameter(std::string id, bool output, bool require, std::string opName, std::string des, std::string t) {
    paramId = id;
    isOutPut = output;
    isRequired = require;
    optionName = opName;
    description = des;
    type = t;
}

Parameter::~Parameter() {

}

void Parameter::setParamID(std::string prID) {
    paramId = prID;
}

std::string Parameter::getParamID() {
    return paramId;
}

void Parameter::setServiceID(std::string svID) {
    serviceId = svID;
}

std::string Parameter::getServiceID() {
    return serviceId;
}

void Parameter::setIsOutput(bool output) {
    isOutPut = output;
}

bool Parameter::getIsOutput() {
    return isOutPut;
}

void Parameter::setIsRequired(bool require) {
    isRequired = require;
}

bool Parameter::getIsRequired() {
    return isRequired;
}

void Parameter::setOptionName(std::string opName) {
    optionName = opName;
}

std::string Parameter::getOptionName() {
    return optionName;
}

void Parameter::setDescription(std::string des) {
    description = des;
}

std::string Parameter::getDescription() {
    return description;
}

void Parameter::setType(std::string t) {
    type = t;
}

std::string Parameter::getType() {
    return type;
}

void Parameter::setName(std::string na) {
    name = na;
}

std::string Parameter::getName() {
    return name;
}

void Parameter::setDefaultValue(std::string dfValue) {
    defaultValue = dfValue;
}

std::string Parameter::getDefaultValue() {
    return defaultValue;
}

void Parameter::setAct(action ac) {
    act = ac;
}

action Parameter::getAct() {
    return act;
}
