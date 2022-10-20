/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:40:53 by odessein          #+#    #+#             */
/*   Updated: 2022/10/20 10:53:57 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	execute_cmd(t_info *exec_in, t_dict *env, char *cmd_path)
{
	(exec_in->turn)++;
	if (exec_in->pipe || !check_builtins(exec_in->argv))
		forking(cmd_path, exec_in, env);
	else
		execve_builtin_alone(cmd_path, env, exec_in);
	if (exec_in->open_fd > -1)
		close(exec_in->open_fd);
	if (exec_in->out_fd > -1)
		close(exec_in->out_fd);
	if (exec_in->end)
	{
		if (exec_in->pipe_fd[0] > -1)
			close(exec_in->pipe_fd[0]);
		if (exec_in->pipe_fd[1] > -1)
			close(exec_in->pipe_fd[1]);
	}
}

void	exec(t_info *exec_in, t_leaf *leaf, t_dict *env)
{
	char	*cmd_path;
	int		i;

	i = -1;
	while (exec_in->argv[++i])
		exec_in->argv[i] = handle_quote(exec_in->argv[i]);
	check_redirection(exec_in, leaf->content);
	cmd_path = check_cmd(exec_in->argv, env);
	add_to_gc(SIMPLE, cmd_path, get_gc());
	if (command_not_found(exec_in, cmd_path, leaf->content)
		&& !check_builtins(exec_in->argv))
	{
		if (exec_in->end)
			exec_in->cmd_not_found = TRUE;
		if (exec_in->open_fd != -1 && exec_in->open_fd != -2
			&& close(exec_in->open_fd))
			exec_in->open_fd = -1;
		if (exec_in->out_fd != -1 && exec_in->out_fd != -2
			&& close(exec_in->out_fd))
			exec_in->out_fd = -1;
	}
	execute_cmd(exec_in, env, cmd_path);
}
