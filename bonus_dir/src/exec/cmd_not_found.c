/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_not_found.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 18:28:43 by odessein          #+#    #+#             */
/*   Updated: 2022/10/20 13:18:04 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	check_cmd_in_sub(t_line *sub)
{
	t_block	*buff;

	if (!sub)
		return (FALSE);
	buff = sub->head;
	while (buff)
	{
		if (buff->token == CMD_ARG)
			return (TRUE);
		buff = buff->next;
	}
	return (FALSE);
}

t_bool	command_not_found(t_info *exec_in, char *cmd_path,
		t_line *sub)
{
	if (errno == 13 || errno == 2 || g_exit_status == 126)
		return (TRUE);
	if ((!cmd_path || exec_in->open_fd == -2))
	{
		if (exec_in->pipe_fd[1] > -1)
			close(exec_in->pipe_fd[1]);
		if (exec_in->end && exec_in->pipe_fd[0] > -1)
			close(exec_in->pipe_fd[0]);
		if (errno != 13 && exec_in->open_fd != -2 && !cmd_path
			&& check_cmd_in_sub(sub))
			print_error(exec_in->argv[0], 2);
		return (TRUE);
	}
	return (FALSE);
}
