/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 22:35:16 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/22 23:21:19 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//expand before wildcards, so we have the right thing at hand
//$something*, expand $something, then * is handled
//everything that begins with a . is discarded from the list of possible matches
//it is then alphabetically ordered
//trying to order them ? using ft_strncmp

//order the list using ft_strncmp
// we have a list

int		get_nb_files(void)
{
	DIR				*dir;
	struct dirent	*ent;
	int				i;

	i = 0;
	dir = opendir(".");
	if (!dir)
		return (perror("shellbasket"), 0);
	errno = 0;
	while (1)
	{
		ent = readdir(dir);
		if (errno != 0)
			perror("shellbasket");
		if (ent == NULL)
			break;
		if (ent->d_name[0] == '.')
			continue ;
		i++;
	}
	if (closedir(dir) == -1)
		return (perror("shellbasket"), 0);
	return (i);
}

void	fill_filenames(char **filenames, DIR **dir, struct dirent **ent)
{
	int	i;

	i = 0;
	*dir = opendir(".");
	if (!*dir)
		perror("shellbasket");
	errno = 0;
	while (1)
	{
		*ent = readdir(*dir);
		if (errno != 0)
			perror("shellbasket");
		if (*ent == NULL)
			break ;
		if ((*ent)->d_name[0] == '.')
			continue ;
		filenames[i++] = ft_strdup((*ent)->d_name);
	}
	if (closedir(*dir) == -1)
		perror("shellbasket");
	filenames[i] = NULL;
}

char	**get_filenames(void)
{
	DIR				*dir;
	struct dirent	*ent;
	char			**filenames;
	int				size;

	size = get_nb_files();
	filenames = (char **) malloc(sizeof(char *) * (get_nb_files() + 1));
	if (!filenames)
		free_exit();
	add_to_gc(DOUBLE, filenames, get_gc());
	if (size == 0)
		return (filenames[0] = NULL, filenames);
	fill_filenames(filenames, &dir, &ent);
	return (filenames);
}

int	get_min(char **filenames, int i)
{
	int	j;
	int	cmp;
	int	size;
	int	min;

	j = i;
	min = j;
	while (filenames[j])
	{
		if (!filenames[j + 1])
		{
			j++;
			continue;
		}
		size = ft_strlen(filenames[j + 1] + 1);
		cmp = ft_strncmp(filenames[j + 1], filenames[j], size);
		if (cmp <= 0)
			min = j;
		j++;
	}
	return (min);
}

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

t_bool	check_match(char *filename, char **patterns, t_bool end_pat)
{
	int		i;
	int		j;
	char	*ptr;

	i = 0;
	j = 0;
	ptr = filename;
	while (patterns[i])
	{
		ptr = ft_strnstr(ptr, patterns[i], ft_strlen(filename + j));
		if (ptr == NULL)
			return (FALSE);
		ptr += ft_strlen(patterns[i]);
		if (patterns[i + 1] == NULL && end_pat == TRUE)
			if (*(ptr + 1) == '\0')
				return (TRUE);
		i++;
	}
	return (TRUE);
}

void	select_filenames(char **filenames, char **patterns, char **matches, t_bool end_pat)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (filenames[i])
	{
		if (check_match(filenames[i], patterns, end_pat))
		{
			matches[j] = ft_strdup(filenames[i]);
			j++;
		}
		i++;
	}
}

char	**handle_wildcards(t_block *block)
{
	char	*word;
	char	**filenames;
	char	**matches;
	char	**patterns;
	int		i;

	i = 0;
	if (!block->word)
		return (NULL);
	word = block->word;
	filenames = get_filenames();
	order_filenames(filenames);
	while (filenames[i])
		i++;
	matches = malloc(sizeof(*matches) * (i + 1));
	patterns = ft_split(word, '*');
	if (!patterns)
		free_exit();
	if (patterns[0] == NULL)
	{
		i = 0;
		while (filenames[i])
		{
			matches[i] = ft_strdup(filenames[i]);
			i++;
		}
		matches[i] = NULL;
		free(word);
	}
	else
	{
		if (word[ft_strlen(word) - 1] != '*')
			select_filenames(filenames, patterns, matches, TRUE);
		else
			select_filenames(filenames, patterns, matches, FALSE);
	}
	return (matches);
}
