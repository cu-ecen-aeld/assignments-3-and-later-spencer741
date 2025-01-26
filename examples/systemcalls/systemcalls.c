#include "systemcalls.h"
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

    int result = system(cmd);
    
    if (result == -1) {
        // system() call failed to execute
        return false;
    }
    
    // Check if the command itself executed successfully
    // system() returns the exit status in the format specified by waitpid()

    // I was able to find wait if exited and wait exit status, which extract
    // the particular return code and return a bool.
    return WIFEXITED(result) && (WEXITSTATUS(result) == 0);

    return true;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    pid_t pid = fork();
    if (pid == -1) 
    {
        return false;  // Fork call failed.
    }
    
    // Child process
    if (pid == 0) {  
        execv(command[0], command);
        // Only reaches here if execv fails.
        // execv will take over the child process.
        exit(1);     
    }
    
    // Parent process
    int status;
    if (waitpid(pid, &status, 0) == -1) 
    {
        return false;
    }
    
    va_end(args);
    return WIFEXITED(status) && (WEXITSTATUS(status) == 0);
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    pid_t pid = fork();
    if (pid == -1) 
    {
        return false;
    }

    // Child process
    if (pid == 0) 
    {  
        // open a file write, create if doesn't exist.

        // 6 (owner): read(4) + write(2)
        // 4 (group): read(4)
        // 4 (others): read(4)
        // It's in octal TODO: research more into this.
        int fd = open(outputfile, O_WRONLY | O_CREAT, 0644);

        if (fd == -1) 
        {
            exit(1);
        }

        if (dup2(fd, STDOUT_FILENO) == -1) {
            close(fd);
            exit(1);
        }
        close(fd);
    
        close(fd);
        execv(command[0], command);
        exit(1);
    }

    int status;
    if (waitpid(pid, &status, 0) == -1) 
    {
        return false;
    }
    
    va_end(args);
    return WIFEXITED(status) && (WEXITSTATUS(status) == 0);
}
