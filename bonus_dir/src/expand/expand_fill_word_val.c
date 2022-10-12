/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_fill_word_val.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:26:17 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 18:55:13 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	fill_val_arr(char **key_arr, char **val_arr, t_dict *dict)
{
	int		i;
	char	*value;

	i = -1;
	while (key_arr[++i])
	{
		if (key_arr[i][0] == '?' && !key_arr[i][1])
		{
			value = ft_itoa(g_exit_status);
			check_null_val(value);
			val_arr[i] = value;
			continue ;
		}
		value = dict_get_value(dict, key_arr[i]);
		if (!value)
			val_arr[i] = ft_strdup("");
		else
		{
			value = ft_strdup(value);
			check_null_val(value);
			val_arr[i] = value;
		}
	}
	val_arr[i] = NULL;
}

static void	init_i_j_k(int *i, int *j, int *k)
{
	*i = 0;
	*j = 0;
	*k = 0;
}

static void	fill_new_word_part_1(char *word, int *i)
{
	while (word[*i] && check_char(word[*i]))
	{
		if (*i - 1 >= 0 && word[*i - 1] == '$'
			&& char_is_num(word[*i], i) && word[*i] == '?')
			break ;
		(*i)++;
	}
}

void	fill_new_word(char *new_word, char *word, char **val_arr, int *indexes)
{
	int		i;
	int		j;
	int		k;

	init_i_j_k(&i, &j, &k);
	if (handle_simple_word(new_word, word, indexes))
		return ;
	while (word[i])
	{
		while (i != indexes[k] && word[i])
			new_word[j++] = word[i++];
		if (word[i] && i == indexes[k])
		{
			i++;
			fill_new_word_part_1(word, &i);
			replace_key(new_word, &j, &val_arr[k++]);
			if (i - 1 >= 0 && word[i - 1] == '$' && word[i] == '?')
				i++;
		}
	}
	new_word[j] = '\0';
}

int	size_doll_val(char *word, int i)
{
	int	size;

	size = 0;
	i++;
	if ((word[i] >= '0' && word[i] <= '9') || word[i] == '?')
		return (++size);
	while (word[i] && check_char(word[i]))
	{
		size++;
		i++;
	}
	return (size);
}
