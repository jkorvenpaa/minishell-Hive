

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
char	*file_name(t_shell *shell)
{
	char *str;
	char	*file_name;
	static unsigned int counter = 0;
	char	*number;

	file_name = arena_alloc(shell->arena, 8 + 1 + ft_strlen(num)+ ft_strlen.txt));
	if (!str)
		return (NULL);
	str =  strlcpy(str, "heredoc_", 8);

	file_name = ar_strjoin(str, num);
	file_name = ar_strjoin(file_name, ".txt")
	if (access(file_name, F_OK) == 0)
		return(NULL);
	else
		return (file_name);

}

int store_line_to_file(t_shell *shell, t_command *command, int fd)
{
	char	*buf;
	char	*line;
	ssize_t	r;
	char	*newline;

	r = 1;
	line = NULL;
	buf = arena_alloc(shell->arena, 10);
	if (!buf)
		return(1);

	while (1)
	{
		ft_putstr_fd ("> ", STDOUT_FILENO);
		r = read(STDIN_FILENO, buf, 10-1);
		if (r <= 0)
			break;
		buf[r] = '\0';
		if (ft_strncmp(buf, command->heredoc,ft_strlen(command->heredoc)) == 0)
			break;
		newline = ft_strchr(buf, '\n');
		if (newline)
			*newline = '\0';
		if (!line)
			line = buf;
		else
			line = ar_strjoin(shell->arena, line, buf);
		buf = NULL;
		//if ft_strchr(buf, '$') == 0;
			//hdoc_line_exp(shell->arena, buf, t_expansion *data, int hdoc_quoted)
		
		ft_putstr_fd(line, fd);
		line = NULL;
	}
	return (0);
}

int	handle_heredoc(char *argv, t_shell *shell, t_command *command)// t_expansion *data, int hdoc_quoted))
{
	
	char	*file;
	int		fd;
	

	//file = file_name(shell);
	if (!file)
		perror ("hd_file failed");
	fd = open(file, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open hd_file failed");
		return(1);
	}
	while (command)
	{
		if (store_line_to_file(shell, command, fd) == 0)
			command->infile = file;
		command = command->next;
	}
	close (fd);
	return (0);
}
*/