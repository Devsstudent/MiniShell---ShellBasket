/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_handle_quotes_utils.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 16:50:33 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/10/20 10:45:12 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	str_fill_till_quote(char *word, int *i, int *k, char *block)
{
	char	c;

	(*k)++;
	if (*i > 0)
		c = word[*i - 1];
	while ((size_t)(*i) < ft_strlen(word) && word[*i] != c)
	{
		block[*k] = word[*i];
		(*i)++;
		(*k)++;
	}
	if ((size_t)(*i) < ft_strlen(word))
	{
		block[*k] = word[*i];
		(*i)++;
		(*k)++;
	}
}

void	str_adv_count_till_quote(char *word, int *i, int *k)
{
	char	c;

	if (*i > 0)
		c = word[*i - 1];
	while ((size_t)(*i) < ft_strlen(word) && word[*i] != c)
	{
		(*i)++;
		(*k)++;
	}
	if ((size_t)(*i) < ft_strlen(word))
	{
		(*i)++;
		(*k)++;
	}
	(*k)++;
}

void	str_advance_till_quote(char *word, int *i)
{
	char	c;

	if (*i > 0)
		c = word[*i - 1];
	while ((size_t)(*i) < ft_strlen(word) && word[*i] != c)
	{
		(*i)++;
	}
	if ((size_t)(*i) < ft_strlen(word))
		(*i)++;
}

char	**handle_quotes_in_blocks(char **splitos)
{
	int	i;

	i = 0;
	while (splitos[i])
	{
		splitos[i] = handle_quote(splitos[i]);
		i++;
	}
	return (splitos);
}

char	**ft_free(char **splitos, int size)
{
	int	i;

	i = 0;
	while (i <= size)
	{
		free(splitos[i]);
		i++;
	}
	free(splitos);
	splitos = NULL;
	return (splitos);
}
