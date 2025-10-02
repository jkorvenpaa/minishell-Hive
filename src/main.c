/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:10:40 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/02 16:01:16 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*	NEEDS TO BE CHECKED/FIXED:

-unset PATH doesn't work anymore: i think it might be because in find_command_path we use the 
system's getenv() so it doesn't read it from our own env_list. It must be there for sure because
i tried to do unset PATH and then env and it's deleted, so it's a problem of looking in the wrong place.
Solution to try: replace char *path for t_env *node and do node = get_env_node(shell->env_list, "PATH") 
and add safety check after of if (!node || !node->value) return NULL

-heredoc temporary files are not being deleted after success :( maybe because now we only unlink if
there's the signal. Solution to try: move it also to be after the command while loop in heredoc.c, before init_signals 
so it also deletes when there's success.

directory stuck

signals \n

*/


#include "minishell.h"
#include "execution.h"

t_shell	*init_shell(char const **envp)
{
	t_shell	*shell;
	t_mem_arena *arena;
	t_mem_arena *env_arena;

	arena = arena_init();
	env_arena = arena_init();
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->exit_status = 0;
	shell->env_list = init_env_list(env_arena, envp);
	shell->arena = arena;
	shell->env_arena = env_arena;
	shell->expansion.env_arena = env_arena;
	shell->expansion.env = shell->env_list;
	shell->expansion.exit_status = 0;
	shell->fd_in = dup(STDIN_FILENO);
	return (shell);
}
t_parser_context	init_parser_context_from_shell(t_shell *shell)
{
	t_parser_context	data;

	data.arena = shell->arena;
	data.env_arena = shell->env_arena;
	data.env = shell->env_list;
	data.exit_status = shell->exit_status;
	return (data);
}

void	exit_shell(t_shell *shell)
{
	int	e;

	rl_clear_history();
	e = shell->exit_status;
	printf("exit\n");
	close (shell->fd_in);
	free_arena(shell->env_arena);
	if (shell->arena)
		free_arena(shell->arena);
	free(shell);
	exit(e);
}

int main(int argc, char **argv, char const **envp)
{
	t_shell	*shell;
	t_command	*command_list;
	t_parser_context data;
	char	*input;
	
	(void) argv;
	(void) argc;
	
	shell = init_shell(envp);
	while (1)
	{
		init_signals();
		dup2(shell->fd_in, STDIN_FILENO);
		input = readline("minishell$ ");
		if (input == NULL)
			exit_shell(shell);
		if(ft_strlen(input) < ARG_MAX)
		{
			data = init_parser_context_from_shell(shell);
			command_list = run_parser(input, &data, shell);
			if (command_list && handle_heredoc(shell, command_list) == 0)
			{
				add_history(input);
				execution(shell, command_list);
			}
			arena_reset(shell->arena);
		}
		else
			printf("ARG_MAX exceeded\n");
		free(input);
	}
	return (0);
}
