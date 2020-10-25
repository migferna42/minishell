#include "minishell.h"

char	**ft_strdup_matrix(char **envp)
{
	char	**tmp_envp;
	int		l;
	int		i;

	i = 0;
	l = 0;
	while (envp[l])
		l++;
	if (!(tmp_envp = malloc((l + 1) * sizeof(char *))))
		return (NULL);
	while (*envp)
	{
		tmp_envp[i] = ft_strdup(*envp);
		envp++;
		i++;
	}
	tmp_envp[i] = NULL;
	return (tmp_envp);
}

char	*absolute_bin_path(char *path, char *bin)
{
	char *tmp;

	tmp = ft_strjoin(path, "/");
	path = ft_strjoin(tmp, bin);
	free(tmp);
	return (path);
}

char	**realloc_matrix(char **envp, int additional)
{
	char	**tmp_envp;
	int		l;

	l = 0;
	while (envp[l])
		l++;
	if (!(tmp_envp = malloc((l + additional + 1) * sizeof(char *))))
		return (NULL);
	return (tmp_envp);
}

char	**add_env(char **variable, char **env, int n)
{
	char	**tmp_envp;
	int		i;

	i = 0;
	if (env[n])
		tmp_envp = realloc_matrix(env, 0);
	else
		tmp_envp = realloc_matrix(env, 1);
	tmp_envp[n] = *variable;
	while (*env)
	{
		if (i != n)
			tmp_envp[i] = ft_strdup(*env);
		env++;
		i++;
	}
	if (i != n)
		tmp_envp[i] = NULL;
	else
		tmp_envp[i + 1] = NULL;
	return (tmp_envp);
}

char	*ft_strjoin_noleak(char *str1, char *str2, short free_first, short free_second)
{
	char	*tmp;

  if (!str1)
  {
    if (!(str1 = malloc(sizeof(char))))
      return(NULL);
    *str1 = '\0';
  }
  if (!str2)
  {
    if (!(str2 = malloc(sizeof(char))))
      return(NULL);
    *str2 = '\0';
  }
	tmp = ft_strjoin(str1, str2);
	if (free_first)
		free(str1);
	if (free_second)
		free(str2);
	return (tmp);
}
