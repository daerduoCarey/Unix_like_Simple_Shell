#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <malloc.h>

#include "parser.h"

char* getToken(int *flag)
{
	char c = command[0];
	int i = 0;

	//extraneous spaces in the most front
	while(c==' ') 
	{
		++command;
		c = command[0];
		if(c=='\0') break;
	}

	char *s = command;

	//deal with different cases
	if(c=='\0') 
	{
		*flag = -1;
		return null;
	}
	else if(c=='|')
	{
		*flag = 1;
		++command;
		return null;
	}
	else if(c=='<')
	{
		*flag = 2;
		++command;
		return null;
	}
	else if(c=='>')
	{
		*flag = 3;
		++command;
		return null;
	}
	else
	{
		while(1)
		{
			++i;
			c = s[i];
			if(c=='\0'||c=='|'||c=='<'||c=='>'||c==' ') break;
		}
		char *res = (char *)malloc(i * sizeof(char));
		int j;
		for(j = 0; j < i; ++j) res[j] = s[j];
		res[i] = '\0';
		*flag = 0;

		command += i;
		return res;
	}
}

struct cmdLine* parse(char* s, int *flag)
{
	int l = strlen(s), i=1;
	
	//delete all additional spaces at the end
	while(l-i>=0&&s[l-i]==' ') ++i;
	if(l-i==0&&s[l-i]==' ') 
	{
		*flag = -1;
		return null;
	}

	struct cmdLine *res;
	res = (struct cmdLine*)malloc(sizeof(struct cmdLine));
	
	//deal with ampersand
	if(s[l-i]=='&')
	{
		res->isBackground = 1;
		++i;
	}
	else
	{
		res->isBackground = 0;
	}

	if(l-i<0) 
	{
		*flag = -1;
		return null;
	}
	s[l-i+1]='\0';

	//split tokens
	int k = 0; i = 0;
	command = s;
	while(command[0]==' '&&command[0]!='\0') ++command;
	if(command[0]=='\0') 
	{
		*flag = -1;
		return null;
	}
  
	struct cmd *cur, *last = 0; 
	int NOT_YET = 1;

	while(1)
	{
		int flg;
		char *token = getToken(&flg);
		if(flg < 0) break;
		else if(flg == 0)
		{
			++k;
			if(k==1)
			{
				cur = (struct cmd*)malloc(sizeof(struct cmd));
				if(NOT_YET) 
				{
					res->head = cur;
					NOT_YET = 0;
				}
				cur->args = 0; cur->next = null;
				cur->fd_in = cur->fd_out = 0;
				cur->infile = cur->outfile = null;
				if(last != null) last->next = cur;
			}
			cur->argv[cur->args] = token;
			++cur->args;
		}
		else if(flg == 1)
		{
			k = 0;
			last = cur;
		}
		else if(flg == 2)
		{
			cur->fd_in = 1;
			int f;
			cur->infile = getToken(&f);
			if(f) 
			{
				printf("Error: no input file given!\n");
				*flag = -1;
				return null;
			}
		}
		else	//flg == 3
		{
			cur->fd_out = 1;
			int f;
			cur->outfile = getToken(&f);
			if(f) 
			{
				printf("Error: no output file given!\n");
				*flag = -1;
				return null;
			}
			
		}
	}

	*flag = 0;
	return res;
}
