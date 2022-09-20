/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subprocess.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:58:19 by odessein          #+#    #+#             */
/*   Updated: 2022/09/19 21:23:33 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static t_bool	dup_stdout(t_info *exec_in)
{
	if (exec_in->out_fd != -1 && exec_in->out_fd != -2)
	{
		if (dup2(exec_in->out_fd, STDOUT_FILENO) == -1)
		{
			ft_putnbr_fd(exec_in->out_fd, 2);
			write(2, "\n", 1);
			return (perror_false("basket"));
		}
	}
	else if (!exec_in->end && exec_in->pipe_fd[1] != -1)
	{
		if (dup2(exec_in->pipe_fd[1], STDOUT_FILENO) == -1)
			return (perror_false("shell"));
	}
	else
	{
		if (exec_in->out_fd == -1)
		{
			if (dup2(exec_in->stdou, STDOUT_FILENO) == -1)
				return (perror_false("sheet"));
			if (exec_in->stdou != -1)
				close(exec_in->stdou);
			exec_in->stdou = -1;
		}
	}
	return (TRUE);
}

t_bool	dup_cmd_alone(t_info *exec_in)
{
	if (exec_in->open_fd != -1 && exec_in->open_fd != -2)
	{
		if (dup2(exec_in->open_fd, STDIN_FILENO) == -1)
			return (perror_false("shellbasket"));
	}
	if (exec_in->out_fd != -1 && exec_in->out_fd != -2)
		if (dup2(exec_in->out_fd, STDOUT_FILENO) == -1)
			return (perror_false("shellbasket"));
	return (TRUE);
}

//Gerer les erreurs et afficher perror return un booleen
t_bool	dup_in_pipe(t_info *exec_in)
{
	if (!dup_stdout(exec_in))
		return (FALSE);
	if (exec_in->open_fd != -1 && exec_in->open_fd != -2)
	{
		if (dup2(exec_in->open_fd, STDIN_FILENO) == -1)
			return (perror_false("set"));
	}
	else if (exec_in->tmp_fd != -1 /*&& !exec_in->start*/)
	{
		if (dup2(exec_in->tmp_fd, STDIN_FILENO) == -1)
			return (perror_false("lasy"));
	}
	return (TRUE);
}

void	close_subprocess_fd(t_info *exec_in)
{
	if (exec_in->pipe_fd[0] != -1)
		close(exec_in->pipe_fd[0]);
	if (exec_in->pipe_fd[1] != -1)
		close(exec_in->pipe_fd[1]);
	if (exec_in->stdi != -1)
		close(exec_in->stdi);
	if (exec_in->stdou != -1)
		close(exec_in->stdou);
	if (exec_in->open_fd != -1 && exec_in->open_fd != -2)
		close(exec_in->open_fd);
	if (exec_in->out_fd != -1 && exec_in->out_fd != -2)
		close(exec_in->out_fd);
	if (exec_in->tmp_fd != -1)
		close(exec_in->tmp_fd);
}
