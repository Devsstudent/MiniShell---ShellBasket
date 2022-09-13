/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 18:44:13 by odessein          #+#    #+#             */
/*   Updated: 2022/09/12 16:07:40 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	init_wait_sub_process(t_info *exec_info, int *i)
{
	*i = -1;
	if (exec_info->tmp_fd != -1)
		close(exec_info->tmp_fd);
	if (dup2(exec_info->stdi, STDIN_FILENO) == -1)
		perror("sell");
	close(exec_info->stdi);
	if (exec_info->stdou != -1 && dup2(exec_info->stdou, STDOUT_FILENO) == -1)
		perror("basket");
	if (exec_info->stdou != -1)
		close(exec_info->stdou);
	if (exec_info->out_fd != -1 && exec_info->out_fd != -2)
		close(exec_info->out_fd);
	if (exec_info->open_fd != -1 && exec_info->open_fd != -2)
		close(exec_info->open_fd);
}

void	wait_sub_process(t_info *exec_info)
{
	int	i;
	int	w_status;

	init_wait_sub_process(exec_info, &i);
	while (++i < exec_info->turn)
	{
		w_status = -81;
		waitpid(exec_info->pid[i], &w_status, WCONTINUED);
		if (exec_info->pid[i] == -1 || w_status == -81)
		{
			if (exec_info->pid[i++] != -1 && w_status == -81)
				g_exit_status = 130;
			continue ;
		}
		if (WIFEXITED(w_status) && !exec_info->cmd_not_found)
			g_exit_status = WEXITSTATUS(w_status);
		else if (WIFSIGNALED(w_status) && WTERMSIG(w_status) == 2)
			g_exit_status = 130;
		else if (WIFSIGNALED(w_status) && WTERMSIG(w_status) == 3)
		{
			write(2, "Quit (core dumped)\n", 19);
			g_exit_status = 131;
			return ;
		}
	}
}
