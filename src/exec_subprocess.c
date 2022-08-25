/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subprocess.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:58:19 by odessein          #+#    #+#             */
/*   Updated: 2022/08/25 12:56:19 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	forking_cmd_alone(char *cmd_path, t_info *exec_in, t_dict *env)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (perror("pipe in forking cmd allone"));
	exec_in->pid[exec_in->turn] = fork();
	if (exec_in->pid[exec_in->turn] < 0)
		return (perror("shellbasket"));
	if (exec_in->open_fd != -1 && exec_in->open_fd != -2)
		if (dup2(exec_in->open_fd, STDIN_FILENO) == -1)
			return (perror("shellbasket"));
	if (exec_in->out_fd != -1 && exec_in->out_fd != -2)
		if (dup2(exec_in->out_fd, STDOUT_FILENO) == -1)
			return (perror("shellbasket"));
	if (exec_in->out_fd == -2)
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			return (perror("close file crashed"));
	if (exec_in->open_fd == -2)
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			return (perror("open_file crashed"));
	if (exec_in->pid[exec_in->turn] == 0)
	{
		if (exec_in->open_fd != -1)
			close(exec_in->open_fd);
		if (exec_in->out_fd != -1)
			close(exec_in->out_fd);
		if (!execve_test(cmd_path, exec_in->argv, env, 1))
		{
			perror(exec_in->argv[0]);
			exit(1);
		}
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	free(cmd_path);
}

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
	if (exec_in->out_fd != -1 && exec_in->out_fd != -2)
	{
		if (dup2(exec_in->out_fd, STDOUT_FILENO) == -1)
			return (perror("basket"));
	}
	else if (!exec_in->end)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			return (perror("shell"));
	}
	else
	{
		if (exec_in->out_fd != -2)
		{
			if (dup2(exec_in->stdou, STDOUT_FILENO) == -1)
				return (perror("sheet"));
			close(exec_in->stdou);
			exec_in->stdou = -1;
		}
	}
	if (exec_in->open_fd != -1 && exec_in->open_fd != -2)
	{
		if (dup2(exec_in->open_fd, STDIN_FILENO) == -1)
			return (perror("set"));
	}
	else if (exec_in->tmp_fd != -1 && !exec_in->start)
	{
		if (dup2(exec_in->tmp_fd, STDIN_FILENO) == -1)
			return (perror("lasy"));
	}
	/*
	else if (exec_in->open_fd == -2)
	{
		
	}*/
	if (exec_in->pid[exec_in->turn] == 0)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		if (exec_in->open_fd != -1 && exec_in->open_fd != -2)
			close(exec_in->open_fd);
		if (exec_in->out_fd != -1 && exec_in->out_fd != -2)
			close(exec_in->out_fd);
		if (exec_in->tmp_fd != -1)
			close(exec_in->tmp_fd);
		if (!execve_test(cmd_path, exec_in->argv, env, 1))
		{
			perror(exec_in->argv[0]);
			exit(1);
		}
	}
	if (exec_in->tmp_fd != -1)
		close(exec_in->tmp_fd);
	exec_in->tmp_fd = pipe_fd[0];
	//find a wait all pid after the exec so need to store each pid;
}
