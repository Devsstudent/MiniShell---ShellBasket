/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quote_before_exec.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 17:43:59 by odessein          #+#    #+#             */
/*   Updated: 2022/08/29 17:53:13 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

size_t		size_till_next_char(char *word, char c, size_t *i)
{
	size_t	size;

	size = 0;
	while (word[*i] && word[*i] != c)
	{
		(*i)++;
		size++;
	}
	if ((size_t) ((*i) + 1) < ft_strlen(word))
		(*i)++;
	return (size);
}

size_t	size_new_word_quote(char *word)
{
	size_t		size;
	size_t		i;

	size = 0;
	i = 0;
	while ((size_t) i < ft_strlen(word))
	{
		if (word[i] == '\'')
		{
			i++;
			size += size_till_next_char(word, '\'', &i);
		}
		else if (word[i] == '\"')
		{
			i++;
			size += size_till_next_char(word, '\"', &i);
		}
		else
		{
			size++;
			i++;
		}
	}
	return (size);
}

void	str_cp_till_quote(char *word, char *new_word, size_t *i, size_t *j)
{
	char	c;

	if (*i > 0)
		c = word[*i - 1];
	while (*i < ft_strlen(word) && word[*i] != c)
	{
		new_word[*j] = word[*i];
		(*j)++;
		(*i)++;
	}
	if ((size_t) (*i) + 1 < ft_strlen(word))
		(*i)++;
}

void	str_quote_parse(char *word, char *new_word)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < ft_strlen(word))
	{
		if (word[i] == '\'')
		{
			i++;
			str_cp_till_quote(word, new_word, &i, &j);
		}
		else if (word[i] == '\"')
		{
			i++;
			str_cp_till_quote(word, new_word, &i, &j);
		}
		else
		{
			new_word[j] = word[i];
			j++;
			i++;
		}
	}
	new_word[j] = 0;
}

void	remove_useless_quote(t_block *block)
{
	char	*new_word;
	size_t		size;

	size = size_new_word_quote(block->word);
	new_word = malloc(sizeof(*new_word) * size + 1);
	if (!new_word)
		free_exit();
	str_quote_parse(block->word, new_word);
	add_to_gc(SIMPLE, block->word, get_gc());
	block->word = new_word;
}

