#include "GlobalsVars.hpp"
//#include "Objects/KP_ParametersList.hpp"
#include "KP_Manager.hpp"
#include "StringUtils.hpp"

using namespace std;

enum Options {NONE, SERVICE_ID, RUN_METHOD_ID};

void sender(boost::shared_ptr<KP_Manager> & man, string serviceId, string methodId, KP_ParametersList params)
{
    string threadId;
    unsigned int threadIdUint = pthread_self();
    threadId = StringUtils::UintToStr(threadIdUint);
    cout << "Bp: threadId: caller:"<<threadId << endl;
    try
    {
        string * id = man -> submitJob(serviceId, methodId, &params);

        if(id != NULL)
        {
            cout << "JobId:" << (*id) << ". SubmitJob success!." << endl;
            delete id;
        }
        else
            cout << "SubmitJob fail!\n";
    }
    catch (KP_Exception *ex)
    {
        cout << "Exception:" << ex->getMessage() << endl;
        delete ex;
    }
}

int main(int argc, char*argv[])
{
    std::string userID = "tuan.np@anlab.info";
    std::string pwd = "nguyenphutuan";
	std::string uri = "ws://localhost:9003/kportal";
    if(argc >= 2)
        uri = "ws://" + string(argv[1]) + "/kportal";
    else if(argc >= 3)
    {
        if(argv[2] == "-tls")
        {
            uri = "wss://" + string(argv[1]) + "/kportal";
        }
    }
//    try
//    {
        KP_ManagerMC(man, userID, pwd);
//    }
//    catch (KP_Exception * e)
//    {
//        std::cout << "Exception: " << e -> getMessage() << endl;
//        delete e;
//    }
    while(true)
    {
        char cmd [1024];
        cout << "Command>";
        cin.getline(cmd, 1024);


        std::vector<std::string> scmds;
        char space = ' ';
        char startChar = '(';
        char endChar = ')';

        unsigned int size = StringUtils::Split(cmd, scmds, space, startChar, endChar);

        //cout << "Size:" << size << ", vecter.size:" << scmds.size() << endl;

        string command = "";
        if(scmds.size() > 0)
            command = scmds[0];

        if(command == "\n" || command == "")
        {
            cout << "\n";
            continue;
        }

        if(command == "close" || command == "/c")
        {
            man -> close();
            break;
        }

        try
        {
            if(command == "authorize"|| command == "/a")
            {
                cout << "UserId:"; cin >> userID;

                char  *pass;
                pass = getpass("Enter your password:");
                //printf("\nThe password you entered:%s\n",pass);

                pwd = string(pass);

                if(userID != "" && pwd != "")
                {
                    man -> setUserId(userID);
                    man -> setPwd(pwd);
                    man -> setUri(uri);
                    if(man -> authorize())
                        cout << "Success!\n";
                    else
                        cout << "Fail!\n";
                }
            }

            else if(command == "logout" || command == "/l")
            {
                if(man -> logout())
                    cout << "Success!\n";
                else
                    cout << "Fail\n";
            }

//          else if(command == "submitJob" || command == "/sj")
//            {
//                string serviceId, methodId;
//                Options op = NONE;
//                KP_ParametersList params = KP_ParametersList();
//                int i = 1;
//                int iParam = 0;
//                while(i < scmds.size())
//                {
//                    //cout << scmds[i] << endl;
//                    if(op != NONE)
//                    {
//                        if(op == SERVICE_ID)    serviceId = scmds[i];
//                        if(op == RUN_METHOD_ID)    methodId = scmds[i];
//                        op = NONE;
//                    }
//                    else if(scmds[i] == "-s") op = SERVICE_ID;
//                    else if(scmds[i] == "-r") op = RUN_METHOD_ID;
//                    else
//                    {
//                        string istr = StringUtils::IntToStr(++iParam);
//                        params.add(scmds[i], istr);
//                    }
//                    ++i;
//                }
//                cout << "ServiceId:" << serviceId << ", methodId:" << methodId << endl;
//                if(serviceId == "" || methodId == "")
//                {
//                    cout << "Invalid command! \nSystax: submitJob -s serviceId -r runMethodId [param1] [param2] ...\n";
//                    continue;
//                }

//                /// Just test  ->>

////                string * id = man -> submitJob(serviceId, methodId, &params);

////                if(id != NULL)
////                    cout << "JobId: " << id << endl;
////                else
////                    cout << "SubmitJob fail\n";

////                continue;

//                /// -->>> Just test

//                KP_Service* service = man -> getService(serviceId);

//                if(service != NULL)
//                {
//                    KP_Job * job = NULL;
//                    job = service -> submitJob(methodId, &params);
//                    //cout << "Bp: job: " << endl;
//                    if(job != NULL)
//                    {
//                        string jobId = *job->getJobId();
//                        if(jobId != "0")
//                            cout << "JobId: " << jobId << " : Success!\n";
//                        else
//                            cout << "Fail!\n";
//                    }
//                    delete service;
//                    service = NULL;
//                }

//            }


            else if(strcmp(command.data(), "submitJob") == 0 || strcmp(command.data(), "/sj") == 0)
            {
                string serviceId, methodId;
                string type = "", parId = "", jobId = "",inputPath = "", entryPath = "";
                Options op = NONE;
                KP_ParametersList params = KP_ParametersList();
                int i = 1;
                while(i < scmds.size())
                {
                    //cout << scmds[i] << endl;
                    if(op != NONE)
                    {
                        if(op == SERVICE_ID)    serviceId = scmds[i];
                        if(op == RUN_METHOD_ID)    methodId = scmds[i];
                        op = NONE;
                    }
                    else if(scmds[i] == "-s") op = SERVICE_ID;
                    else if(scmds[i] == "-r") op = RUN_METHOD_ID;
                    else
                    {
                        string scm = scmds[i];
                        unsigned int from =  scm.find("(");
                        unsigned int to = scm.find(")");
                        if (from != std::string::npos
                                && to != std::string::npos
                                && from != UINT_MAX
                                && to != UINT_MAX
                                && from < to)
                        {
                            //cout << "from : " << from << ", to : " << to << endl;

                            string str = scm.substr(from + 1,to - 1);
                            //cout << "group: " << str << endl;

                            vector<string> strs;
                            StringUtils::Split(str, strs, ',');
//                            for(int j = 0 ; j < strs.size() ; j ++)
//                            {
//                                cout << "strs[" << i << "]:" << strs[j] << endl;
//                            }

                            //cout << "Bp: strs.size():" << strs.size() << endl;
                            if(strs.size() == 1)
                            {
                                cout << "BP: add Option parameter" << endl;
                                params.add(strs[0]);
                            }
                            else if(strs.size() == 2)
                            {
                                params.add(strs[1], strs[0]);
                            }
                            else if(strs.size() > 2)
                            {
                                type = strs[0]; parId = strs[1];
                                if(parId == "")
                                {
                                    cout << "Id parameter have to NOT EMPTY\n";
                                }
                                else if(strcmp(type.data(), "local") == 0)
                                {
                                    entryPath = strs[2];
                                    if(entryPath != "")
                                    {
                                        KP_Entry entry1(entryPath, LOCAL);
                                        params.add(entry1, parId);
                                    }
                                    else
                                    {
                                        cout << "Invalid: JobId and EntryPath have to different EMPTY!\n";
                                    }
                                }
                                else if(strcmp(type.data(), "remote") == 0)
                                {
                                    if(strs.size() > 3)
                                    {
                                        jobId = strs[2]; entryPath = strs[3];
                                        if(jobId != "" && entryPath != "")
                                        {
                                            KP_Entry entry2(jobId, entryPath, REMOTE);
                                            params.add(entry2, parId);
                                        }
                                        else
                                        {
                                            cout << "Invalid: JobId and EntryPath have to NOT EMPTY\n";
                                        }
                                    }
                                    else
                                    {
                                        cout << "Bp: strs.size():" << strs.size() << endl;
                                    }
                                }
                                else
                                {
                                    cout << "Unknow type:\"" << type << "\".\nCorrect type is: local, remote\n";
                                }
                                parId = ""; inputPath = ""; jobId = ""; entryPath = "";
                            }
                        }
                    }
                    ++i;
                }
                /// Just test  ->>

                string * id = man -> submitJob(serviceId, methodId, &params);

                /// ->> Just test

                if(id != NULL)
                {
                    cout << "JobId:" << (*id) << ". SubmitJob success!." << endl;
                    delete id;
                }
                else
                    cout << "SubmitJob fail!\n";

                continue;

            }

            else if(strcmp(command.data(), "submitJob1") == 0 || strcmp(command.data(), "/sj1") == 0)
            {
                string serviceId, methodId;
                string type = "", parId = "", jobId = "",inputPath = "", entryPath = "";
                Options op = NONE;
                KP_ParametersList params = KP_ParametersList();
                int i = 1;
                while(i < scmds.size())
                {
                    //cout << scmds[i] << endl;
                    if(op != NONE)
                    {
                        if(op == SERVICE_ID)    serviceId = scmds[i];
                        if(op == RUN_METHOD_ID)    methodId = scmds[i];
                        op = NONE;
                    }
                    else if(scmds[i] == "-s") op = SERVICE_ID;
                    else if(scmds[i] == "-r") op = RUN_METHOD_ID;
                    else
                    {
                        string scm = scmds[i];
                        unsigned int from =  scm.find("(");
                        unsigned int to = scm.find(")");
                        if (from != std::string::npos
                                && to != std::string::npos
                                && from != UINT_MAX
                                && to != UINT_MAX
                                && from < to)
                        {
                            //cout << "from : " << from << ", to : " << to << endl;

                            string str = scm.substr(from + 1,to - 1);
                            //cout << "group: " << str << endl;

                            vector<string> strs;
                            StringUtils::Split(str, strs, ',');
//                            for(int j = 0 ; j < strs.size() ; j ++)
//                            {
//                                cout << "strs[" << i << "]:" << strs[j] << endl;
//                            }

                            //cout << "Bp: strs.size():" << strs.size() << endl;
                            if(strs.size() == 1)
                            {
                                cout << "BP: add Option parameter" << endl;
                                params.add(strs[0]);
                            }
                            else if(strs.size() == 2)
                            {
                                params.add(strs[1], strs[0]);
                            }
                            else if(strs.size() > 2)
                            {
                                type = strs[0]; parId = strs[1];
                                if(parId == "")
                                {
                                    cout << "Id parameter have to NOT EMPTY\n";
                                }
                                else if(strcmp(type.data(), "local") == 0)
                                {
                                    entryPath = strs[2];
                                    if(entryPath != "")
                                    {
                                        KP_Entry entry1(entryPath, LOCAL);
                                        params.add(entry1, parId);
                                    }
                                    else
                                    {
                                        cout << "Invalid: JobId and EntryPath have to different EMPTY!\n";
                                    }
                                }
                                else if(strcmp(type.data(), "remote") == 0)
                                {
                                    if(strs.size() > 3)
                                    {
                                        jobId = strs[2]; entryPath = strs[3];
                                        if(jobId != "" && entryPath != "")
                                        {
                                            KP_Entry entry2(jobId, entryPath, REMOTE);
                                            params.add(entry2, parId);
                                        }
                                        else
                                        {
                                            cout << "Invalid: JobId and EntryPath have to NOT EMPTY\n";
                                        }
                                    }
                                    else
                                    {
                                        cout << "Bp: strs.size():" << strs.size() << endl;
                                    }
                                }
                                else
                                {
                                    cout << "Unknow type:\"" << type << "\".\nCorrect type is: local, remote\n";
                                }
                                parId = ""; inputPath = ""; jobId = ""; entryPath = "";
                            }
                        }
                    }
                    ++i;
                }
                /// Just test  ->>
                boost::thread thrd1(&sender, man, serviceId, methodId, params);
                boost::thread thrd2(&sender, man, serviceId, methodId, params);
                boost::thread thrd3(&sender, man, serviceId, methodId, params);
                string * id = man -> submitJob(serviceId, methodId, &params);
                thrd1.join();
                thrd2.join();
                thrd3.join();
                /// ->> Just test

                if(id != NULL)
                    cout << "JobId:" << (*id) << ". SubmitJob success!." << endl;
                else
                    cout << "SubmitJob fail!\n";

                continue;

            }
            else if(command == "getService" || command == "/gs")
            {
                string serviceId = "";
                if(scmds.size() > 1)
                    serviceId = scmds[1];

                KP_Service* service = man -> getService(serviceId);
                if(service != NULL)
                    cout << "ServiceId: " << service -> getServiceId() << ", providerId: " << service -> getProviderId() << endl;
                else
                    cout << "GetService: " << serviceId << " not found!\n";
            }

            else if(command == "findServices" || command == "/fs")
            {
                string providerId = "";
                if(scmds.size() > 1)
                    providerId = scmds[1];

                list<KP_Service> services = man -> findServices(providerId);                
                cout << "There are " << services.size() << " services.\n";
            }

            else if(command == "getRunMethodsFile" || command == "/grmf")
            {
                string serviceId = "";
                if(scmds.size() > 1)
                    serviceId = scmds[1];

                string path = "";
                if(scmds.size() > 2)
                    path = scmds[2];
                if(serviceId == "" || path == "")
                {
                    cout << "Systax: getRunMethodsFile <serviceId> <path/to/save/file>\n";
                    continue;
                }


                if(serviceId != "" && path != "")
                {
                    if(man -> getRunMethodsFile(serviceId, path))
                        cout << "Save file \"" << path << "\" sucess!\n";
                    else
                        cout << "getRunMethodsFile fail\n";
                }
            }

            else if(command == "queryStatus" || command == "/qs")
            {
                string jobId = "";
                if(scmds.size() > 1)
                    jobId = scmds[1];
                if(jobId == "")
                {
                    cout << "Systax: queryStatus  <jobId>\n";
                    continue;
                }
                KP_JobStatus status = man -> queryStatus(jobId);
                if(status == JOB_UNKNOW)
                    cout << "queryStatus fail!\n";
                else
                    cout << "JobId:" << jobId << ", status: " << KP_Job::StatusToString(status) << endl;
            }

            else if(command == "getOutputFile" || command == "/gof")
            {
                /// Just test  ->>
                string jobIdtest = "";
                if(scmds.size() > 1)
                    jobIdtest = scmds[1];

                string entryPathtest = "";// cout << "EntryPath: "; cin >> entryPath;
                if(scmds.size() > 2)
                    entryPathtest = scmds[2];

                string localPathtest = "";
                if(scmds.size() > 3)
                    localPathtest = scmds[3];

                if(jobIdtest == "" || entryPathtest == "" || localPathtest == "")
                {
                    cout << "Systax: getOutputFile <JobId>  <entryFile> <path/to/save/file>\n";
                    continue;
                }

                bool isSucess1 = man -> getOutputFile(jobIdtest, entryPathtest, localPathtest);
                if(isSucess1)
                    cout << "Received file:"<< entryPathtest << ", save to: " << localPathtest << " success!" << endl;
                else
                    cout << "getOutputFile fail !\n";
                continue;
                /// -->>> Just test

                string serviceId = "Service2";
                if(scmds.size() > 1)
                    serviceId = scmds[1];

                //string jobId; cout << "JobjId: ";   cin >> jobId;
                string methodId = "1";
                if(scmds.size() > 2)
                    methodId = scmds[2];

                string entryPath = "output.txt";// cout << "EntryPath: "; cin >> entryPath;
                if(scmds.size() > 3)
                    entryPath = scmds[3];

                string localPath = "/home/tuannp/tmp/outputFile.txt";
                if(scmds.size() > 4)
                    localPath = scmds[4];

                bool isSucess = false;

                //isSucess = man -> getOutputFile(string("jobId"), entryPath, localPath); // just test

                KP_Service * service = NULL;
                service = man -> getService(serviceId);
                if(service != NULL)
                {
                    KP_Job *job = NULL;

                    KP_ParametersList* params = new KP_ParametersList();
                    std::string param1 = "5000";
                    std::string param2 = ">";
                    std::string param3 = "output.txt";
                    params->add(param1, "1");
                    params->add(param2, "2");
                    params->add(param3, "3");
                    job = service->submitJob(methodId, params);

                    if(job != NULL)
                    {
                        isSucess = job -> getOutputFile(entryPath, localPath);
                    }
                    delete params;
                }
                if(isSucess)
                    cout << "Received file:"<< entryPath << ", save to: ./" << entryPath << endl;
                else
                    cout << "Fail !\n";
            }

            else if(command == "jobList" || command == "/jl")
            {
                std::list<KP_Job> jobs =  man -> jobList();

                for (std::list<KP_Job>::iterator it = jobs.begin(); it != jobs.end(); it ++)
                {
                    std::cout << "JobId: " << (*it).getJobId() << endl;
                }
                cout << "There are " << jobs.size() << " jobs.\n";
            }

            else if(command == "deleteJob" || command == "/dj")
            {
                string jobId = "";
                if(scmds.size() > 1)
                    jobId = scmds[1];

                KP_JobStatus status =  man -> deleteJob(jobId);
                if(status == JOB_UNKNOW)
                    cout << "DeleteJob fail!\n";
                else
                {
                    cout << "JobId:" << jobId << ", status: " << KP_Job::StatusToString(status) << endl;
                    cout << "DeleteJob success!\n";
                }
            }

            else if(command == "restartJob" || command == "/rj")
            {
                string jobId = "";
                if(scmds.size() > 1)
                    jobId = scmds[1];
                KP_JobStatus status =  man -> restartJob(jobId);
                //cout << "JobId:" << jobId << ", status: " << status << endl;

                if(status == JOB_UNKNOW)
                    cout << "RestartJob fail!\n";
                else
                {
                    cout << "JobId:" << jobId << ", status: " << KP_Job::StatusToString(status) << endl;
                    cout << "RestartJob success!\n";
                }
            }

            else if(command == "cancelJob" || command == "/cj")
            {
                string jobId = "";
                if(scmds.size() > 1)
                    jobId = scmds[1];
                KP_JobStatus status =  man -> cancelJob(jobId);
                //cout << "JobId:" << jobId << ", status: " << status << endl;

                if(status == JOB_UNKNOW)
                    cout << "CancelJob fail!\n";
                else
                {
                    cout << "JobId:" << jobId << ", status: " << KP_Job::StatusToString(status) << endl;
                    cout << "CancelJob success!\n";
                }
            }

            else
            {
                cout << "Invalid command!\n" << command << endl;
            }
        }
        catch (KP_Exception * e)
        {
            std::cout << "Exception: " << e -> getMessage() << endl;
            delete e;
        }
    }

    t.join();
	return 0;
}
