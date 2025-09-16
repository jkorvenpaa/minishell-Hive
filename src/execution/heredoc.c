

//create file to store stdin + validate file name-function
//redirect input, store line by line to file until exact match to delimeter,
// display prompt > in read loop;
//destroy file after command;
// if heredoc fd = open. (fd, STDIN_FILENO))



// char	*hdoc_line_exp(mem_arena *ar, char *line, t_expansion *data, int hdoc_quoted)
/* typedef struct s_expansion
{
    mem_arena   *env_arena;
    t_env   *env;
    int exit_status;
}   t_expansion;

// line = readline("> "); 
*/

#include "minishell.h"
#include "execution.h"

void unlink_infile(t_command *command)
{
	while (command)
	{
		if (command->infile && ft_strncmp(command->infile, "hd_temp_file", 12) == 0)
		unlink(command->infile);
		command = command->next;
	}
}
char	*file_name(t_shell *shell)
{
	char	*file_name;
	static unsigned int count = 0;
	char	*num;

	
	while (count <= 16)
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

int store_line_to_file(t_shell *shell, t_command *command, int fd)
{
	char	*buf;
	char	*line;
	ssize_t	r;
	char	*newline = NULL;
	char	*exp;

	r = 1;
	line = NULL;
	buf = arena_alloc(shell->arena, 10 + 1);
	if (!buf)
		return(1);
	ft_putstr_fd ("> ", STDOUT_FILENO);
	while (1)
	{
		
		r = read(STDIN_FILENO, buf, 10);
		if (r <= 0)
			break;
		buf[r] = '\0';
		if (!line)
			line = arena_strdup(shell->arena, buf);
		else
			line = ar_strjoin(shell->arena, line, buf);
		newline = ft_strchr(buf, '\n');
		if (newline)
		{
			*newline = '\0';
			if (ft_strncmp(buf, command->heredoc,ft_strlen(command->heredoc)) == 0)
				break;
			exp = hdoc_line_exp(shell->arena, line, &shell->expansion, command->heredoc_quoted);
			ft_putstr_fd(exp, fd);
			line = arena_strdup(shell->arena, "");
			ft_putstr_fd ("> ", STDOUT_FILENO);
		}
	}
	return (0);
}

int	handle_heredoc (t_shell *shell, t_command *command)// t_expansion *data, int hdoc_quoted))
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
				return(1);
			}
			if (store_line_to_file(shell, command, fd) == 0)
				command->infile = file;
			close(fd);
		}
		command = command->next;
	}
	return (0);
}

