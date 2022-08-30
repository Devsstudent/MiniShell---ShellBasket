/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subprocess.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:58:19 by odessein          #+#    #+#             */
/*   Updated: 2022/08/30 13:48:55 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	perror_false(char *str)
{
	perror(str);
	return (FALSE);
}

t_bool	dup_cmd_alone(t_info *exec_in, int pipe_fd[2])
{
	//Need to return FALSE
	if (exec_in->open_fd != -1 && exec_in->open_fd != -2)
	{
		write(2, ft_itoa(exec_in->open_fd), 1);
		if (dup2(exec_in->open_fd, STDIN_FILENO) == -1)
			return (perror_false("shellbasket"));
	}
	if (exec_in->out_fd != -1 && exec_in->out_fd != -2)
		if (dup2(exec_in->out_fd, STDOUT_FILENO) == -1)
			return (perror_false("shellbasket"));
	if (exec_in->out_fd == -2)
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			return (perror_false("close file crashed"));
	if (exec_in->open_fd == -2)
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			return (perror_false("open_file crashed"));
	return (TRUE);
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
	else  if (exec_in->pid[exec_in->turn] == 0)
	{
		if (exec_in->open_fd != -1)
			close(exec_in->open_fd);
		if (exec_in->out_fd != -1)
			close(exec_in->out_fd);
		if (!execve_test(cmd_path, exec_in->argv, env, 1))
		{
			perror(exec_in->argv[0]);
			g_exit_status = errno;
			exit(1);
		}
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

static t_bool	dup_stdout(t_info *exec_in, int pipe_fd[2])
{
	if (exec_in->out_fd != -1 && exec_in->out_fd != -2)
	{
		if (dup2(exec_in->out_fd, STDOUT_FILENO) == -1)
			return (perror_false("basket"));
	}
	else if (!exec_in->end)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			return (perror_false("shell"));
	}
	else
	{
		if (exec_in->out_fd != -2)
		{
			if (dup2(exec_in->stdou, STDOUT_FILENO) == -1)
				return (perror_false("sheet"));
			close(exec_in->stdou);
			exec_in->stdou = -1;
		}
	}
	return (TRUE);
}


t_bool	dup_in_pipe(t_info *exec_in, int pipe_fd[2])
{
	//Gerer les erreurs et afficher perror return un booleen
	if (!dup_stdout(exec_in, pipe_fd))
		return (FALSE);
	if (exec_in->open_fd != -1 && exec_in->open_fd != -2)
	{
		if (dup2(exec_in->open_fd, STDIN_FILENO) == -1)
			return (perror_false("set"));
	}
	else if (exec_in->tmp_fd != -1 && !exec_in->start)
		if (dup2(exec_in->tmp_fd, STDIN_FILENO) == -1)
			return (perror_false("lasy"));
	return (TRUE);
}

static void	close_subprocess_fd(t_info *exec_in, int pipe_fd[2])
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	if (exec_in->open_fd != -1 && exec_in->open_fd != -2)
		close(exec_in->open_fd);
	if (exec_in->out_fd != -1 && exec_in->out_fd != -2)
		close(exec_in->out_fd);
	if (exec_in->tmp_fd != -1)
		close(exec_in->tmp_fd);
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
	if (!dup_in_pipe(exec_in, pipe_fd))
		return ;
	if (exec_in->pid[exec_in->turn] == 0)
	{
		close_subprocess_fd(exec_in, pipe_fd);
		if (!execve_test(cmd_path, exec_in->argv, env, 1))
		{
			perror(exec_in->argv[0]);
			g_exit_status = errno;
			exit(1);
		}
	}
	if (exec_in->tmp_fd != -1)
		close(exec_in->tmp_fd);
	exec_in->tmp_fd = pipe_fd[0];
}
