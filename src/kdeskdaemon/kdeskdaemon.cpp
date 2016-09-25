#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cctype>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <map>
#include <fstream>      // std::ofstream

#include <algorithm>
#include <cstdlib>

#include "../slavedaemon/header.hpp"
#include "../slavedaemon/message.hpp"
#include "../slavedaemon/system_command_utils.hpp"
#include "../slavedaemon/message_socket.h"
#include "../slavedaemon/socket_exception.h"
#include "stream_redirect.hpp"

#define PATH_SEPARATOR "/"
#define PATH_SEPARATOR_CHAR '/'
const char* kJobDirName = "/kportal/jobs/";
const char* kLogExtension = ".stdout";
const char* kErrExtension = ".stderr";
std::string g_home_job_dir = "";

class SlaveDaemonSession{
  private:
    std::string m_receieving_file;
    std::ofstream m_logging_stream;
  public:
    void ProcessMessage(Message &msg, Socket &socket);
};

int g_kdesk_port = 0;
const int BUFFER_SIZE = 512;
std::map<int, SlaveDaemonSession> g_session;
StreamRedirector* g_log_redirector;
StreamRedirector* g_err_redirector;
//
// The connection handler thread routin decodes incoming requests.
//
void * ConnectionHandler(void *thrd_args);

//
//
//
bool ProcessMessage(Message& msg, Socket& socket);
void ProcessSubmitJob(Socket& socket, const std::string job_id, const std::vector<std::string>& recv_files);
void ProcessSubmitJob(Socket& socket, const std::string job_id, const std::string& exe_path, const std::vector<std::string>& recv_files);
std::string RunSubmitJob(const std::string job_id);
std::string RunPjsub(const std::string& job_id, const std::string& exe_path);
//std::string RunPjstat(const std::string& pjm_job_id);
void RunPjstat(const std::string& pjm_job_id, std::string& state, std::string& start_date, std::string& eslapse_time);
bool RunPjwait(Socket& sock, const std::string& kp_job_id, const std::string& pjm_job_id);
bool RunKAccountJ(Socket& socket, const std::string& kdeskacc);
bool SendStatusEvent(Socket& kp_socket, const std::string& kp_job_id, const std::string& pjm_job_id, const std::string& status, const std::string& start_date, const std::string& eslapse_time);
int RecursiveFileList(const std::string dir_path, std::vector<std::string>& files);
std::vector<std::string> GetNewFiles(const std::string& directory, const std::vector<std::string>& olds_files);
void Split(const std::string &str, char del,
                std::vector<std::string> *out);
std::string GetFileName(std::string path);

//
// Main function
//
int main (int argc, char **argv) {
  // Get hostname
  std::string hostname = SystemCommandUtils::Exec("hostname");
  std::cout << "Host name: " << hostname.c_str() << std::endl;

  // Redirect cout to log file <programe name>.stdout;
  char* app_name = argv[0];
  std::stringstream ss;
  ss << app_name << kLogExtension;
  std::string log_filename = ss.str();
  g_log_redirector = NULL;
  std::ofstream log_strm(log_filename.c_str());
  g_log_redirector = new StreamRedirector(std::cout, log_strm.rdbuf());

  ss.str("");
  ss << app_name << kErrExtension;
  std::string err_filename = ss.str();

  g_err_redirector = NULL;
  std::ofstream err_strm(err_filename.c_str());
  g_err_redirector = new StreamRedirector(std::cerr, err_strm.rdbuf());

  std::cout << "Host name: " << hostname.c_str() << std::endl;

  // Create job home path
  std::string homepath = getenv("HOME");
  g_home_job_dir = homepath + kJobDirName;
  std::cout << "Home job: " << g_home_job_dir << std::endl;
  if (argc < 2) {
    std::cout << "kdeskdaemon <port no>\n";
    delete g_err_redirector;
    delete g_log_redirector;
    return 1;
  }

  //
  // Server socket
  //
  try {
    // Create the socket
    //ServerSocket server ( 30000 );
    Socket server;
    g_kdesk_port = atoi(argv[1]);
    if (!server.Create() ) {
      throw SocketException ( "Could not create server socket." );
    }

    if (!server.Bind(g_kdesk_port)) {
      throw SocketException ( "Could not bind to port." );
    }
    std::cout << "bind done" << std::endl;

    if ( ! server.Listen() ) {
      throw SocketException ( "Could not listen to socket." );
    }
    std::cout << "Waiting for incoming connections..." << std::endl;


    while ( true ) {
      Socket* client_sock = new Socket();
      std::cout << "before accept\n";
      //server.accept ( new_sock );
      if ( ! server.Accept(*client_sock)) {
        delete client_sock;
        throw SocketException ( "Could not accept socket." );
      }
      std::cout << "Connection accepted" << std::endl;
      //ConnectionHandler((void*)&client_sock);
      //continue;

      pthread_t sniffer_thread;
      //thrd_args->socket = &client_sock;
      if (pthread_create( &sniffer_thread , NULL ,  ConnectionHandler , (void*)client_sock) < 0) {
        //perror("could not create thread");
        std::cout << "could not create thread\n";
        continue;
      }

      //Now join the thread , so that we dont terminate before the thread
      //pthread_join( sniffer_thread , NULL);
      std::cout <<"Handler assigned" << std::endl;
    }
  } catch ( SocketException& e ) {
    std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
  }

  if(g_log_redirector != NULL)
    delete g_log_redirector;
  if(g_err_redirector != NULL)
    delete g_err_redirector;
  return 0;
}

//
// This will handle connection for each command
// Format message: (Type message) - 1byte, (size of message) - sizeof(BYTE), (content) - ...
//
void * ConnectionHandler(void *thrd_args) {
  std::cout <<"ConnectionHandler\n";
  std::string hostname = SystemCommandUtils::Exec("hostname");
  std::cout << "Host name inside thread: " << hostname.c_str() << std::endl;

  //std::ofstream log_strm("kdeskdaemon.log");
  //log_strm  << "Execute node: " << hostname << std::endl;

  //Get the socket descriptor
  Socket* socket = (Socket*) thrd_args;

  // Receive a message from client
  try {
    Message rec_msg;
    int readed_size = socket->Recv(rec_msg);
    std::cout <<"after call Recv: readed_size = " << readed_size <<std::endl;
    if(readed_size == 0){
      sleep(3);
    } else if(readed_size < 0){
      throw SocketException("socket error");
    }else {
      std::cout << "Received signal from client\n";
      if(ProcessMessage(rec_msg, *socket)){
        std::cout << "finish handler connection\n";
      }
    }
  } catch ( SocketException& e) {
    std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
  }
  //delete args->socket;
  delete socket;
  return NULL;
}

bool ProcessMessage(Message& msg, Socket& socket){
    bool isFinish = true;
    std::vector<std::string> argv = msg.GetCmdArgs();
    int argc = argv.size();

    Header hdr(MT_COMMAND, g_kdesk_port, 0, 0, CMD_ACK_OK);
    Message res_msg = Message(hdr, NULL);
    std::string job_id, exe_path;
    std::stringstream ss;
    std::vector<std::string> recv_files;
    switch(msg.GetHeader()->GetCommand()){
      case CMD_SUBMITJOB:
        std::cout << "CMD_SUBMITJOB <job id><exe file>";
        if(argc >= 2){
          job_id = argv[0];
          exe_path = argv[1];
          std::cout << "send response init job sucess, wait to receive folder: " << g_home_job_dir << "/" << job_id << std::endl;
          // Send response
          if(socket.Send(res_msg)){
            if(socket.RecvFolder(g_home_job_dir, recv_files)){
              std::cout << "receieve folder success\n";
              ProcessSubmitJob(socket, job_id, exe_path, recv_files);
            } else {
              std::cout << "receieve folder success\n";
              hdr.SetCommand(CMD_ACK_ERR);
              res_msg.SetHeader(hdr);
              socket.Send(res_msg);
            }

            // Clear job folderx
            if(g_home_job_dir.size() > 0){
              std::string cmd = std::string("rm -r ") + g_home_job_dir +  std::string("/") + job_id ;
              std::cout << "cmd: " << cmd << std::endl;
              std::string stdout = "ingore remove just test";// = SystemCommandUtils::Exec(cmd.c_str());
              std::cout << "stdout: " << stdout << std::endl;
            }
          } else {
            std::cout << "send response int job fail\n";
          }
        } else {
          std::cout << "command argument is missing.\n ";
        }
        break;
      case CMD_GET_TIME_RESOURCE:
        std::cout << "CMD_GET_TIME_RESOURCE <kdesk acc> \n";
        if(argc >= 1){;
          if(RunKAccountJ(socket, argv[0])){
            std::cout << "success\n";
          } else{
            std::cout << "[ERR] run command fail\n";
          }
        } else {
          std::cout << "[ERR] command required arg is missing.\n";
        }
        break;
      case CMD_REMOVETUNNEL:
        std::cout << "CMD_REMOVETUNNEL\nExit now!!\n";
        exit(0);
        break;
      default:
        std::cout << "[ERR] Unknown message: " << msg.GetHeader()->GetCommand() << std::endl;
        break;
    }
    return isFinish;
}

void RunExtendSh(std::string job_id, std::string shfile){
  std::cout << "RunExtendSh: " << shfile << std::endl;
  std::stringstream ss;
  ss << "cd " << g_home_job_dir << job_id << ";";
  ss << "chmod 755 " << shfile << ";";
  ss << "[ -f " << shfile.c_str() << " ] && sh " << shfile.c_str() << ";";
  std::string cmd = ss.str();
  std::cout << "cmd: " << cmd << std::endl;
  std::string stdout = SystemCommandUtils::Exec(cmd.c_str());
  std::cout << "stdout log: ====== \n";
  std::cout << stdout << std::endl << "================"<< std::endl;
}

void ProcessSubmitJob(Socket& socket, const std::string job_id, const std::string& exe_path, const std::vector<std::string>& recv_files){
  std::cout << "ProcessSubmitJob: " << job_id << std::endl;
  Message res_msg(Header(MT_COMMAND, g_kdesk_port, 0, 0, CMD_JOB_STATE), NULL);
  Header err_hdr(MT_COMMAND, g_kdesk_port, 0, 0, CMD_ACK_ERR);

  // Pre-processing command
  std::string pre_file = job_id + "_pre.sh";
  std::cout << "Pre-processing commands: " << pre_file <<std::endl;
  RunExtendSh(job_id, pre_file);
  

  // Run submit job command
  std::cout << "run pjsub.\n";
  std::string pjm_jid = RunPjsub(job_id, exe_path);
  if(pjm_jid.size() <= 0){
    std::cout << "run pjsub fail\n";
    res_msg.SetHeader(err_hdr);
    if(socket.Send(res_msg)){
      std::cout << "send error response success.\n";
    } else {
      std::cout << "send error response fail.\n";
    }
    return;
  }

  // RunPjwait command
  if(RunPjwait(socket, job_id, pjm_jid)){
    std::cout << "run pjwait success.\n";
  } else {
    std::cout << "run pjwait fail.\n";
  }
  
  // Post-processing command
  std::string post_file = job_id + "_post.sh";
  std::cout << "Post-processing commands:" << post_file << std::endl; 
  RunExtendSh(job_id, post_file);

  // Send file to slavedaemon
  std::string job_path = g_home_job_dir + job_id;
  std::vector<std::string> new_files = GetNewFiles(job_path, recv_files);
  std::cout << "num of new file: " << new_files.size() << "files\n";

  Message notify_msg = Message(Header(MT_COMMAND, g_kdesk_port, 0, 0, CMD_JOB_STATE), NULL),
      err_msg = Message(Header(MT_COMMAND, g_kdesk_port, 0, 0, CMD_ACK_ERR), NULL);

  if(socket.SendListFile(new_files, job_path + PATH_SEPARATOR)){
    std::cout << "send file to slavedaemon success.\n";
    std::string status, start_date, eslapse_time;
    RunPjstat(pjm_jid, status, start_date, eslapse_time);
    notify_msg.AddArgv(job_id);
    notify_msg.AddArgv(pjm_jid);
    notify_msg.AddArgv(status);
    notify_msg.AddArgv(start_date);
    notify_msg.AddArgv(eslapse_time);

    //status = RunPjstat(pjm_jid);
    //res_msg.AddArgv(pjm_jid);
    //res_msg.AddArgv(status);
    if(socket.Send(notify_msg)){
      std::cout << "notify job finished success.\n";
    } else {
      std::cout << "notify job finished false.\n";
    }
    return;
  } else {
    std::cout << "send file to slavedaemon fail.\n";
  }
  if(socket.Send(err_msg)){
    std::cout << "send error message success.\n";
    return;
  } else {
    std::cout << "send error message fail.\n";
  }
}

std::vector<std::string> GetNewFiles(const std::string& path_folder, const std::vector<std::string>& olds_files){
  std::cout << "GetNewFiles( path = " << path_folder << " )\n";
  std::cout << "list old files: " << std::endl;

  for (unsigned int j = 0; j < olds_files.size(); j++) {
    std::cout << "file: " << olds_files[j] << std::endl;
  }
  std::vector<std::string> new_files;
/*
  DIR *dir;
  dirent *pdir;
  dir = opendir(path_folder.c_str());
  if (dir == NULL) {
    std::cout << "folder not exist\n";
    return new_files;
  }
*/
  std::string _path_folder;
  if (path_folder.at(path_folder.length() - 1) == PATH_SEPARATOR_CHAR){
    _path_folder = path_folder.substr(0, path_folder.length() - 1); 
  } else {
    _path_folder = path_folder;
  }

  std::vector<std::string> cur_files;
  int rlt = RecursiveFileList(_path_folder, cur_files);
  
/*
  while (pdir = readdir(dir)) {
    struct stat st;

    if(strcmp(pdir->d_name, ".") == 0 || strcmp(pdir->d_name, "..") == 0) {
      continue;
    }

    if (fstatat(dirfd(dir), pdir->d_name, &st, 0) < 0) {
      std::cout << "Some error in fstatat\n";
      continue;
    }

    if (S_ISREG(st.st_mode)) {
      std::string dname(pdir->d_name);
      std::cout << "cur file: " << _path_folder + pdir->d_name << std::endl;
      cur_files.push_back(_path_folder + pdir->d_name);
    } 
  }
*/
  for (unsigned int i = 0; i < cur_files.size(); i++) {
    bool foo = false;
    for (unsigned int j = 0; j < olds_files.size(); j++) {
      if (cur_files.at(i) == olds_files.at(j)) {
        foo = true;
        break;
      }
    }
    if (!foo){
      std::cout << cur_files.at(i) << " is a new file.\n";
      new_files.push_back(cur_files.at(i));
    }
  }
  std::cout << "num of file: " << new_files.size() << std::endl;
  //closedir(dir);
  std::cout << "  finish"<< std::endl;
  return new_files;
}

int RecursiveFileList(const std::string dir_path, std::vector<std::string>& files){
  DIR *dir;
  dirent *pdir;
  dir = opendir(dir_path.c_str());
  if (dir == NULL) {
    std::cout << dir_path.c_str() << " can not open.\n";
    return -1;
  }
  //d::vector<std::string> cur_files;
  while (pdir = readdir(dir)) {
    struct stat st;
    if(strcmp(pdir->d_name, ".") == 0 || strcmp(pdir->d_name, "..") == 0) {
      continue;
    }
    if (fstatat(dirfd(dir), pdir->d_name, &st, 0) < 0) {
      std::cout << "Some error in fstatat\n";
      continue;
    }
    if (S_ISREG(st.st_mode)) {
      std::string dname(pdir->d_name);
      std::cout << "cur file: " << dir_path + pdir->d_name << std::endl;
      files.push_back(dir_path + PATH_SEPARATOR + pdir->d_name);
    } else if (S_ISDIR(st.st_mode)){
      RecursiveFileList(dir_path + PATH_SEPARATOR + pdir->d_name, files);
    }
  }
  closedir(dir);
}

bool RunPjwait(Socket& socket, const std::string& kp_job_id, const std::string& pjm_job_id){
  std::cout << "RunPjwait\n";
  // pjsub ./sample.sh

  std::cout << "fake pjwait: ";
  std::string status_last;
  while(1){
  // Err: no execute. Need ask administrator to add permission
//  std::stringstream ss;
//  ss << "pjwait " <<  pjm_job_id << "";
//  std::string cmd = ss.str();
//  std::cout << "cmd: " << cmd << std::endl;
//  std::string stdout = SystemCommandUtils::Exec(cmd.c_str());
//  std::cout << "stdout log: =======>>>>>\n";
//  std::cout << stdout << std::endl << "================"<< std::endl;

    // Parse get current processing state of job
    std::string status, start_date, eslapse_time;
    RunPjstat(pjm_job_id, status, start_date, eslapse_time);


    // Check status
//    Current processing state of the job
//    ACC: Accepted job submission
//    RJT: Rejected job submission
//    QUE: Waiting for job execution
//    SIN: In the processing of the stage-in
//    RDY: Ready to execute the job
//    RNA: Acquiring resources required for job execution
//    RUN: Executing job
//    RNO: Waiting for completion of job termination processing
//    SOT: In the processing of the stage-out
//    EXT: Completed of the job termination processing
//    CCL: Exit by job cancelation
//    HLD: Holding status by user operation
//    ERR: In fixed state due to an error
    std::cout << "job status " << status << "; ";
    if(status == "RJT" || status == "EXT" || status == "CCL"){
      std::cout << "begin change status from " << status_last << " to " << status << ", send a notify to slavedaemon\n";
      if(SendStatusEvent(socket, kp_job_id, pjm_job_id, status, start_date, eslapse_time)){
      }
      std::cout << "job finished\n";
      break;
    } else {
      if((status == "RUN" && status_last != "RUN" ) ||
         (status != "RUN" && status_last == "RUN" )){
        // job begin start
        std::cout << "begin change status from " << status_last << " to " << status << ", send a notify to slavedaemon\n";
        if(SendStatusEvent(socket, kp_job_id, pjm_job_id, status, start_date, eslapse_time)){
          //change_status_sent = true;
        }
        std::cout << "job begin running \n";
      }
    }
    status_last = status;
    sleep(1);
  }

  std::cout << " end query status, next step\n";
  // Check result via stdout string
  //[INFO]

  return true;
}

bool SendStatusEvent(Socket& kp_socket, const std::string& kp_job_id, const std::string& pjm_job_id, const std::string& status, const std::string& start_date, const std::string& eslapse_time){
  std::cout << "SendStatusEvent\n";
  Message notify_msg(Header(MT_COMMAND, 0, 0, 0, CMD_JOB_STATE), NULL);
  notify_msg.AddArgv(kp_job_id);
  notify_msg.AddArgv(pjm_job_id);
  notify_msg.AddArgv(status);
  notify_msg.AddArgv(start_date);
  notify_msg.AddArgv(eslapse_time);
  if(kp_socket.Send(notify_msg)){
    std::cout << "notify job success.\n";
    return true;
  } else {
    std::cout << "notify job false.\n";
    return false;
  }
}

std::string GetFileName(std::string path)
{
    std::string filename = "";
    filename = path.substr(path.find_last_of("/") + 1, path.length());
    //std::cout << "Bp:FileName:"<< filename << std::endl;
    return filename;
}

std::string RunPjsub(const std::string& job_id, const std::string& exe_path){
  std::cout << "RunSubmitJob\n";\
  // pjsub ./sample.sh

  std::string filename = GetFileName(exe_path);
  std::stringstream ss;
  ss << "cd " << g_home_job_dir << job_id << ";";
  ss << "chmod 755 " << filename << ";";
  ss << "pjsub " << job_id << ".sh";
  std::string cmd = ss.str();
  std::cout << "cmd: " << cmd << std::endl;
  std::string stdout = SystemCommandUtils::Exec(cmd.c_str());
  std::cout << "stdout log: =======>>>>>\n";
  std::cout << stdout << std::endl << "================"<< std::endl;

  if(stdout.find("ERR.") != std::string::npos){
    std::cout << "pjsub fail\n";
    return "";
  }

  // Check result via stdout string
  //[INFO] PJM 0000 pjsub Job 5081637 submitted.
  std::vector<std::string> words;
  Split(stdout, ' ',&words);
  std::size_t found = stdout.find("submitted.");
  std::string jm_job_id;
  if (found != std::string::npos && words.size() >= 7){
    jm_job_id = words[5];
    std::cout << "submit job " << jm_job_id << " success.\n";
  } else {
    std::cout << "submit job fail.\n";
  }

  return jm_job_id;
}

void RunPjstat(const std::string& pjm_job_id, std::string& status, std::string& start_date, std::string& eslapse_time){
  std::cout << "RunPjstat\n";
  std::stringstream ss;
  ss << "pjstat " << pjm_job_id << ";";
  std::string cmd = ss.str();

  std::string stdout = SystemCommandUtils::Exec(cmd.c_str());
  std::vector<std::string> lines;

  // Get job status from stdout
  Split(stdout, '\n', &lines);
  for(unsigned int i = 0; i < lines.size() ; i++){
    if(lines[i].find(pjm_job_id) != std::string::npos){
      std::vector<std::string> cells;
      Split(lines[i], ' ', &cells);
      for(unsigned int j = 0, col = 0; j < cells.size() && col < 9; j ++){
        if(cells[j].size() > 0){
          switch(col){
            case 3:
              status = cells[j];
              break;
            case 6:
              start_date = cells[j];
              break;
            case 7:
              start_date += " ";
              start_date += cells[j];
            case 8:
              eslapse_time = cells[j];
              break;
          }
          col ++;
        }
      }
      if(status.size() > 0 && start_date.size() > 0 && eslapse_time.size() > 0){
        break;
      }
    }
  }
  std::cout <<"{" << status << ", " << start_date << ", " << eslapse_time << "} ";
  //return status;
}

bool RunKAccountJ(Socket& socket, const std::string& kdeskacc){//, std::string& total, std::string& usage, std::string& remain){
  std::cout << "RunKAccountJ( " << kdeskacc << " )\n";
  Message res(Header(MT_COMMAND, g_kdesk_port, 0, 0, CMD_ACK_OK), NULL);
  Header err_hdr(MT_COMMAND, g_kdesk_port, 0, 0, CMD_ACK_ERR);
  std::string stdout = SystemCommandUtils::Exec("k_accountj -h Ms");
  std::cout << "Log: " << stdout << std::endl;
  std::string unittag = "unit[";
  std::string unit, total, usage, remain;
  std::vector<std::string> lines;
  Split(stdout, '\n', &lines);
  unsigned int i = 0, ig = -1;

  std::cout << "lines size = " << lines.size() << std::endl;
  for(; i < lines.size(); i ++){
    std::cout << "line " << i << ": " << lines[i] << std::endl;
    if(i == 0){
      unsigned int si = lines[i].find(unittag);
      if(si != std::string::npos){
        si += unittag.size();
        std::cout << " start unit index = " << si << std::endl;
        for(;si < lines[i].size() && lines[i][si] != ']'; si++){
          unit += lines[i][si];
        }

        std::cout << " end unit index = " << si << std::endl;
      }
      std:: cout << "unit = " << unit << std::endl;
    } else {
      if(lines[i].find("GROUP") != std::string::npos){
        ig = i;
      }
      if( i > ig && ig > 0 && lines[i].size() > 0 &&
          lines[i][0] == '*' &&
          lines[i].find("*--") == std::string::npos){
        break;
      }
    }
  }

  if(i == lines.size()){
    std::cout << "not found line \n";
    res.SetHeader(err_hdr);
  } else {
    std::vector<std::string> words;
    std::cout << " before split: " << lines[i] << std::endl;
    Split(lines[i], ' ', &words);

    std::cout << "words size: " << words.size() << std::endl;
    int col = 0;

    for(unsigned int j = 0; j < words.size() && col < 4 ; j ++){
      if(words[j].size() > 0){
        std::cout << "col " << col << " = " << words[j] << ", ";
        switch(col){
          case 1: // LIMIT
            total = words[j];
            total.erase(std::remove(total.begin(), total.end(), ','), total.end());
            break;
          case 2: // USAGE
            usage = words[j];
            usage.erase(std::remove(usage.begin(), usage.end(), ','), usage.end());
            break;
          case 3: // AVAIABLE
            remain = words[j];
            remain.erase(std::remove(remain.begin(), remain.end(), ','), remain.end());
            break;
        }
        col ++;
      }
    }
    std::cout << "create message: " << unit << ", " << total << ", " << usage << ", " << remain << "\n";
    res.AddArgv(unit);
    res.AddArgv(total);
    res.AddArgv(usage);
    res.AddArgv(remain);
  }

  if(socket.Send(res)){
    std::cout << "response success\n";
    return true;
  } else {
    std::cout << "[ERR] response success\n";
    return false;
  }
}

void Split(const std::string &str, char del,
                std::vector<std::string> *out) {
  std::stringstream ss(str);
  std::string s;
  while (std::getline(ss, s, del)) {
      out->push_back(s);
  }
}



