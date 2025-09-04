#include "minishell.h"
#include "execution.h"

int	handle_outfile(t_command *command)
{
	int	fd;

	if (command->append == 1)
	{
		fd = open(command->outfile, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0644);
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
		fd = open(command->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

int	handle_infile(t_command *command)
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

int	prepare_files(t_command	*command) // t_shell *shell
{
	int	res;

	res = 0;
	//if (command->heredoc)
	//	res = handle_heredoc(command);
	if (command->outfile)
		res = handle_outfile(command);
	if (command->infile)
		res = handle_infile(command);
	return (res);
}
