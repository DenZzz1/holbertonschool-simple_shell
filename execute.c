#include "shell.h"

/**
 * execute_command - Executes a command using execve
 * @input: The input string containing command and arguments
 */
void execute_command(char *input)
{
	pid_t pid;
	int status;
	char *argv[1024];
	int argc;

	if (input == NULL || strlen(input) == 0)
		return;

	argc = parse_arguments(input, argv);

	if (argc == 0)
		return;

	pid = fork();

	if (pid == -1)
	{
		perror("Error");
		return;
	}
	else if (pid == 0)
	{
		/* Child process */
		if (execve(argv[0], argv, environ) == -1)
		{
			perror("./shell");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/* Parent process */
		wait(&status);
	}
}
