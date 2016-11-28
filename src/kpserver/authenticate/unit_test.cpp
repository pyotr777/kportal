#include <string>
#include <sstream>
#include "ssh_utils.hpp"
const std::string kSshKeyDir = "~/.ssh/kportal/";
const std::string kDeskHost = "k.aics.riken.jp";

int main(int argc, char** argv){
  std::stringstream ss;
  std::string ssh_public_key_str;
  std::string remote_user = "a04030";
  ss << kSshKeyDir << remote_user << "." << kDeskHost;
  std::string ssh_key_file = ss.str();
  std::cout << "ssh key file: " << ssh_key_file << ", kdesk user: " << remote_user << std::endl;
  if(SshUtils::GenerateSshKey(ssh_key_file,ssh_public_key_str)){
    if(SshUtils::AddHost(kDeskHost, remote_user, ssh_key_file)){
      std::cout << "Insert users to DB\n";
    } else {
      std::cout << "add host fail\n";
    }
  } else {
    std::cout << "generate ssh key fail\n";
  }
  return 0;
}
