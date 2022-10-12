/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abstract_syntax_tree_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 15:47:32 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 22:41:16 by odessein         ###   ########.fr       */
/*   Updated: 2022/09/19 22:29:09 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	subshell_redir(t_block **buff, t_block **new, t_bool *par, t_line **sub)
{
	int	i;

	i = 0;
	if (*buff && ((*buff)->token == RED_IN
			|| (*buff)->token == RED_OUT_TRUNC
			|| (*buff)->token == RED_OUT_APPEND)
		&& (*buff)->next->next)
	{
		while (i < 2)
		{
			*new = new_block((*buff)->word);
			if (!new)
				free_exit();
			(*new)->token = (*buff)->token;
			line_lst_addback(*sub, *new);
			*buff = (*buff)->next;
			i++;
		}
		*par = TRUE;
		return (TRUE);
	}
	return (FALSE);
}

static t_bool	simple_cmd(t_line *line)
{
	t_block	*buff;

	buff = line->head;
	while (buff)
	{
		if (buff->token == AND || buff->token == OR || buff->token == PIPE)
			return (FALSE);
		buff = buff->next;
	}
	return (TRUE);
}

static void	ast(t_leaf *leaf, t_line *line, int lay_par)
{
	t_block	*buff;
	t_line	*sub;
	t_bool	par;
	int		new_lay_par;

	buff = line->head;
	sub = NULL;
	while (buff)
	{
		fill_sub(&buff, &par, &sub);
		if (buff == NULL)
			return (handle_last_elem(leaf, sub, lay_par, par));
		new_lay_par = fill_leaf(leaf, buff, lay_par, sub);
		if (leaf->left->type == PRTS)
		{
			if (simple_cmd(sub))
				add_to_gc(LINE, sub, get_gc());
			ast(leaf->left, sub, ++new_lay_par);
		}
		leaf->right = new_leaf_bonus(NULL, lay_par, leaf->type);
		leaf = leaf->right;
		buff = buff->next;
	}
}

void	last_elem(t_line *line, t_leaf *leaf, int lay_par)
{
	leaf->left = new_leaf_bonus(line, lay_par, leaf->type);
	if (leaf->left->type == PRTS)
	{
		free(leaf->left);
		leaf->left = NULL;
		add_to_gc(LINE, line, get_gc());
		ast(leaf, line, ++lay_par);
	}
}

void	fill_ast_bonus(t_line *line, t_tree *tree)
{
	tree->head = new_leaf_bonus(NULL, 0, NONE);
	ast(tree->head, line, 0);
}
