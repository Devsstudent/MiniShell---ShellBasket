/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_fill.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 20:16:38 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 20:17:28 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

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

void	fill_matches(char *word, char **filenames, char **patterns,
	char **matches)
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
