#include "../stdafx.h"
#include "image_docker.hpp"

ImageDocker::ImageDocker(){
    image_id = "";
    image_name = "";
    provider_email = "";
    is_public = false;
    comment = "";
}

ImageDocker::ImageDocker(std::string id, std::string name, std::string prov_email, bool isp, std::string com) {
    image_id = id;
    image_name = name;
    provider_email = prov_email;
    is_public = isp;
    comment = com;
}

ImageDocker::~ImageDocker() {

}

void ImageDocker::setImageId(std::string id) {
    image_id = id;
}

std::string ImageDocker::getImageId() {
    return image_id;
}

void ImageDocker::setImageName(std::string name) {
    image_name = name;
}

std::string ImageDocker::getImageName() {
    return image_name;
}

void ImageDocker::setProviderEmail(std::string email) {
    provider_email = email;
}

std::string ImageDocker::getProviderEmail() {
    return provider_email;
}

void ImageDocker::setIsPublic(bool isp) {
    is_public = isp;
}

bool ImageDocker::getIsPublic() {
    return is_public;
}

void ImageDocker::setComment(std::string com) {
    comment = com;
}

std::string ImageDocker::getComment() {
    return comment;
}

void ImageDocker::setTarStr(std::string ts) {
    tarStr = ts;
}

std::string ImageDocker::getTarStr() {
    return tarStr;
}

