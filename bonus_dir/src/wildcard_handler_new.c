/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_handler_new.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 22:35:16 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/09/16 16:35:02 by mbelrhaz         ###   ########.fr       */
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

int	get_nb_files(void)
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
		if (ent->d_type == DT_DIR)
			i++;
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
		if ((*ent)->d_type == DT_DIR)
		{
			filenames[i++] = ft_strjoin(ft_strdup((*ent)->d_name), "/");
			if (!filenames[i - 1])
				free_exit();
		}
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
		size = ft_strlen(filenames[j]) + 1;
		cmp = ft_strncmp(filenames[j], filenames[min], size);
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

t_bool	check_match(char *filename, char **patterns, char *word)
{
	int		i;
	int		j;
	char	*ptr;

	i = 0;
	j = 0;
	ptr = filename;
	if ((filename[0] == '.' && word[0] != '.')
			|| (filename[ft_strlen(filename) - 1] == '/' 
					&& word[ft_strlen(word) - 1] != '/'))
		return (FALSE);
	while (patterns[i])
	{
		if (i == 0 && word[0] != '*' 
				&& ft_strncmp(filename, patterns[0],
					ft_strlen(patterns[0])) != 0)
			return (FALSE);
		ptr = ft_strnstr(ptr, patterns[i], ft_strlen(filename + j));
		if (ptr == NULL)
			return (FALSE);
		ptr += ft_strlen(patterns[i]);
		if (patterns[i + 1] == NULL && word[ft_strlen(word) - 1] != '*')
			if (*ptr && *(ptr + 1) != '\0')
				return (FALSE);
		i++;
	}
	return (TRUE);
}

void	select_filenames(char **filenames, char **patterns, char **matches, char *word)
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

void	fill_matches(char *word, char **filenames, char **patterns, char **matches)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (patterns[0] == NULL)
	{
		while (filenames[i])
		{
			if (filenames[i][0] == '.'
				|| filenames[i][ft_strlen(filenames[i]) - 1] == '/')
			{
				i++;
				continue ;
			}
			matches[j] = ft_strdup(filenames[i]);
			i++;
			j++;
		}
		matches[j] = NULL;
	}
	else
		select_filenames(filenames, patterns, matches, word);
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
	patterns = ft_split(word, '*');
	if (!patterns)
		free_exit();
	fill_matches(word, filenames, patterns, matches);
	return (matches);
}

char	*double_arr_to_char(char **items);

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

char	*double_arr_to_char(char **items)
{
	int	i;
	int	j;
	int	size;
	char	*word;
	int		k;

	i = 0;
	size = 0;
	while (items[i])
	{
		size += ft_strlen(items[i]);
		size++;
		i++;
	}
	word = malloc(sizeof(*word) * size + 1);
	if (!word)
		free_exit();
	i = 0;
	k = 0;
	while (items[i])
	{
		j = 0;
		while (items[i][j])
			word[k++] = items[i][j++];
		i++;
		if (items[i])
			word[k++] = ' ';
		else
			word[k] = 0;
	}
	return (word);
}

//if / at the end, only the directories should be displayed, if any
/*
int	main(void)
{
	char	**files;

	files = handle_wildcards("*i*s/");
	int	i = 0;
	while (files[i])
	{
		printf("%s\n", files[i]);
		i++;
	}
	printf("THIS IS THE END\n");
}*/
