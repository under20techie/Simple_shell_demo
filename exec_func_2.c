#include "main.h"
/* Implement set_logical_and */
void exec_logical_and(char **token, int *token_ptr) 
{
    if (token[*token_ptr - 1] && token[*token_ptr + 1])
    {
        if (token[*token_ptr - 1])
        {
            execute_command(token, *token_ptr - 1);
            (*token_ptr)++;
            if (status != 0)
            {
                (*token_ptr)++;
                shell_exit(status);
            }
        }
        else if (token[*token_ptr] && !status)
        {
            execute_command(token, *token_ptr);
            (*token_ptr)++;
        }
        else
        {
            shell_exit(status);
        }
    }
    
}

/* Implement set_logical_or */
void exec_logical_or(char **token, int *token_ptr) 
{
    if (token[*token_ptr - 1] && token[*token_ptr + 1])
    {
        if (token[*token_ptr - 1])
        {
            execute_command(token, *token_ptr - 1);
            (*token_ptr)++;
            if (status != 0)
            {
                (*token_ptr)++;
                shell_exit(status);
            }
        }
        else if (token[*token_ptr] && !status)
        {
            execute_command(token, *token_ptr);
            (*token_ptr)++;
        }
        else
        {
            shell_exit(status);
        }
    }
    
}

/* Implement set_command_separator */
void exec_command_separator(char **token, int *token_ptr) 
{
    if (token[*token_ptr - 1] && token[*token_ptr + 1])
    {
        if (token[*token_ptr - 1])
        {
            execute_command(token, *token_ptr - 1);
            (*token_ptr)++;
            
        }
        else if (token[*token_ptr])
        {
            execute_command(token, *token_ptr);
            (*token_ptr)++;
        }
        
    }
      
}
void exec_simple_command (char **cmd, int *token)
{
    execute_command(cmd, *token);

}
