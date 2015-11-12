#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define null 0

#define N 512

char *command;

struct cmd
{
	int args;
	char *argv[256];
	
	int fd_in, fd_out;
	char *infile, *outfile;

	struct cmd *next;
};

struct cmdLine
{
	int isBackground;
	struct cmd *head;
};

struct cmdLine* parse(char*, int*);

char* getToken(int *);
