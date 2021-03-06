/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migferna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 13:17:27 by migferna          #+#    #+#             */
/*   Updated: 2021/02/18 23:33:06 by migferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	numeric_arg_req(t_shell *shell)
{
	char		*msg;
	char		*tmp;

	msg = ft_strjoin(shell->binary, ": ");
	tmp = ft_strdup(msg);
	free(msg);
	msg = ft_strjoin(tmp, shell->args[1]);
	free(tmp);
	print_errors(shell, " numeric argument required", msg);
	free(msg);
	exit(255);
}

static short	is_sign(char c, int *sign)
{
	if (c == '-' || c == '+')
	{
		if (c == '-')
			*sign = 1;
		return (1);
	}
	return (0);
}

static int		exit_atoi(t_shell *shell, const char *nptr)
{
	uintmax_t	num;
	int			sign;

	num = 0;
	sign = 0;
	while (ft_isspace(*nptr))
		nptr++;
	if (is_sign(*nptr, &sign))
		nptr++;
	while (*nptr == '0')
		nptr++;
	while (*nptr)
	{
		if ((!ft_isdigit(*nptr) && *nptr != ' ' && *nptr != '\t')
		|| ft_strlen(nptr) > 19)
			numeric_arg_req(shell);
		if (*nptr >= '0' && *nptr <= '9')
			num = num * 10 + *nptr - '0';
		nptr++;
	}
	if ((!sign && num > INTMAX_MAX) || (sign && num > -(uintmax_t)(INTMAX_MIN)))
		numeric_arg_req(shell);
	if (sign == 1)
		num = -1 * num;
	return ((int)num);
}

void			ft_exit(t_shell *shell)
{
	int		value;
	char	*nbr;

	if (shell->args[1])
	{
		nbr = shell->args[1];
		value = exit_atoi(shell, nbr);
		if (shell->args[2])
		{
			print_errors(shell, " too many arguments", shell->binary);
			exit(1);
		}
	}
	else
		value = 0;
	shell->stat_loc = value;
	clean_shell(shell);
	exit(value);
}
