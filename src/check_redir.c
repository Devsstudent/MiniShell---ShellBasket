/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 17:22:33 by odessein          #+#    #+#             */
/*   Updated: 2022/09/01 17:26:04 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	check_red_in(t_block *files, t_info *exec)
{
	int	f_open;

	if (exec->open_fd != -1)
		close(exec->open_fd);
	exec->open_fd = -1;
	if (ft_strncmp(files->word, "", 2) == 0)
		print_error(NULL, 1);
	else if (ft_strncmp(files->word, "\"\"", 3) == 0
		|| ft_strncmp(files->word, "\'\'", 3) == 0)
	{
		files->word[0] = ' ';
		ft_bzero(files->word + 1, ft_strlen(files->word) - 1);
	}
	f_open = open(files->word, O_RDONLY);
	if (f_open == -1)
	{
		exec->open_fd = -2;
		perror(files->word);
	}
	else
		exec->open_fd = f_open;
}

void	check_red_out(t_block *files, t_info *exec, t_block *red)
{
	int	f_open_out;

	if (exec->out_fd != -1)
		close(exec->out_fd);
	exec->out_fd = -1;
	if (ft_strncmp(files->word, "", 2) == 0)
		return (print_error(NULL, 1));
	else if (ft_strncmp(files->word, "\"\"", 3) == 0
		|| ft_strncmp(files->word, "\'\'", 3) == 0)
	{
		write(2, " :", 2);
		ft_bzero(files->word, ft_strlen(files->word));
	}
	if (red->token == RED_OUT_TRUNC)
		f_open_out = open(files->word, O_CREAT | O_RDWR | O_TRUNC, 0600);
	else
		f_open_out = open(files->word, O_CREAT | O_RDWR | O_APPEND, 0600);
	if (f_open_out == -1)
	{
		exec->out_fd = -2;
		perror(files->word);
	}
	else
		exec->out_fd = f_open_out;
}

void	check_redirection(t_info *exec, t_line *sub)
{
	t_block	*buff;

	if (sub->head)
		buff = sub->head;
	else
		return ;
	exec->open_fd = -1;
	exec->out_fd = -1;
	while (buff)
	{
		if (buff->token == RED_IN)
		{
			if (!check_ambiguous(buff->next->word, exec, FALSE))
				check_red_in(buff->next, exec);
			else
				return ;
		}
		if (buff->token == RED_OUT_TRUNC || buff->token == RED_OUT_APPEND)
			if (!check_ambiguous(buff->next->word, exec, TRUE))
				check_red_out(buff->next, exec, buff);
		if (buff->token == HERE_DOC)
			exec->open_fd = exec->fd_arr[exec->turn];
		buff = buff->next;
	}
}
