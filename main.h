#ifndef _MAIN_H_
#define _MAIN_H_
#define MAX_TOKENS 64
#define BUF_SIZE 1024
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

extern char **environ;
// Enumeration for command types
typedef enum {
    CMD_UNKNOWN,
    CMD_SIMPLE,
    CMD_PIPE,
    CMD_REDIRECT_IN,
    CMD_REDIRECT_OUT,
    CMD_BACKGROUND,
    CMD_LOGICAL_AND,
    CMD_LOGICAL_OR,
    CMD_COMMAND_SEPARATOR
} CommandType;

typedef enum {
    TOKEN_UNKNOWN,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_BACKGROUND,
    TOKEN_LOGICAL_AND,
    TOKEN_LOGICAL_OR,
    TOKEN_COMMAND_SEPARATOR,
    TOKEN_OPEN_BRACKET,
    TOKEN_CLOSE_BRACKET
} TokenType;

// Structure for simple commands
typedef struct SimpleCommand {
    CommandType type;
    char *command;
    char **arguments;
    char *filename;
} SimpleCommand;


typedef struct {
    char *name;
    char *value;
} EnvironmentVariable;

ssize_t my_get_line(char **lineptr, size_t *n);
void print_my_prompt (void);
char *my_strchr(const char *str, int ch);
char *my_strtok(char *str, const char *delim);
char *my_strdup(const char *str);
int  get_token_type(const char *token);
void *my_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
int _strcmp(char *s1, char *s2);

//The command-line parser
void *parse_command(char **token, int num_tokens,  SimpleCommand *cmds[]);
void parse_cmd_line(char *cmd_line);

// Updating  the complexCommand tree
void set_simple_command(char **token, int *token_ptr, SimpleCommand *cmd);
void set_pipe(char **token, int *token_ptr, SimpleCommand *cmd);
 void set_redirect_in(char **token, int *token_ptr, SimpleCommand *cmd);
void set_redirect_out(char **token, int *token_ptr, SimpleCommand *cmd);
void set_background (char **token, int *token_ptr, SimpleCommand *cmd);
void set_logical_and (char **token, int *token_ptr, SimpleCommand *cmd);
void set_logical_or(char **token, int *token_ptr, SimpleCommand *cmd);
void set_command_separator(char **token, int *token_ptr, SimpleCommand *cmd);
void set_open_bracket(char **token, int *token_ptr, SimpleCommand *cmd);
void handle_single_line_comment(char* line);
void handle_multi_line_comment(char* line);
char* my_strstr(const char* string, const char* substring);
void* my_memset(void* dest, int val, size_t len);
SimpleCommand * init_simple_command();
void free_simple_command(SimpleCommand *cmd);
void free_arguments(char **arguments);
char *my_strcpy(char *to, char *from);
int my_strlen(const char *str);
char *
my_strcat(char *s, char *append);

#endif /* _MAIN_H_ */
