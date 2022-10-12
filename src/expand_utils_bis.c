/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_bis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 15:17:01 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 15:19:34 by odessein         ###   ########.fr       */
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
