#include "ssh_utils.hpp"
#include <cstdlib>

bool SshUtils::AddHost(const std::string& remote_host, const std::string& remote_user, const std::string& path_to_private_key){
  std::cout << "SshUtils::AddHost remote_host " << remote_host << ", remote_user " << remote_user << ", path_to_private_key " << path_to_private_key <<std::endl;
  bool rlt = false;

//  struct passwd *pw = getpwuid(getuid());
//  char *homedir = pw->pw_dir;
  std::string homepath = getenv("HOME");
  std::cout << "$HOME: " <<  homepath << std::endl;
  std::stringstream ss;
  ss << homepath << "/" << kSshConfigFile;
  std::string ssh_config_file = ss.str();
  ss.str("");
  std::cout << "Ssh config file: " << ssh_config_file << std::endl;

  std::ifstream cf(ssh_config_file.c_str());
  std::string line;
  std::vector<std::string> lines;
  //int begin_index = -1, end_index = -1;
  std::string config_host_name = std::string (remote_user + "." + remote_host);
  std::cout << "Add host: " << config_host_name << std::endl;
  // Find exist
  try{
    while (std::getline(cf, line)) {
      lines.push_back(line);
      if(line.find(config_host_name) != std::string::npos){
        cf.close();
        std::cout << "the host name " << config_host_name << " is exists. Ignore add host.\n";
        return true;
      }
    }
    cf.close();
  } catch (int & ex){
    std::cout << "Exception caunt during read ssh config file, err code = " << ex << "\n";
    return false;
  }

  // Add
  ss.str("");
  //std::stringstream ss;
  ss << kHostTag << " " << remote_user << "." << remote_host << std::endl;
  ss << "\t" << kHostNameTag << "=" << remote_host << std::endl;
  ss << "\t" << kUserTag << "=" << remote_user << std::endl;
  ss << "\tStrictHostKeyChecking=no" << std::endl;
  ss << "\t" << kIdentityFileTag << "=" << path_to_private_key << std::endl;
  std::string tmp = ss.str();
  std::cout << "Add string: \n";
  std::cout << tmp;
  //if(begin_index < 0 && end_index < 0){
    std::ofstream ocf(ssh_config_file.c_str(), std::ofstream::out | std::ofstream::trunc);
    try{
      for(int i = 0 ; i < lines.size() ; i++){
        ocf << lines[i] <<std::endl;
        std::cout << lines[i] <<std::endl;
      }
      ocf << tmp;
      ocf.close();
      rlt = true;
    }catch(int& ex){
      std::cout << "Exception caunt during save ssh config file, err code = " << ex << "\n";
    }
//  } else {
//    std::cout << remote_host << " hostname exists for other host.\n";
//  }

  return rlt;
}


bool SshUtils::RemoveHost(const std::string remote_host){
  bool rlt = false;
  std::ifstream cf(kSshConfigFile.c_str(), std::ifstream::in);
  std::string line;
  std::vector<std::string> lines;
  int begin_index = -1, end_index = 0;

  // Find position
  try{
    while (std::getline(cf, line)) {
      lines.push_back(line);
      //Start scope
      std::size_t found = line.find_first_of(kHostTag);
      std::size_t host_found = line.find_first_of(remote_host);
      if (found == 0){
        if(host_found != std::string::npos && host_found > found) {
          begin_index = lines.size() - 1;
          end_index = begin_index;
          continue;
        }
      }

      std::size_t identityfile_found = line.find_first_of(kIdentityFileTag);
      if(identityfile_found != std::string::npos && begin_index > 0){
        end_index = lines.size() - 1;
        rlt = true;
        break;
      }
    }
    cf.close();
  } catch (int & ex){
    std::cout << "Exception caunt during read ssh config file, err code = " << ex << "\n";
    return false;
  }

  // Save file without host
  std::ofstream ocf(kSshConfigFile.c_str(), std::ofstream::out | std::ofstream::trunc);
  try{
    for(int i = 0 ; i < lines.size() ; i++){
      if(i >= begin_index && i <= end_index)
        continue;
      ocf << lines[i];
    }
    ocf.close();
    rlt = true;
  }catch(int& ex){
    std::cout << "Exception caunt during save ssh config file, err code = " << ex << "\n";
  }

  return rlt;
}

bool SshUtils::GenerateSshKey(const std::string path_to_save, std::string& public_key_str){//, const std::string remote_user, const){
  std::cout << "SshUtils::GenerateSshKey" << std::endl;
  // Gen
  std::stringstream ss;
  ss << "mv " << path_to_save  << " " << path_to_save << "1;"
	 << "mv " << path_to_save << ".pub"  << " " << path_to_save << ".pub1;"
     << "ssh-keygen -t rsa -N \"\" -f " << path_to_save <<";"
     << "chmod 700 " <<path_to_save << ";";
  std::string cmd = ss.str();
  std::cout << "run command: " << cmd << std::endl;
  std::string stdout = Exec(cmd.c_str());

  std::cout << "Stdout: ==============" << stdout << "\n===============\n";
  if(stdout.find("The key fingerprint is:") != std::string::npos){
    std::cout << "gen ssh success\n";
  } else  {
    std::cout << "gen ssh fail. \n";
    return false;
  }

  // Get public key
  struct passwd *pw = getpwuid(getuid());
  char *homedir = pw->pw_dir;
  std::cout << "$HOME: " <<  homedir << std::endl;
  ss.str("");ss << path_to_save << ".pub";
  std::string pub_file = ss.str();
  std::cout << "read file: " << pub_file << std::endl;
  std::cout << "replace ~ by $HOME path\n";
  ss.str("");
  if(pub_file.size() > 0 && pub_file[0] == '~'){
    ss << homedir << pub_file.c_str() + 1;
  }
  pub_file = ss.str();
  std::cout << "full path public key file : " << pub_file << std::endl;

  std::ifstream fin(pub_file.c_str());
  std::string temp;
  try{
    if(std::getline(fin, temp)){
      public_key_str = temp;
    }
    fin.close();
    std::cout << "publickey :" << public_key_str << std::endl;
  } catch (int & ex){
    std::cout << "Exception caught during read " << pub_file << " file.\n";
    return false;
  }


//  ss.str("");
//  ss << "cat " << path_to_save << ".pub";
//  std::string tmp = ss.str();
//  std::cout << "run command: " << tmp << std::endl;

//  stdout = Exec(cmd.c_str());
//  std::cout << "stdout: " << stdout << std::endl;

//  if(stdout.find("ssh-rsa") != std::string::npos){
//    public_key_str = stdout;
//    std::cout << "cat ssh publickey success\n";
//  } else {
//    std::cout << "cat ssh publickey fail\n";
//  }
  return true;
}

std::string SshUtils::Exec(const char* command) {
  std::string stdout;
  FILE *fpipe;
  char c = 0;

  if (0 == (fpipe = (FILE*)popen(command, "r"))) {
    perror("popen() failed.");
    exit(1);
  }

  while (fread(&c, sizeof c, 1, fpipe)) {
    stdout += c;
    //printf("%c", c);
  }

  pclose(fpipe);
  return stdout;

  /*
  std::string data;
  FILE * stream;
  const int max_buffer = 4*1024;
  char buffer[max_buffer];
  std::string cmd_str = cmd;
  cmd_str.append(" 2>&1");

  stream = popen(cmd_str.c_str(), "r");
  if (stream) {
    while (!feof(stream))
    if (fgets(buffer, max_buffer, stream) != NULL)
      data.append(buffer);
    pclose(stream);
  }
  return data;
  */
         /*
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;*/
}
