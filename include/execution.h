/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:18:06 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/09/15 14:16:41 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include <fcntl.h>
# include <unistd.h> 
# include <stdbool.h>
# include <sys/wait.h>
# include <errno.h>

typedef struct s_shell t_shell;
typedef struct s_command t_command;
typedef struct s_env t_env;

//void	command_loop(t_command *command, t_shell *shell);
void	execution(t_shell *shell, t_command	*command_list);

bool	check_if_built_in(t_command *command);
void	execute_built_in(t_command *command, t_shell *shell);
char	*find_command_path(t_command *command, t_shell *shell);
char	**env_to_array(t_shell *shell);
char	**ar_split(mem_arena *arena, char const *s, char c);

int		echo(t_command *command);
int		cd(char *next_cmd);
int 	pwd();
void	exit_builtin(t_shell *shell);

int		export(char	*next_cmd, t_shell *shell);
t_env	*new_env(t_env *new, t_shell *shell, char *next_cmd);
t_env	*update_env(t_env *new, t_shell *shell, char *next_cmd);
int		unset(char *next_cmd, t_shell *shell);
int		env_builtin(t_shell *shell);

int		handle_outfile(t_command *command);
int		handle_infile(t_command *command);
int		prepare_files(t_command	*command);

int	handle_heredoc(char *argv, t_shell *shell, t_command *command);

#endif