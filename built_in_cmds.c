#include "main.h"
void alias_built_in(char **cmd, int token)
{
	(void) cmd;
	(void) token;
}
void cd_built_in(char **cmd, int token)
{
	(void) cmd;
	(void) token;
}
void setenv_built_in(char **cmd, int token)
{
	(void) cmd;
	(void) token;
}
void unsetenv_built_in(char **cmd, int token)
{
	(void) cmd;
	(void) token;
}
void echo_built_in(char **cmd, int token)
{
	(void) cmd;
	(void) token;
}
void exit_built_in(char **cmd, int token)
{
	(void) token;
	if(cmd[0] && cmd[1])
	{
		status = my_atoi(cmd[1]);
		shell_exit(status);
	}
	status = 0;
        shell_exit(status);
}
