
#include "minishell.h"

//remove heredoc files after command is executed
void	unlink_infile(t_command *command)
{
	while (command)
	{
		if (command->infile
			&& ft_strncmp(command->infile, "hd_temp_file", 12) == 0)
			unlink(command->infile);
		command = command->next;
	}
}

//finds an unique filename, until 16 heredoc files.
char	*file_name(t_shell *shell)
{
	char				*file_name;
	static unsigned int	count = 0;
	char				*num;

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
