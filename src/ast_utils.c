#include "minishell.h"

void	clean_tree(t_leaf *leaf)
{
	t_leaf	*right;
	t_leaf	*left;

	if (!leaf)
		return ;
	right = leaf->right;
	left = leaf->left;
	if (leaf->content)
		line_clear(leaf->content);
	free(leaf);
	if (right != NULL)
		clean_tree(leaf->right);
	else
		return ;
	if (left != NULL)
		clean_tree(leaf->left);
	else
		return ;
}
