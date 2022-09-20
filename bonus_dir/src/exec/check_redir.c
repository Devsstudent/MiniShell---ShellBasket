/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 17:22:33 by odessein          #+#    #+#             */
/*   Updated: 2022/09/19 20:49:56 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	check_red_in(t_block *files, t_info *exec)
{
	if (exec->open_fd != -1 && exec->open_fd != -2)
		close(exec->open_fd);
	exec->open_fd = -1;
	if (ft_strncmp(files->word, "", 2) == 0)
	{
		print_error(NULL, 1);
		return (FALSE);
	}
	else if (ft_strncmp(files->word, "\"\"", 3) == 0
		|| ft_strncmp(files->word, "\'\'", 3) == 0)
	{
		files->word[0] = ' ';
		ft_bzero(files->word + 1, ft_strlen(files->word) - 1);
	}
	exec->open_fd = open(files->word, O_RDONLY);
	if (exec->open_fd == -1)
	{
		perror_false(files->word);
		exec->open_fd = -2;
		return (FALSE);
	}
	return (TRUE);
}

t_bool	check_red_out(t_block *files, t_info *exec, t_bool red)
{
	if (exec->out_fd != -1 && exec->out_fd != -2)
		close(exec->out_fd);
	exec->out_fd = -1;
	if (ft_strncmp(files->word, "", 2) == 0)
	{
		print_error(NULL, 1);
		return (FALSE);
	}
	else if (ft_strncmp(files->word, "\"\"", 3) == 0
		|| ft_strncmp(files->word, "\'\'", 3) == 0)
	{
		write(2, " :", 2);
		ft_bzero(files->word, ft_strlen(files->word));
	}
	if (red)
		exec->out_fd = open(files->word, O_CREAT | O_RDWR | O_TRUNC, 0600);
	else
		exec->out_fd = open(files->word, O_CREAT | O_RDWR | O_APPEND, 0600);
	if (exec->out_fd == -1)
	{
		perror(files->word);
		exec->out_fd = -2;
		return (FALSE);
	}
	return (TRUE);
}

static t_bool	check_token_red_in(t_block *buff, t_info *exec)
{
	if (!check_file_permission(buff->next->word, exec, 1))
		return (FALSE);
	else if (!check_ambiguous(buff->next->word, exec, FALSE)
		&& !check_red_in(buff->next, exec))
		return (FALSE);
	return (TRUE);
}

static t_bool	check_token_red_out(t_block *buff, t_info *exec)
{
	if (!check_file_permission(buff->next->word, exec, 0))
		return (FALSE);
	else if (!check_ambiguous(buff->next->word, exec, TRUE))
	{

		if (buff->token == RED_OUT_TRUNC && !check_red_out(buff->next, exec, FALSE))
			return (FALSE);
		else if (buff->token == RED_OUT_APPEND && !check_red_out(buff->next, exec, TRUE))
			return (FALSE);
	}
	return (TRUE);
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
			if (!check_token_red_in(buff, exec))
				return ;
		if (buff->token == RED_OUT_TRUNC || buff->token == RED_OUT_APPEND)
			if (!check_token_red_out(buff, exec))
				return ;
		if (buff->token == HERE_DOC)
			exec->open_fd = exec->fd_arr[exec->turn];
		buff = buff->next;
	}
}
