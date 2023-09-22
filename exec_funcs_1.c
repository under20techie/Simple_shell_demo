#include "main.h"
void exec_pipe(char **token, int *token_ptr) 
{
   (void) token;
   (void) token_ptr;
	
;

}

/* Implement set_redirect_in */
void exec_redirect_in(char **token, int *token_ptr) 
{

 if (token[*token_ptr - 1] && token[*token_ptr + 1])
    {
        
        if (token[*token_ptr - 1])
        {
            execute_command(token, *token_ptr - 1);
            (*token_ptr)++;
        }

}

}

/* Implement set_redirect_out */
void exec_redirect_out(char **token, int *token_ptr) 
{
    
    if (token[*token_ptr - 1] && token[*token_ptr + 1])
    {
        
        if (token[*token_ptr - 1])
        {
            execute_command(token, *token_ptr - 1);
            (*token_ptr)++;
        }
    
    
    }
    
}

/* Implement set_background */
void exec_background(char **token, int *token_ptr) 
{
    
    (void) token;
    (void) token_ptr;
    
    
}
