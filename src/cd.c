#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int args, char *argv[])
{
	if(args > 1)
	{
		int fd;

		if( (fd = open(argv[1], O_RDONLY)) < 0 )
		{
			printf("Error: open error!\n");
			return -1;
		}

		if( fchdir(fd) < 0 )
		{
			printf("Error: cannot change to new dir!\n");
			return -1;
		}
	}

	return 0;
}
