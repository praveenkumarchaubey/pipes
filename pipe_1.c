/*Write a program where the parent process writes in the pipe and child process reads from the pipe continuously. To send data to the child the parent takes it from the user. When parent process receives the string “exit” from the user, it sends “exit” on the pipe, closes the write end of the pipe and exits. Upon receiving “exit”, the client (child) terminates.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>

#define READ 0
#define WRITE 1
#define MAX 100

int main()
{
	pid_t pid;
	char buffer[MAX];
	memset(buffer, 0, MAX*sizeof(char));
	char input[MAX];
	memset(input, 0, MAX*sizeof(char));

	int fd[2]; //file descriptors
	unsigned int bytes;
	int len = 0;
 
	if(-1 == pipe(fd))
	{
		printf("Error in creating a pipe\n");
		perror("Pipe Error");
		exit(-1);
	}
	else
	{
		printf("Pipe Creation Successfull\n\n");
	}


	pid = fork();
	while(1)
	{
		if(-1 == pid)
		{
			printf("Error in forking the process\n");
			perror("Fork Error");
			exit(-1);
		}
		if (0 == pid)
		{
			close(fd[WRITE]); //child cannot write to the pipe					
			bytes = read(fd[READ], buffer, sizeof(char)*MAX);
			printf("Child %d is reading from the pipe\n", getpid());
			printf("Child %d Reads %u bytes\n\n", getpid(), bytes);
			if(strncmp(buffer,"exit",4) == 0)
			{
				close(fd[READ]);
				printf("Child %d is Exiting now\n\n", getpid());
				exit(0);
			}
		}

		else
		{
			printf("Parent %d Writes to the pipe\n", getpid());
			printf("Please Enter The data\n");
			fgets(input, MAX*sizeof(char), stdin);
			len = strlen(input);				
			close(fd[READ]); //parent cannot read from the pipe
			write(fd[WRITE], input, sizeof(char) * len);
			printf("Parent %d writes Successfully\n\n", getpid());

			if(strncmp(input,"exit",4) == 0)
			{
				close(fd[WRITE]);
				printf("Parent %d is Exiting now\n\n", getpid());
				exit(0);
			}
		}
	}//end of while
	return 0;
}//end of main
