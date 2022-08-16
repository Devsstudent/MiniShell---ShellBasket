/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_lst_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 15:13:44 by odessein          #+#    #+#             */
/*   Updated: 2022/08/16 14:36:02 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_block	*new_block(char *word)
{
	t_block	*new_block;

	new_block = malloc(sizeof(t_block));
	if (!new_block)
		free_exit();
	new_block->word = word;
	new_block->next = NULL;
	new_block->prev = NULL;
	return (new_block);
}

void	line_lst_addback(t_line *line, t_block *new)
{
	t_block	*buff;

	if (!line)
		return (free_exit());
	if (!new)
		return (free_exit());
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

void	line_cpy_till_pipe(t_block **buff, t_line *sub_lst)
{
	t_block	*new;

	while (*buff && (*buff)->token != PIPE)
	{
		new = new_block((*buff)->word);
		if (!new)
			return (free_exit());
		new->token = (*buff)->token;
		line_lst_addback(sub_lst, new);
		*buff = (*buff)->next;
	}
}
