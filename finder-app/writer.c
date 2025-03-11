#include <stdio.h>
#include <syslog.h>
#include <string.h>

#define NUM_OF_ARGUENTS 3u

int main (int arg1, char *arg2[]);

int main (int arg1, char *arg2[])
{
	int errno = 1;
	FILE* fptr;
	openlog(NULL, 0 , LOG_USER);
	if(arg1 == NUM_OF_ARGUENTS)
	{
		fptr = fopen(arg2[1], "w");
		if(fptr == NULL)
		{
			syslog(LOG_ERR,"Error opening the file");
		}
		else
		{
			syslog(LOG_DEBUG,"Writing %s to %s",arg2[2], arg2[1]);
			fputs(arg2[2], fptr);
			errno = 0;
		}
		fclose(fptr);
	}
	else
	{
		syslog(LOG_ERR,"Invalid number of arguments: %d", (arg1-1));
	}
	closelog();
	return errno;
}

