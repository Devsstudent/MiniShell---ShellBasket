/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subprocess.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:58:19 by odessein          #+#    #+#             */
/*   Updated: 2022/09/27 19:44:21 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static t_bool	dup_stdout(t_info *exec_in, int pipe_fd[2])
{
	/*
	if (exec_in->out_fd == -1 && exec_in->final_out != -1)
	{
		if (dup2(exec_in->final_out, STDOUT_FILENO) == -1)
			return (perror_false("coco"));
		return (TRUE);
	}*/
	if (exec_in->out_fd > -1)
	{
		if (dup2(exec_in->out_fd, STDOUT_FILENO) == -1)
			return (perror_false("basket"));
	}
	else if (exec_in->end)
	{
		printf("END?\n");
		if (dup2(STDOUT_FILENO, exec_in->stdou) == -1)
			return (perror_false("error in dup out"));
		exec_in->end = FALSE;
	}
	else if (exec_in->left)
	{
		if (dup2(exec_in->pipe_fd_actual[1], STDOUT_FILENO) == -1)
			return (perror_false("perror YAY"));
	}
	else if (exec_in->prev_pipe && exec_in->right)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			return (perror_false("right to write previous pipe"));
	}
	else if (exec_in->right)
	{
		if (dup2(exec_in->pipe_fd_actual[1], STDOUT_FILENO) == -1)
			return (perror_false("right to write to actual pipe"));
	}
	else if (exec_in->left)
	{
		if (dup2(exec_in->pipe_fd_actual[1], STDOUT_FILENO) == -1)
			return (perror_false("left to write to actual pipe"));
	}
	return (TRUE);
}

t_bool	dup_cmd_alone(t_info *exec_in)
{
	if (exec_in->out_fd == -1 && exec_in->final_out > -1)
	{
		if (dup2(exec_in->final_out, STDOUT_FILENO) == -1)
			return (perror_false("coco"));
		return (TRUE);
	}
	if (exec_in->open_fd > -1)
		if (dup2(exec_in->open_fd, STDIN_FILENO) == -1)
			return (perror_false("shellbasket"));
	if (exec_in->out_fd > -1)
		if (dup2(exec_in->out_fd, STDOUT_FILENO) == -1)
			return (perror_false("shellbasket"));
	return (TRUE);
}

//Gerer les erreurs et afficher perror return un booleen
t_bool	dup_in_pipe(t_info *exec_in, int pipe_fd[2])
{
	if (!dup_stdout(exec_in, pipe_fd))
		return (FALSE);
	if (exec_in->open_fd > -1)
	{
		if (dup2(exec_in->open_fd, STDIN_FILENO) == -1)
			return (perror_false("set"));
	}
	else if (exec_in->prev_pipe && exec_in->left)
	{
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			return (perror_false("lasy"));
	}
	else if (exec_in->prev_pipe && exec_in->right)
	{
		if (dup2(exec_in->pipe_fd_actual[0], STDIN_FILENO) == -1)
			return (perror_false("read on acutal pipe"));
	}
	else if (exec_in->right)
	{
		if (dup2(exec_in->pipe_fd_actual[0], STDIN_FILENO) == -1)
			return (perror_false("mamaaaa right"));
	}
	return (TRUE);
}

void	close_subprocess_fd(t_info *exec_in, int pipe_fd[2])
{
	if (pipe_fd)
	{
		if (pipe_fd[0] > -1)
			close(pipe_fd[0]);
		if (pipe_fd[1] > -1)
			close(pipe_fd[1]);
	}
	if (exec_in->pipe_fd_actual[0] > -1)
		close(exec_in->pipe_fd_actual[0]);
	if (exec_in->pipe_fd_actual[1] > -1)
		close(exec_in->pipe_fd_actual[1]);
	if (exec_in->stdi > -1)
		close(exec_in->stdi);
	if (exec_in->stdou > -1)
		close(exec_in->stdou);
	if (exec_in->open_fd > -1)
		close(exec_in->open_fd);
	if (exec_in->final_out > -1)
		close(exec_in->final_out);
	if (exec_in->out_fd > -1)
		close(exec_in->out_fd);
}
