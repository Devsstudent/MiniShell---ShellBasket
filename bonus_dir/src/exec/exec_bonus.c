/* ************************************************************************** */
/*                                                                            */ 
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 15:14:15 by odessein          #+#    #+#             */
/*   Updated: 2022/10/05 18:08:40 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	exec_cmd(t_info *exec_info, t_leaf *leaf, t_dict *env, t_leaf *prev)
{
	expand(leaf->content, env);
	wildcard(leaf->content);
	exec_info->argv = get_cmd_arg(leaf->content);
	if (exec_info->open_fd != -2)
		exec(exec_info, leaf, env, prev);
}

static void	leaf_type_or(t_leaf *leaf, t_info *exec_in, t_dict *env)
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
		exec_tree(leaf->right, exec_in, env, leaf);
	else if (g_exit_status == 0 && leaf->right->parentheses != leaf->parentheses)
		return ;
}

static void	leaf_type_and(t_leaf *leaf, t_info *exec_in, t_dict *env, t_leaf *prev)
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

static void	leaf_type_cmd_pipe(t_leaf *leaf, t_info *exec_in, t_dict *env, t_leaf *prev)
{
	int	pipe_fd[2];

	if (leaf->type == PIPE_L)
	{
		if (leaf->left->parentheses > leaf->parentheses)
		{
			if (pipe(pipe_fd) == -1)
				return (perror("Error creating pipe in pipe_exec"));
			if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
				return (perror("Error dup in exec_bonus"));
			exec_in->pipe_fd[1] = pipe_fd[1];
			exec_in->pipe_fd[0] = pipe_fd[0];
		}
		exec_in->pipe = TRUE;
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
		exec_in->left = TRUE;
		exec_in->right = FALSE;
		exec_tree(leaf->left, exec_in, env, leaf);
		exec_in->right = TRUE;
		exec_in->left = FALSE;
		if (leaf->left->parentheses > leaf->parentheses)
			if (dup2(exec_in->stdou, STDOUT_FILENO) == -1)
				return (perror("back to stdout"));
		if ((leaf->head && leaf->right->type != PIPE_L && exec_in->right))
			exec_in->end = TRUE;
		exec_tree(leaf->right, exec_in, env, leaf);
	}
	else
	{
		if (!prev)
			exec_in->end = TRUE;
		exec_cmd(exec_in, leaf, env, prev);
	}
}

void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env, t_leaf *prev)
{
	if (leaf->parentheses > exec_in->par_lvl)
	{
		exec_subshell(leaf, exec_in, env);
		exec_in->fork = FALSE;
		return ;
	}
	else if (leaf->type == PIPE_L || leaf->type == CMD)
		leaf_type_cmd_pipe(leaf, exec_in, env, prev);
	else if (leaf->type == OR_L)
		leaf_type_or(leaf, exec_in, env);
	else if (leaf->type == AND_L)
		leaf_type_and(leaf, exec_in, env, prev);
	else if (leaf->type == RED_IN_L)
	{
		exec_in->open_fd = open(leaf->right->content->head->word, O_RDONLY);
		if (exec_in->open_fd < 0)
			return (perror("redir open fail"));
		if (leaf->left->parentheses > leaf->parentheses)
			if (dup2(exec_in->open_fd, STDOUT_FILENO) == -1)
				return (perror("back to stdout"));
		exec_tree(leaf->left, exec_in, env, leaf);
	}
	else if (leaf->type == RED_OUT_TRUNC_L)
	{
		exec_in->out_fd = open(leaf->right->content->head->word, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (exec_in->out_fd < 0)
			return (perror("redir out fail"));
		if (leaf->left->parentheses > leaf->parentheses)
			if (dup2(exec_in->out_fd, STDOUT_FILENO) == -1)
				return (perror("back to stdout"));
		exec_tree(leaf->left, exec_in, env, leaf);
	}
	else if (leaf->type == RED_OUT_APPEND_L)
	{
		exec_in->out_fd = open(leaf->right->content->head->word, O_CREAT | O_RDWR | O_APPEND, 0644);
		if (exec_in->out_fd < 0)
			return (perror("redir out fail"));
		if (leaf->left->parentheses > leaf->parentheses)
			if (dup2(exec_in->out_fd, STDOUT_FILENO) == -1)
				return (perror("back to stdout"));
		exec_tree(leaf->left, exec_in, env, leaf);
	}
}
