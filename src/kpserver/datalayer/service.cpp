#include "../stdafx.h"
#include "service.hpp"

Service::Service() {
    serviceID = "";
    serviceName = "";
    producer = "";
    status = false;
    numberOfUser = 0;
    numberOfWaitUser = 0;
    description = "";
    pathExcuteFile = "";
    pathShFile = "";
    shTemplate = "";
    pathPostShFile = "";
    shPostCommand = "";
    shPreCommand = "";
    pathPreShFile = "";
    shTemplate = "";
    image_id = "";
    providerEmail = "";
    iconStr = "";
    act = 0;
}

Service::Service(std::string id, std::string name, std::string prod, bool st, int nOU, int nOWU, std::string des, std::string path, std::string shpath, std::string shprepath, std::string shpostpath, std::string imageId, std::string provEmail) {
    serviceID = id;
    serviceName = name;
    producer = prod;
    status = st;
    numberOfUser = nOU;
    numberOfWaitUser = nOWU;
    description = des;
    pathExcuteFile = path;
    pathShFile = shpath;
    pathPostShFile = shpostpath,
    pathPreShFile = shprepath,
    shTemplate = "";
    image_id = imageId;
    providerEmail = provEmail;
}

Service::Service(const Service& copy)
{
  this -> serviceID = copy.serviceID;
  this -> serviceName = copy.serviceName;
  this -> producer = copy.producer;
  this -> status = copy.status;
  this -> numberOfUser = copy.numberOfUser;
  this -> numberOfWaitUser = copy.numberOfWaitUser;
  this -> description = copy.description;
  this -> pathExcuteFile = copy.pathExcuteFile;
  this -> pathShFile = copy.pathShFile;
  this -> shTemplate = copy.shTemplate;
  this -> pathPostShFile = copy.pathPostShFile;
  this -> shPostCommand = copy.shPostCommand;
  this -> pathPreShFile = copy.pathPreShFile;
  this -> shPreCommand = copy.shPreCommand;
  this -> providerEmail = copy.providerEmail;
  this -> act = copy.act;
  this -> listParam = copy.listParam;
  this -> image_id = copy.image_id;
  this -> iconStr = copy.iconStr;
  this -> numberOfNode = copy.numberOfNode;
  this -> maxElapseTime = copy.maxElapseTime;
  this -> stageinDirs = copy.stageinDirs;
  this -> stageoutDirs = copy.stageoutDirs;
}

Service::~Service() {
}

void Service::setServiceID(std::string svID) {
    serviceID = svID;
}

std::string Service::getServiceID() {
    return serviceID;
}

void Service::setServiceName(std::string name) {
    serviceName = name;
}

std::string Service::getServiceName() {
    return serviceName;
}

void Service::setProducer(std::string prod) {
    producer = prod;
}

std::string Service::getProducer() {
    return producer;
}

void Service::setStatus(bool st) {
    status = st;
}

bool Service::getStatus() {
    return status;
}

void Service::setNumberOfUser(int n) {
    numberOfUser = n;
}

int Service::getNumberOfUser() {
    return numberOfUser;
}

void Service::setNumberOfWaitUser(int n) {
    numberOfWaitUser = n;
}

int Service::getNumberOfWaitUser() {
    return numberOfWaitUser;
}

void Service::setDescription(std::string des) {
    description = des;
}

std::string Service::getDescription() {
    return description;
}

void Service::setPathExcuteFile(std::string path) {
    pathExcuteFile = path;
}

std::string Service::getPathExcuteFile() {
    return pathExcuteFile;
}

void Service::setPathShFile(std::string path) {
    pathShFile = path;
}

std::string Service::getPathShFile() {
    return pathShFile;
}

void Service::setShTemplate(std::string content) {
    shTemplate = content;
}

std::string Service::getShTemplate() {
    return shTemplate;
}

void Service::setPathPostShFile(std::string path) {
    pathPostShFile = path;
}

std::string Service::getPathPostShFile() {
    return pathPostShFile;
}

void Service::setShPostCommand(std::string content) {
    shPostCommand = content;
}

std::string Service::getShPostCommand() {
    return shPostCommand;
}

void Service::setPathPreShFile(std::string path) {
    pathPreShFile = path;
}

std::string Service::getPathPreShFile() {
    return pathPreShFile;
}

void Service::setShPreCommand(std::string content) {
    shPreCommand = content;
}

std::string Service::getShPreCommand() {
    return shPreCommand;
}

void Service::setImageId(std::string imageId) {
    image_id = imageId;
}

std::string Service::getImageId() {
    return image_id;
}

void Service::setProvider(std::string prov) {
    providerEmail = prov;
}

std::string Service::getProvider() {
    return providerEmail;
}

void Service::setIconStr(std::string istr){
    iconStr = istr;
}

std::string Service::getIconStr(){
    return iconStr;
}

std::vector<Parameter>* Service::getListParam(){
    return &listParam;
}

void Service::setAct(int a){
    act = a;
}

int Service::getAct(){
    return act;
}


void Service::setStageinDirs(const vector<std::string>& dirs){
  stageinDirs = dirs;
}

std::vector<std::string>* Service::getStageinDirs(){
  return &stageinDirs;
}

void Service::setStageoutDirs(const vector<std::string>& dirs){
  stageoutDirs = dirs;
}

std::vector<std::string>* Service::getStageoutDirs(){
  return &stageoutDirs;
}

void Service::setMaxElapseTime(int met){
  maxElapseTime = met;
}

int Service::getMaxElapseTime(){
  return maxElapseTime;
}

void Service::setNumberOfNode(int non){
  numberOfNode = non;
}

int Service::getNumberOfNode(){
  return numberOfNode;
}
