#include "main.h"
int env_count = 0;
int alias_count = 0;
int status = 0;
int env_capacity = 60;

int main (int argc, char *argv[], char *envp[])
{
    int i = 0;
    (void)argv;
    (void) argc;

    for (i = 0; envp[i] != NULL; i++)
    {
	 char *name = my_strtok(envp[i], "=");
         char *value = my_strtok(NULL, "");
         add_environment_variable(name, value); 
    }
    if (!isatty(STDIN_FILENO))
        non_interactive_mode((void *) 0);
    else
        interactive_mode();
   return status;
}

void non_interactive_mode(char *filename)
{
	(void)filename; 
    /*
    if(access())
    */
    /*
    int fd_read = open(filename, O_RDONLY);
    */   
    
    
    
}

void interactive_mode(void)
{
        char *line = NULL;
        size_t n = 1024;
        ssize_t bytes_read = 0;

        while (1) 
        {
            print_my_prompt();
            fflush(stdout);
            
            bytes_read = my_get_line(&line, &n); /* Read user input */

            if (bytes_read == -1) 
            {
            /* Error occurred, handle it if needed */
                exit(-1);
            } 
            else if (bytes_read == 0) 
            {
            /* End of file (Ctrl+D) or empty line, exit the shell */
                exit(-1);
            }
            handle_single_line_comment(line);
            handle_multi_line_comment(line);
            parse_cmd_line(line);
        }
    free(line);
}


void print_my_prompt(void)
{
        char cwd[1024]; 
        /* Buffer to hold the current working directory path */

        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s/MyShell> ", cwd);
            /* Print the prompt with the current working directory
            */
        } 
        else 
        {
            perror("getcwd"); 
            /* Error occurred while getting the current working directory */
            printf("MyShell> "); 
            /* Print the default prompt without the current working directory
            */
        }

}

void parse_cmd_line(char *cmd_line)
{
        
        char *token = my_strtok(cmd_line, " ");
        int token_no = 0, i = 0;
        char *tokens[MAX_TOKENS];

        while (token != NULL)
        {
                if (token_no >= MAX_TOKENS)
                {
                        perror("Too many tokens. Some Tokens were not stored.\n");
                        tokens[token_no] = NULL;
                        break;
                }
                tokens[token_no] = my_strdup(token);
                token_no = token_no + 1; 
                token = my_strtok(NULL, " ");
        }
        tokens[token_no] = NULL;
        /* Call parse */
	while(tokens[i])
	{
		printf("%s\n",tokens[i++]);
	}
    parse_command(tokens, token_no);
    
}

void parse_command(char **token, int num_tokens)
{
	int i;
	if (num_tokens <= 0)
	{
		perror("No tokens in array");
		exit(-1);
	}
	if (!check_for_special_characters(token))
	    exec_simple_command(token, &num_tokens);
	
	for (i = 0; i < num_tokens; i++)
	{
		switch (get_token_type(token[i]))
		{
			case  TOKEN_PIPE:
			exec_pipe(token, &i);
			break;
			case TOKEN_REDIRECT_IN:
			exec_redirect_in(token, &i);
			break;
			case  TOKEN_REDIRECT_OUT:
			exec_redirect_out(token, &i);
			break;
			case TOKEN_BACKGROUND:
			exec_background(token, &i);
			break;
			case TOKEN_LOGICAL_AND:
			exec_logical_and(token, &i);
			break;
			case TOKEN_LOGICAL_OR:
			exec_logical_or(token, &i);
			break;
			case  TOKEN_COMMAND_SEPARATOR:
			exec_command_separator(token, &i);
			break;
			
		}
	}
}
