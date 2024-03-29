/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_check_key.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:23:11 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 21:17:11 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	fill_key_arr(t_block *block, char **key_arr, int *indexes)
{
	int	i;
	int	j;
	int	size_val;

	i = 0;
	j = 0;
	while (indexes[i] != -1)
	{
		size_val = size_doll_val(block->word, indexes[i]);
		key_arr[j] = ft_substr(block->word, indexes[i] + 1, size_val);
		j++;
		i++;
	}
	key_arr[j] = NULL;
}

int	*get_indexes_expandables(t_block *block, int dollar)
{
	int		i;
	int		j;
	int		*indexes;
	t_bool	d_quote;

	indexes = malloc(sizeof(int) * (dollar + 1));
	init_get_indexes_expandables(&i, &j, indexes, &d_quote);
	while (block->word[i])
	{
		advance_if_in_s_quote(&d_quote, block->word, &i);
		if (block->word[i + 1] && block->word[i] == '$'
			&& check_char(block->word[i + 1]))
		{
			indexes[j++] = i;
		}
		else if (block->word[i + 1] && block->word[i + 1] == '?'
			&& block->word[i] == '$')
			indexes[j++] = i;
		i++;
	}
	indexes[j] = -1;
	return (indexes);
}

void	encompass_quotes(char *new_word, int *j, char quote)
{
	if (quote == '\'')
	{
		new_word[(*j)++] = '\"';
		new_word[(*j)++] = '\'';
		new_word[*j] = '\"';
	}
	else if (quote == '\"')
	{
		new_word[(*j)++] = '\'';
		new_word[(*j)++] = '\"';
		new_word[*j] = '\'';
	}
}

void	replace_key(char *new_word, int *j, char **val_arr)
{
	int		i;

	i = 0;
	if (!(*val_arr))
		return ;
	while ((*val_arr)[i])
	{
		if ((*val_arr)[i] == '\'' || (*val_arr)[i] == '\"')
			encompass_quotes(new_word, j, (*val_arr)[i]);
		else
			new_word[*j] = (*val_arr)[i];
		i++;
		(*j)++;
	}
}

int	get_nb_of_dollar(t_block *block)
{
	int		i;
	int		dollar;
	t_bool	d_quote;

	i = 0;
	dollar = 0;
	d_quote = FALSE;
	if (block->word[i] == '$' && block->word[i + 1]
		&& (block->word[i + 1] == '\'' || block->word[i + 1] == '\"'))
		return (dollar);
	while (block->word[i])
	{
		advance_if_in_s_quote(&d_quote, block->word, &i);
		if (block->word[i] == '$' && block->word[i + 1]
			&& check_char(block->word[i + 1]))
			dollar++;
		else if (block->word[i] == '$' && block->word[i + 1] == '?')
			dollar++;
		i++;
	}
	return (dollar);
}
