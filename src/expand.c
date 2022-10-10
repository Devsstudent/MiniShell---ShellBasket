/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 17:58:23 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/10/10 21:44:33 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	handle_simple_word(char *new_word, char *word, int *indexes)
{
	if (indexes[0] == -1)
	{
		if (word[0] == '$' && word[1] && (word[1] == '\'' || word[1] == '\"'))
			ft_strlcpy(new_word, &word[1], ft_strlen(word));
		else
			ft_strlcpy(new_word, word, ft_strlen(word) + 1);
		return (TRUE);
	}
	return (FALSE);
}

void	expand_block(t_block *block, char **key_arr, char **val_arr,
		int *indexes)
{
	int		size;
	char	*new_word;

	size = ft_strlen(block->word) + total_char_to_add(val_arr)
		- total_char_to_remove(key_arr);
	if (size > 2)
		if (block->word[0] == '$' && (block->word[1] == '\"'
				|| block->word[1] == '\"'))
			size--;
	new_word = (char *) malloc(sizeof(*new_word) * (size + 1));
	if (!new_word)
		free_exit();
	fill_new_word(new_word, block->word, val_arr, indexes);
	free(block->word);
	block->word = ft_strjoin(ft_strjoin(ft_strdup("\""), new_word), ft_strdup("\""));
}

void	handle_dollar_in_block(t_block *block, t_dict *dict)
{
	char	**key_arr;
	char	**val_arr;
	int		size_double_arr;
	int		*indexes;

	size_double_arr = get_nb_of_dollar(block);
	indexes = get_indexes_expandables(block, size_double_arr);
	add_to_gc(SIMPLE, indexes, get_gc());
	key_arr = (char **) malloc(sizeof(*key_arr) * (size_double_arr + 1));
	if (!key_arr)
		free_exit();
	add_to_gc(DOUBLE, key_arr, get_gc());
	fill_key_arr(block, key_arr, indexes);
	val_arr = (char **) malloc(sizeof(*val_arr) * (size_double_arr + 1));
	if (!val_arr)
		free_exit();
	add_to_gc(DOUBLE, val_arr, get_gc());
	fill_val_arr(key_arr, val_arr, dict);
	expand_block(block, key_arr, val_arr, indexes);
}

void	expand(t_line *line, t_dict *env)
{
	t_block	*buff;

	buff = line->head;
	while (buff)
	{
		if (buff->token == CMD_ARG || buff->token == FILES)
			handle_dollar_in_block(buff, env);
		buff = buff->next;
	}
}
