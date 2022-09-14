/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:12:52 by odessein          #+#    #+#             */
/*   Updated: 2022/09/14 20:45:53 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	child_process(char *cmd_path, t_info *exec_in, int pipe_fd[2],
			t_dict *env)
{
	if (exec_in->pid[exec_in->turn] == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		if (!dup_in_pipe(exec_in, pipe_fd))
			return ;
		close_subprocess_fd(exec_in, pipe_fd);
		if (!execve_cmd(cmd_path, exec_in, env, pipe_fd))
		{
			perror(exec_in->argv[0]);
			free_exit();
		}
	}
}

static t_bool	check_new_shell(char *cmd_path)
{
	if (ft_strlen(cmd_path) >= 9)
		if (ft_strncmp(&cmd_path[ft_strlen(cmd_path) - 9],
				"minishell", 10) == 0)
			return (TRUE);
	return (FALSE);
}

void	forking(char *cmd_path, t_info *exec_in, t_dict *env, int pipe_fd[2])
{
	exec_in->pid[exec_in->turn] = fork();
	if (exec_in->pid[exec_in->turn] < 0)
		return (perror("shebasket"));
	if (check_new_shell(cmd_path))
		signal(SIGINT, SIG_IGN);
	else
		signal(SIGINT, sigint_handler_exec);
	if (exec_in->end)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	child_process(cmd_path, exec_in, pipe_fd, env);
	if (exec_in->tmp_fd != -1)
		close(exec_in->tmp_fd);
	exec_in->tmp_fd = pipe_fd[0];
}

static t_bool	child_process_alone_cmd(char *cmd_path, t_info *exec_in,
			t_dict *env)
{
	if (exec_in->pid[exec_in->turn] < 0)
	{
		perror("shellbasket");
		return (FALSE);
	}
	else if (exec_in->pid[exec_in->turn] == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		if (exec_in->open_fd != -1 && exec_in->open_fd != -2)
			close(exec_in->open_fd);
		if (exec_in->out_fd != -1 && exec_in->out_fd != -2)
			close(exec_in->out_fd);
		close_subprocess_fd(exec_in, NULL);
		if (exec_in->out_fd == -2)
			free_exit();
		if (!execve_cmd(cmd_path, exec_in, env, NULL))
		{
			perror(exec_in->argv[0]);
			free_exit();
		}
	}
	return (TRUE);
}

t_bool	exec_cmd_alone_not_builtin(t_info *exec_in, t_dict *env, char *cmd_path)
{
	if (!check_builtins(exec_in->argv))
	{
		if (check_new_shell(cmd_path))
			signal(SIGINT, SIG_IGN);
		else
			signal(SIGINT, sigint_handler_exec);
		exec_in->pid[exec_in->turn] = fork();
		if (!child_process_alone_cmd(cmd_path, exec_in, env))
			return (FALSE);
	}
	return (TRUE);
}
