#include "main.h"
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
