#include "tunnel_manager.hpp"
#include "../authenticate/StringUtils.hpp"
extern std::map<std::string, unsigned int> g_tunnels;
extern pthread_mutex_t g_tunnels_lock;
#define  KDESK_HOST ".k.aics.riken.jp"
#define KDESKDAEMON_PATH "/bin/kdeskdaemon"
TunnelManager::TunnelManager() {
  //exit_code = 0;
}

struct ConnectionThreadArg
{
  std::string k_acc;
  unsigned int port;
};

void * TunnelManager::SetupTunnel(void *thrd_args) {
  std::cout << "TunnelManager::start tunnel command\n";
  ConnectionThreadArg* args = (ConnectionThreadArg*) thrd_args;
  int tunnel_port = args->port;
  std::string kacc = args->k_acc;
  delete thrd_args;

  std::string khome_path;
  std::stringstream ss;
  std::string stdout;std::string cmd = "";
  //
  // Get home path of kdesk user
  //
  ss << "ssh "<< kacc << KDESK_HOST  << " pwd;";
  cmd = ss.str();
  std::cout << "Get kdesk home path: cmd = " << cmd <<std::endl;
  stdout = SystemCommandUtils::Exec(cmd.c_str());
  std::cout << "stdout: " << stdout << std::endl;\
  std::vector<std::string> lines;
  if(StringUtils::Split(stdout, lines, '\n') > 0){
    khome_path = lines[0] ;
  }else {
    std::cout << "get path error \n";
  }
  stdout = "";
  ss.str("");

  //
  // check file kdeskdaemon exist
  //
  std::string remote_path =  khome_path + std::string("/kportal/kdeskdaemon");
  ss << "ssh "<< kacc << KDESK_HOST  << " test -e " << remote_path <<"; echo $?";
  cmd = ss.str();
  std::cout << "Check kdeskdaemon exist. cmd = " << cmd <<std::endl;
  stdout = SystemCommandUtils::Exec(cmd.c_str());
  std::cout << "stdout: " << stdout << std::endl;
  int isNotExist = 1;
  try{
    isNotExist = atoi(stdout.c_str());
  } catch (int &ex){
    std::cout << "error when convert aoti \n";
  }
  stdout = "";
  ss.str("");

  //
  // Scp kdeskdaemon to kacc
  //
  if(isNotExist){
    // Create folder
    ss << "ssh "<< kacc << KDESK_HOST  << " mkdir -p " << khome_path << "/kportal/";
    cmd = ss.str();
    std::cout << "create kportal folder. cmd = " << cmd <<std::endl;
    stdout = SystemCommandUtils::Exec(cmd.c_str());
    std::cout << "stdout: " << stdout << std::endl;
    stdout = "";
    ss.str("");

    // scp
    ss << "scp " << KDESKDAEMON_PATH << " "<< kacc << KDESK_HOST  << ":" << remote_path;
    cmd = ss.str();
    std::cout << "cmd = " << cmd <<std::endl;
    stdout = SystemCommandUtils::Exec(cmd.c_str());
    std::cout << "stdout: " << stdout << std::endl;
    stdout = "";
    ss.str("");
  } else {
    std::cout <<"file existed, Ignore scp!\n";
  }

  //
  // change chmod file exe
  //
  if(isNotExist)
  {
    ss.str("");
    ss << "ssh "<< kacc << KDESK_HOST  << " chmod 755 " << remote_path;
    cmd = ss.str();
    std::cout << "chmod kdeskdaemon. cmd = " << cmd <<std::endl;
    stdout = SystemCommandUtils::Exec(cmd.c_str());
    std::cout << "stdout: " << stdout << std::endl;
    stdout = "";
    ss.str("");
  } else {
    std::cout <<"file existed, Ignore scp!\n";
  }

  //
  // Ssh tunnel
  //
  ss.str("");
  ss << "ssh -t -f -L *:"<< tunnel_port <<":localhost:"<< tunnel_port <<" -v " << kacc << KDESK_HOST << " " << remote_path << " " << tunnel_port << "; echo $?;";
  cmd = ss.str();
  std::cout << "cmd = " << cmd <<std::endl;
  stdout = SystemCommandUtils::Exec(cmd.c_str());
  std::cout << "stdout: " << stdout << std::endl;
  if(stdout.size() > 0){
    std::cout << "ssh tunnel fail, remove to maps\n";
    pthread_mutex_lock(&g_tunnels_lock);
    g_tunnels.erase(kacc);
    exit_code = 1;
    pthread_mutex_unlock(&g_tunnels_lock);
  }
  return NULL;
}

void TunnelManager::ConfigureTunnel(std::string kacc, unsigned int & port ){
  pthread_mutex_lock(&g_tunnels_lock);
  std::cout << "TunnelManager::ConfigureTunnel( " << kacc << " , " << port << ")\n";
  std::map<std::string,unsigned int>::iterator it = g_tunnels.find(kacc);
  std::cout << "TunnelManager::ConfigureTunnel\n";
  if (it == g_tunnels.end()) {
    g_tunnels[kacc] = tunnel_next_port;
    pthread_mutex_unlock(&g_tunnels_lock);

    // Create a shared tunnel object
    pthread_t sniffer_thread;
    std::cout << "start new tunnel \n";
    ConnectionThreadArg* thr_arg = new ConnectionThreadArg();
    thr_arg -> k_acc = kacc;
    thr_arg -> port = tunnel_next_port;
    if (pthread_create( &sniffer_thread , NULL ,  SetupTunnel , (void*)thr_arg) < 0) {
      delete thr_arg;
      perror("could not create thread");

    } else {
      port = tunnel_next_port;
      tunnel_next_port ++;
    }
  } else {
    std::cout << "Tunnel existed. ";
    port = it->second;
    std::cout << "port use: " << port << std::endl;
    pthread_mutex_unlock(&g_tunnels_lock);
  }
}
