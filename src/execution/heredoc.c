/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:14:04 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/15 12:05:44 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//readline loop to store input line by line until delimeter is matched
static int	store_to_file(t_shell *shell, t_command *cmd, int fd)
{
	char	*line;
	char	*exp;

	if (cmd->heredoc_quoted == 1)
		cmd->heredoc = remove_quotes(shell->arena, cmd->heredoc);
	while (1)
	{
		line = readline("> ");
		if (!line)
			ft_putstr_fd("warning: "
				"here-document delimited by end-of-file\n", 1);
		if (!line || g_sig == SIGINT)
		{
			free(line);
			return (1);
		}
		exp = arena_strdup(shell->arena, line);
		free (line);
		if (ft_strncmp(exp, cmd->heredoc, ft_strlen(cmd->heredoc) + 1) == 0)
			break ;
		exp = hd_exp(shell->arena, exp, &shell->expansion, cmd->heredoc_quoted);
		ft_putstr_fd(exp, fd);
		ft_putstr_fd("\n", fd);
	}
	return (0);
}

//opens hd_file and sends to readline loop
static int	open_file(t_shell *shell, t_command *command, char *file)
{
	int	fd;
	int	error;

	heredoc_signals();
	fd = open(file, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open hd_file failed");
		return (-1);
	}
	error = store_to_file(shell, command, fd);
	command->infile = file;
	close(fd);
	return (error);
}

//finds and returns an unique filename, until 16 heredoc files.
static char	*file_name(t_shell *shell)
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

int	check_sigint(t_shell *shell, t_command *command)
{
	if (g_sig == SIGINT)
	{
		unlink_infile(command);
		shell->exit_status = 130;
		return (1);
	}
	return (0);
}

//main heredoc loop:
//if commands include heredocs, creates a tempfile to store input
int	handle_heredoc(t_shell *shell, t_command *command)
{
	char	*file;
	int		err;

	while (command)
	{
		if (command->heredoc)
		{
			file = file_name(shell);
			if (!file)
				return (1);
			err = open_file(shell, command, file);
			if (err != 0)
			{
				command->heredoc_error = 1;
				unlink_infile(command);
			}
			if (check_sigint(shell, command) == 1)
				return (1);
		}
		else
			err = 0;
		command = command->next;
	}
	init_signals();
	return (err);
}
