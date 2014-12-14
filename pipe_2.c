/*Write a program to implement the command “ls | wc -l”.*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<errno.h>

int main(void)
{
	int pfds[2];
	pid_t pid;

	if(-1 == pipe(pfds))
	{
		printf("Error in creating a pipe\n");
		perror("Pipe Error");
		exit(-1);
	}

	pid = fork();
	if(-1 == pid)
	{
		printf("Error in forking the process\n");
		perror("Fork Error");
		exit(-1);
	}

	else if (0 == pid)
	{
		close(1);       /* close normal stdout */
		dup(pfds[1]);   /* make stdout same as pfds[1] */
		close(pfds[0]); /* we don't need this. closes the read end of the pipe */
		execlp("ls", "ls", NULL);
	}

	else
	{
		close(0);       /* close normal stdin */
		dup(pfds[0]);   /* make stdin same as pfds[0] */
		close(pfds[1]); /* we don't need this. closes the write end */
		execlp("wc", "wc", "-l", NULL);
	}

	return 0;
}

