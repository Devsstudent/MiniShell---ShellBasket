/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 19:49:46 by odessein          #+#    #+#             */
/*   Updated: 2022/08/31 19:49:53 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//Faut bien stocker les malloc de word dans le gc pour l'attributon des 1er
//block, comme ca pas besoin de les free apres (Inshallah)
t_gc	*gc_new_node(t_type type, void *ptr)
{
	t_gc	*new;

	new = (t_gc *)malloc(sizeof(t_gc));
	if (!new)
		free_exit();
	new->next = NULL;
	new->type = type;
	new->content = ptr;
	return (new);
}

t_bool	add_to_gc(t_type type, void *ptr, t_gc **gc)
{
	t_gc	*new;
	t_gc	*buff;

	if (!(*gc))
	{
		new = gc_new_node(type, ptr);
		if (!new)
			return (FALSE);
		*gc = new;
		return (TRUE);
	}
	buff = *gc;
	new = gc_new_node(type, ptr);
	if (!new)
	{
		free_gc(gc);
		return (FALSE);
	}
	while (buff->next != NULL)
		buff = buff->next;
	buff->next = new;
	return (TRUE);
}
