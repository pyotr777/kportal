#ifndef IMAGE_DOCKER_HPP
#define IMAGE_DOCKER_HPP

class ImageDocker {
private:
    std::string image_id;
    std::string image_name;
    std::string provider_email;
    bool is_public;
    std::string comment;
    std::string tarStr;

public:
    ImageDocker();
	ImageDocker(std::string id, std::string name, std::string prov_email, bool isp, std::string com);
    ~ImageDocker();
    void setImageId(std::string id);
    std::string getImageId();

    void setImageName(std::string name);
    std::string getImageName();

    void setProviderEmail(std::string email);
    std::string getProviderEmail();

    void setIsPublic(bool isp);
    bool getIsPublic();

    void setComment(std::string com);
    std::string getComment();

    void setTarStr(std::string tarStr);
    std::string getTarStr();
};
#endif //IMAGE_DOCKER_HPP
