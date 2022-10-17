/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 18:44:13 by odessein          #+#    #+#             */
/*   Updated: 2022/10/17 22:02:01 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	init_wait_sub_process(t_info *exec_info)
{
	if (!exec_info->fork && dup2(exec_info->stdi, STDIN_FILENO) == -1)
		perror("sell");
	if (!exec_info->fork && exec_info->stdou > -1
		&& dup2(exec_info->stdou, STDOUT_FILENO) == -1)
		perror("basketttttt");
	if (exec_info->out_fd > -1)
		close(exec_info->out_fd);
	if (exec_info->open_fd > -1)
		close(exec_info->open_fd);
	if (exec_info->pipe_fd[0] > -1)
		close(exec_info->pipe_fd[0]);
	if (exec_info->pipe_fd[1] > -1)
		close(exec_info->pipe_fd[1]);
}

static void	loop_wait_process(t_pid *buff, t_info *exec_info, int w_status)
{
	while (buff)
	{
		w_status = -81;
		waitpid(buff->pid, &w_status, WCONTINUED);
		if (buff->pid == -1 || w_status == -81)
		{
			if (buff->pid != -1 && w_status == -81)
				g_exit_status = 130;
			buff = buff->next;
			continue ;
		}
		if (WIFEXITED(w_status) && !exec_info->cmd_not_found)
		{
			g_exit_status = WEXITSTATUS(w_status);
		}
		else if (WIFSIGNALED(w_status) && WTERMSIG(w_status) == 2)
			g_exit_status = 130;
		else if (WIFSIGNALED(w_status) && WTERMSIG(w_status) == 3)
		{
			write(2, "Quit (core dumped)\n", 19);
			g_exit_status = 131;
			break ;
		}
		buff = buff->next;
	}
}

void	wait_sub_process(t_info *exec_info)
{
	t_pid	*buff;
	int		w_status;

	w_status = -81;
	buff = exec_info->pid_li->head;
	init_wait_sub_process(exec_info);
	loop_wait_process(buff, exec_info, w_status);
	pid_li_clear(exec_info->pid_li);
	exec_info->argv = NULL;
	exec_info->cmd_not_found = FALSE;
	exec_info->pid_li->head = NULL;
	exec_info->pid_li->last = NULL;
}
