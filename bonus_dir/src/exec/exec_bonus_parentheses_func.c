/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus_parentheses_func.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 10:55:20 by odessein          #+#    #+#             */
/*   Updated: 2022/10/20 11:54:53 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	parentheses_pipe(t_leaf *leaf, t_info *exec_in)
{
	int	pipe_fd[2];

	(void) leaf;
	if (pipe(pipe_fd) == -1)
		return (perror_false("Error creating pipe in pipe_exec"));
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		return (perror_false("Error dup in exec_bonus"));
	exec_in->pipe_fd[1] = pipe_fd[1];
	exec_in->pipe_fd[0] = pipe_fd[0];
	exec_in->pipe = TRUE;
	return (TRUE);
}

t_bool	fill_sub_std(t_info *exec_in)
{
	char	c;

	exec_in->sub_std = open(".tmp_sub_std", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (exec_in->sub_std < 0)
		return (perror_false("open .tmp_sub_std failed"));
	while (read(exec_in->pipe_fd[0], &c, 1) > 0)
	{
		write(2, "X", 1);
	}
	return (TRUE);
}
