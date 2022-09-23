/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:12:52 by odessein          #+#    #+#             */
/*   Updated: 2022/09/21 16:17:50 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	child_process(char *cmd_path, t_info *exec_in, t_dict *env)
{
	if (exec_in->pid_li->last->pid == 0)
	{
		if (exec_in->open_fd == -2 || exec_in->out_fd == -2)
		{
			g_exit_status = 1;
			return (free_exit());
		}
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		if (!dup_in_pipe(exec_in))
			return ;
		close_subprocess_fd(exec_in);
		if (!execve_cmd(cmd_path, exec_in, env))
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

void	forking(char *cmd_path, t_info *exec_in, t_dict *env)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		return (perror("shebasket"));
	pid_li_addback(exec_in->pid_li, new_pid(pid));
	if (check_new_shell(cmd_path))
		signal(SIGINT, SIG_IGN);
	else
		signal(SIGINT, sigint_handler_exec);
	child_process(cmd_path, exec_in, env);
	if (exec_in->tmp_fd > -1)
		close(exec_in->tmp_fd);
	exec_in->tmp_fd = exec_in->pipe_fd[0];
	if (exec_in->pipe_fd[1] > -1)
		close(exec_in->pipe_fd[1]);
}
