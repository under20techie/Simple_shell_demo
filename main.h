#ifndef MAIN_H
#define MAIN_H 

#define MAX_TOKENS 64
#define BUF_SIZE 1024
#define ENV_CAPACITY 20
#define MAX_ALIAS_COUNT MAX_TOKENS 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
extern char **__envp;
extern int env_count;
extern int alias_count;
extern int status;
extern int env_capacity;
extern char *tokens[MAX_TOKENS];
typedef struct
{
    char *alias;
    char *command;
} AliasEntry;
AliasEntry aliases[MAX_ALIAS_COUNT];

/* Enumeration for command types */
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


typedef struct {
    char *name;
    char *value;
} EnvironmentVariable;
EnvironmentVariable env_vars[ENV_CAPACITY];

ssize_t my_get_line(char **lineptr, size_t *n);
void print_my_prompt (void);
char *my_strchr(char *str, int ch);
char *my_strtok(char *str, char *delim);
char *my_strdup(char *str);
int  get_token_type(char *token);
void *my_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
int _strcmp(char *s1, char *s2);

/* The command-line parser */
void parse_command(char **token, int num_tokens);
void parse_cmd_line(char *cmd_line);
void exec_pipe(char **token, int *token_ptr);
 void exec_redirect_in(char **token, int *token_ptr);
void exec_redirect_out(char **token, int *token_ptr);
void exec_redirect_out(char **token, int *token_ptr);
void exec_background (char **token, int *token_ptr);
void exec_logical_and (char **token, int *token_ptr);
void exec_logical_or(char **token, int *token_ptr);
void exec_command_separator(char **token, int *token_ptr);
void exec_open_bracket(char **token, int *token_ptr);
void handle_single_line_comment(char* line);
void handle_multi_line_comment(char* line);
char* my_strstr(char* string, char* substring);
void* my_memset(void* dest, int val, size_t len);
char *my_strcpy(char *to, char *from);
int my_strlen(char *str);
char *my_strcat(char *s, char *append);
void interactive_mode (void);
void non_interactive_mode (char *filename);
void execute_external_command(char **command, int token);
void add_environment_variable(char *name, char *value);
void print_environment_variables();
char* get_env(char *name);
void set_environment_variable(char *name, char *value);
void unset_environment_variable(char *name); 
void free_environment();
void free_tokens();
char *get_command_path(char *command);
void add_alias(char *alias, char *command);
void remove_alias(char *alias); 
char *get_alias(char *alias); 
void list_aliases();
void free_aliases();
void handle_builtin_command(char **cmd, int token);
void execute_command(char **cmd, int token);
int shell_exit(int exit_status);
int my_atoi(char* str);
void exec_simple_command(char **cmd, int *token);
void search_forward(char **tokens, int start_index);
void search_backward(char **tokens, int end_index);
int check_for_special_characters(char **tokens);
void alias_built_in(char **cmd, int token);
void cd_built_in(char **cmd, int token);
void setenv_built_in(char **cmd, int token);
void unsetenv_built_in(char **cmd, int token);
void echo_built_in(char **cmd, int token);
void exit_built_in(char **cmd, int token);
void update_first_token(char **cmd, char *new_path);
#endif /* MAIN_H_ */


