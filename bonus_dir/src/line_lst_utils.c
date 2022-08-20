/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_lst_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 15:13:44 by odessein          #+#    #+#             */
/*   Updated: 2022/08/16 15:02:40 by odessein         ###   ########.fr       */
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
		if (buff->word)
			free(buff->word);
		free(buff);
		buff = tmp;
	}
	free(line);
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

void    line_cpy_till_ope(t_block **buff, t_line *sub_lst)
{
    t_block *new;

    while (*buff && !((*buff)->token == PIPE
            || (*buff)->token == OR || (*buff)->token == AND))
    {
        new = new_block((*buff)->word);
        if (!new)
            return (free_exit());
        new->token = (*buff)->token;
        line_lst_addback(sub_lst, new);
        *buff = (*buff)->next;
    }
}

void	line_cpy_till_pend(t_block **buff, t_line *sub_lst)
{
	t_block	*new;
	int		count;

	count = -12;
	while (*buff && count != 0)
	{
		if ((*buff)->token == P_OPEN)
		{
			if (count == -12)
				count = 0;
			count++;
		}
		if ((*buff)->token == P_CLOSE)
			count--;
		new = new_block((*buff)->word);
		if (!new)
			return (free_exit());
		new->token = (*buff)->token;
		line_lst_addback(sub_lst, new);
		*buff = (*buff)->next;
	}
}
