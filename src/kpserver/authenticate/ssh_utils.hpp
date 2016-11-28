#ifndef SSH_UTILS_HPP
#define SSH_UTILS_HPP
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "system_command_utils.hpp"

const std::string kSshConfigFile = ".ssh/config";
const std::string kHostTag = "Host";
const std::string kHostNameTag = "HostName";
const std::string kUserTag = "User";
const std::string kIdentityFileTag = "IdentityFile";

class SshUtils{
  public:
    static bool AddHost(const std::string& remote_host, const std::string& remote_user, const std::string& path_to_private_key);
    static bool RemoveHost(const std::string remote_host);
    static bool GenerateSshKey(const std::string path_to_save, std::string& public_key_str);
  private:
    static std::string Exec(const char* cmd);
};

#endif //SSH_UTILS_HPP
