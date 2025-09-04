

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
/*
int	file_name(t_shell *shell, char *file_name)
{
	static int num = 0;
	int i;

	if (!file_name)
		file_name = arena_alloc(shell->arena, );

}*/
int store_line_to_file(t_shell *shell, t_command *command, int fd)
{
	char	*buf;
	char	*line;
	int		in;
	ssize_t	r;
	char	*input;

	r = 1;
	in = STDIN_FILENO;
	line = NULL;
	buf = arena_alloc(shell->arena, 10);
	if (!buf)
		return(1);

	while (1)
	{
		input = readline("> ");
		if (ft_strncmp(input, command->heredoc,ft_strlen(command->heredoc)) == 0)
			break;
		while (r > 0)
		{
			r = read(in, buf, 10);
			buf[r] = '\0';
			if (r == 0 || ft_strchr(buf, '\n'))
				break;
			if (!line)
				line = buf;
			line = ar_strjoin(shell->arena, line, buf);
			buf = NULL;
		//if ft_strchr(buf, '$') == 0;
			//hdoc_line_exp(shell->arena, buf, t_expansion *data, int hdoc_quoted)
	}
	ft_putstr_fd(line, fd);
	line = NULL;
	}
	return (0);
}

int	handle_heredoc(char *argv, t_shell *shell, t_command *command)// t_expansion *data, int hdoc_quoted))
{
	
	static char	*file;
	int		fd;
	

	file = argv; //file_name(shell, file);
	fd = open(file, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open hd_file failed");
		return(1);
	}
	if (store_line_to_file(shell, command, fd) == 0)
		command->infile = file;
	close (fd);
	return (0);
}
