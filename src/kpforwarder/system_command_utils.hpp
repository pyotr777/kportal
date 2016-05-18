#ifndef SYSTEM_COMMAND_UTILS_HPP
#define SYSTEM_COMMAND_UTILS_HPP

#include <string>
#include <iostream>
#include <cstdio>
#include <memory>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>    /* For O_RDWR */

class SystemCommandUtils
{
public:
  static std::string Exec(const char* cmd);
  static pid_t System(const char * command, int * infp, int * outfp);
};

#endif
