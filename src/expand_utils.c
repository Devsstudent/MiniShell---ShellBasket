/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:19:05 by odessein          #+#    #+#             */
/*   Updated: 2022/08/31 18:31:06 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	total_char_to_remove(char **key_arr)
{
	int	size;
	int	i;

	i = 0;
	size = 0;
	while (key_arr[i])
	{
		size += ft_strlen(key_arr[i]) + 1;
		i++;
	}
	return (size);
}

int	total_char_to_add(char **val_arr)
{
	int	size;
	int	i;

	i = 0;
	size = 0;
	while (val_arr[i])
	{
		size += ft_strlen(val_arr[i]);
		i++;
	}
	return (size);
}

t_bool	check_char(char c)
{
	if (c > -1 && c < 48)
		return (FALSE);
	return (TRUE);
}

void	advance_if_in_s_quote(t_bool *d_quote, char *word, int *i)
{
	char	c;

	c = word[*i];
	if (!(*d_quote) && c == '\"')
		*d_quote = TRUE;
	else if (*d_quote && c == '\"')
		*d_quote = FALSE;
	if (!(*d_quote) && c == '\'')
	{
		(*i) += 1;
		while (word[*i] && word[*i] != '\'')
			(*i) += 1;
	}
}

void	check_null_val(char *val)
{
	if (!val)
		free_exit();
}
