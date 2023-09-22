#include "main.h"
int main (int argc, char *argv[])
{
    (void) argc;
    (void) argv;
	
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
        ssize_t bytes_read;

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


ssize_t my_get_line(char **lineptr, size_t *n) {
    static char *buffer = NULL;
    static size_t buf_size = 0;
    ssize_t total_read = 0;
    ssize_t bytes_read, i;
    char *new_buffer;

    if (!buffer || *n > buf_size) 
    {
        buf_size = *n;
        new_buffer = buffer ? my_realloc(buffer,buf_size / 2, buf_size + 1) : malloc(buf_size + 1);
        if (!new_buffer) 
        { 
            perror("allocation error");
            return -1; 
            
        }
        buffer = new_buffer;
    }

    while ((bytes_read = read(STDIN_FILENO, buffer + total_read, buf_size - total_read)) > 0)
    {
        total_read += bytes_read;
        for (i = 0; i < total_read; i++) 
        {
            if (buffer[i] == '\n') 
            {
                buffer[i] = '\0';
                *lineptr = buffer;
                return i + 1;
                
            }
        }

        if ((size_t)total_read >= buf_size - 1) 
        {
            buf_size *= 2;
            new_buffer = my_realloc(buffer, buf_size / 2, buf_size + 1);
            if (!new_buffer) 
            {
                perror("realloc"); 
                return -1; 
                
            }
            buffer = new_buffer;
        }
    }

    if (bytes_read < 0)
    { 
        perror("read"); 
        return -1;
    }
    if (total_read == 0)
    { 
        return 0; 
        
    }
    buffer[total_read] = '\0';
    *lineptr = buffer;
    return total_read;
}

void *my_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
        char *p;
        unsigned int i, max = new_size;
        char *oldp = ptr;

        if (ptr == NULL)
        {
                p = malloc(new_size);
                return (p);
        }
        else if (new_size == 0)
        {
                free(ptr);
                return (NULL);
        }
        else if (new_size == old_size)
                return (ptr);

        p = malloc(new_size);
        if (p == NULL)
                return (NULL);
        if (new_size > old_size)
                max = old_size;
        for (i = 0; i < max; i++)
                p[i] = oldp[i];
        free(ptr);
        return (p);
}



char *my_strchr(char *str, int ch) 
{
    char c = ch;

    for (;; ++str) 
    {
        if (*str == c)
            return ((char *)str);
        if (*str == '\0')
            return (NULL);
    }
}

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
        int token_no = 0;
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
char *my_strcpy(char *to, char *from)
{
	char *save = to;

	for (; (*to = *from); ++from, ++to);
	return(save);
}
int my_strlen(char *str)
{
	register char *s;

	for ((s = str); *s; ++s)
	    ;
	    return(s - str);
}
char *my_strcat(char *s, char *append)
{
	char *save = s;

	for (; *s; ++s)
	    ;
	while ((*s++ = *append++))
	    ;
	return(save);
}
/* Implement set_pipe */
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
int _strcmp(char *s1, char *s2) 
{
    while (*s1 && (*s1 == *s2)) 
    {
        s1++;
        s2++;
    }

    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

void add_environment_variable(char *name, char *value) 
{
    if (env_count >= env_capacity) {
        /* Increase the capacity if needed */
            perror("No space in array");
            exit(EXIT_FAILURE);
    }

    env_vars[env_count].name = my_strdup(name);
    env_vars[env_count].value = my_strdup(value);
    env_count++;
}

void set_environment_variable(char *name, char *value) 
{
    int i;
    for (i = 0; i < env_count; i++) 
    {
        if (_strcmp(env_vars[i].name, name) == 0) 
        {
            free(env_vars[i].value);
            env_vars[i].value = my_strdup(value);
            return;
        }
    }

    /* If the variable doesn't exist, add it */
    add_environment_variable(name, value);
}

void unset_environment_variable(char *name) 
{
    int i, j;
    for (i = 0; i < env_count; i++) 
    {
        if (_strcmp(env_vars[i].name, name) == 0) 
        {
            free(env_vars[i].name);
            free(env_vars[i].value);
            /* Shift the elements after the removed variable */
            for (j = i; j < env_count - 1; j++) 
            {
                env_vars[j] = env_vars[j + 1];
            }
            env_count--;
            return;
        }
    }
}

void print_environment_variables() 
{
    int i;
    for (i = 0; i < env_count; i++) 
    {
        printf("%s=%s\n", env_vars[i].name, env_vars[i].value);
    }
}
char* get_env(char *name)
{
    int i;
    for (i = 0; i < env_count; i++) 
    {
        if (_strcmp(env_vars[i].name, name) == 0)
        {
            return env_vars[i].value;
        }
    }
    return NULL;
}
void free_environment()
{
	int i;
        for (i = 0; i < env_count; i++) 
        {
            if(env_vars[i].name && env_vars[i].value)
            {
                free(env_vars[i].name);
                free(env_vars[i].value);
            }
        }
        env_count = 0;
        env_capacity = 0;
}

char *get_command_path(char *command)
{
    char *path = get_env("PATH");
    char *dir;
    char *path_copy = my_strdup(path);

    dir = my_strtok(path_copy, ":");
    while (dir)
    {
        char *command_path = (char *)malloc(my_strlen(dir) + my_strlen(command) + 2);
        my_strcpy(command_path, dir);
	my_strcpy(command_path, "/");
	my_strcpy(command_path, command);

        if (access(command_path, X_OK) == 0)
        {
            free(path_copy);
            return command_path;
        }

        free(command_path);
        dir = my_strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

void add_alias(char *alias, char *command) 
{
    if (alias_count >= MAX_ALIAS_COUNT) 
    {
        perror("Alias: Maximum number of aliases reached.");
        return;
    }

    aliases[alias_count].alias = my_strdup(alias);
    aliases[alias_count].command = my_strdup(command);
    alias_count++;
}

void list_aliases() 
{
    int i;
    for (i = 0; i < alias_count; i++) 
    {
        printf("alias %s='%s'\n", aliases[i].alias, aliases[i].command);
    }
}

void remove_alias(char *alias) 
{
    int i, j;
    for (i = 0; i < alias_count; i++)
    {
        if (_strcmp(aliases[i].alias, alias) == 0) 
        {
            free(aliases[i].alias);
            free(aliases[i].command);
            /* Shift the elements after the removed alias
            */
            for (j = i; j < alias_count - 1; j++)
            {
                aliases[j] = aliases[j + 1];
            }
            alias_count--;
            return;
        }
    }
}

char *get_alias(char *alias) 
{
    int i;
    for (i = 0; i < alias_count; i++) 
    {
        if (_strcmp(aliases[i].alias, alias) == 0) 
        { 
            return aliases[i].command;
        }
    }
    return NULL;
}

void free_aliases() 
{
    int i;
    for (i = 0; i < alias_count; i++) 
    {
        if(aliases[i].alias && aliases[i].command)
        {
            free(aliases[i].alias);
            free(aliases[i].command);
        }
    }
    alias_count = 0;
}


void handle_builtin_command(char **cmd, int token) {
    char **command = cmd;

    if (_strcmp(command[token - 1], "exit") == 0)
    {
        /** Handle exit command
         *  You may perform any necessary
         * cleanup here before exiting.
         */
         shell_exit(my_atoi(command[token]));
         
    }
    else if (_strcmp(command[token - 1], "alias") == 0) 
    {
        /* Handle alias command */
        if (command[token - 1] && command[token])
        {
            add_alias(command[token - 1], command[token]);
        } 
	else
	{
            list_aliases();
        }
    }
    else if (_strcmp(command[token - 1], "unalias") == 0) 
    {
        /* Handle unalias command  */
        if (command[token])
	{
            remove_alias(command[token]);
        }
    } 
    else if (_strcmp(command[token - 1], "cd") == 0) 
    {
        /* Handle cd command */
        if (command[token])
        {
            if (chdir(command[token]) == -1)
            {
                perror("cd");
            }
        }
        else 
        {
            perror("cd: Missing argument");
        }
    }
    else if (_strcmp(command[token - 1], "setenv") == 0) 
    {
        /* Handle setenv command */
        if (command[token - 1] && command[token]) 
        {
            set_environment_variable(command[token - 1], command[token]);
        } 
        else 
        {
            perror("setenv: Invalid arguments");
        }
    }
    else if (_strcmp(command[token - 1], "unsetenv") == 0)
    {
        /* Handle unsetenv command */
        if (command[token]) 
        {
            unset_environment_variable(command[token]);
        }
        else 
        {
            perror("unsetenv: Missing argument");
        }
    }
    else 
    {
        /* Not a built-in command */
        execute_external_command(command, token);
    }
}

void execute_command(char **cmd, int token)
{
        handle_builtin_command(cmd,  token);
}


void execute_external_command(char **command, int token)
{
    /** Resolve the command path using the
     * provided function
    */
    char *command_path = get_command_path(command[*token - 1]);

    if (command_path) 
    {
        /** Fork a child process to execute
         *  the command
        */
        pid_t pid = fork();
        if (pid < 0) 
        {
            perror("fork");
        } 
        else if (pid == 0) 
        {
            /** Child process: execute the
             *  command
             */
            execve(command_path, command, environ);

            /** If execve returns, there was
             * an error
             */
            perror("execve");
            exit(EXIT_FAILURE);
        } 
        else 
        {
            /* Parent process: wait for the child to complete
            */
            waitpid(pid, &status, 0);

            /* Save the exit status of the last executed command */
            shell_exit(WEXITSTATUS(status));
        }

        free(command_path);
    }
    else 
    {
        /* Command not found in PATH */
        perror("Command not found");
    }
    
}
int my_atoi(char* str)
{
    int res = 0, i;
    for (i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';
        
    return res;
}


void shell_exit(int exit_status)
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
