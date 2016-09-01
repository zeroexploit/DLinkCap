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

bool Daemonizer::runExternal(std::string cmd, std::vector<std::string> argvs, bool wait)
{
    std::string path = "/bin/sh";
    std::string sh = "sh";
    std::string cCommand = "-c";
    std::string argList = "";
    
    argvs.insert(argvs.begin(), cmd);
    
    for(int i = 0; i < argvs.size(); i++)
    {
        argList += argvs.at(i);
        
        if(i < argvs.size() - 1)
            argList += " ";
    }
 
    char *argv[] = {&sh[0], &cCommand[0], &argList[0], NULL};
 
    posix_spawn_file_actions_t action;
    posix_spawn_file_actions_init(&action);
    posix_spawn_file_actions_addclose(&action, STDOUT_FILENO);
    posix_spawn_file_actions_addclose(&action, STDIN_FILENO);
    posix_spawn_file_actions_addclose(&action, STDERR_FILENO);
    
    pid_t pid;
    int status = posix_spawn(&pid, &path[0], &action, NULL, argv, environ);

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