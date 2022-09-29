/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abstract_syntax_tree_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 15:47:32 by odessein          #+#    #+#             */
/*   Updated: 2022/09/29 23:21:20 by mbelrhaz         ###   ########.fr       */
/*   Updated: 2022/09/19 22:29:09 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//When we have somthing in parentheses we should handle it as a subshell and just use the result so need to know if its in parenthese or not

void	remove_parentheses(t_line *line)
{
	t_block	*buff;

	if (line->head->token == P_OPEN)
	{
		buff = line->head->next;
		free(line->head);
		line->head = buff;
		while (buff)
		{
			if (buff->next->next == NULL && buff->next->token == P_CLOSE)
			{
				free(buff->next);
				buff->next = NULL;
				return ;
			}
			buff = buff->next;
		}
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

t_type_leaf	get_type(t_token token)
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

static void	last_elem(t_line *line, t_leaf *leaf, int lay_par)
{
	leaf->left = new_leaf_bonus(line, lay_par, leaf->type);
	if (leaf->left->type == PRTS)
	{
		leaf->left = NULL;
		test(leaf, line, ++lay_par);
	}
}
static void	last_elem_not_par(t_leaf *leaf, int lay_par, t_line *sub)
{
	leaf->type = CMD;
	leaf->content = sub;
	leaf->parentheses = lay_par;
}

int	fill_leaf(t_leaf *leaf, t_block *buff, int lay_par, t_line *sub)
{
	leaf->type = get_type(buff->token);
	leaf->parentheses = lay_par;
	leaf->left = new_leaf_bonus(sub, lay_par, leaf->type);
	return (lay_par);
}

void	fill_sub(t_block **buff, t_bool *par, t_line **sub)
{
	int		i;
	t_block	*new;

	i = 0;
	if ((*buff)->token == P_OPEN)
	{
		*sub = fill_parentheses_block(buff);
		if (*buff && ((*buff)->token == RED_IN
			|| (*buff)->token == RED_OUT_TRUNC
			|| (*buff)->token == RED_OUT_APPEND)
			&& (*buff)->next->next)
		{
			while (i < 2)
			{
				new = new_block((*buff)->word);
				if (!new)
					return (free_exit());
				new->token = (*buff)->token;
				line_lst_addback(*sub, new);
				*buff = (*buff)->next;
				i++;
			}
			*par = TRUE;
			return ;
		}
		*par = TRUE;
	}
	else
	{
		*sub = fill_till_ope(buff);
		*par = FALSE;
	}
}

void	handle_last_elem(t_leaf *leaf, t_line *sub, int lay_par, t_bool par)
{
	if (par)
		last_elem(sub, leaf, lay_par);
	else
		last_elem_not_par(leaf, lay_par, sub);
}

void	test(t_leaf *leaf, t_line *line, int lay_par)
{
	t_block	*buff;
	t_line	*sub;
	t_bool	par;
	int		new_lay_par;

	buff = line->head;
	while (buff)
	{
		fill_sub(&buff, &par, &sub);
		if (buff == NULL)
		{
			handle_last_elem(leaf, sub, lay_par, par);
			return ;
		}
		new_lay_par = fill_leaf(leaf, buff, lay_par, sub);
		if (leaf->left->type == PRTS)
			test(leaf->left, sub, ++new_lay_par);
		leaf->right = new_leaf_bonus(NULL, lay_par, leaf->type);
		leaf = leaf->right;
		buff = buff->next;
	}
}

void	fill_ast_bonus(t_line *line, t_tree *tree)
{
	tree->head = new_leaf_bonus(NULL, 0, NONE);
	test(tree->head, line, 0);
}
