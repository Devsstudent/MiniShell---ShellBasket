/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 16:21:55 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/10/08 19:17:14 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	close_fds_in_subshell(t_info *exec_in)
{
	if (exec_in->stdi != -1)
		close(exec_in->stdi);
	if (exec_in->stdou != -1)
		close(exec_in->stdou);
	if (exec_in->pipe_fd[0] != -1)
		close(exec_in->pipe_fd[0]);
	if (exec_in->pipe_fd[1] != -1)
		close(exec_in->pipe_fd[1]);
}

void	exec_subshell(t_leaf *leaf, t_info *exec_in, t_dict *env)
{
	int		pid;
	t_info	*sub_exec_in;

	exec_in->par_lvl = leaf->parentheses;
	pid = fork();
	if (pid < 0)
		return (perror("fork error"));
	if (pid > 0)
		pid_li_addback(exec_in->pid_li, new_pid(pid));
	if (pid == 0)
	{
		leaf->head = TRUE;
		sub_exec_in = init_exec_info();
		sub_exec_in->fork = TRUE;
		sub_exec_in->par_lvl = exec_in->par_lvl;
		init_pid_lst(sub_exec_in);
		close_fds_in_subshell(exec_in);
		exec_tree(leaf, sub_exec_in, env, leaf);
		wait_sub_process(sub_exec_in);
		if (sub_exec_in->stdou != -1)
			close(sub_exec_in->stdou);
		if (sub_exec_in->stdi != -1)
			close(sub_exec_in->stdi);
		pid_li_clear(exec_in->pid_li);
		free_exit();
	}
}
