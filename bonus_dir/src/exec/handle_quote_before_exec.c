/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote_before_exec.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:40:33 by odessein          #+#    #+#             */
/*   Updated: 2022/10/11 20:02:46 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	size_till_next_char(char *word, char c, int *i)
{
	int	size;

	size = 0;
	while (word[*i] && word[*i] != c)
	{
		(*i)++;
		size++;
	}
	if ((size_t)((*i) + 1) < ft_strlen(word))
		(*i)++;
	return (size);
}

static int	size_new_word_quote(char *word)
{
	int		size;
	int		i;

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

static void	str_cp_till_quote(char *word, char *new_word, int *i, int *j)
{
	char	c;

	if (*i > 0)
		c = word[*i - 1];
	while ((size_t)(*i) < ft_strlen(word) && word[*i] != c)
	{
		new_word[*j] = word[*i];
		(*j)++;
		(*i)++;
	}
	if ((size_t)(*i) + 1 < ft_strlen(word))
		(*i)++;
}

static void	str_quote_parse(char *word, char *new_word)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while ((size_t) i < ft_strlen(word))
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
			new_word[j++] = word[i++];
	}
	new_word[j] = 0;
}

//if word is "" or '', the new word should be an empty string, 
//not the same string even though in most cases, 
//we don't see the difference, but we see it in echo '', 
//and echo '""', which provide different outputs

char	*handle_quote(char *word)
{
	char	*new_word;
	int		size;

	if (word && ft_strlen(word) >= 2 && word[0] == '\"'
		&& word[1] == '\"' && !word[2])
		return (free(word), ft_strdup(""));
	if (word && ft_strlen(word) >= 2 && word[0] == '\''
		&& word[1] == '\'' && !word[2])
		return (free(word), ft_strdup(""));
	size = size_new_word_quote(word);
	new_word = malloc(sizeof(*new_word) * size + 1);
	if (!new_word)
		free_exit();
	str_quote_parse(word, new_word);
	free(word);
	return (new_word);
}
