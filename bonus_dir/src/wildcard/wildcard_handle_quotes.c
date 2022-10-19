/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_handle_quotes.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 13:37:45 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/10/19 19:36:25 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	get_size_splitos(char *word)
{
	int	i;
	int	size;
	int	max;

	i = 0;
	size = 1;
	max = ft_strlen(word) - 1;
	if (word[0] == '*' && !word[1])
		return (0);
	while (word[i])
	{
		if (word[i] == '*' && i != 0 && i != max)
			size++;
		if (word[i] == '\'' && i++)
			str_advance_till_quote(word, &i);
		else if (word[i] == '\"' && i++)
			str_advance_till_quote(word, &i);
		else
			i++;
	}
	return (size);
}

static void	get_size_blocks(char *word, int *sizes)
{
	int	ijk[3];

	ijk[0] = 0;
	ijk[1] = 0;
	ijk[2] = 0;
	if (word[0] == '*' && !word[1])
		return ;
	while (word[ijk[0]])
	{
		while (word[ijk[0]] != '*' && word[ijk[0]])
		{
			if (word[ijk[0]] == '\'' && ijk[0]++)
				str_adv_count_till_quote(word, &(ijk[0]), &(ijk[2]));
			else if (word[ijk[0]] == '\"' && ijk[0]++)
				str_adv_count_till_quote(word, &(ijk[0]), &(ijk[2]));
			else if (++(ijk[0]))
				ijk[2]++;
		}
		sizes[ijk[1]] = ijk[2];
		if (word[ijk[0]] == '*' && ijk[0] != 0)
			ijk[1]++;
		ijk[2] = 0;
		if (word[ijk[0]])
			ijk[0]++;
	}
}

static void	fill_splitos_block(char *block, char *word, int *i, int *k)
{
	while (word[*i] != '*' && word[*i])
	{
		if (word[*i] == '\'')
		{
			block[*k] = word[*i];
			(*i)++;
			str_fill_till_quote(word, i, k, block);
		}
		else if (word[*i] == '\"')
		{
			block[*k] = word[*i];
			(*i)++;
			str_fill_till_quote(word, i, k, block);
		}
		else
		{
			block[*k] = word[*i];
			(*i)++;
			(*k)++;
		}
	}
}

static void	fill_splitos(char **splitos, char *word)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	if (splitos[0] == NULL)
		return ;
	while (word[i])
	{
		fill_splitos_block(splitos[j], word, &i, &k);
		splitos[j][k] = 0;
		if (word[i] == '*' && i != 0)
			j++;
		if (word[i])
			i++;
		k = 0;
	}
}

char	**ft_split_wildcard(char *word)
{
	char	**splitos;
	int		size;
	int		i;
	int		*sizes;

	size = get_size_splitos(word);
	splitos = malloc(sizeof(char *) * (size + 1));
	if (!splitos)
		return (NULL);
	sizes = malloc(sizeof(int) * (size));
	if (!sizes)
		return (free(splitos), NULL);
	add_to_gc(SIMPLE, sizes, get_gc());
	get_size_blocks(word, sizes);
	splitos[size] = NULL;
	i = 0;
	while (i < size)
	{
		splitos[i] = malloc(sizeof(char) * (sizes[i] + 1));
		if (!splitos[i++])
			return (ft_free(splitos, i));
	}
	fill_splitos(splitos, word);
	return (handle_quotes_in_blocks(splitos));
}
