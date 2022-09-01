/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:59:38 by odessein          #+#    #+#             */
/*   Updated: 2022/08/31 17:15:16 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	browse_line_check_red_in(t_leaf *leaf, t_dict *env)
{
	if (!leaf)
		return ;
	if (leaf->type == PIPE_L)
	{
		check_red_in_sub(leaf->left->content, env);
		if (leaf->right)
			browse_line_check_red_in(leaf->right, env);
	}
	else if (leaf->type == CMD)
		check_red_in_sub(leaf->content, env);
}

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

void	check_file_permission(t_block *buff)
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
			buff->next->crash = TRUE;
		}
	}
	else
	{
		ft_putstr_fd(buff->next->word, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		buff->next->crash = TRUE;
	}
}
