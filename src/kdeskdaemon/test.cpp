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
#include <vector>
#include <algorithm>
#include <sstream>
#include "../kpforwarder/system_command_utils.hpp"

bool RunPjwait(const std::string& kp_job_id, const std::string& pjm_job_id);

void RunPjstat(const std::string& pjm_job_id, std::string& status, std::string& start_date, std::string& eslapse_time);
void Split(const std::string &str, char del,
                std::vector<std::string> *out);

int main (int argc, char **argv) {

  RunPjwait("job01", "5101834");
  return 0;
}

bool RunPjwait(const std::string& kp_job_id, const std::string& pjm_job_id){
  std::cout << "RunPjwait\n";
  // pjsub ./sample.sh

  std::cout << "fake pjwait: ";
  std::string status_last;
  bool change_status_sent = false;
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
//    if(status == "RJT" || status == "EXT" || status == "CCL" || status == "RJT"){
//      if(!change_status_sent){
//        std::cout << "begin change status from " << status_last << " to " << status << ", send a notify to slavedaemon\n";
//        if(SendStatusEvent(socket, kp_job_id, pjm_job_id, status, start_date, eslapse_time)){
//          change_status_sent = true;
//        }
//      }
//      std::cout << "sleep wait out put file.\n";
//      sleep(8);
//      std::cout << "job finished\n";
//      break;
//    } else {
//      if(status == "RUN" && status_last != "RUN" && (! change_status_sent)){
//        // job begin start
//        std::cout << "begin change status from " << status_last << " to " << status << ", send a notify to slavedaemon\n";
//        if(SendStatusEvent(socket, kp_job_id, pjm_job_id, status, start_date, eslapse_time)){
//          change_status_sent = true;
//        }
//      }
//    }
    status_last = status;
    //sleep(3);
  }

  std::cout << " finish\n";
  // Check result via stdout string
  //[INFO]

  return true;
}

//bool SendStatusEvent(Socket& kp_socket, const std::string& kp_job_id, const std::string& pjm_job_id, const std::string& status, const std::string& start_date, const std::string& eslapse_time){
//  std::cout << "SendStatusEvent\n";
//  Message notify_msg(Header(MT_COMMAND, 0, 0, 0, CMD_JOB_STATE), NULL);
//  notify_msg.AddArgv(kp_job_id);
//  notify_msg.AddArgv(pjm_job_id);
//  notify_msg.AddArgv(status);
//  notify_msg.AddArgv(start_date);
//  notify_msg.AddArgv(eslapse_time);
//  if(kp_socket.Send(notify_msg)){
//    std::cout << "notify job finished success.\n";
//    return true;
//  } else {
//    std::cout << "notify job finished false.\n";
//    return false;
//  }
//}

//std::string GetFileName(std::string path)
//{
//    std::string filename = "";
//    filename = path.substr(path.find_last_of("/") + 1, path.length());
//    //std::cout << "Bp:FileName:"<< filename << std::endl;
//    return filename;
//}

//std::string RunPjsub(const std::string& job_id){
//  std::cout << "RunSubmitJob\n";\
//  // pjsub ./sample.sh

//  std::stringstream ss;
//  ss << "cd " << kHomeJobsDir << job_id << ";";
//  ss << "pjsub " << job_id << ".sh";
//  std::string cmd = ss.str();
//  std::cout << "cmd: " << cmd << std::endl;
//  std::string stdout = SystemCommandUtils::Exec(cmd.c_str());
//  std::cout << "stdout log: =======>>>>>\n";
//  std::cout << stdout << std::endl << "================"<< std::endl;

//  if(stdout.find("ERR.") != std::string::npos){
//    std::cout << "pjsub fail\n";
//    return "";
//  }

//  // Check result via stdout string
//  //[INFO] PJM 0000 pjsub Job 5081637 submitted.
//  std::vector<std::string> words;
//  Split(stdout, ' ',&words);
//  std::size_t found = stdout.find("submitted.");
//  std::string jm_job_id;
//  if (found != std::string::npos && words.size() >= 7){
//    jm_job_id = words[5];
//    std::cout << "submit job " << jm_job_id << " success.\n";
//  } else {
//    std::cout << "submit job fail.\n";
//  }

//  return jm_job_id;
//}

void RunPjstat(const std::string& pjm_job_id, std::string& status, std::string& start_date, std::string& eslapse_time){
  std::cout << "RunPjstat\n";
  std::stringstream ss;
  ss << "pjstat " << pjm_job_id << ";";
  std::string cmd = ss.str();

  std::cout << "cmd " << cmd << std::endl;
  std::string stdout = SystemCommandUtils::Exec(cmd.c_str());
  std::vector<std::string> lines;

  std::cout << "stdout " << stdout << std::endl;
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
  std::cout <<"result: " << status << ", " << start_date << ", " << eslapse_time << std::endl;
  //return status;
}

//bool RunKAccountJ(Socket& socket, const std::string& kdeskacc){//, std::string& total, std::string& usage, std::string& remain){
//  std::cout << "RunKAccountJ( " << kdeskacc << " )\n";
//  Message res(Header(MT_COMMAND, g_kdesk_port, 0, 0, CMD_ACK_OK), NULL);
//  Header err_hdr(MT_COMMAND, g_kdesk_port, 0, 0, CMD_ACK_ERR);
//  std::string stdout = SystemCommandUtils::Exec("k_accountj");
//  std::cout << "Log: " << stdout << std::endl;
//  std::string unittag = "unit[";
//  std::string unit, total, usage, remain;
//  std::vector<std::string> lines;
//  Split(stdout, '\n', &lines);
//  unsigned int i = 0, ig = -1;

//  std::cout << "lines size = " << lines.size() << std::endl;
//  for(; i < lines.size(); i ++){
//    std::cout << "line " << i << ": " << lines[i] << std::endl;
//    if(i == 0){
//      int si = lines[i].find(unittag);
//      if(si != std::string::npos){
//        si += unittag.size();
//        std::cout << " start unit index = " << si << std::endl;
//        for(;si < lines[i].size() && lines[i][si] != ']'; si++){
//          unit += lines[i][si];
//        }

//        std::cout << " end unit index = " << si << std::endl;
//      }
//      std:: cout << "unit = " << unit << std::endl;
//    } else {
//      if(lines[i].find("GROUP") != std::string::npos){
//        ig = i;
//      }
//      if( i > ig && ig > 0 && lines[i].size() > 0 &&
//          lines[i][0] == '*' &&
//          lines[i].find("*--") == std::string::npos){
//        break;
//      }
//    }
//  }

//  if(i == lines.size()){
//    std::cout << "not found line \n";
//    res.SetHeader(err_hdr);
//  } else {
//    std::vector<std::string> words;
//    std::cout << " before split: " << lines[i] << std::endl;
//    Split(lines[i], ' ', &words);

//    std::cout << "words size: " << words.size() << std::endl;
//    int col = 0;

//    for(unsigned int j = 0; j < words.size() && col < 4 ; j ++){
//      if(words[j].size() > 1){
//        std::cout << "col " << col << " = " << words[j] << ", ";
//        switch(col){
//          case 1: // LIMIT
//            total = words[j];
//            total.erase(std::remove(total.begin(), total.end(), ','), total.end());
//            break;
//          case 2: // USAGE
//            usage = words[j];
//            usage.erase(std::remove(usage.begin(), usage.end(), ','), usage.end());
//            break;
//          case 3: // AVAIABLE
//            remain = words[j];
//            remain.erase(std::remove(remain.begin(), remain.end(), ','), remain.end());
//            break;
//        }
//        col ++;
//      }
//    }
//    std::cout << "create message: " << unit << ", " << total << ", " << usage << ", " << remain << "\n";
//    res.AddArgv(unit);
//    res.AddArgv(total);
//    res.AddArgv(usage);
//    res.AddArgv(remain);
//  }

//  if(socket.Send(res)){
//    std::cout << "response success\n";
//    return true;
//  } else {
//    std::cout << "[ERR] response success\n";
//    return false;
//  }
//}

void Split(const std::string &str, char del,
                std::vector<std::string> *out) {
  std::stringstream ss(str);
  std::string s;
  while (std::getline(ss, s, del)) {
      out->push_back(s);
  }
}



