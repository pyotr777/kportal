#ifndef SERVICE_HPP
#define SERVICE_HPP
#include "../datalayer/parameter.h"

class Service {
private:
    std::string serviceID;
    std::string serviceName;
    std::string producer;
    bool        status;
    int         numberOfUser;
    int         numberOfWaitUser;
    std::string description;
    std::string pathExcuteFile;
    std::string image_id;
    std::string providerEmail;
    std::string iconStr;
    int         act;
    int         numberOfNode;
    int         maxElapseTime;
	std::vector<Parameter> listParam;
  std::vector<std::string> stageinDirs;
  std::vector<std::string> stageoutDirs;

public:
    Service();
    //Service(std::string id, std::string name, std::string prod, bool st, int nON, int nOU, int nOWU, std::string des, std::string path, std::string provId, unsigned int timeElapse);
	Service(std::string id, std::string name, std::string prod, bool st, int nOU, int nOWU, std::string des, std::string path, std::string imageId, std::string provId);
	Service(const Service &copy);
    ~Service();

    void setServiceID(std::string svID);
    std::string getServiceID();

    void setServiceName(std::string name);
    std::string getServiceName();

    void setProducer(std::string prod);
    std::string getProducer();

    void setStatus(bool st);
	bool getStatus();

    void setNumberOfUser(int n);
    int getNumberOfUser();

    void setNumberOfWaitUser(int n);
    int getNumberOfWaitUser();

    void setDescription(std::string des);
    std::string getDescription();

    void setPathExcuteFile(std::string path);
    std::string getPathExcuteFile();

    void setImageId(std::string imageId);
    std::string getImageId();

    void setProvider(std::string prov);
    std::string getProvider();

    void setIconStr(std::string prov);
    std::string getIconStr();

    void setAct(int);
    int getAct();

    std::vector<Parameter>* getListParam();

    void setStageinDirs(const std::vector<std::string>& dirs);
    std::vector<std::string>* getStageinDirs();

    void setStageoutDirs(const std::vector<std::string>& dirs);
    std::vector<std::string>* getStageoutDirs();

    void setMaxElapseTime(int met);
    int getMaxElapseTime();

    void setNumberOfNode(int non);
    int getNumberOfNode();
};

#endif // SERVICE_HPP
