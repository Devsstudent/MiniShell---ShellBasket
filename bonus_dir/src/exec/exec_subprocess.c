/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subprocess.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:58:19 by odessein          #+#    #+#             */
/*   Updated: 2022/10/11 14:45:44 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static t_bool	dup_stdout(t_info *exec_in)
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
		if (dup2(STDOUT_FILENO, exec_in->stdou) == -1)
			return (perror_false("error in dup out"));
		close(exec_in->pipe_fd[1]);
		exec_in->end = FALSE;
	}
	else if (exec_in->pipe && (exec_in->left || exec_in->right))
	{
		if (dup2(exec_in->pipe_fd[1], STDOUT_FILENO) == -1)
			return (perror_false("write to actual pipe"));
	}
	return (TRUE);
}

t_bool	dup_cmd_alone(t_info *exec_in)
{
	if (exec_in->open_fd > -1)
		if (dup2(exec_in->open_fd, STDIN_FILENO) == -1)
			return (perror_false("shellbasket"));
	if (exec_in->out_fd > -1)
		if (dup2(exec_in->out_fd, STDOUT_FILENO) == -1)
			return (perror_false("shell90basket"));
	if (exec_in->out_fd == -2 || exec_in->open_fd == -2)
		return (FALSE);
	return (TRUE);
}

//Gerer les erreurs et afficher perror return un booleen
t_bool	dup_in_pipe(t_info *exec_in)
{
	if (!dup_stdout(exec_in))
		return (FALSE);
	if (exec_in->open_fd > -1)
	{
		if (dup2(exec_in->open_fd, STDIN_FILENO) == -1)
			return (perror_false("set"));
	}
	return (TRUE);
}

void	close_subprocess_fd(t_info *exec_in)
{
	if (exec_in->pipe_fd[0] > -1)
		close(exec_in->pipe_fd[0]);
	if (exec_in->pipe_fd[1] > -1)
		close(exec_in->pipe_fd[1]);
	if (exec_in->stdi > -1)
		close(exec_in->stdi);
	if (exec_in->stdou > -1)
		close(exec_in->stdou);
	if (exec_in->open_fd > -1)
		close(exec_in->open_fd);
	if (exec_in->out_fd > -1)
		close(exec_in->out_fd);
}
