#include "minishell.h"

static int	handle_last_outfile(char *outfile, int append)
{
	int	fd;

	if (append == 1)
	{
		fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror("outfile open failed");
			return (1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else
	{
		fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("outfile open failed");
			return (1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (0);
}

static int	handle_outfile(char *outfile, int append)
{
	int	fd;

	if (append == 1)
	{
		fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror("outfile open failed");
			return (1);
		}
		close(fd);
	}
	else
	{
		fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("outfile open failed");
			return (1);
		}
		close(fd);
	}
	return (0);
}

static int	handle_infile(t_command *command)
{
	int	fd;

	fd = open(command->infile, O_RDONLY);
	if (fd < 0)
	{
		perror("infile open failed");
		return (2);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	prepare_files(t_command	*command)
{
	int	res;
	int	i;

	i = 0;
	res = 0;
	if (command->infile)
		res = handle_infile(command);
	while (command->outfile_list && command->outfile_list[i])
	{
		res = handle_outfile(command->outfile_list[i], command->append);
		i++;
	}
	if (command->outfile)
		res = handle_last_outfile(command->outfile, command->append);
	return (res);
}
