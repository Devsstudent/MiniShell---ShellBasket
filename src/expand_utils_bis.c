/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_bis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 15:17:01 by odessein          #+#    #+#             */
/*   Updated: 2022/10/20 11:02:33 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	init_get_indexes_expandables(int *i, int *j, int *ind, t_bool *d_quote)
{
	*i = 0;
	*j = 0;
	if (!ind)
		free_exit();
	*d_quote = FALSE;
}

t_bool	char_is_num(char c, int *i)
{
	if ((c >= '0' && c <= '9') || c == '?')
	{
		(*i)++;
		return (TRUE);
	}
	return (FALSE);
}
