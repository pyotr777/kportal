#include <argp.h>
#include <stdlib.h>
#include "../kpserver/stdafx.h"
#include "../kpserver/datalayer/DataManager.hpp"

const char *argp_program_version = "kptool 2.0";
const char *argp_program_bug_address = "<long.nm@anlab.vn>";
static char doc[] =
        "Kptool example: ./kptool -s Service1 -u User1 add-user \n program with somewhat more complicated\
        \n \t \t options\
        \vThis part of the documentation comes *after* the options;\
note that the text is automatically filled, but it's possible\
to force a line-break, e.g.\n<-- here.";

// A description of the arguments we accept.
//static char args_doc[] = " add_slaves \n remove_slaves \n view_slaves \n add_providers \n remove_providers \n view_providers \n add_service \n remove_service \n update_service \n view_services \n add_user \n remove_user \n view_users";
//static char args_doc[] = " add_providers \n update_provider \n remove_providers \n view_providers \n";
static char args_doc[] = " add_admins \n remove_admins \n view_admins \n";

// Keys for options without short-options.
#define OPT_ABORT  1            // --abort

// The options we understand.
static struct argp_option options[] = {
    {"Service",     's', "SERVICE",         0, "Service name" },
    {"Username",    'u', "USERNAME",        0, "User name" },
    {"Provider",    'p', "PROVIDER",        0, "Provider" },
    {"Password",    'k', "PASSWORD",        0, "Password" },
    {"Email",       'e', "EMAIL",           0, "Email" },
    { 0 }
};


struct arguments
{
    char *service;                  // service
    char *username;                 // username
    char *command;                  // command
    char *provider;                 // provider
    char *password;                 // password
    char *email;                    // email
    arguments () {
        service = NULL;
        username = NULL;
        command = NULL;
        provider = NULL;
        password = NULL;
        email = NULL;
    }
};

bool checkSyntaxCommand(struct arguments *arg) {
    if (arg->command != NULL) {
        if (strcmp(arg->command,"remove_admins") == 0) {
            if (arg->username != NULL
                    || arg->service != NULL
                    || arg->password != NULL
                    || arg->email == NULL
                    || arg->provider != NULL) {
                return false;
            }
        } else if (strcmp(arg->command,"add_admins") == 0) {
            if (arg->username != NULL
                    || arg->service != NULL
                    || arg->password != NULL
                    || arg->email == NULL
                    || arg->provider != NULL
                    ) {
                return false;
            }
        } else if (strcmp(arg->command,"view_admins") == 0) {
            if (arg->username != NULL
                    || arg->service != NULL
                    || arg->provider != NULL
                    || arg->password != NULL
                    || arg->email != NULL) {
                return false;
            }
//        }  else if (strcmp(arg->command,"update_admin") == 0) {
//            if (arg->username != NULL
//                    || arg->service != NULL
//                    || arg->password != NULL
//                    || arg->email == NULL
//                    || arg->provider != NULL) {
//                return false;
//            }
        }
        return true;
    } else {
        return false;
    }
}

// Parse a single option.
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = (struct arguments *)state->input;

    switch (key)
    {
    case 's':
        if (arguments->service) {            
            argp_usage (state);
        } else {
            arguments->service = arg;
        }
        break;
    case 'u':
        if (arguments->username) {
            argp_usage (state);
        } else {
            arguments->username = arg;
        }
        break;
    
    case 'p':
        if (arguments->provider) {
            argp_usage (state);
        } else {
            arguments->provider = arg;
        }
        break;
    case 'k':
        if (arguments->password) {
            argp_usage (state);
        } else {
            arguments->password = arg;
        }
        break;
    case 'e':
        if (arguments->email) {
            argp_usage (state);
        } else {
            arguments->email = arg;
        }
        break;
    case ARGP_KEY_ARG:
        if (arguments->command) {
            argp_usage (state);
        } else {
            arguments->command = arg;
            if (
//                    strcmp(arguments->command,"remove_providers") != 0
//                    && strcmp(arguments->command,"add_providers") != 0
//                    && strcmp(arguments->command,"view_providers") != 0
//                    && strcmp(arguments->command,"update_provider") != 0
                    strcmp(arguments->command,"remove_admins") != 0
                    && strcmp(arguments->command,"add_admins") != 0
                    && strcmp(arguments->command,"view_admins") != 0
                ) {
                argp_usage (state);
            }
        }
        break;
    case ARGP_KEY_END:
        if (!checkSyntaxCommand(arguments)) {
            argp_usage (state);
        }
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

// Argp parser.
static struct argp argp = { options, parse_opt, args_doc, doc };



int parseArg(std::string str, std::vector<std::string> &listStr)
{
    std::string delimiter = ",";
    std::string s = str;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        listStr.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    if (s.length() > 0) {
        listStr.push_back(s);
    }

    return 0;
}

std::map<std::string,ImageDocker> SampleImages;

int main (int argc, char **argv)
{
    //readConfigFile();
    int ret;
  
    struct arguments arguments;
    argp_parse (&argp, argc, argv, 0, 0, &arguments);

    if (strcmp(arguments.command,"remove_admins") == 0) {
        ResponseCode ret = DATA_ERROR;
        DataManager data_manager(PATH_OF_DATABASE);
        std::vector<std::string> listEmail;
        parseArg(arguments.email, listEmail);
        ret = data_manager.connectDB();
        if ( ret == DATA_SUCCESS) {
            if (data_manager.beginTransaction() == DATA_SUCCESS) {
                for (unsigned int i = 0; i < listEmail.size(); i++) {
                    User admin(listEmail[i], 4);
                    ret = data_manager.deleteUser(admin);
                    if (ret != DATA_SUCCESS) {
                        break;
                    }
                }
                if (ret != DATA_SUCCESS) {
                    data_manager.rollback();
                } else {
                    ret = data_manager.endTransaction();
                }
            }
            if (data_manager.disConnectDB() != DATA_SUCCESS) {
                std::cout << "Error close db when remove admin";
            }
        }
        
    } else if (strcmp(arguments.command,"add_admins") == 0) {
        ResponseCode ret = DATA_ERROR;
        DataManager data_manager(PATH_OF_DATABASE);
        std::vector<std::string> listEmail;
        parseArg(arguments.email, listEmail);
        ret = data_manager.connectDB();
        if ( ret == DATA_SUCCESS) {
            if (data_manager.beginTransaction() == DATA_SUCCESS) {
                for (unsigned int i = 0; i < listEmail.size(); i++) {
                    User admin(listEmail[i], 4);
                    ret = data_manager.insertUser(admin);
                    if (ret != DATA_SUCCESS) {
                        break;
                    }
                }
                if (ret != DATA_SUCCESS) {
                    data_manager.rollback();
                } else {
                    ret = data_manager.endTransaction();
                }
            }
            if (data_manager.disConnectDB() != DATA_SUCCESS) {
                std::cout << "Error close db when insert admin";
            }
        }
        
    } else if (strcmp(arguments.command,"view_admins") == 0) {
        ResponseCode ret = DATA_ERROR;
        DataManager data_manager(PATH_OF_DATABASE);
        ret = data_manager.connectDB();
        if ( ret == DATA_SUCCESS) {
            std::vector<User> listAdmin;
            ret = data_manager.getAllAdmin(listAdmin);
            if (ret == DATA_SUCCESS) {
                if (listAdmin.size() > 0) {
                    std::cout << "***Admins on kportal***\n";
                    for (unsigned int i =0; i < listAdmin.size(); i++) {
                        std::cout << listAdmin[i].getEmail() << std::endl;
                    }
                    std::cout << "*************************\n";
                } else {
                    std::cout << "Number admin is 0\n";
                }
            } else {
                std::cout << "Error open db when view admins";
            }
            if (data_manager.disConnectDB() != DATA_SUCCESS) {
                std::cout << "Error close db when view admins\n";
            }
        }
       
//    } else if (strcmp(arguments.command,"update_provider") == 0) {
//        ResponseCode ret = DATA_ERROR;
//        DataManager data_manager(PATH_OF_DATABASE);
//        ret = data_manager.connectDB();
//        if ( ret == DATA_SUCCESS) {
//            if (data_manager.beginTransaction() == DATA_SUCCESS) {
//                Provider prov(arguments.email);
//                prov.setProviderID(arguments.provider);
//                ret = data_manager.updateProvider(prov);
//                ret = data_manager.endTransaction();
//            }
//            if (data_manager.disConnectDB() != DATA_SUCCESS) {
//                std::cout << "Error close db when update provider\n";
//            }
//        }
    }
    exit (0);
}
