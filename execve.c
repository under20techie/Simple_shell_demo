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
    (void)command;
    (void) token;
    
}
