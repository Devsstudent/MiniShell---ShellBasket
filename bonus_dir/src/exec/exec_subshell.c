/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 16:21:55 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/09/30 16:01:56 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	exec_subshell(t_leaf *leaf, t_info *exec_in, t_dict *env)
{
	int		pid;
	t_info	*sub_exec_in;

	exec_in->par_lvl = leaf->parentheses;
	pid = fork();
	if (pid < 0)
		return (perror("fork error"));
	pid_li_addback(exec_in->pid_li, new_pid(pid));
	if (pid == 0)
	{
		leaf->head = TRUE;
		sub_exec_in = init_exec_info();
		sub_exec_in->fork = TRUE;
		sub_exec_in->par_lvl = exec_in->par_lvl;
		init_pid_lst(sub_exec_in);
		exec_tree(leaf, sub_exec_in, env, leaf);
		wait_sub_process(sub_exec_in);
		if (sub_exec_in->stdou != -1)
			close(sub_exec_in->stdou);
		if (sub_exec_in->stdi != -1)
			close(sub_exec_in->stdi);
		free(exec_in->pid_li->last);
		free_exit();
	}
}
