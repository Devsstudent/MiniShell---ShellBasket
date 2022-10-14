/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus_leaf_type.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 22:10:24 by odessein          #+#    #+#             */
/*   Updated: 2022/10/14 21:40:04 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	leaf_type_or(t_leaf *leaf, t_info *exec_in, t_dict *env)
{
	exec_tree(leaf->left, exec_in, env, leaf);
	wait_sub_process(exec_in);
	if (leaf->left->parentheses > leaf->parentheses)
		if (dup2(exec_in->stdou, STDOUT_FILENO) == -1)
			return (perror("back to stdout"));
	if (g_exit_status == 0 && leaf->right->parentheses == leaf->parentheses
		&& (leaf->right->type == AND_L))
		exec_tree(leaf->right, exec_in, env, leaf);
	else if (g_exit_status != 0)
	{
		if (exec_in->fork)
			exec_in->open_fd = exec_in->sub_std;
			//Have to unlink to file too
	//	if (dup2(exec_in->sub_std, STDIN_FILENO) == -1)
	//		perror("brie le fromage");
		exec_tree(leaf->right, exec_in, env, leaf);
	}
	else if (g_exit_status == 0
		&& leaf->right->parentheses != leaf->parentheses)
		return ;
}

void	leaf_type_and(t_leaf *leaf, t_info *exec_in, t_dict *env, t_leaf *prev)
{
	if (prev && prev->type == OR_L && g_exit_status != 0)
	{
		exec_tree(leaf->left, exec_in, env, leaf);
		wait_sub_process(exec_in);
	}
	else if (!prev || prev->type != OR_L)
	{
		exec_tree(leaf->left, exec_in, env, leaf);
		wait_sub_process(exec_in);
	}
	if (leaf->left->parentheses > leaf->parentheses)
		if (dup2(exec_in->stdou, STDOUT_FILENO) == -1)
			return (perror("back to stdout"));
	if (g_exit_status == 0)
		exec_tree(leaf->right, exec_in, env, leaf);
}

void	leaf_type_pipe(t_leaf *leaf, t_info *exec_in,
				t_dict *env, t_leaf *prev)
{
	if (leaf->type == PIPE_L)
	{
		if (!parentheses_pipe(leaf, exec_in))
			return ;
		if (prev && prev->type == PIPE_L)
		{
			if (prev->head && exec_in->right)
				leaf->head = TRUE;
			exec_in->prev_pipe = TRUE;
		}
		else
		{
			leaf->head = TRUE;
			exec_in->prev_pipe = FALSE;
		}
		if (!exec_left_right_pipe(leaf, exec_in, env))
			return ;
	}
}

void	leaf_type_cmd(t_leaf *leaf, t_info *exec_in, t_dict *env, t_leaf *prev)
{
	if (!prev)
		exec_in->end = TRUE;
	exec_cmd(exec_in, leaf, env);
}

t_bool	leaf_red_in(t_leaf *leaf, t_info *exec_in, t_dict *env)
{
	if (leaf->type == RED_IN_L)
	{
		exec_in->open_fd = open(leaf->right->content->head->word, O_RDONLY);
		if (exec_in->open_fd < 0)
			return (perror_false("redir open fail"));
		if (leaf->left->parentheses > leaf->parentheses)
			if (dup2(exec_in->open_fd, STDIN_FILENO) == -1)
				return (perror_false("back to stdout"));
		exec_tree(leaf->left, exec_in, env, leaf);
	}
	return (TRUE);
}
