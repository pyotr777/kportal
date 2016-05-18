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
#include <unistd.h>

//#include "tunnel.hpp"
#include "system_command_utils.hpp"
void * ConnectionHandler(void *thrd_args);

int main (int argc, char **argv) {
  if (argc < 2) {
      std::cout << "kpforwarder <port no>\n";
      return 0;
  }

  // Create a shared tunnel object
  pthread_t sniffer_thread;
  //thrd_args->socket = &new_sock;

  if (pthread_create( &sniffer_thread , NULL ,  ConnectionHandler , NULL) < 0) {
    perror("could not create thread");
  }

  sleep(10);

  return 0;
}


void * ConnectionHandler(void *thrd_args) {
  //ConnectionThreadArg* args = (ConnectionThreadArg*) thrd_args;
  std::string cmd = "ssh -L *:10003:localhost:10003 -v a03320@k.aics.riken.jp server 10003 1>log.txt 2>err.txt";
  std::cout << "cmd = " << cmd <<std::endl;

  std::string stdout = SystemCommandUtils::Exec(cmd.c_str());
  std::cout << "stdout: " << stdout << std::endl;
//  if (system(cmd.c_str()))
//    puts ("Ok");
//  else {
//    puts("Error");
//    exit (EXIT_FAILURE);
//  }
//  printf ("Executing command DIR...\n");

  return NULL;
}

