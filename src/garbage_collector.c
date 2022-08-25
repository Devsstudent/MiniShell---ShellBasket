/* ************************************************************************** */
/*                                                                            */ /*                                                        :::      ::::::::   */
/*   gc_collector.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <mbelrhaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 16:29:54 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/03 15:38:32 by odessein         ###   ########.fr       */
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

void	gc_free_node(t_gc *node)
{
	size_t	i;

	i  = 0;
	if (!node)
		return ;
	if (node->type == SIMPLE)
		free(node->content);
	else if (node->type == DOUBLE)
	{
		while (((char **)(node->content))[i])
		{
			free(((char **)(node->content))[i]);
			i++;
		}
		free(node->content);
	}
	else if (node->type == DICT)
		dict_clear(node->content);
	else if (node->type == LINE)
		line_clear(node->content);
	else if (node->type == TREE)
	{
		if (((t_tree *)(node->content))->head)
			clean_tree(((t_tree *)(node->content))->head);
		free(node->content);
	}
	free(node);
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

void	free_gc(t_gc **gc)
{
	t_gc	*tmp;

	if (!gc)
		return ;
	while (*gc)
	{
		tmp = (*gc)->next;
		gc_free_node(*gc);
		*gc = tmp;
	}
	rl_clear_history();
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

t_bool	free_each_turn(t_gc **gc)
{
	t_gc	*tmp;

//Clean tous sauf l'environnemnt
	if (!gc)
		return ;
	while (*gc)
	{
		tmp = (*gc)->next;
		if (tmp->type != ENV)
			gc_free_node(*gc);
		*gc = tmp;
	}
}
