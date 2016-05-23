#include "../stdafx.h"
#include "user.hpp"

User::User(){
	email = "";
	container_id = "";
	_phone = "";
	_address = "";
	_description = "";
	typeUser = USER_GROUP;
  _kdesk_acc = "";
  //_kdesk_port = 0;
  _time_usable = 0;
  _time_usage = 0;
}

User::User(std::string e) {
	email = e;
	container_id = "";
	_phone = "";
	_address = "";
	_description = "";
	typeUser = USER_GROUP;
  _kdesk_acc = "";
  //_kdesk_port = 0;
  _public_key = "";
  _time_usable = 0;
  _time_usage = 0;
}

User::User(std::string e, BYTE type) {
	email = e;
	container_id = "";
	_phone = "";
	_address = "";
	_description = "";
	typeUser = type;
  _kdesk_acc = "";
  //_kdesk_port = 0;
  _public_key = "";
  _time_usable = 0;
  _time_usage = 0;
}

User::~User() {
}

void User::setEmail(std::string email_str){
    email = email_str;
}

std::string User::getEmail() const {
    return email;
}

void User::setType(BYTE type) {
    typeUser = type;
}

BYTE User::getType() const{
    return typeUser;
}

void User::setContainerId(std::string id) {
    container_id = id;
}

std::string User::getContainerId() const {
    return container_id;
}

void User::setPhone(std::string phone){
	_phone = phone;
}

std::string User::getPhone()const {
	return _phone;
}

void User::setAddress(std::string address){
  _address = address;
}

std::string User::getAddress()const {
  return _address;
}

void User::setKdeskAcc(std::string kdesk_acc){
  _kdesk_acc = kdesk_acc;
}

std::string User::getKdeskAcc()const {
  return _kdesk_acc;
}


//void User::setKdeskPort(int port){
//  _kdesk_port = port;
//}

//int User::getKdeskPort()const{
//  return _kdesk_port;
//}

void User::setTimeUsable(double tu){
  _time_usable = tu;
}

double User::getTimeUsable()const{
  return _time_usable;
}

void User::setTimeUsage(double tu){
  _time_usage = tu;
}

double User::getTimeUsage()const{
  return _time_usage;
}

void User::setPublicKey(std::string pk){
  _public_key = pk;
}

std::string User::getPublicKey() const{
  return _public_key;
}

void User::setDescription(std::string description){
	_description = description;
}

std::string User::getDescription()const {
	return _description;
}
