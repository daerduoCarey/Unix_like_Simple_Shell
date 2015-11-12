#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "parser.h"

int bg_pid[512];
int bg_pid_num=0;

int execute(struct cmdLine* cmdline, int flag)
{
	if(cmdline == null) return 0;
	
	struct cmd *cur = cmdline->head;
	if(cur == null) return 0;

	int pipe_fd[2], l_pipe_fd[2];

	int k = 0;
	while(1)
	{
		++k;
		int in_fd = 0, out_fd = 1;
		int stdin_b = dup(0), stdout_b = dup(1);
		
		if(cur->next!=null)
		{
			pipe(pipe_fd);
		}
		pid_t pid = fork();
	
		if(pid == 0)
		{
			//file
			if(cur->fd_in)
			{
				in_fd = open(cur->infile, O_RDONLY);
				dup2(in_fd, 0);
				close(in_fd);
			}
			if(cur->fd_out)
			{
				out_fd = open(cur->outfile, O_RDWR|O_CREAT|O_TRUNC, 0644);
				dup2(out_fd, 1);
				close(out_fd);
			}
			
			//pipe
			if(k>1)
			{
				dup2(l_pipe_fd[0], 0);
				close(l_pipe_fd[0]);
				close(l_pipe_fd[1]);
			}
			if(cur->next!=null)
			{
				close(pipe_fd[0]);
				dup2(pipe_fd[1], 1);
				close(pipe_fd[1]);
			}
			//execute
			if( execvp(cur->argv[0], cur->argv) < 0 )
			{
				printf("Error: execution error!\n");
			}
			exit(0);
		}
		else
		{
			waitpid(pid, null, 0);
			if(cur->fd_in)
			{
				dup2(stdin_b, 0);
			}
			if(cur->fd_out)
			{
				dup2(stdout_b, 1);
			}
			if(k>1)
			{
				close(l_pipe_fd[0]);
				close(l_pipe_fd[1]);
			}
			if(cur->next!=null)
			{
				close(pipe_fd[1]);
				l_pipe_fd[0] = pipe_fd[0];
				l_pipe_fd[1] = pipe_fd[1];
			}
		}		
	
		cur = cur->next;
		if(cur == null) break;
	}

	exit(0);
}

void cd(char *para)
{
	int fd;

	if( (fd = open(para, O_RDONLY)) < 0 )
	{
		printf("Error: open error!\n");
	}

	if( fchdir(fd) < 0 )
	{
		printf("Error: cannot change to new dir!\n");
	}
}

void print(struct cmdLine* cmdline)
{
	struct cmd *cur = cmdline->head;

	while(cur!=null)
	{
		int i;
		for(i=0;i<cur->args;++i)
		{
			printf("%d: %s\n", i, cur->argv[i]);
		}
		printf("\n");
		cur=cur->next;
	}
}

int check_bg_pid()
{
	int i;
	for(i=0;i<bg_pid_num;++i)
	{
		waitpid(bg_pid[i], null, 0);
	}

	return 1;
}

int main()
{
	char buffer[N];
	char curdir[1000];
	
	while(1)
	{
		getcwd(curdir, 1000);
		printf("SHELL:%s$ ", curdir);

		bzero(buffer, N);
		if( fgets(buffer, N, stdin) == null )
		{
			if( check_bg_pid() ) break;
		}
		buffer[strlen(buffer) - 1] = '\0';

		int flag;
		struct cmdLine* res = (struct cmdLine*)parse(buffer, &flag);
		//print(res);
		if(!flag) 
		{
			if(res == null) continue;
			struct cmd* cur = res->head;
			if(cur == null) continue;

			if( strcmp(cur->argv[0], "exit") == 0 )
			{
				if( check_bg_pid() ) exit(0);
			}
			else if( strcmp(cur->argv[0], "cd") == 0 )
			{
				if(cur->args > 1) cd(cur->argv[1]);
			}
			else
			{
				pid_t pid = fork();
				if(pid == 0)
				{
					execute(res, res->isBackground);
					exit(0);
				}
				else
				{
					if(!res->isBackground)
					{
						waitpid(pid, null, 0);
					}
					else
					{
						bg_pid[bg_pid_num++] = pid;
						//signal(SIGCHLD, SIG_IGN);
						printf("[%d] Background Execution\n", pid);
					}
				}
			}
		}
	}

	return 0;
}
