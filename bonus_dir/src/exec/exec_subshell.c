/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 16:21:55 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/09/27 17:31:13 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_subshell(t_leaf *leaf, t_info *exec_in, t_dict *env, int *PAR)
{
	int		pid;
	t_info	*sub_exec_in;

	(void)exec_in;
	*PAR = leaf->parentheses;
	pid = fork();
	if (pid < 0)
		return (perror("fork error"));
	pid_li_addback(exec_in->pid_li, new_pid(pid));
	if (pid == 0)
	{
		sub_exec_in = init_exec_info();
		init_pid_lst(sub_exec_in);
		exec_tree(leaf, sub_exec_in, env);
		wait_sub_process(sub_exec_in);
		exit(g_exit_status);
	}
	//waitpid or not ?
}
