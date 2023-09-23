#include "main.h"
void handle_builtin_command(char **cmd, int token)
{
    char **command = cmd;

    if (_strcmp(command[token - 1], "exit") == 0)
    {
        /** Handle exit command
         *  You may perform any necessary
         * cleanup here before exiting.
         */
         shell_exit(my_atoi(command[token]));
         
    }
    else if (_strcmp(command[token - 1], "alias") == 0) 
    {
        /* Handle alias command */
        if (command[token - 1] && command[token])
        {
            add_alias(command[token - 1], command[token]);
        } 
	else
	{
            list_aliases();
        }
    }
    else if (_strcmp(command[token - 1], "unalias") == 0) 
    {
        /* Handle unalias command  */
        if (command[token])
	{
            remove_alias(command[token]);
        }
    } 
    else if (_strcmp(command[token - 1], "cd") == 0) 
    {
        /* Handle cd command */
        if (command[token])
        {
            if (chdir(command[token]) == -1)
            {
                perror("cd");
            }
        }
        else 
        {
            perror("cd: Missing argument");
        }
    }
    else if (_strcmp(command[token - 1], "setenv") == 0) 
    {
        /* Handle setenv command */
        if (command[token - 1] && command[token]) 
        {
            set_environment_variable(command[token - 1], command[token]);
        } 
        else 
        {
            perror("setenv: Invalid arguments");
        }
    }
    else if (_strcmp(command[token - 1], "unsetenv") == 0)
    {
        /* Handle unsetenv command */
        if (command[token]) 
        {
            unset_environment_variable(command[token]);
        }
        else 
        {
            perror("unsetenv: Missing argument");
        }
    }
    else 
    {
        /* Not a built-in command */
        execute_external_command(command, token);
    }
}

void execute_command(char **cmd, int token)
{
        handle_builtin_command(cmd,  token);
}


void execute_external_command(char **command, int token)
{
    /** Resolve the command path using the
     * provided function
    */
    char *command_path = get_command_path(command[token - 1]);
    int status = 0;
    if (command_path) 
    {
        /** Fork a child process to execute
         *  the command
        */
        pid_t pid = fork();
        if (pid < 0) 
        {
            perror("fork");
        } 
        else if (pid == 0) 
        {
            /** Child process: execute the
             *  command
             */
            execve(command_path, command, environ);

            /** If execve returns, there was
             * an error
             */
            perror("execve");
            exit(EXIT_FAILURE);
        } 
        else 
        {
            /* Parent process: wait for the child to complete
            */
            waitpid(pid, &status, 0);

            /* Save the exit status of the last executed command */
            shell_exit(WEXITSTATUS(status));
        }

        free(command_path);
    }
    else 
    {
        /* Command not found in PATH */
        perror("Command not found");
    }
    
}
