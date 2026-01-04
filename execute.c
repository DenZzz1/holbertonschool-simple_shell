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
	char *command_path;
	int argc;

	if (input == NULL || strlen(input) == 0)
		return;

	argc = parse_arguments(input, argv);

	if (argc == 0)
		return;

	/* Find the command in PATH or use as-is if it's a path */
	command_path = find_command(argv[0]);

	if (command_path == NULL)
	{
		fprintf(stderr, "./shell: 1: %s: not found\n", argv[0]);
		return;
	}

	pid = fork();

	if (pid == -1)
	{
		perror("Error");
		if (command_path != argv[0])
			free(command_path);
		return;
	}
	else if (pid == 0)
	{
		/* Child process */
		if (execve(command_path, argv, environ) == -1)
		{
			perror("./shell");
			if (command_path != argv[0])
				free(command_path);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/* Parent process */
		wait(&status);
		if (command_path != argv[0])
			free(command_path);
	}
}
