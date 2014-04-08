/*
 * err_sys.c
 */
          
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

//const char	*sys_errlist[];
int sys_nerr;
int errno;

err_sys(message)
char	*message;
{
	perror(message);
        fprintf(stderr,"%d\n",errno);
	exit(1);
}


char *
sys_err_str()
{
	static char	msgstr[200];

	if (errno != 0) {
		if (errno > 0 && errno < sys_nerr)
			sprintf(msgstr,"(%s)",sys_errlist[errno]);
		else
			sprintf(msgstr,"(errno=%d)",errno);
	} else {
		msgstr[0]='\0';
	}
        return &msgstr;
}


err_dump(message)
char	*message;
{
	perror(message);
        fprintf(stderr,"%d\n",errno);
	abort();
	exit(1);
}

