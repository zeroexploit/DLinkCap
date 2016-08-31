#include "include/daemonizer.h"

extern char **environ;

/**
 * Daemonize this Application using fork(). 
 * 
 * On Error this Function will shutdown the entire Application!
 * This Function will terminate the (calling) Main Process and close all
 * STD I/O Operations!
 * 
 * @return true if done
 */
bool Daemonizer::daemonize(void)
{
    pid_t pid, sid;
  
    pid = fork();

    if (pid < 0) 
    {
        std::cout << "Failed to created Child Process!\n";
        exit(EXIT_FAILURE);
    }

    if (pid > 0) 
    {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();

    if (sid < 0) 
    {
        std::cout << "Failed to get Session ID for Child Process!\n";
        exit(EXIT_FAILURE);
    }
    
    if((chdir("/")) < 0) 
    {
        std::cout << "Failed to change the working Directory!\n";
        exit(EXIT_FAILURE);
    }
    
    std::cout << "Process Daemonized...\n";
    
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    return true;
}

bool Daemonizer::runExternal(const char* cmd, std::vector<const char*> argvs, bool wait)
{
    argvs.insert(argvs.begin(), cmd);
    char* argv[argvs.size() + 1];
    std::copy(argvs.begin(), argvs.end(), argv);
    argv[argvs.size()] = NULL;
    
    pid_t pid;
    int status = posix_spawn(&pid, cmd, NULL, NULL, argv, environ);

    if(status == 0) 
    {
        if(wait)
        {
            if(waitpid(pid, &status, 0) != -1) 
                return true;
            else 
                return false;
        }
        
        return true;
    } 
    
    return false;
}