/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:18:05 by odessein          #+#    #+#             */
/*   Updated: 2022/09/22 16:47:47 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	clean_tree(t_leaf *leaf)
{
	t_leaf	*right;
	t_leaf	*left;

	if (!leaf)
	{
		free(leaf);
		return ;
	}
	right = leaf->right;
	left = leaf->left;
	if (leaf->content)
		line_clear(leaf->content);
	free(leaf);
	if (right != NULL)
		clean_tree(right);
	if (left != NULL)
		clean_tree(left);
}

t_leaf	*new_leaf(t_line *cmd, t_type_leaf type)
{
	t_leaf	*leaf;

	leaf = (t_leaf *) malloc(sizeof(t_leaf));
	if (!leaf)
		free_exit();
	leaf->content = NULL;
	if (type != PIPE_L && cmd)
		leaf->content = cmd;
	leaf->type = type;
	leaf->right = NULL;
	leaf->left = NULL;
	return (leaf);
}

t_leaf	*new_leaf_bonus(t_line *sub, int lay_par, t_type_leaf p_type)
{
	t_leaf	*leaf;

	leaf = (t_leaf *)malloc(sizeof(t_leaf));
	if (!leaf)
		free_exit();
	leaf->content = sub;
	leaf->right = NULL;
	leaf->parentheses = lay_par;
	leaf->parent_type = p_type;
	leaf->left = NULL;
	if (sub != NULL)
	{
		if (sub->head && sub->head->token == P_OPEN)
		{
			if (sub->last->token == P_CLOSE)
				remove_parentheses(sub);
			leaf->type = PRTS;
		}
		else
			leaf->type = CMD;
	}
	return (leaf);
}
