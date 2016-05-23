#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP
#include <string>
#include <stdio.h>
#include <sys/stat.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstring>

#include "../ResponeInterface.hpp"
#include "user.hpp"
#include "service.hpp"
#include "parameter.h"
#include "user_service.hpp"
#include "image_docker.hpp"
#include "sqlite3.h"
#define PATH_OF_DATABASE "/etc/kportal/kportal.db"
//#define MAX_STRING 100
class DataManager {
private:
    sqlite3 *db;
    std::string dbFilePath;
    bool isConnect;
    //bool changeLock;
    //function
    ResponseCode createDB();
    bool    dbIsExist();

public:
    DataManager();
    DataManager(std::string pathFile);
    ~DataManager();
    void setDBPath(std::string pathFile);
    ResponseCode connectDB();
    ResponseCode disConnectDB();

    ResponseCode beginTransaction();
    ResponseCode endTransaction();
    ResponseCode rollback();

    ResponseCode insertUserDb(User &user);
    ResponseCode updateUserDb(User &user);
    ResponseCode deleteUserDb(User &user);
    ResponseCode getUserDb(User &user);

    ResponseCode insertUser(User &user);
    ResponseCode updateUser(User &user);
	ResponseCode deleteUser(User &user);

    ResponseCode getAllUser(std::vector<User> &listUser);
    ResponseCode getUser(User &user);
    ResponseCode getAllAdmin(std::vector<User> &listAdmin);
    ResponseCode getAdmin(User &admin);
    ResponseCode getAllProvider(std::vector<User> &listProvider);
    ResponseCode getProvider(User &provider);
    ResponseCode getAllWaitProviders(std::vector<User> &listWaitProvides);

    ResponseCode insertService(Service &sv);
    ResponseCode updateService(Service &sv);
    ResponseCode deleteService(Service &sv);
    ResponseCode getAllService(std::vector<Service> &listService);
    ResponseCode getAllServiceOfProvider(User &provider, std::vector<Service> &listService);
    ResponseCode getAllServiceOfProviderHaveCandidate(User &provider, std::vector<Service> &listService);
    ResponseCode getInfoOfService(Service &sv);
    ResponseCode checkPathExcute(std::string pathExe, bool &result);
//    ResponseCode getServicesByPathExcute(Service &sv, std::vector<Service> &listService);

    ResponseCode insertParam(Parameter &param);
    ResponseCode updateParam(Parameter &param);
    ResponseCode deleteParam(Parameter &param);
    ResponseCode getParamOfService(Service &sv, std::vector<Parameter> &listParam);

    ResponseCode insertUserService(User_Service &us);
    ResponseCode updateUserService(User_Service &us);
    ResponseCode deleteUserService(User_Service &us);
    ResponseCode getInfoOfUserService(User_Service &us);
    ResponseCode getAllReadyUserOfService(std::string serviceId, std::vector<std::string> &listUser);
    ResponseCode getAllWaitUserOfService(std::string serviceId, std::vector<std::string> &listWaitUser);
    ResponseCode getAllReadyServiceOfUser(std::string userEmail, std::vector<Service> &listReadyService);
    ResponseCode getAllWaitServiceOfUser(std::string userEmail, std::vector<Service> &listWaitService);
	ResponseCode checkNameService(std::string name);
    ResponseCode getAllServiceByImage(std::string imageId, std::vector<Service> &listService);

    ResponseCode insertImageDocker(ImageDocker &imgDocker);
    ResponseCode updateImageDocker(ImageDocker &imgDocker);
    ResponseCode deleteImageDocker(ImageDocker &imgDocker);
	ResponseCode getAllImageDockerOfProvider(std::string providerEmail, std::vector<ImageDocker>& imgDocker);
	ResponseCode checkIdImageDocker(std::string id);
	ResponseCode checkNameImageDocker(std::string name);
	ResponseCode getImageDockerById(std::string id, ImageDocker &outImageDocker);
	ResponseCode getImageDockerByName(std::string name, ImageDocker &outImageDocker);
	ResponseCode getAllPublicImageDocker(std::vector<ImageDocker> &listImages);

};

#endif
