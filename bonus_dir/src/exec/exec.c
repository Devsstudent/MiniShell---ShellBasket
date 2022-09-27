/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:40:53 by odessein          #+#    #+#             */
/*   Updated: 2022/09/27 19:19:24 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	execute_cmd(t_info *exec_in, t_dict *env, char *cmd_path, int pipe_fd[2])
{
	if (exec_in->pipe || !check_builtins(exec_in->argv))
	{
		if (exec_in->final_out != -2 && exec_in->open_fd != -2)
			forking(cmd_path, exec_in, env, pipe_fd);
	}
	else
		execve_builtin_alone(cmd_path, env, exec_in);
	if (exec_in->open_fd > -1)
		close(exec_in->open_fd);
	if (exec_in->out_fd > -1)
		close(exec_in->out_fd);
	exec_in->turn++;
}

void	exec(t_info *exec_in, t_leaf *leaf, t_dict *env, t_leaf *prev)
{
	char	*cmd_path;
	int		i;

	i = -1;
	check_redirection(exec_in, leaf->content);
	while (exec_in->argv[++i])
		exec_in->argv[i] = handle_quote(exec_in->argv[i]);
	cmd_path = check_cmd(exec_in->argv, env);
	add_to_gc(SIMPLE, cmd_path, get_gc());
	if (command_not_found(exec_in, cmd_path, leaf->content))
	{
		if (exec_in->end)
			exec_in->cmd_not_found = TRUE;
		if (exec_in->open_fd != -1 && exec_in->open_fd != -2)
			close(exec_in->open_fd);
		if (exec_in->out_fd != -1 && exec_in->out_fd != -2)
			close(exec_in->out_fd);
		if (exec_in->final_out > -1)
			close(exec_in->final_out);
		return ;
	}
	if (prev)
		execute_cmd(exec_in, env, cmd_path, prev->pipe_fd);
	else
		execute_cmd(exec_in, env, cmd_path, NULL);
}
