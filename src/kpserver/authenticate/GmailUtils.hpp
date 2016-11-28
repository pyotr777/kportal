#ifndef GMAIL_UTILS_HPP
#define GMAIL_UTILS_HPP

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <string>
#include <cstring>
#include <curl/curl.h>
//#include <curl/types.h>
#include <curl/easy.h>

#include <sstream>
#include <dirent.h>

#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <time.h>

using namespace std;

class GmailUtils
{
public:
  static bool GenLoginID(std::string& id);
  static bool Oauth20(std::string email, std::string token);
  static bool ClientLoginAuth(std::string userId, std::string pwd);
  static bool SendEmail(std::string to, std::string name, std::string message, const std::string public_key_str);
private:
	static size_t write_to_string(void *ptr, size_t size, size_t count, void *stream);
	static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);
};

std::string Exec(const char* cmd);
#endif //GMAIL_UTILS_HPP
