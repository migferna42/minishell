#include "minishell.h"

static char	*get_string_between_quotes(char *str, int opening, int closing, char quote)
{
	char	*buff;
	int		i;
	int		j;

	j = 0;
	i = opening + 1;
	if (!(buff = malloc((closing - opening
		+ n_special_chars(str, opening, closing, quote) + 1) * sizeof(char))))
		return (NULL);
	while (i < closing)
	{
		if (quote == '\'' && (is_special_char(str[i]) ||
			(str[i - 1] == '\\' && is_alpha(str[i]))))
			buff[j++] = '\\';
		else
		{
			if (is_space(str[i])
				|| str[i] == '|' || str[i] == ';' || str[i] == '\'')
				buff[j++] = '\\';
			if (str[i - 1] == '\\' && (str[i] == '|' || is_alpha(str[i])))
				buff[j++] = '\\';
		}
		buff[j++] = str[i++];
	}
	buff[j] = '\0';
	return (buff);
}

static void	handle_quote_error(char quote)
{
	if (quote == '\'')
		print_errors(NULL, "unexpected EOF while looking for matching `''", NULL);
	else
		print_errors(NULL, "unexpected EOF while looking for matching `\"'", NULL);
}

static char	*find_closing_quote(char *str, char quote, int opening, int *closing)
{
	int 	i;

	i = opening + 1;
	*closing = 0;
	if (str[i] == quote && i >= 2 && str[i - 2] == ' ')
	{
		*closing += opening + 1;
		return (ft_strdup("\"\" "));
	}
	while (str[i])
	{
		if (str[i] == quote)
		{
			if ((quote == '\'' || check_prev_backslashes(str, i)))
			{
				*closing += opening + 1;
				return (get_string_between_quotes(str, opening, *closing, quote));
			}
		}
		*closing += 1;
		i++;
	}
	handle_quote_error(quote);
	return (NULL);
}

/*
** char	*remove_quotes(char *str) attemps to find first string
** between quotes. If there is a valid quote couple it returns
** string between them (which is returned by find_closing_quote
** function) If there isn't couple quotes, it returns input str.
*/

static char	*remove_quotes(char *str, int *opening, int *closing) 
{
	char 	quote;

	*opening = 0;
	while (str[*opening])
	{
		if ((str[*opening] == '\'' || str[*opening] == '\"')
			&& check_prev_backslashes(str, *opening))
		{
			quote = str[*opening];
			return(find_closing_quote(str, quote, *opening, closing));
		}
		*opening += 1;
	}
	*opening = -1;
	return (str);
}

static char	*join_parsed_str(char *str, char *str_in_quotes,
							char *buff, int opening)
{
	char 	*str_pre_quotes;
	char 	*tmp;
	char 	*tmp2;

	//Liberar
	str_in_quotes = embrace_expansion(str_in_quotes);
	if (opening != -1)
		str_in_quotes = mantain_expansion_spaces(str_in_quotes);
	if (opening && opening != -1)
	{
		if (!(str_pre_quotes = malloc((opening + 1) * sizeof(char))))
			return (NULL);
		ft_strlcpy(str_pre_quotes, str, opening + 1);
		str_pre_quotes = embrace_expansion(str_pre_quotes);
		tmp = ft_strjoin(buff, str_pre_quotes);
		tmp2 = ft_strjoin(tmp, str_in_quotes);
		free(str_pre_quotes);
		free(tmp);
	}
	else
	{
		if (ft_strcmp(buff, ""))
			tmp2 = str_in_quotes;
		else
			tmp2 = ft_strjoin(buff, str_in_quotes);
	}
	free(buff);
	/*
	if (str_in_quotes != str)
		free(str_in_quotes);
	*/
	return (tmp2);
}

char		*parse_quotes(char *str)
{
	char	*str_in_quotes;
	char	*buff;
	int		opening;
	int		closing;

	buff = ft_strdup("");
	while(*str)
	{
		if (!(str_in_quotes = remove_quotes(str, &opening, &closing)))
		{
			free (buff);
			exit(2);
		}
		buff = join_parsed_str(str, str_in_quotes, buff, opening);
		if (opening != -1)
			str += (closing + 1);
		else
			break;
	}
	return (buff);
}
