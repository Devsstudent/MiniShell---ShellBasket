/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_ambiguous.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 18:10:06 by odessein          #+#    #+#             */
/*   Updated: 2022/09/14 20:21:13 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ambiguous_case(t_bool type, t_info *exec_in)
{
	char	*msg;

	msg = "shellbasket: ambiguous redirect\n";
	if (type)
		exec_in->out_fd = -2;
	if (!type)
		exec_in->open_fd = -2;
	write(2, msg, ft_strlen(msg));
}
//Empty ou contient des whitespace
t_bool	check_ambiguous(char *word, t_info *exec_in, t_bool type)
{
	int		i;
	t_bool	quote;
	t_bool	d_quote;

	quote = FALSE;
	d_quote = FALSE;
	i = -1;
	if (!word[0])
	{
		ambiguous_case(type, exec_in);
		return (TRUE);
	}
	while (word[++i])
	{
		check_quote_redir(&d_quote, &quote, word[i]);
		if (i > 0 && !is_white_space(word[i - 1]) && is_white_space(word[i])
			&& !quote && !d_quote)
		{
			ambiguous_case(type, exec_in);
			return (TRUE);
		}
	}
	return (FALSE);
}
