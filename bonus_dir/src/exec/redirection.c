/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:59:38 by odessein          #+#    #+#             */
/*   Updated: 2022/09/19 19:27:29 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
/*
	else if (type)
	{
		ft_putstr_fd(buff->next->word, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		//if (type)
			exec->open_fd = -2;
		//else
		//	exec->out_fd = -2;
		return (FALSE);
	}*/

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

t_bool	check_file_permission(char *file, t_info *exec, int type)
{
	struct stat	statbuff;

	if (access(file, R_OK) == 0)
	{
		if (stat(file, &statbuff) == -1)
			perror("stat broslinecheck");
		if ((statbuff.st_mode & S_IFMT) == S_IFDIR)
		{
			ft_putstr_fd(file, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			if (type)
				exec->open_fd = -2;
			else
				exec->out_fd = -2;
			return (FALSE);
		}
	}
	return (TRUE);
}
