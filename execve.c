#include "main.h"
void handle_builtin_command(char **cmd, int token)
{
	if (cmd[0] != NULL) {
        	if (_strcmp(cmd[0], "alias") == 0) {
            		alias_built_in(cmd, token);
        	} else if (_strcmp(cmd[0], "cd") == 0) {
            		cd_built_in(cmd, token);
        	} else if (_strcmp(cmd[0], "setenv") == 0) {
            		setenv_built_in(cmd, token);
        	} else if (_strcmp(cmd[0], "unsetenv") == 0) {
            		unsetenv_built_in(cmd, token);
        	} else if (_strcmp(cmd[0], "echo") == 0) {
            		echo_built_in(cmd, token);
        	} else if (_strcmp(cmd[0], "exit") == 0) {
            		exit_built_in(cmd, token);
        	} else {
            		execute_external_command(cmd, token);
        	}
    	} else {
        /* Handle case where cmd[0] is NULL */
        	perror("No command provided.");
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
    char *command_path = get_command_path(command[0]);
    (void)token;

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
            update_first_token(command, command_path);
            execve(command[0], command, __envp);

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

            /* Save the exit status of the last executed command*/
            status = WEXITSTATUS(status);
        }
	free(command_path);
 
    }
    else
    {
        /* Command not found in PATH */
        perror("Command not found");
    }

}
void update_first_token(char **cmd, char *new_path)
{
    /* Check if cmd is not NULL and has at least one token */
    if (cmd != NULL && cmd[0] != NULL)
    {
        /* Free the existing memory for the first token */
        free(cmd[0]);

        /* Allocate memory for the new path */
        cmd[0] = malloc(my_strlen(new_path) + 1);

        /* Check if memory allocation is successful */
        if (cmd[0] == NULL)
	{
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        /* Copy the new path to the first token */
        my_strcpy(cmd[0], new_path);
    }
    else
    {
        /* Handle the case where cmd is NULL or has no tokens */
        printf("Invalid command tokens.\n");
    }

}
