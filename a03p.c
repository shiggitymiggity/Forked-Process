#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

static int		com[2];

int main(int argc, char **argv)
{
	pid_t	pid;

	// set up pipe

	if (pipe(com))
		{
		printf("pipe error\n");
		return -1;
		}

	printf("CS201 - Assignment 3 Premium - Bowen Johnson\n");

	//call fork()
	
	pid = fork();

	if (pid == 0) 
		{

		// -- running in child process --

		int		sum = 0; 
		int		input = 0;
		int 	sign;

		// Receive characters from parent process via pipe
		// one at a time, and count them.

		for (int idx = 1; idx < argc; idx++) 
		{
			read(com[0], &input, 4);
			sum = sum + input;
		}
			
			if(sum < 0) //So the sum can be passed as positive so it wont be changed
			{
				sign = 2;
				sum = sum * -1;
				write(com[1], &sign, 4);	//Sending a flag back so it can
			}								            //be converted to negative again
			if(sum >= 0)
			{
				sign = 1;
				write(com[1], &sign, 4);
			}
			
		close(com[1]);

		// Return sum of numbers.

			return sum;

		}

	else {
		// -- running in parent process --

		int sum;
		int output = 0;
		int sign = 1;

		// Send numbers (datatype: int, 4 bytes) from command line arguments
		// starting with argv[1] one at a time through pipe to child process.

		for (int idx = 1; idx < argc; idx++)
		{
			output = atoi(argv[idx]);
			write(com[1], &output, 4);
		}

		pid = wait(&sum);
		read(com[0], &sign, 4);

		close(com[1]);

		// Wait for child process to return. Reap child process.
		// Receive sum of numbers via the value returned when
		// the child process is reaped.

		pid = waitpid(pid, &sum, 0);

			sum = sum >> 8; //I have no idea why it's shifting my bits...rude

			if (sign == 1)
			{
				printf("sum = %d\n", sum);	
			}

			if (sign == 2) //If it gets the flag it will convert back to negative
			{
				sum = sum * -1;
				printf("sum = %d\n", sum);
			}

		return 0;
		}
}
