/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migferna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 20:20:41 by migferna          #+#    #+#             */
/*   Updated: 2021/02/22 12:45:40 by migferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		get_args_and_binary(t_shell *shell, int it)
{
	char	*tmp;

	tmp = expansion(shell, shell->commands[it]);
	free(shell->commands[it]);
	shell->args = get_args(tmp);
	free(tmp);
	shell->binary = ft_strdup(shell->args[0]);
}

static void		exec_arg(t_shell *shell)
{
	char	*path;

	if (!check_builtin(shell))
	{
		path = get_path(shell);
		if (path)
		{
			if (check_permissions(shell, path))
				execve(path, shell->args, shell->env);
		}
	}
}

static int		child_pipe(t_shell *shell, int it, int p[2])
{
	pid_t	pid;

	if ((pid = fork()) == 0)
	{
		close(p[0]);
		if (shell->commands[it + 1])
			dup2(p[1], 1);
		find_redirections(shell);
		exec_arg(shell);
		close(p[1]);
		exit(shell->stat_loc);
	}
	return (pid);
}

static void		wait_child(t_shell *shell, int *p, pid_t pid, int it)
{
	close(p[1]);
	dup2(p[0], 0);
	if (shell->commands[it])
		waitpid(pid, &shell->stat_loc, WNOHANG);
	shell->stat_loc = WEXITSTATUS(shell->stat_loc);
	close(p[0]);
	clean_matrix(shell->args);
	free(shell->args);
	free(shell->binary);
}

void			find_pipes(t_shell *shell)
{
	pid_t	pid;
	pid_t	aux_pid;
	int		p[2];
	int		it;
	int		std[2];

	it = -1;
	std[0] = dup(0);
	std[1] = dup(1);
	while (shell->commands[++it])
	{
		get_args_and_binary(shell, it);
		pipe(p);
		pid = child_pipe(shell, it, p);
		wait_child(shell, p, pid, it);
	}
	while ((aux_pid = wait(&shell->stat_loc)) > 0)
		if (aux_pid < pid)
			shell->stat_loc = WEXITSTATUS(shell->stat_loc);
	shell->stat_loc = WEXITSTATUS(shell->stat_loc);
	dup2(std[0], 0);
	dup2(std[1], 1);
}
