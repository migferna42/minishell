/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migferna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 10:08:20 by migferna          #+#    #+#             */
/*   Updated: 2020/12/13 01:05:35 by migferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	to_lower(char *input)
{
	size_t	it;

	it = -1;
	while (input[++it])
	{
		input[it] = ft_tolower(input[it]);
	}
}

char	*search_binary(t_shell *shell, char **paths, char exited)
{
	(void)exited;
	size_t			it;
	struct stat		s;
	struct dirent	*direntp;
	DIR				*pdir;

	it = -1;
	to_lower(shell->args[0]);
	while (paths[++it])
	{
		if (stat(paths[it], &s) != -1)
		{
			if(s.st_mode & S_IFDIR)
			{
				if (!(pdir = opendir(paths[it])))
					return (NULL);
				while ((direntp = readdir(pdir)))
				{
					if (ft_strcmp(direntp->d_name, shell->args[0]))
					{
						closedir(pdir);
						return (paths[it]);
					}
					direntp++;
				}
				closedir(pdir);
			}
		}
	}
	return (NULL);
}

char	*get_env(char **env, char *arg, char *binary)
{
	size_t 	it;
	size_t 	len;
	char	*msg;

	len = ft_strlen(arg);
	it = 0;
	while (env[it])
	{
		if (ft_strncmp(env[it], arg, len) == 0)
			return (ft_strchr(env[it], '=') + 1);
		it++;
	}
	msg = ft_strjoin(" ", arg);
	msg = ft_strjoin(msg, " not set");
	print_errors(NULL, msg, binary, 0);
	return (NULL);
}
