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

void	exec_subshell(t_leaf *leaf, t_info *exec_in, t_dict *env, t_leaf *prev)
{
	int		pid;
	int		pipe_fd[2];
	t_info	*sub_exec_in;

	exec_in->par_lvl = leaf->parentheses;
	if (pipe(pipe_fd) == -1)
		return (perror("ERREUR CREATING PIPE"));
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	//exec_in->pipe_fd[0] = pipe_fd[0];
	//exec_in->pipe_fd[1] = pipe_fd[1];
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
		if (prev && prev->type == PIPE_L)
		{
	//		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
	//			perror("erreur dup pipe");
		}
		if (prev && (prev->type == RED_OUT_APPEND_L || prev->type == RED_OUT_TRUNC_L))
		{
	//		if (dup2(exec_in->out_fd, STDOUT_FILENO) == -1)
	//			perror("erreur dup pipe");
		}
		if (prev && (prev->type == RED_IN_L))
		{
	//		if (dup2(exec_in->open_fd, STDIN_FILENO) == -1)
	//			perror("erreur dup open pipe");
		}
		init_pid_lst(sub_exec_in);
		exec_tree(leaf, sub_exec_in, env, leaf);
		wait_sub_process(sub_exec_in);
	//	if (exec_in->pipe_fd[0] != -1)
	//		close(exec_in->pipe_fd[0]);
	//	if (exec_in->pipe_fd[1] != -1)
	//		close(exec_in->pipe_fd[1]);
		if (sub_exec_in->stdou != -1)
			close(sub_exec_in->stdou);
		if (sub_exec_in->stdi != -1)
			close(sub_exec_in->stdi);
		free(exec_in->pid_li->last);
		free_exit();
	}
}
