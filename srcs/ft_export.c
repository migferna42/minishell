#include "minishell.h"

static size_t	is_bigger(char *s1, char *s2)
{
	int ret;

	ret = 0;
	if (*s1 == *s2)
		ret = is_bigger(s1 + 1, s2 + 1);
	else if (*s1 > *s2)
		ret = 1;
	return (ret);
}

static char		**sort_alfa(char **envp)
{
	char	*tmp;
	size_t	len;
	size_t	i;
	size_t	j;

	len = 0;
	i = 0;
	while (envp[len + 1])
		len++;
	while (i < len)
	{
		j = 0;
		while (j < len - i)
		{
			if (is_bigger(envp[j], envp[j + 1]))
			{
				tmp = envp[j];
				envp[j] = envp[j + 1];
				envp[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
	return (envp);
}

static size_t	print_sorted_env(t_shell *shell)
{
	char **tmp_envp;

	tmp_envp = ft_strdup_matrix(shell->env);
	ft_env(shell->args + 1, sort_alfa(tmp_envp));
	clean_matrix(tmp_envp);
	free(tmp_envp);
	return (1);
}

int				ft_export(t_shell *shell)
{
	int  i;
	int  j;
	char *value;
	char *tmp;
	char **tmp_env;

	j = 1;
	if (shell->args[j] || !print_sorted_env(shell))
		while (shell->args[j])
		{
			i = 0;
			if ((tmp = ft_strchr(shell->args[j], '=')) && *tmp != *shell->args[j])
			{
				value = ft_strdup(shell->args[j]);
				*tmp = '\0';
				while (shell->env[i] && ft_strncmp(shell->env[i], shell->args[j],
							ft_strlen(shell->args[j])))
					i++;
				tmp_env = add_env(&value, shell->env, i);
				clean_env(shell);
				shell->env = tmp_env;
			}
			j++;
		}
	return (1);
}
