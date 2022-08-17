/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abstract_syntax_tree_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 15:47:32 by odessein          #+#    #+#             */
/*   Updated: 2022/08/15 19:22:53 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//When we have somthing in parentheses we should handle it as a subshell and just use the result so need to know if its in parenthese or not

t_leaf	*new_leaf_bonus(t_line *sub, int lay_par)
{
	t_leaf	*leaf;

	leaf = (t_leaf *)malloc(sizeof(t_leaf));
	if (!leaf)
		free_exit();
	add_to_gc(SIMPLE, leaf, get_gc());
	leaf->content = sub;
	leaf->right = NULL;
	leaf->parentheses = lay_par;
	leaf->left = NULL;
	if (sub != NULL)
	{
		if (sub->head && sub->head->token == P_OPEN)
		{
			remove_parentheses(sub);
			leaf->type = PRTS;
		}
		else
			leaf->type = CMD;
	}
	return (leaf);
}

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
	else
		add_to_gc(LINE, new_sub, get_gc());
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
	else
		return (AND_L);
}

t_line	*fill_till_ope(t_block **buff)
{
	t_line	*new_sub;

	new_sub = (t_line *)malloc(sizeof(t_line));
	if (!new_sub)
		free_exit();
	else
		add_to_gc(LINE, new_sub, get_gc());
	new_sub->head = NULL;
	line_cpy_till_ope(buff, new_sub);
	return (new_sub);
}

static void	last_elem(t_line *line, t_leaf *leaf, int lay_par)
{
	leaf->left = new_leaf_bonus(line, lay_par);
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
	leaf->left = new_leaf_bonus(sub, lay_par);
	return (lay_par);
}

void	fill_sub(t_block **buff, t_bool *par, t_line **sub)
{
	if ((*buff)->token == P_OPEN)
	{
		*sub = fill_parentheses_block(buff);
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
		leaf->right = new_leaf_bonus(NULL, lay_par);
		leaf = leaf->right;
		buff = buff->next;
	}
}

void	fill_ast_bonus(t_line *line, t_tree *tree)
{
	tree->head = new_leaf_bonus(NULL, 0);
	test(tree->head, line, 0);
}