/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_bonus_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 22:18:55 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 22:41:55 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static t_type_leaf	get_type(t_token token)
{
	if (token == PIPE)
		return (PIPE_L);
	else if (token == OR)
		return (OR_L);
	else if (token == AND)
		return (AND_L);
	else if (token == RED_IN)
		return (RED_IN_L);
	else if (token == RED_OUT_TRUNC)
		return (RED_OUT_TRUNC_L);
	else if (token == RED_OUT_APPEND)
		return (RED_OUT_APPEND_L);
	else
		return (HERE_DOC_L);
}

int	fill_leaf(t_leaf *leaf, t_block *buff, int lay_par, t_line *sub)
{
	leaf->type = get_type(buff->token);
	leaf->parentheses = lay_par;
	leaf->left = new_leaf_bonus(sub, lay_par, leaf->type);
	return (lay_par);
}

t_line	*fill_till_ope(t_block **buff)
{
	t_line	*new_sub;

	new_sub = (t_line *)malloc(sizeof(t_line));
	if (!new_sub)
		free_exit();
	new_sub->head = NULL;
	line_cpy_till_ope(buff, new_sub);
	return (new_sub);
}

void	fill_sub(t_block **buff, t_bool *par, t_line **sub)
{
	t_block	*new;

	if ((*buff)->token == P_OPEN)
	{
		*sub = fill_parentheses_block(buff);
		if (subshell_redir(buff, &new, par, sub))
			return ;
		*par = TRUE;
	}
	else
	{
		*sub = fill_till_ope(buff);
		*par = FALSE;
	}
}
