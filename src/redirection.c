/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:59:38 by odessein          #+#    #+#             */
/*   Updated: 2022/09/01 16:11:48 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	check_quote_redir(t_bool *d_quote, t_bool *quote, char word)
{
	if (word == '\"' && !(*d_quote))
		*d_quote = TRUE;
	else if (word == '\"' && *d_quote)
		*d_quote = FALSE;
	if (word == '\'' && !(*quote))
		*quote = TRUE;
	else if (word == '\'' && *quote)
		*quote = FALSE;
}

t_bool	check_file_permission(t_block *buff, t_info *exec, int type)
{
	struct stat	statbuff;

	if (access(buff->next->word, R_OK) == 0)
	{
		if (stat(buff->next->word, &statbuff) == -1)
			perror("stat broslinecheck");
		if ((statbuff.st_mode & S_IFMT) != S_IFREG)
		{
			ft_putstr_fd(buff->next->word, 2);
			ft_putstr_fd(": Not a file\n", 2);
			if (type)
				exec->open_fd = -2;
			else
				exec->out_fd = -2;
			return (FALSE);
		}
	}
/*	else
	{
		ft_putstr_fd(buff->next->word, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		if (type)
			exec->open_fd = -2;
		else
			exec->out_fd = -2;
		return (FALSE);
	}*/
	return (TRUE);
}
