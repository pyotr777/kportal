#ifndef TUNNEL_MANAGER_HPP
#define TUNNEL_MANAGER_HPP
#include <map>
#include <iostream>
#include <sstream>
#include "system_command_utils.hpp"
class TunnelManager {
private:
public:
    static unsigned int tunnel_next_port;
    static int exit_code;
    TunnelManager();
    void ConfigureTunnel(std::string kacc, unsigned int & port );
    static void * SetupTunnel(void *thrd_args);
};

#endif
