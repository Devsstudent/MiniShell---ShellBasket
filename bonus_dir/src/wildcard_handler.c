/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 22:35:16 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/22 00:01:54 by mbelrhaz         ###   ########.fr       */
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

t_bool	get_filenames(t_list *filenames)
{
	DIR				*dir;
	struct dirent	*ent;
	t_block			*new_block;

	dir = opendir(".");
	if (!dir)
		return (perror("shellbasket"), FALSE);
	errno = 0;
	while (1)
	{
		ent = readdir(dir);
		if (errno != 0)
			perror("shellbasket");
		if (ent == NULL)
			break ;
		if (ent->d_name[0] == '.')
			continue;
		new_block = new_block(ft_strdup(ent->d_name));
		if (!new_block)
			free_exit();
		line_lst_addback(filenames, new_block);
	}
	if (closedir(dir) == -1)
		return (perror("shellbasket"), FALSE);
}

void	handle_wildcards(t_block *block)
{
	char	*word;
	t_list	*filenames;

	filenames = NULL;
	if (!block->word)
		return ;
	word = block->word;
	add_to_gc(LIST, filenames, get_gc());
	if (!get_filenames(filenames))
		return ;
	order_lst(filenames);
}
