#include "main.h"
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
