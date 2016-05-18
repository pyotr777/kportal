#include "KP_ParametersList.hpp"

KP_ParametersList::KP_ParametersList(){}

KP_ParametersList::~KP_ParametersList()
{
    _parameters.clear();
}


void KP_ParametersList::add(int i, std::string name)
{
    KP_Parameter par = KP_Parameter(i, name);
    _parameters.push_back(par);
}

void KP_ParametersList::add(long i, std::string name)
{
    KP_Parameter par = KP_Parameter(i, name);
    _parameters.push_back(par);
}

void KP_ParametersList::add(float i, std::string name)
{
    KP_Parameter par = KP_Parameter(i, name);
    _parameters.push_back(par);
}

void KP_ParametersList::add(double i, std::string name)
{
    KP_Parameter par = KP_Parameter(i, name);
    _parameters.push_back(par);
}

void KP_ParametersList::add(string i, std::string name)
{
    KP_Parameter par = KP_Parameter(i, name);
    _parameters.push_back(par);
}

void KP_ParametersList::add(string name)
{
    KP_Parameter par = KP_Parameter(name);
    _parameters.push_back(par);
}

void KP_ParametersList::add(KP_Entry &entry, std::string name)
{
    KP_Parameter par = KP_Parameter(entry, name);
    _parameters.push_back(par);
}

std::list<KP_Parameter> * KP_ParametersList::getParameters()
{
    return &_parameters;
}
