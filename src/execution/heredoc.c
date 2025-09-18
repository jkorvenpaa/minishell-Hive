//create file to store stdin + validate file name-function
//redirect input, store line by line to file until exact match to delimeter,
// display prompt > in read loop;

#include "minishell.h"
#include "execution.h"

char	*update_line(t_shell *shell, char *line, char *buf)
{
	if (!line)
		line = arena_strdup(shell->arena, buf);
	else
		line = ar_strjoin(shell->arena, line, buf);
	return (line);
}

char	*read_to_buffer(char *buf)
{
	ssize_t	r;

	r = 1;
	r = read(STDIN_FILENO, buf, 10);
	if (r <= 0)
		return (NULL);
	buf[r] = '\0';
	return (buf);
}

void	read_loop(t_shell *shell, t_command *cmd, int fd, char *buf)
{
	char	*line;
	char	*newline;
	char	*exp;

	line = NULL;
	while (1)
	{
		buf = read_to_buffer(buf);
		if (!buf)
			break ;
		line = update_line(shell, line, buf);
		newline = ft_strchr(buf, '\n');
		if (newline)
		{
			*newline = '\0';
			if (ft_strncmp(buf, cmd->heredoc, ft_strlen(cmd->heredoc) + 1) == 0)
				break ;
			exp = hdoc_line_exp(shell->arena, line, &shell->expansion, cmd->heredoc_quoted);
			ft_putstr_fd(exp, fd);
			line = arena_strdup(shell->arena, "");
			ft_putstr_fd ("> ", STDOUT_FILENO);
		}
	}
}

//store stdin to file, line by line until delimeter matches the line
void	store_to_file(t_shell *shell, t_command *command, int fd, char *file)
{
	char	*buf;

	buf = arena_alloc(shell->arena, 10 + 1);
	if (!buf)
		return ;
	ft_putstr_fd ("> ", STDOUT_FILENO);
	read_loop(shell, command, fd, buf);
	command->infile = file;
}

//Checks if command has a heredoc, creates a file to command->infile if so
int	handle_heredoc(t_shell *shell, t_command *command)
{
	char	*file;
	int		fd;

	while (command)
	{
		if (command->heredoc)
		{
			file = file_name(shell);
			if (!file)
			{
				perror ("hd_file failed");
				return (1);
			}
			fd = open(file, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror("open hd_file failed");
				return (1);
			}
			store_to_file(shell, command, fd, file);
			close(fd);
		}
		command = command->next;
	}
	return (0);
}
