#include "main.h"
int my_atoi(char* str)
{
    int res = 0, i;
    for (i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';
        
    return res;
}


int shell_exit(int exit_status)
{

    exit(exit_status);
    /* Add any additional cleanup or termination steps if needed
    */
}  

int check_for_special_characters(char **tokens) 
{
    int i;
    for (i = 0; tokens[i] != NULL; i++) 
    {
        char *token = tokens[i];
        if (my_strchr(token, '|') || my_strchr(token, ';') ||
            my_strchr(token, '&') || my_strchr(token, '<') ||
            my_strchr(token, '>') || my_strstr(token, "&&") ||
            my_strstr(token, "||")) 
            {
            return 1; /* Special character found */
        }
    }
    
    return 0; /* No special characters found */
}
