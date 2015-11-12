***************************************************************************
**  	  			My Simple Shell 	       		 **
**	     		      Arthor: Kaichun Mo		       	 **
***************************************************************************

				* INTRODUCTION *

In this shell, I implement some basic functions mentioned in the website
https://202.120.38.39/~jzhou/courses/F14.OS/, which is one of our OS little
project, Fall 2014. My shell works as a command interpreter which provides 
user convenience when using their computer. 

The basic funtionalities include:
	
	1. Implement "cd %para" where %para can be absolute path or relative 
	path to current directory. And, a current working path can be showed
	each time before typing in next command as a prompt.

	2. Implement two way of exiting the shell. One is to type "exit", 
	another is to use "Ctrl-D". Both execution will terminate this shell,
	but only after all background programs are finished. If not, this 
	shell will hang on for a while waiting for them.
	
	3. For each command being typed into this shell, it will search for
	the executable file named with the first parameter from the files 
	your system PATH variable indicates. 

	4. Implement '<', '>' and '|' functionalities. For the first two, one
	can use them to redirect the input or oufput to files. For the pipe
	character, this shell can use pipe to redirect the output of one 
	command to its subsequent one as the input.

	5.Implement '&' funtionality. If you type '&' at the end of a command,
	this command will be executed in the background and a prompt for next
	command will show up immediately. When this background execution is 
	finally finished, it will print what it should print on the screen.




	           * HOW TO COMPILTE AND USE THIS SHELL *

1. To compile this project, you can simple type 
		
			make

in your shell and you will see a executable file named "shell" in your 
directory.

2. To run my simple shell, you can simply type

			./shell

to execute this program.




            * DESIGN CONSIDERATIONS AND IMPLEMENTATION DETAILS *

1. This whole program runs a while loop waiting for next command with a prompt
firstly printed to the user. After get the input command, this shell parse the 
raw input and convert it to a list of meaningful tokens. After this parsing
procedure, I pass the result to a function named "execute" to execute each of
these commands and deal with the redirections.


2. Parsing: In my implementation, I create a C structure named "cmd" which 
store all parameters for each single commmand. For compound command connected 
with a '|', I simply list all of them from one to the last. During the parsing 
phase, I deal with '<' and '>' and store the file target in "cmd" structure. 
Note that if more than one '>'(or '<') encountered, I will only store the last
file mentioned and use this file as the final destination(source). When facing
'&', I just record this information in the head single command structure.

3. Executing: Before executing any command, a new process will be forked and 
to execute it. If this command is a background executing command, the parent
process will not stop and do the next while loop instantly. If not, the parent
process will wait for it and you will see no prompt for next command. For a 
single command, this shell just simply judge whether this command is "exit" 
or "cd". If is, the shell will run the built-in function to execute it. If not,
the shell will call "execvp" to find a executable file in your PATH environment.
For a compound command with '|', this shell create a pipe and appropriately 
control the input of output of both sides of the pipe and let the message be 
redirected. For a command with stored file destination(source), this shell uses
"dup" and "dup2" to connect the file descriptor with 0 or 1 and fulfill the file
redirections. 


		     * LIMITATIONS AND TO BE IMPLEMENTED *

1. In my shell, you should always type "cd" or "exit" without '|'. If you do this,
you will encounter an error since no procedure in your PATH environment named "cd"
or "exit". 

2. Since it's hard for me to implement interprocess synchronization now, I cannot 
let you kill any background executing process. Thus, you need to wait all background
process to finish before terminate the shell.

3. It's quite amazing if we know how to implement tab completion and history record,
but it's really hard for me to understand how to use "ncursors" with time limitations.



