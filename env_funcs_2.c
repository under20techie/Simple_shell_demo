#include "main.h"
void add_environment_variable(char *name, char *value) 
{
    int env_count = 0;
    int env_capacity = ENV_CAPACITY;
    if (env_count >= env_capacity) {
        /* Increase the capacity if needed */
            perror("No space in array");
            exit(EXIT_FAILURE);
    }

    env_vars[env_count].name = my_strdup(name);
    env_vars[env_count].value = my_strdup(value);
    env_count++;
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
            return command_path;}

        free(command_path);
        dir = my_strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}
