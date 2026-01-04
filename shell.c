#include "shell.h"

/**
 * print_prompt - Displays the shell prompt
 */
void print_prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "#cisfun$ ", 9);
}

/**
 * read_input - Reads a line of input from the user
 * @buffer: Buffer to store the input
 * @size: Size of the buffer
 *
 * Return: Number of characters read, or -1 on EOF
 */
ssize_t read_input(char *buffer, size_t size)
{
	ssize_t n;

	n = getline(&buffer, &size, stdin);
	return (n);
}

/**
 * parse_input - Removes newline character from input
 * @input: The input string to parse
 *
 * Return: Pointer to the parsed string
 */
char *parse_input(char *input)
{
	size_t len;

	if (input == NULL)
		return (NULL);

	len = strlen(input);
	if (len > 0 && input[len - 1] == '\n')
		input[len - 1] = '\0';

	return (input);
}

/**
 * execute_command - Executes a command using execve
 * @command: The command to execute
 */
void execute_command(char *command)
{
	pid_t pid;
	int status;
	char *argv[2];

	if (command == NULL || strlen(command) == 0)
		return;

	argv[0] = command;
	argv[1] = NULL;

	pid = fork();

	if (pid == -1)
	{
		perror("Error");
		return;
	}
	else if (pid == 0)
	{
		/* Child process */
		if (execve(command, argv, environ) == -1)
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

/**
 * main - Entry point for the simple shell
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	char *input = NULL;
	size_t bufsize = 0;
	ssize_t nread;

	while (1)
	{
		print_prompt();

		nread = getline(&input, &bufsize, stdin);

		if (nread == -1)
		{
			/* Handle EOF (Ctrl+D) */
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		parse_input(input);

		/* Skip empty lines */
		if (strlen(input) == 0)
			continue;

		execute_command(input);
	}

	free(input);
	return (0);
}
