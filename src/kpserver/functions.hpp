#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <string>
#include "ResponeInterface.hpp"

void readConfigFile();
bool checkProcess(std::string nameProcess);
ResponseCode ReloadListJob();
ResponseCode StartMasterInstance();

#endif
