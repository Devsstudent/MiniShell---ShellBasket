/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abstract_syntax_tree_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 15:47:32 by odessein          #+#    #+#             */
/*   Updated: 2022/08/13 23:06:20 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_leaf  *new_leaf_bonus(t_line *sub)
{
    t_leaf  *leaf;
    
    leaf = (t_leaf *)malloc(sizeof(t_leaf));
    if (!leaf)
        free_exit();
    add_to_gc(SIMPLE, leaf, get_gc());
    leaf->content = sub;
    leaf->right = NULL;
    leaf->left = NULL;
    if (sub != NULL)
    {
        if (sub->head && sub->head->token == P_OPEN)
            leaf->type = PRTS;
        else
            leaf->type = CMD;
    }
    return (leaf);
}

void    remove_parentheses(t_line *line)
{
    t_block *buff;

    if (line->head->token == P_OPEN)
    {
        buff = line->head->next;
        free(line->head);
        line->head = buff;
        while (buff)
        {
            if (buff->next->next == NULL) //&& buff->next->token == P_CLOSE)
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

t_type_leaf get_type(t_token token)
{
    if (token == PIPE)
        return (PIPE_L);
    else if (token == OR)
        return (OR_L);
    else
        return (AND_L);
}

t_line  *fill_till_ope(t_block **buff)
{
    t_line  *new_sub;
    
    new_sub = (t_line *)malloc(sizeof(t_line));
    if (!new_sub)
        free_exit();
    else
        add_to_gc(LINE, new_sub, get_gc());
    new_sub->head = NULL;
    line_cpy_till_ope(buff, new_sub);
    return (new_sub);
}

void    last_elem(t_line *line, t_leaf *leaf)
{
    leaf = new_leaf_bonus(line);
    if (leaf->type == PRTS)
    {
        leaf->left = new_leaf_bonus(line);
        test(leaf->left, line);
    }
}

void	test(t_leaf *leaf, t_line *line)
{
	t_block	*buff;
	t_line	*sub;

	remove_parentheses(line);
	buff = line->head;
	while (buff)
	{
		if (buff->token == P_OPEN)
			sub = fill_parentheses_block(&buff);
		else
			sub = fill_till_ope(&buff);
		if (buff == NULL)
		{
			last_elem(sub, leaf);
			return ;
		}
		leaf->type = get_type(buff->token);
		leaf->left = new_leaf_bonus(sub);
		if (leaf->left->type == PRTS)
			test(leaf->left, sub);
		leaf->right = new_leaf_bonus(NULL);
		leaf = leaf->right;
		buff = buff->next;
	}
}
	
void	fill_ast_bonus(t_line *line, t_tree *tree)
{
    tree->head = new_leaf_bonus(line);
    test(tree->head, line);
}
