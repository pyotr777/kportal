#include "system_command_utils.hpp"

std::string SystemCommandUtils::Exec(const char* cmd) {
  std::string data;
  FILE * stream;
  const int max_buffer = 256;
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

pid_t SystemCommandUtils::System(const char * command, int * infp, int * outfp)
{

    int p_stdin[2];
    int p_stdout[2];
    pid_t pid;

    if (pipe(p_stdin) == -1)
        return -1;

    if (pipe(p_stdout) == -1) {
        close(p_stdin[0]);
        close(p_stdin[1]);
        return -1;
    }

    pid = fork();

    if (pid < 0) {
        close(p_stdin[0]);
        close(p_stdin[1]);
        close(p_stdout[0]);
        close(p_stdout[1]);
        return pid;
    } else if (pid == 0) {
        close(p_stdin[1]);
        dup2(p_stdin[0], 0);
        close(p_stdout[0]);
        dup2(p_stdout[1], 1);
        dup2(::open("/dev/null", O_RDONLY), 2);
        /// Close all other descriptors for the safety sake.
        for (int i = 3; i < 4096; ++i)
            ::close(i);

        setsid();
        execl("/bin/sh", "sh", "-c", command, NULL);
        _exit(1);
    }

    close(p_stdin[0]);
    close(p_stdout[1]);

    if (infp == NULL) {
        close(p_stdin[1]);
    } else {
        *infp = p_stdin[1];
    }

    if (outfp == NULL) {
        close(p_stdout[0]);
    } else {
        *outfp = p_stdout[0];
    }

    return pid;
}
