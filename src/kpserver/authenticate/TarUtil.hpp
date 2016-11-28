#ifndef TARUTIL_HPP
#define TARUTIL_HPP
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "../stdafx.h"

ResponseCode UpdateRepoTagOfTarball(std::string pathTarball, std::string repo, std::string tag, std::string &idImage);

#endif
