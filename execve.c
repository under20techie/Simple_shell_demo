#include "main.h"
void handle_builtin_command(char **cmd, int token)
{
	switch (cmd[0])
	{
		case "alias":
		alias_built_in(cmd, token);
		break;
		case "cd":
		cd_built_in(cmd, token);
		break;
		case "setenv":
		setenv_built_in(cmd, token);
		break;
		case "unsetenv":
		unsetenv_built_in(cmd, token);
		break;
		case "echo":
		echo_built_in(cmd, token);
		break;
		case "exit":
		exit_built_in(cmd, token);
		break;
		default:
		execute_external_command(cmd, token);
			
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
