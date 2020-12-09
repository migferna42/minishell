/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   permissions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migferna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 12:53:16 by migferna          #+#    #+#             */
/*   Updated: 2020/12/07 14:43:10 by migferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_permissions(t_shell *shell, char *path, char exited)
{
	struct stat s;

	if (stat(path, &s) != -1)
	{
		if (s.st_mode & S_IFDIR)
		{
			shell->stat_loc = 126;
			print_errors(shell, " is a directory", shell->binary, exited);
			exit(shell->stat_loc);
		}
		if (!(s.st_mode & S_IRUSR) || (s.st_mode & S_IRUSR && (!(s.st_mode & S_IXUSR))))
		{
			shell->stat_loc = 126;
			print_errors(shell, " Permission denied", shell->binary, exited);
			exit(shell->stat_loc);
		}
		

		if (s.st_mode & S_ISUID || s.st_mode & S_ISGID)
			exit (shell->stat_loc);
	}
	else
	{
		shell->stat_loc = 127;
		print_errors(shell, " command not found", shell->binary, exited);
		exit(shell->stat_loc);
	}
}
