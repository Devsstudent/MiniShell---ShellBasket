/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:12:52 by odessein          #+#    #+#             */
/*   Updated: 2022/09/08 20:02:05 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	forking(char *cmd_path, t_info *exec_in, t_dict *env, int pipe_fd[2])
{
	exec_in->pid[exec_in->turn] = fork();
	if (exec_in->pid[exec_in->turn] < 0)
		return (perror("shebasket"));
	if (ft_strncmp(cmd_path, "./minishell", ft_strlen("./minishell") + 1) == 0)
		signal(SIGINT, SIG_IGN);
	else
		signal(SIGINT, sigint_handler_exec);
	if (exec_in->end)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (!dup_in_pipe(exec_in, pipe_fd))
		return ;
	if (exec_in->pid[exec_in->turn] == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		close_subprocess_fd(exec_in, pipe_fd);
		if (!execve_cmd(cmd_path, exec_in->argv, env))
		{
			perror(exec_in->argv[0]);
			free_exit();
		}
	}
	if (exec_in->tmp_fd != -1)
		close(exec_in->tmp_fd);
	exec_in->tmp_fd = pipe_fd[0];
}

t_bool	exec_cmd_alone_not_builtin(t_info *exec_in, t_dict *env, char *cmd_path)
{
	if (!check_builtins(exec_in->argv))
	{
		if (ft_strncmp(cmd_path, "./minishell", ft_strlen("./minishell") + 1) == 0)
			signal(SIGINT, SIG_IGN);
		else
			signal(SIGINT, sigint_handler_exec);
		exec_in->pid[exec_in->turn] = fork();
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
			if (!execve_cmd(cmd_path, exec_in->argv, env))
			{
				perror(exec_in->argv[0]);
				free_exit();
			}
		}
	}
	return (TRUE);
}
