#include "main.h"
int main (int argc, char *argv[])
{
    if (argc == 2) {
        //execute_file_commands(argv[1]);
    } else if (argc > 2) {
        perror( "Usage: no [filename]");
        return 1;
    } else {
        interactive_mode();
    }
}
void non_interactive_mode(char *filename)
{
    //if(access())
    int fd_read = open(filename, O_RDONLY);   
    
    
    
}
    void interactive_mode(void)
    {
        char *line = NULL;
        size_t n = 1024;
        ssize_t bytes_read;

        while (1) {
       // char cwd[BUF_SIZE]; // Buffer to hold the current working directory path
            print_my_prompt();
            fflush(stdout);
            bytes_read = my_get_line(&line, &n); // Read user input

            if (bytes_read == -1) {
            // Error occurred, handle it if needed
                continue;
            } else if (bytes_read == 0) {
            // End of file (Ctrl+D) or empty line, exit the shell
                break;
            }
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

    if (!buffer || *n > buf_size) {
        buf_size = *n;
        new_buffer = buffer ? my_realloc(buffer,buf_size / 2, buf_size + 1) : malloc(buf_size + 1);
        if (!new_buffer) { perror("allocation error"); return -1; }
        buffer = new_buffer;
    }

    while ((bytes_read = read(STDIN_FILENO, buffer + total_read, buf_size - total_read)) > 0) {
        total_read += bytes_read;
        for (i = 0; i < total_read; i++) {
            if (buffer[i] == '\n') {
                buffer[i] = '\0';
                *lineptr = buffer;
                return i + 1;
            }
        }

        if ((size_t)total_read >= buf_size - 1) {
            buf_size *= 2;
            new_buffer = my_realloc(buffer, buf_size / 2, buf_size + 1);
            if (!new_buffer) { perror("realloc"); return -1; }
            buffer = new_buffer;
        }
    }

    if (bytes_read < 0) { perror("read"); return -1; }
    if (total_read == 0) { return 0; }
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



char *my_strchr(const char *str, int ch) {
    char c = ch;

    for (;; ++str) {
        if (*str == c)
            return ((char *)str);
        if (*str == '\0')
            return (NULL);
    }
}

char *my_strtok(char *s, const char *delim) {
    static char *last = NULL; // Keeps track of the previous position
    char *tok;
    
    if (s == NULL && (s = last) == NULL)
        return NULL;

    // Skip leading delimiters
    while (*s && my_strchr(delim, *s))
        s++;

    if (*s == '\0') {
        last = NULL;
        return NULL;
    }

    tok = s;

    // Find the end of the token
    while (*s && !my_strchr(delim, *s))
        s++;

    if (*s) {
        *s = '\0'; // Null-terminate the token
        last = s + 1; // Move to the next character after the delimiter
    } else {
        last = NULL;
    }

    return tok;
}

char *my_strdup(const char *str) 
{
    size_t len = my_strlen(str) + 1;
    char *dup_str = malloc(len);
    if (dup_str) {
        my_strcpy(dup_str, (char*)str);
    }
    return dup_str;
}


void print_my_prompt(void)
{
char cwd[1024]; // Buffer to hold the current working directory path

        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s/MyShell> ", cwd); // Print the prompt with the current working directory
        } else {
            perror("getcwd"); // Error occurred while getting the current working directory
            printf("MyShell> "); // Print the default prompt without the current working directory
        }

}

void parse_cmd_line(char *cmd_line)
{
        int i = 0;
        char *token = my_strtok(cmd_line, " ");
        int token_count = 0;
        char *tokens[MAX_TOKENS];

        while (token != NULL)
        {
                if (token_count >= MAX_TOKENS)
                {
                        perror("Too many tokens. Some Tokens were not stored.\n");
                        tokens[token_count] = NULL;
                        break;
                }
                tokens[token_count] = my_strdup(token);
                token_count++;
                token = my_strtok(NULL, " ");
        }
        tokens[token_count] = NULL;
        //Call parse
	SimpleCommand *cmd_tree[24];
	parse_command(tokens, token_count,cmd_tree);
	while (token[i])
	{
	free(tokens[i]);
	i++;
	}
}


void *parse_command(char **token, int num_tokens,  SimpleCommand *cmds[])
{
	int i = 0;
	if (num_tokens <= 0)
	{
		perror("No tokens in array");
		exit(1);
	}
	
    SimpleCommand *cmd;
	if (num_tokens > 0 && num_tokens <= 2)
	{
		set_simple_command(token, &i,  cmds[i]);
		return cmds;
	}
	for (i = 0; i < num_tokens; i++)
	{
		switch (get_token_type(token[i]))
		{
			case  TOKEN_PIPE:
			set_pipe(token, &i, cmds[i]);
			break;
			case TOKEN_REDIRECT_IN:
			set_redirect_in(token, &i, cmds[i]);
			break;
			case  TOKEN_REDIRECT_OUT:
			set_redirect_out(token, &i, cmds[i]);
			break;
			case TOKEN_BACKGROUND:
			set_background(token, &i, cmds[i]);
			break;
			case TOKEN_LOGICAL_AND:
			set_logical_and(token, &i, cmds[i]);
			break;
			case TOKEN_LOGICAL_OR:
			set_logical_or(token, &i, cmds[i]);
			break;
			case  TOKEN_COMMAND_SEPARATOR:
			set_command_separator(token, &i, cmds[i]);
			break;
		}
      cmds[i] = NULL;
	}
}
int get_token_type (const char *token)
{
    int return_value = 0;
    if(_strcmp((char *)token, "&&") == 0)
        return TOKEN_LOGICAL_AND;
    else if(_strcmp((char *)token, "||") == 0)
        return TOKEN_LOGICAL_OR;
    switch (*token)
    {
        case '|':
        return_value = TOKEN_PIPE;
        break;
        case '<':
        return_value = TOKEN_REDIRECT_IN;
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
    
}
void set_simple_command(char **token, int *token_ptr, SimpleCommand *simple_cmd) {
    int no_of_arg = 0;
    int j = *token_ptr;
    if (get_token_type(token[token_ptr != TOKEN_UNKNOWN]))
        (*token_ptr)++;
    if(token[*token_ptr][0] != '-' && token[*token_ptr][0] != '/')
        simple_cmd->command = my_strdup(token[*token_ptr]);
    else if (token[*token_ptr][0] == '-')
    {   perror("Invalid Command Syntax");
        exit(1);
    }
    simple_cmd->arguments = malloc(6 * sizeof(char*));
    (*token_ptr)++;
    if(token[*token_ptr][0] == '-' ) {
        while(token[*token_ptr][0] == '-' && no_of_arg < 5)
        {
        simple_cmd->arguments[no_of_arg] = my_strdup(token[*token_ptr]);
        no_of_arg++;
        (*token_ptr)++;
        }
    }
    if(token[*token_ptr - 1][0] == '-')
        simple_cmd->arguments[no_of_arg] = NULL;
    else
        free(simple_cmd->arguments);
     if (get_token_type(token[*token_ptr]) == 0 && _strcmp(token[j], "<") == 0 && token[*token_ptr]) {
        if (token[*token_ptr]) {
            simple_cmd->filename= my_strdup(token[*token_ptr]);
        } else {
            perror("Invalid command line syntax: Missing input file after redirect symbol");
            exit(EXIT_FAILURE);
        }
    } else if (get_token_type(token[*token_ptr]) == 0  && _strcmp(token[j], ">") == 0 && token[*token_ptr]) {
        // Handle output file redirection
        if (token[*token_ptr]) {
            simple_cmd->filename = my_strdup(token[*token_ptr]);
        } else {
            perror("Invalid command line syntax: Missing output file after redirect symbol");
            exit(EXIT_FAILURE);
        }
    } else if (get_token_type(token[*token_ptr]) == 0 && token[*token_ptr]){
        simple_cmd->filename = my_strdup(token[*token_ptr]);
    }
    simple_cmd->filename = NULL;
}

SimpleCommand * init_simple_command() {
SimpleCommand *simple_cmd = malloc(sizeof(SimpleCommand));
    if (simple_cmd) {
        simple_cmd->command = NULL;
        simple_cmd->arguments = NULL;
        simple_cmd->filename = NULL;
    }
return simple_cmd;
}

void free_simple_command(SimpleCommand *cmd) {
    if (cmd) {
        if(cmd->command)
            free(cmd->command);
        if(cmd->arguments)
        free_arguments(cmd->arguments);
        // Check if file_out is not NULL before freeing
        if (cmd->filename) {
            free(cmd->filename);
        }

    }
}
void free_arguments(char **arguments) {
    if (arguments) {
        for (int i = 0; arguments[i] != NULL; i++) {
            free(arguments[i]);
        }
        free(arguments);
    }
}
void handle_single_line_comment(char* line) {
    // Find the first occurrence of the comment marker #
    char* comment_start = my_strchr(line, '#');

    // If a comment marker is found, replace it with a null-terminator
    if (comment_start != NULL) {
        *comment_start = '\0';
    }
}
void handle_multi_line_comment(char* line) {
    // Find the first occurrence of the comment start marker "/*"
    char* comment_start = my_strstr(line, "/*");

    // Find the first occurrence of the comment end marker "*/"
    char* comment_end = my_strstr(line, "*/");

    // If both the start and end markers are found and the start marker appears before the end marker
    if (comment_start != NULL && comment_end != NULL && comment_start < comment_end) {
        // Calculate the length of the comment (including the end marker)
        size_t comment_length = (comment_end + 2) - comment_start;

        // Replace the comment with spaces or remove it completely
        my_memset(comment_start, ' ', comment_length);
    }
}
char* my_strstr(const char* string, const char* substring) {
    const char* a;
    const char* b;

    // First scan quickly through the two strings looking for a
    // single-character match. When it's found, then compare the
    // rest of the substring.
    b = substring;
    if (*b == '\0') {
        return (char*)string;
    }
    for (; *string != '\0'; string++) {
        if (*string != *b) {
            continue;
        }
        a = string;
        while (1) {
            if (*b == '\0') {
                return (char*)string;
            }
            if (*a++ != *b++) {
                break;
            }
        }
        b = substring;
    }
    return NULL;
}
void * my_memset(void* dest, int val, size_t len) {
    unsigned char* ptr = (unsigned char*)dest;
    while (len-- > 0) {
        *ptr++ = (unsigned char)val;
    }
return dest;
}
char *
my_strcpy(char *to, char *from)
{
	char *save = to;

	for (; *to = *from; ++from, ++to);
	return(save);
}
int
my_strlen(const char *str)
{
	register const char *s;

	for (s = str; *s; ++s);
	return(s - str);
}
char *
my_strcat(char *s, char *append)
{
	char *save = s;

	for (; *s; ++s);
	while (*s++ = *append++);
	return(save);
}
// Implement set_pipe
void set_pipe(char **token, int *token_ptr, SimpleCommand *cmd) {
    // Create a complex structure for the pipe
   int n = *token_ptr;
    int *ptr = &n;
    cmd = init_simple_command();
    if (cmd != NULL) {
        // Set the type of the complex command to CMD_PIPE
        cmd->type = CMD_PIPE;
        while(ptr >= 0)
        {
            if(get_token_type(token[*ptr]) !=  TOKEN_UNKNOWN)
                break;
            (*ptr)--;
        }
        //for left of the pipe
        if(*ptr  >= 0 && get_token_type(token[*ptr]) !=  TOKEN_PIPE)
            set_simple_command(token, ptr, cmd);
         else if (get_token_type(token[*ptr]) ==  TOKEN_PIPE && *ptr >= 0)
         {
             set_simple_command(token, token_ptr, cmd);
             return ;
         }
         //for right
         if(token[*token_ptr + 1])
         {
             set_simple_command(token, token_ptr, cmd);
         }
    } else {
        // Error handling for memory allocation failure
        perror("Unable to allocate space for structure");
    }

}

// Implement set_redirect_in
void set_redirect_in(char **token, int *token_ptr, SimpleCommand *cmd) {
    // Create a complex structure for the redirect in
    cmd = init_simple_command();
    if (cmd != NULL) {
        // Set the type of the complex command to CMD_REDIRECT_IN
        cmd->type = CMD_REDIRECT_IN;

        // Set the complexity of the tokens on the left and right of the redirect in
        if(token[*token_ptr - 1] && token[*token_ptr + 1])
            set_simple_command(token, token_ptr, cmd);
    } else {
        // Error handling for memory allocation failure
        perror("Unable to allocate space for structure");
    }

    // Return the pointer to the redir_in_cmd structure
}

// Implement set_redirect_out
void set_redirect_out(char **token, int *token_ptr, SimpleCommand *cmd) {
    // Create a complex structure for the redirect out
    cmd = init_simple_command();
    if (cmd != NULL) {
        // Set the type of the complex command to CMD_REDIRECT_OUT
        cmd->type = CMD_REDIRECT_OUT;

        // Set the complexity of the tokens on the left and right of the redirect out
        if(token[*token_ptr - 1] && token[*token_ptr + 1])
            set_simple_command(token, token_ptr, cmd);
    } else {
        // Error handling for memory allocation failure
        perror("Unable to allocate space for structure");
    }

    // Return the pointer to the redir_out_cmd structure
}

// Implement set_background
void set_background(char **token, int *token_ptr, SimpleCommand *cmd) {
    int n = *token_ptr;
    int *ptr = &n;
    (*ptr)--;
    // Create a complex structure for the background
    cmd = init_simple_command();
    if (cmd != NULL) {
        // Set the type of the complex command to CMD_BACKGROUND
        cmd->type = CMD_BACKGROUND;

        // Set the complexity of the tokens on the left and right of the background
        if(token[*token_ptr - 1] )
            set_simple_command(token, ptr, cmd);
    } else {
        // Error handling for memory allocation failure
        perror("Unable to allocate space for structure");
    }

    // Return the pointer to the background_cmd structure
}

// Implement set_logical_and
void set_logical_and(char **token, int *token_ptr, SimpleCommand *cmd) {
    int n = *token_ptr;
    int *ptr = &n;
    // Create a complex structure for the logical and
    cmd = init_simple_command();
    if (cmd != NULL) {
        // Set the type of the complex command to CMD_LOGICAL_AND
        cmd->type = CMD_LOGICAL_AND;

        // Set the complexity of the tokens on the left and right of the logical and
       if(*ptr  >= 0 && get_token_type(token[*ptr]) !=  TOKEN_LOGICAL_AND)
            set_simple_command(token, ptr, cmd);
         else if (get_token_type(token[*ptr]) ==  TOKEN_LOGICAL_AND && *ptr >= 0)
         {
             set_simple_command(token, token_ptr, cmd);
             return ;
         }
         //for right
         if(token[*token_ptr + 1])
         {
             set_simple_command(token, token_ptr, cmd);
         }
    } else {
        // Error handling for memory allocation failure
        perror("Unable to allocate space for structure");
    }

    // Return the pointer to the logical_and_cmd structure

}

// Implement set_logical_or
void set_logical_or(char **token, int *token_ptr, SimpleCommand *cmd) {
    int n = *token_ptr;
    int *ptr = &n;
     // Create a complex structure for the logical and
    cmd = init_simple_command();
    if (cmd != NULL) {
        // Set the type of the complex command to CMD_LOGICAL_AND
        cmd->type = CMD_LOGICAL_OR;

        // Set the complexity of the tokens on the left and right of the logical and
       if(*ptr  >= 0 && get_token_type(token[*ptr]) !=  TOKEN_LOGICAL_OR)
             set_simple_command(token, ptr, cmd);
         else if (get_token_type(token[*ptr]) ==  TOKEN_LOGICAL_OR && *ptr >= 0)
         {
             set_simple_command(token, token_ptr, cmd);
             return ;
         }
         //for right
         if(token[*token_ptr + 1])
         {
             set_simple_command(token, token_ptr, cmd);
         }
    } else {
        // Error handling for memory allocation failure
        perror("Unable to allocate space for structure");
    }

    // Return the pointer to the logical_or_cmd structure
}

// Implement set_command_separator
void set_command_separator(char **token, int *token_ptr, SimpleCommand *cmd) {
    int n = *token_ptr;
    int *ptr = &n;
    // Create a complex structure for the command separator
    cmd = init_simple_command();
    if (cmd != NULL) {
        // Set the type of the complex command to CMD_COMMAND_SEPARATOR
        cmd->type = CMD_COMMAND_SEPARATOR;

        // Set the complexity of the tokens on the left and right of the command separator
        if(*ptr  >= 0 && get_token_type(token[*ptr]) !=  TOKEN_LOGICAL_OR)
            set_simple_command(token, ptr, cmd);
         else if (get_token_type(token[*ptr]) ==  TOKEN_LOGICAL_OR && *ptr >= 0)
         {
             set_simple_command(token, token_ptr, cmd);
             return ;
         }
         //for right
         if(token[*token_ptr + 1])
         {
             set_simple_command(token, token_ptr, cmd);
         }
        
    } else {        // Error handling for memory allocation failure
        perror("Unable to allocate space for structure");
    }

    // Return the pointer to the command_separator_cmd structure
}int _strcmp(char *s1, char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }

    return *(unsigned char *)s1 - *(unsigned char *)s2;
}
