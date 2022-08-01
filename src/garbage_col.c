/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_col.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42->fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 14:35:33 by odessein          #+#    #+#             */
/*   Updated: 2022/07/29 15:17:21 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	add_to_gc(t_type type, void *ptr, t_gc **gc)
{
	t_gc	*new;
	t_gc	*buff;

	if (!(*gc))
	{
		new = (t_gc *) malloc(sizeof(t_gc));
		if (!new)
			return (FALSE);
		new->next = NULL;
		new->content = ptr;
		new->type = type;
		*gc = new;
		return (TRUE);
	}
	buff = *gc;
	new = (t_gc *) malloc(sizeof(t_gc));
	if (!new)
	{
//		free_gc(gc);
		return (FALSE);
	}
	new->next = NULL;
	new->content = ptr;
	new->type = type;
	while (buff->next != NULL)
		buff = buff->next;
	buff->next = new;
	return (TRUE);
}
