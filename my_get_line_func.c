#include "main.h"
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
