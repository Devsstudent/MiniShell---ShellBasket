/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 22:35:16 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/10/12 21:15:42 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//expand before wildcards, so we have the right thing at hand
//$something*, expand $something, then * is handled
//everything that begins with a . is discarded from the list of possible matches
//it is not when the pattern begins with a .
//if the pattern ends with a /, only the directories are taken into account
//it is then alphabetically ordered
//trying to order them ? using ft_strncmp

//order the list using ft_strncmp
// we have a list

void	order_filenames(char **filenames)
{
	int		i;
	int		size;
	int		min;
	char	*temp;

	i = 0;
	while (filenames[i])
		i++;
	size = i;
	i = 0;
	while (i < size)
	{
		min = get_min(filenames, i);
		temp = filenames[i];
		filenames[i] = filenames[min];
		filenames[min] = temp;
		i++;
	}
}

void	select_filenames(char **filenames, char **patterns, char **matches,
	char *word)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (filenames[i])
	{
		if (check_match(filenames[i], patterns, word))
		{
			matches[j] = ft_strdup(filenames[i]);
			j++;
		}
		i++;
	}
	matches[j] = NULL;
}

char	**handle_wildcards(char *word)
{
	char	**filenames;
	char	**matches;
	char	**patterns;
	int		i;

	i = 0;
	if (!word || ft_strchr(word, '*') == NULL)
		return (NULL);
	filenames = get_filenames();
	order_filenames(filenames);
	while (filenames[i])
		i++;
	matches = malloc(sizeof(*matches) * (i + 1));
	if (!matches)
		free_exit();
	add_to_gc(DOUBLE, matches, get_gc());
	patterns = ft_split(word, '*');
	if (!patterns)
		free_exit();
	add_to_gc(DOUBLE, patterns, get_gc());
	fill_matches(word, filenames, patterns, matches);
	return (matches);
}

void	wildcard(t_line *sub)
{
	t_block	*buff;
	char	**new_item;

	buff = sub->head;
	while (buff)
	{
		new_item = handle_wildcards(buff->word);
		if (new_item && new_item[0] != NULL)
		{
			free(buff->word);
			buff->word = double_arr_to_char(new_item);
		}
		buff = buff->next;
	}
}
