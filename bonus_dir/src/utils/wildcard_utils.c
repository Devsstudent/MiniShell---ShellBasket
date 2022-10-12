/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 20:18:08 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 20:35:54 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	*double_arr_to_char(char **items)
{
	int		i;
	int		j;
	int		size;
	char	*word;
	int		k;

	i = -1;
	size = 0;
	while (items[++i])
		size += ft_strlen(items[i]) + 1;
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
			break ;
		if (ent->d_type == DT_DIR)
			i++;
		i++;
	}
	if (closedir(dir) == -1)
		return (perror("shellbasket"), 0);
	return (i);
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
