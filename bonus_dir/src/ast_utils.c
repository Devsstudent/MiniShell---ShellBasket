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

t_leaf	*new_leaf_bonus(t_line *sub, int lay_par)
{
	t_leaf	*leaf;

	leaf = (t_leaf *)malloc(sizeof(t_leaf));
	if (!leaf)
		free_exit();
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
