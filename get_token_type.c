#include "main.h"
int get_token_type (char *token)
{
    int return_value = 0;
    if(_strcmp(token , "&&") == 0)
        return return_value = TOKEN_LOGICAL_AND;
    else if(_strcmp(token, "||") == 0)
        return return_value = TOKEN_LOGICAL_OR;
    switch (*token)
    {
        case '|':
        return_value = TOKEN_PIPE;
        break;
        case '<':
        return_value = TOKEN_REDIRECT_IN;
	break;
        case '>':
        return_value = TOKEN_REDIRECT_OUT;
        break;
        case '&':
        return_value = TOKEN_BACKGROUND;
        break;
        case ';':
        return_value = TOKEN_COMMAND_SEPARATOR;
        break;
        default:
        return_value = TOKEN_UNKNOWN;
    }
   return 0; 
}

void handle_single_line_comment(char* line) 
{
    /* Find the first occurrence of the comment marker #
    */
    char* comment_start = my_strchr(line, '#');

    /* If a comment marker is found, replace it with a null-terminator 
    */
    if (comment_start != NULL)
    {
        *comment_start = '\0';
    }

}
void handle_multi_line_comment(char* line)
{
    /* Find the first occurrence of the comment start marker 
    */
    char* comment_start = my_strstr(line, "/*");

    /**
     * Find the first occurrence of the
     * comment end marker
    */
    char* comment_end = my_strstr(line, "*/");

    /**
    * If both the start and end markers are  
    * found and the start marker appears
    * before the end marker
    */
    if (comment_start != NULL && comment_end != NULL && comment_start < comment_end) {
        /** Calculate the length of the
         * comment (including the end marker)
        */
            size_t comment_length = (comment_end + 2) - comment_start;

        /** Replace the comment with spaces or
         * remove it completely
        */
            my_memset(comment_start, ' ', comment_length);
    }
}
char* my_strstr(char* string, char* substring) 
{
    const char* a;
    const char* b;

    /**First scan quickly through the two
     * strings looking for a
     * single-character match. When it's foun
     * , then compare the
     * rest of the substring.
    */
    b = substring;
    if (*b == '\0') 
    {
        return (char*)string;
    }
    for (; *string != '\0'; string++)
    {
        if (*string != *b) 
        {
            continue;
        }
        a = string;
        while (1) 
        {
            if (*b == '\0') 
            {
                return (char*)string;
            }
            if (*a++ != *b++) 
            {
                break;
            }
        }
        b = substring;
    }
    return NULL;
}
void * my_memset(void* dest, int val, size_t len)
{
    unsigned char* ptr = (unsigned char*)dest;
    while (len-- > 0) 
    {
        *ptr++ = (unsigned char)val;
    }
    return dest;
}
