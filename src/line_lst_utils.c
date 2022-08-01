/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_lst_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 15:13:44 by odessein          #+#    #+#             */
/*   Updated: 2022/07/30 19:35:47 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_block	*new_block(char *word)
{
	t_block	*new_block;

	new_block = malloc(sizeof(t_block));
	if (!new_block)
		return (NULL);
	new_block->word = word;
	new_block->next = NULL;
	new_block->prev = NULL;
	return (new_block);
}

void	line_lst_addback(t_line *line, t_block *new)
{
	t_block	*buff;

	if (!line)
		return ;
	if (!new)
		return ;
	if (!(line->head))
	{
		line->head = new;
		line->last = line->head;
		return ;
	}
	buff = line->head;
	while (buff->next != NULL)
		buff = buff->next;
	new->prev = buff;
	buff->next = new;
	line->last = new;
}

void	line_clear(t_line *line)
{
	t_block	*buff;
	t_block	*tmp;

	buff = line->head;
	while (buff != NULL)
	{
		tmp = buff->next;
		free(buff->word);
		free(buff);
		buff = tmp;
	}
}