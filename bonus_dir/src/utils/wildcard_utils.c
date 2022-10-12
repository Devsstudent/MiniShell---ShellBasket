/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 20:18:08 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 20:38:59 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

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
