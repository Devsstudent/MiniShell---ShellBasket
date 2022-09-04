/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:12:52 by odessein          #+#    #+#             */
/*   Updated: 2022/09/04 14:53:10 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	forking(char *cmd_path, t_info *exec_in, t_dict *env, int pipe_fd[2])
{
	exec_in->pid[exec_in->turn] = fork();
	if (exec_in->pid[exec_in->turn] < 0)
		return (perror("shebasket"));
	if (exec_in->end)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (!dup_in_pipe(exec_in, pipe_fd))
		return ;
	if (exec_in->pid[exec_in->turn] == 0)
	{
		close_subprocess_fd(exec_in, pipe_fd);
		if (!execve_test(cmd_path, exec_in->argv, env, exec_in))
		{
			perror(exec_in->argv[0]);
			free_exit();
		}
	}
	if (exec_in->tmp_fd != -1)
		close(exec_in->tmp_fd);
	exec_in->tmp_fd = pipe_fd[0];
}

void	forking_cmd_alone(char *cmd_path, t_info *exec_in, t_dict *env)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (perror("pipe in forking cmd allone"));
	if (!dup_cmd_alone(exec_in, pipe_fd))
		return ;
	exec_in->pid[exec_in->turn] = fork();
	if (exec_in->pid[exec_in->turn] < 0)
		return (perror("shellbasket"));
	else if (exec_in->pid[exec_in->turn] == 0)
	{
		if (exec_in->open_fd != -1)
			close(exec_in->open_fd);
		if (exec_in->out_fd != -1)
			close(exec_in->out_fd);
		if (!execve_test(cmd_path, exec_in->argv, env, exec_in))
		{
			perror(exec_in->argv[0]);
			free_exit();
		}
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}
