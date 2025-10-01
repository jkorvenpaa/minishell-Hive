//create file to store stdin + validate file name-function
//redirect input, store line by line to file until exact match to delimeter,
// display prompt > in read loop;

#include "minishell.h"
#include "execution.h"

void	store_to_file(t_shell *shell, t_command *cmd, int fd)
{
	char	*line;
	char	*exp;

	if (cmd->heredoc_quoted == 1)
		cmd->heredoc = remove_quotes(shell->arena, cmd->heredoc);
	while (1)
	{
		if (g_sig == SIGINT)
			return;
		line = readline("> ");
		if (!line)
			break ;
		exp = arena_strdup(shell->arena, line);
		free (line);
		if (ft_strncmp(exp, cmd->heredoc, ft_strlen(cmd->heredoc) + 1) == 0)
			break ;
		exp = hd_line_exp(shell->arena, exp, &shell->expansion, cmd->heredoc_quoted);
		ft_putstr_fd(exp, fd);
		ft_putstr_fd("\n", fd);
	}
	
}

//Checks if command has a heredoc, creates a file to command->infile if so
int	handle_heredoc(t_shell *shell, t_command *command)
{
	char	*file;
	int		fd;

	heredoc_signals();
	while (command)
	{
		if (command->heredoc)
		{
			file = file_name(shell);
			if (!file)
			{
				perror ("hd_file failed");
				init_signals();
				return (1);
			}
			fd = open(file, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror("open hd_file failed");
				init_signals();
				return (1);
			}
			store_to_file(shell, command, fd);
			command->infile = file;
			close(fd);
			if (g_sig == SIGINT)
			{
				unlink_infile(command);
				g_sig = 0;
				return(1);
			}
		}
		command = command->next;
	}
	init_signals();
	return (0);
}
