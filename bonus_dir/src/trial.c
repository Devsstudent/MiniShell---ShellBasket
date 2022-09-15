/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trial.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 19:40:39 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/09/15 16:06:53 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	exec_cmd(t_info *exec_info, t_line *sub, t_dict *env)
{
	expand(sub, env);
	handle_wildcards(sub->head->word);
	exec_info->argv = get_cmd_arg(sub);
	exec(exec_info, sub, env);
}

void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env)
{
	if (!leaf)
		return ;
	if (leaf->type == CMD)
		exec_cmd(exec_in, leaf->content, env);
	if (leaf->type == OR_L)
	{
		exec_tree(leaf->left, exec_in, env);
		wait_processes(exec_in);
		if (g_exit_status != 0)
			exec_tree(leaf->right, exec_in, env);
	}
	if (leaf->type == AND_L)
	{
		exec_tree(leaf->left, exec_in, env);
		wait_processes(exec_in);
		if (g_exit_status == 0)
			exec_tree(leaf->right, exec_in, env);
	}
	if (leaf->type == PIPE_L)
	{
		//create a pipe and put it exec_in
		exec_tree(leaf->left, exec_in, env);
		exec_tree(leaf->right, exec_in, env);
	}
}

void	wait_processes(t_info *exec_in)
{
	while (pids in linked list)
	{
		waitpid(exec_info->pid);
	}
	remove pids;
}

//the last wait_processes has to be at the end of main like in mandatory part
