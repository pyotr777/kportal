#ifndef KP_JOB_HPP
#define KP_JOB_HPP

#include "MessageHeader.hpp"

enum KP_JobStatus {
    JOB_UNKNOW,
    JOB_FINISH,
    JOB_RUNNING,
    JOB_START,
    JOB_SUSPEND,
    JOB_ERROR,
    JOB_ERROR_INPUT_NOT_EXIST,
    JOB_QUEUE
};

//class KP_Job
//{
//public:
//    KP_Job()
//    {
//        this -> _jobId = "";
//        this -> _status = JOB_UNKNOW;
//        this -> _path = "";
//        this -> files.clear();
//    }

//    KP_Job(const KP_Job& copy)
//    {
//        this -> _jobId = copy._jobId;
//        this -> jobName = copy.jobName; //EH0001
//        this -> _status = copy._status;
//        this -> _path = copy._path;
//        this -> files.assign(copy.files.begin(), copy.files.end());
//        this -> _header = copy._header;
//        this -> timeStartJob = copy.timeStartJob; //EH0001
//        this -> timeEndJob = copy.timeEndJob; //EH0001
//        this -> duration = copy.duration; //EH0001
//        this -> runMethodId = copy.runMethodId; //EH0001
//        this -> params = copy.params; //EH0001
//        this -> jSonString = copy.jSonString;

//        //EH0011
//        this -> numberOfNode = copy.numberOfNode;
//        this -> maxElapsedTime = copy.maxElapsedTime;
//        //Eh0011 end
//    }

//    KP_Job(std::string jobId, KP_JobStatus status){
//        this -> _jobId = jobId;
//        this -> _status = status;
//    }

//    KP_Job(std::string jobId){
//        this -> _jobId = jobId;
//        this -> _status = JOB_UNKNOW;
//    }

//    std::string getJobId(){ return _jobId;  }

//    void setJobId(std::string jobId) { this -> _jobId = jobId;  }

//    KP_JobStatus getStatus(){ return _status; }

//    void setStatus(KP_JobStatus status){ _status = status; } //EH0001

//    std::string getPath() { return _path; }

//    void setPath(std::string path) { this -> _path = path;  }

//    std::vector<string>* getFiles() { return &files;    }

//    MessageHeader getHeader() { return _header; }

//    void setHeader(MessageHeader& header) { this -> _header = header; }

//    //EH0001
//    std::string getTimeStartJob() { return timeStartJob; }
//    void setTimeStartJob(std::string time) { this -> timeStartJob = time;  }

//    std::string getTimeEndJob() { return timeEndJob; }
//    void setTimeEndJob(std::string time) { this -> timeEndJob = time;  }

//    std::string getJobName() { return jobName; }
//    void setJobName(std::string name) { this -> jobName = name;  }

//    std::string getDuration() { return duration; }
//    void setDuration(std::string time) { this -> duration = time;  }

//    //std::string getRunmethodId() { return runMethodId; }
//    //void setRunmethodId(std::string id) { this -> runMethodId = id;  }

//    std::string getParams() { return params; }
//    void setParams(std::string param) { this -> params = param;  }

//    std::string getJsonString() { return jSonString; }
//    void setJsonString(std::string str) { this -> jSonString = str;  }
//    //EH0001 end

//    //EH0011
//    int getNumberOfNode() { return numberOfNode; }
//    void setNumberOfNode(int n) { this -> numberOfNode = n;  }

//    int getMaxElapsedTime() { return maxElapsedTime; }
//    void setMaxElapsedTime(int n) { this -> maxElapsedTime = n;  }
//    //EH0011 end
//private:
//    std::string _jobId;
//    KP_JobStatus _status;
//    std::string _path;
//    std::vector<string> files;
//    MessageHeader _header;
//    //EH0001
//    std::string timeStartJob;
//    std::string timeEndJob;
//    std::string jobName;
//    std::string duration;
//    std::string runMethodId;
//    std::string params;
//    std::string jSonString;
//    //EH0001 end

//    //Eh0011
//    int numberOfNode;
//    int maxElapsedTime;
//    //Eh0011 end
//};

#endif // KP_JOB_HPP

