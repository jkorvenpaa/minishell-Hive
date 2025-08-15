/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_validation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:43:04 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/08/15 15:52:59 by jkorvenp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
typedef struct s_command
{
	char	**argv;
	char	*infile;
	char	*outfile;
	int	append;
	char	*heredoc;
	struct s_command	*next;
} t_command;
*/
/* built-in commands
◦ echo with option -n
◦ cd with only a relative or absolute path
◦ pwd with no options
◦ export with no options
◦ unset with no options
◦ env with no options or arguments
◦ exit with no options
*/



void	check_if_builtin(t_command *command)
{
	char	*cmd;
	char	*next_cmd;
	
	cmd = command->argv[0];
	cmd = command->argv[1];

	if (ft_strncmp(cmd, "echo", 4) == 0)
	{	if(ft_strncmp(next_cmd, "-n", 2) == 0)
			printf("%s", command->argv[2]);
		else
			printf("%s\n",command->argv[1]);
	}
	if (ft_strncmp(cmd, "cd", 2) == 0)
	if (ft_strncmp(cmd, "pwd", 3) == 0)
	if (ft_strncmp(cmd, "export", 6) == 0)
	if (ft_strncmp(cmd, "unset", 5) == 0)
	if (ft_strncmp(cmd, "env", 3) == 0)
	if (ft_strncmp(cmd, "exit", 4) == 0)

}

// define paths for others