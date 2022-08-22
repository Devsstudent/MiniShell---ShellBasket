/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abstract_syntax_tree.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 15:47:32 by odessein          #+#    #+#             */
/*   Updated: 2022/08/22 14:05:16 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//loop on the line without repassing on it (each elem of line_lst);

//read till the next pipe and fill a new "line" or new_struct -> this function could be use for the left and right part of the pipe

//this function should return "line" or new_struct

//Function to create a pipe node

t_line	*fill_till_pipe(t_block **buff)
{
	t_line	*new_sub;

	new_sub = (t_line *) malloc(sizeof(t_line));
	if (!new_sub)
		free_exit();
	new_sub->head = NULL;
	line_cpy_till_pipe(buff, new_sub);
	return (new_sub);
}

void	fill_tree_while_pipe(t_tree **tree, t_line *cmd)
{
	t_leaf	*buff;

	buff = (*tree)->head;
	while (buff->right != NULL)
		buff = buff->right;
	buff->right = new_leaf(NULL, PIPE_L);
	buff->right->left = new_leaf(cmd, CMD);
}

static void	loop_line_to_ast(t_tree **tree, t_bool pipe, t_line *line)
{
	t_block	*buff;
	t_line	*cmd;

	buff = line->head;
	while (buff)
	{
		cmd = fill_till_pipe(&buff);
		if (buff == NULL)
		{
			fill_tree_while_pipe(tree, cmd);
			return ;
		}
		if (buff->token == PIPE && !pipe)
		{
			pipe = TRUE;
			(*tree)->head = new_leaf(NULL, PIPE_L);
			(*tree)->head->left = new_leaf(cmd, CMD);
		}
		else if (buff->token == PIPE && pipe)
			fill_tree_while_pipe(tree, cmd);
		buff = buff->next;
	}
}
/*
	VERSION BETTER I THINK
static void	loop_line_to_ast(t_leaf *leaf, t_bool pipe, t_block *buff)
{
	t_line	*cmd;

	while (buff)
	{
		cmd = fill_till_pipe(&buff);
		if (buff == NULL)
		{
			leaf = new_leaf(cmd, CMD);
			return ;
		}
		if (buff->token == PIPE)
		{
			leaf = new_leaf(NULL, PIPE_L);
			leaf->left = new_leaf(cmd, CMD);
			loop_line_to_ast(leaf, pipe, buff);
		}
		buff = buff->next;
	}
}
*/

void	fill_ast(t_line *line, t_tree *tree)
{
	t_block	*buff;
	t_line	*cmd;
	t_bool	pipe;

	tree->head = NULL;
	buff = line->head;
	pipe = FALSE;
	cmd = fill_till_pipe(&buff);
	if (!buff)
	{
		tree->head = new_leaf(cmd, CMD);
		return ;
	}
	else
		line_clear(cmd);
	loop_line_to_ast(&tree, pipe, line);
}
