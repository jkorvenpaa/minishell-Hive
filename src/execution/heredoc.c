//create file to store stdin + validate file name-function
//redirect input, store line by line to file until exact match to delimeter,
// display prompt > in read loop;

#include "minishell.h"

void	store_to_file(t_shell *shell, t_command *cmd, int fd)
{
	char	*line;
	char	*exp;

	heredoc_signals();
	if (cmd->heredoc_quoted == 1)
		cmd->heredoc = remove_quotes(shell->arena, cmd->heredoc);
	while (1)
	{
		if (g_sig == SIGINT)
			return ;
		line = readline("> ");
		if (!line)
			break ;
		exp = arena_strdup(shell->arena, line);
		free (line);
		if (ft_strncmp(exp, cmd->heredoc, ft_strlen(cmd->heredoc) + 1) == 0)
			break ;
		exp = hd_exp(shell->arena, exp, &shell->expansion, cmd->heredoc_quoted);
		ft_putstr_fd(exp, fd);
		ft_putstr_fd("\n", fd);
	}
}
int	open_file(t_shell *shell, t_command *command, char *file)
{
	int fd;

	fd = open(file, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open hd_file failed");
		return (-1);
	}
	store_to_file(shell, command, fd);
	command->infile = file;
	close(fd);
	return (0);
}
//finds an unique filename, until 16 heredoc files.
char	*file_name(t_shell *shell)
{
	char	*file_name;
	int		count;
	char	*num;

	count = 0;
	while (count < 16)
	{
		count++;
		num = arena_itoa(shell->arena, count);
		if (!num)
			return (NULL);
		file_name = ar_strjoin(shell->arena, "hd_temp_file", num);
		file_name = ar_strjoin(shell->arena, file_name, ".txt");
		if (access(file_name, F_OK) != 0)
			return (file_name);
	}
	return (NULL);
}

//Checks if command has a heredoc, creates a file to command->infile if so
int	handle_heredoc(t_shell *shell, t_command *command)
{
	char	*file;

	//heredoc_signals();
	while (command)
	{
		if (command->heredoc)
		{
			file = file_name(shell);
			if (!file)
				return (1);
			if(open_file(shell, command, file) != 0)
				return (1);
			if (g_sig == SIGINT)
			{
				ft_putstr_fd("\n", STDOUT_FILENO);
				unlink_infile(command);
				hd_exit_signals();
				return (1);
			}
		}
		command = command->next;
	}
	hd_exit_signals();
	return (0);
}
