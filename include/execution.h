/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkorvenp <jkorvenp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:18:06 by jkorvenp          #+#    #+#             */
/*   Updated: 2025/09/28 14:09:18 by jkorvenp         ###   ########.fr       */
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

void	execution(t_shell *shell, t_command	*command_list);

// Execution_utils
void    command_exit_status(t_shell *shell, pid_t pid);
void	execve_error(char *command_name);
void	command_error(char *command_name);
int     cmd_count(t_command *command);
void	wait_kids(t_shell *shell, int *pids, int count);

bool	is_built_in(t_command *command);
void	execute_built_in(t_command *command, t_shell *shell);
char	*find_command_path(t_command *command, t_shell *shell);
char	**env_to_array(t_shell *shell);
char	**ar_split(mem_arena *arena, char const *s, char c);

// Built-ins
int		echo(t_command *command);
int	    cd(t_shell *shell, char	*next);
int 	pwd();
void    exit_builtin(t_shell *shell, t_command *command);

int	    export(t_command *cmd, t_shell *shell);
t_env	*new_env(t_env *new, t_shell *shell, char *next_cmd);
t_env	*update_env(t_env *new, t_shell *shell, char *next_cmd);
int		unset(char *next_cmd, t_shell *shell);
int		env_builtin(t_shell *shell, char *next_cmd);
void	sort_env(t_shell *shell);

// Prepare files
int		handle_outfile(t_command *command);
int		handle_infile(t_command *command);
int		prepare_files(t_command	*command);


// Heredocs
int	    handle_heredoc(t_shell *shell, t_command *command);
char	*file_name(t_shell *shell);
void    unlink_infile(t_command *command);

#endif