/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abstract_syntax_tree_bonus_utils.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 22:15:41 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 22:42:39 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	handle_last_elem(t_leaf *leaf, t_line *sub, int lay_par, t_bool par)
{
	if (par)
		last_elem(sub, leaf, lay_par);
	else
	{
		leaf->type = CMD;
		leaf->content = sub;
		leaf->parentheses = lay_par;
	}
}

t_line	*fill_parentheses_block(t_block **buff)
{
	t_line	*new_sub;

	new_sub = (t_line *) malloc(sizeof(t_line));
	if (!new_sub)
		free_exit();
	new_sub->head = NULL;
	line_cpy_till_pend(buff, new_sub);
	return (new_sub);
}

void	remove_parentheses(t_line *line)
{
	t_block	*buff;

	if (line->head->token == P_OPEN)
	{
		buff = line->head->next;
		free(line->head->word);
		free(line->head);
		line->head = buff;
		while (buff)
		{
			if (buff->next->next == NULL && buff->next->token == P_CLOSE)
			{
				free(buff->next->word);
				free(buff->next);
				buff->next = NULL;
				return ;
			}
			buff = buff->next;
		}
	}
}
