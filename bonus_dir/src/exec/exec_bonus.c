/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 21:08:50 by odessein          #+#    #+#             */
/*   Updated: 2022/10/18 11:53:11 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	exec_cmd(t_info *exec_info, t_leaf *leaf, t_dict *env)
{
	expand(leaf->content, env);
	wildcard(leaf->content);
	exec_info->argv = get_cmd_arg(leaf->content);
	exec(exec_info, leaf, env);
}

t_bool	parentheses_pipe(t_leaf *leaf, t_info *exec_in)
{
	int	pipe_fd[2];

	if (leaf->left->parentheses > leaf->parentheses)
	{
		if (pipe(pipe_fd) == -1)
			return (perror_false("Error creating pipe in pipe_exec"));
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			return (perror_false("Error dup in exec_bonus"));
		exec_in->pipe_fd[1] = pipe_fd[1];
		exec_in->pipe_fd[0] = pipe_fd[0];
	}
	exec_in->pipe = TRUE;
	return (TRUE);
}

t_bool	exec_left_right_pipe(t_leaf *leaf, t_info *exec_in, t_dict *env)
{
	exec_in->left = TRUE;
	exec_in->right = FALSE;
	exec_tree(leaf->left, exec_in, env, leaf);
	exec_in->right = TRUE;
	exec_in->left = FALSE;
	if (leaf->left->parentheses > leaf->parentheses)
		if (dup2(exec_in->stdou, STDOUT_FILENO) == -1)
			return (perror_false("back to stdout"));
	if ((leaf->head && leaf->right->type != PIPE_L && exec_in->right))
		exec_in->end = TRUE;
	exec_tree(leaf->right, exec_in, env, leaf);
	return (TRUE);
}
static t_bool	leaf_red_out(t_leaf *leaf, t_info *exec_in, t_dict *env)
{
	if (leaf->type == RED_OUT_TRUNC_L)
	{
		exec_in->out_fd = open(leaf->right->content->head->word,
				O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (exec_in->out_fd < 0)
			return (perror_false("redir out fail"));
		if (leaf->left->parentheses > leaf->parentheses)
			if (dup2(exec_in->out_fd, STDOUT_FILENO) == -1)
				return (perror_false("back to stdout"));
		exec_tree(leaf->left, exec_in, env, leaf);
	}
	if (leaf->type == RED_OUT_APPEND_L)
	{
		exec_in->out_fd = open(leaf->right->content->head->word,
				O_CREAT | O_RDWR | O_APPEND, 0644);
		if (exec_in->out_fd < 0)
			return (perror_false("redir out fail"));
		if (leaf->left->parentheses > leaf->parentheses)
			if (dup2(exec_in->out_fd, STDOUT_FILENO) == -1)
				return (perror_false("back to stdout"));
		exec_tree(leaf->left, exec_in, env, leaf);
	}
	return (TRUE);
}

void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env, t_leaf *prev)
{
	if (leaf->parentheses > exec_in->par_lvl)
	{
		exec_subshell(leaf, exec_in, env);
		exec_in->fork = FALSE;
		return ;
	}
	else if (leaf->type == PIPE_L)
		leaf_type_pipe(leaf, exec_in, env, prev);
	else if (leaf->type == OR_L)
		leaf_type_or(leaf, exec_in, env);
	else if (leaf->type == CMD)
		leaf_type_cmd(leaf, exec_in, env, prev);
	else if (leaf->type == AND_L)
		leaf_type_and(leaf, exec_in, env, prev);
	else if (!leaf_red_in(leaf, exec_in, env))
		return ;
	else if (!leaf_red_out(leaf, exec_in, env))
		return ;
}
