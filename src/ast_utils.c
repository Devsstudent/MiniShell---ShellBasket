/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:18:05 by odessein          #+#    #+#             */
/*   Updated: 2022/08/31 18:18:06 by odessein         ###   ########.fr       */
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
