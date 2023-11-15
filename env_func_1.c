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
	    if (!env_vars[i].value)
	    {
		    perror("Malloc");
		    exit(EXIT_FAILURE);
	    }
	    if(my_strstr(__envp[i], name))
	    {
		    free(__envp[i]);
		    __envp[i] = (char *)malloc(my_strlen(name)+ my_strlen(value) + 2);
    		    if (!__envp[i])
    		    {
	    		     perror("Malloc");
            		     exit(EXIT_FAILURE);
    		    }

 		    my_strcpy(__envp[i], name);
    	            my_strcat(__envp[i], "=");
    		    my_strcat(__envp[i], value);
	    }
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
	    if(my_strstr(__envp[i], name))
	    {
		    free(__envp[i]);
	    }
            /* Shift the elements after the removed variable */
            for (j = i; j < env_count - 1; j++) 
            {
                env_vars[j] = env_vars[j + 1];
		__envp[j] = __envp[j + 1];
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
