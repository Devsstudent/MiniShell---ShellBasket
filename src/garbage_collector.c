/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 19:49:46 by odessein          #+#    #+#             */
/*   Updated: 2022/09/08 18:36:48 by odessein         ###   ########.fr       */
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

void	gc_free_node_addr(void *ptr, t_gc **gc)
{
	t_gc	*lst;
	t_gc	*prev;

	lst = *gc;
	if (lst->content == ptr)
	{
		*gc = lst->next;
		gc_free_node(lst);
		return ;
	}
	prev = lst;
	while (lst)
	{
		if (lst->content == ptr)
		{
			prev->next = lst->next;
			gc_free_node(lst);
			return ;
		}
		prev = lst;
		lst = lst->next;
	}
}

t_bool	free_each_turn(t_gc **gc)
{
	t_gc	*tmp;

	if (!gc)
		return (1);
	tmp = *gc;
	while (tmp)
	{
		if (tmp->type != ENV && tmp->type != EXEC_INFO)
			gc_free_one_node(tmp, gc);
		tmp = tmp->next;
	}
	free_exec_info(gc);
	return (1);
}
