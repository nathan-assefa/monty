#include "monty.h"

/**
 * * open_file- to open file and analyze
 * * @opcode_file: instruction to be found
 * * @stack: pointer to doubly linked list
 */
void open_file(char *opcode_file, stack_t **stack)
{
	char *token;
	size_t size = 0;
	int read;
	unsigned int line_count = 1;
	int check;
	instruct_func check_opcode;

	var_global.file = fopen(opcode_file, "r");
	if (var_global.file == NULL)
	{
		fprintf(stderr, "Error: Can't open file %s\n", opcode_file);
		exit(EXIT_SUCCESS);
	}


	while ((read = getline(&var_global.buffer, &size, var_global.file)) != -1)
	{
		token = parser(var_global.buffer, line_count);
		if (token == NULL || token[0] == '#')
		{
			line_count++;
			continue;
		}

		check_opcode = check_for_opcode(token);
		if (check_opcode == NULL)
		{
			fprintf(stderr, "L%d: unknown instruction %s\n", line_count, token);
			exit(EXIT_FAILURE);
		}
		check_opcode(stack, line_count);
		line_count++;

	}
	free(var_global.buffer);
	check = fclose(var_global.file);
	if (check == -1)
		exit(-1);
}

/**
 * * parser- to parse the buffer
 * * @buffer: buffer having the contents of the bytcode
 * * @line_number: number of line in monty file
 * * Return: parsed string
 */
char *parser(char *buffer, unsigned int line_number)
{
	char *opcode, *push_argument;

	opcode = strtok(buffer, "\n ");
	if (opcode == NULL)
	{
		return (NULL);
	}
	if (strcmp(opcode, "push") == 0)
	{
		push_argument = strtok(NULL, "\n ");

		if (_isdigit(push_argument) == 1 && push_argument != NULL)
		{
			var_global.push_arg = atoi(push_argument);
		}
		else
		{
			fprintf(stderr, "L%d: usage: push integer\n", line_number);
			exit(EXIT_FAILURE);
		}
	}

	return (opcode);
}

/**
 * * _isdigit- to know if the charachter is number
 * * @str: pointer to the string
 * * Return: int
 */
int _isdigit(char *str)
{
	unsigned int i = 0;

	if (str == NULL)
		return (0);

	if (str[0] == '-')
	{
		i++;
	}
	if (!isdigit(str[i]))
		return (0);

	return (1);
}

/**
 * * check_for_opcode - check if the opcode is valid
 * * @token: opcode to be chacked
 * * Return: instruct_func
 */
instruct_func check_for_opcode(char *token)
{
	int i;

	instruction_t instruct[] = {
		{"push", _push},
		{"pall", _pall},
		{"pint", _pint},
		{"pop", _pop},
		{"swap", _swap},
		{"add", _add},
		{"nop", _nop},
		{"sub", _sub},
		{"mul", _mul},
		{"div", _div},
		{"mod", _mod},
		{"pchar", _pchar},
		{"pstr", _pstr},
		{"rotl", _rotl},
		{"rotr", _rotr},
		{NULL, NULL},
	};

	i = 0;
	while (instruct[i].f != NULL && strcmp(instruct[i].opcode, token) != 0)
	{
		i++;
	}

	return (instruct[i].f);
}
