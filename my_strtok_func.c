#include "main.h"
char *my_strtok(char *s, char *delim) 
{
    static char *last = NULL; 
    /* Keeps track of the previous position */
    char *tok;
    
    if (s == NULL && (s = last) == NULL)
        return NULL;

    /* Skip leading delimiters */
    while (*s && my_strchr(delim, *s))
        s++;

    if (*s == '\0') 
    {
        last = NULL;
        return NULL;
    }

    tok = s;

    /* Find the end of the token */
    while (*s && !my_strchr(delim, *s))
        s++;

    if (*s) 
    {
        *s = '\0'; /* Null-terminate the token */
        last = s + 1; /* Move to the next character after the delimiter
        */
    } 
    else 
    {
        last = NULL;
    }

    return tok;
}

char *my_strdup(char *str) 
{
    size_t len = my_strlen(str) + 1;
    char *dup_str = malloc(len);
    if (dup_str) 
    {
        my_strcpy(dup_str, str);
    }
    else
    {
        perror("malloc");
        exit(0);
    }
    return dup_str;
}
