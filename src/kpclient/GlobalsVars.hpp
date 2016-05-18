#ifndef GLOBALSVARS_HPP
#define GLOBALSVARS_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <semaphore.h>
#include <map>

sem_t sem_conn;
sem_t sem_connect;
sem_t sem_getService;
sem_t sem_findServices;
sem_t sem_getRunMethodFile;
sem_t sem_getRunMethodFile2;
sem_t sem_submitJob;
sem_t sem_submitJob2;
sem_t sem_queryStatus;
sem_t sem_getOutputFile;
sem_t sem_jobList;
sem_t sem_deleteJob;
sem_t sem_restartJob;
sem_t sem_cancelJob;

std::map<std::string, sem_t> sems;


#endif // GLOBALSVARS_HPP
