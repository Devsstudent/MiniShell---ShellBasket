/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 22:35:16 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/23 15:45:54 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../libft/libft.h"

//expand before wildcards, so we have the right thing at hand
//$something*, expand $something, then * is handled
//everything that begins with a . is discarded from the list of possible matches
//it is then alphabetically ordered
//trying to order them ? using ft_strncmp

//order the list using ft_strncmp
// we have a list

#include "dirent.h"
#include "stdio.h"
#include "sys/types.h"
#include "errno.h"

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
		exit(0);
		//free_exit();
	//add_to_gc(DOUBLE, filenames, get_gc());
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
	while (patterns[i])
	{
		if (i == 0 && word[0] != '*' && ft_strncmp(filename, patterns[0], ft_strlen(patterns[0])) != 0)
			return (FALSE);
		ptr = ft_strnstr(ptr, patterns[i], ft_strlen(filename + j));
		if (ptr == NULL)
			return (FALSE);
		ptr += ft_strlen(patterns[i]);
		if (patterns[i + 1] == NULL && word[ft_strlen(word) - 1] != '*')
			if (*(ptr + 1) != '\0')
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

char	**handle_wildcards(char *word)
{
	char	**filenames;
	char	**matches;
	char	**patterns;
	int		i;

	i = 0;
	if (!word)
		return (NULL);
	filenames = get_filenames();
	order_filenames(filenames);
	while (filenames[i])
		i++;
	matches = malloc(sizeof(*matches) * (i + 1));
	patterns = ft_split(word, '*');
	if (!patterns)
		exit(0);
		//free_exit();
	if (patterns[0] == NULL)
	{
		i = 0;
		while (filenames[i])
		{
			matches[i] = ft_strdup(filenames[i]);
			i++;
		}
		matches[i] = NULL;
	}
	else
	{
		if (word[ft_strlen(word) - 1] != '*')
			select_filenames(filenames, patterns, matches, word);
		else
			select_filenames(filenames, patterns, matches, word);
	}
	return (matches);
}

int	main(void)
{
	char	**files;

	files = handle_wildcards("*o*t");
	int	i = 0;
	while (files[i])
	{
		printf("%s\n", files[i]);
		i++;
	}
	printf("THIS IS THE END\n");
}
