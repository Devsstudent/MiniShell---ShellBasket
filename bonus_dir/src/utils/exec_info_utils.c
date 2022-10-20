/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_info_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 20:13:45 by odessein          #+#    #+#             */
/*   Updated: 2022/10/17 21:34:39 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	init_content_exec_info(t_info *exec_info)
{
	exec_info->pid_li = NULL;
	exec_info->pipe = FALSE;
	exec_info->argv = NULL;
	exec_info->fd_arr = NULL;
	exec_info->prev_pipe = FALSE;
	exec_info->fd_arr_size = 0;
	exec_info->pipe_fd[0] = -1;
	exec_info->pipe_fd[1] = -1;
	exec_info->sub_std = -1;
	exec_info->open_fd = -1;
	exec_info->par_lvl = 0;
	exec_info->tmp_fd = -1;
	exec_info->turn = 0;
	exec_info->right = FALSE;
	exec_info->left = FALSE;
	exec_info->out_fd = -1;
	exec_info->end = FALSE;
	exec_info->fork = FALSE;
	exec_info->stdi = dup(STDIN_FILENO);
	exec_info->cmd_not_found = FALSE;
	exec_info->stdou = dup(STDOUT_FILENO);
	exec_info->turn = 0;
}

t_info	*init_exec_info(void)
{
	t_info	*exec_info;

	exec_info = (t_info *) malloc(sizeof(t_info));
	if (!exec_info)
		free_exit();
	add_to_gc(EXEC_INFO, exec_info, get_gc());
	init_content_exec_info(exec_info);
	return (exec_info);
}
