/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector_free.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 19:45:07 by odessein          #+#    #+#             */
/*   Updated: 2022/08/31 19:47:26 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	free_double_arr(t_gc *node)
{
	size_t	i;

	i = 0;
	while (((char **)(node->content))[i])
	{
		free(((char **)(node->content))[i]);
		i++;
	}
	free(node->content);
}

void	gc_free_node(t_gc *node)
{
	if (!node)
		return ;
	if (node->type == DOUBLE)
		free_double_arr(node);
	else if (node->type == ENV)
		dict_clear(node->content);
	else if (node->type == LINE)
		line_clear(node->content);
	else if (node->type == TREE && node->content != NULL)
	{
		if (((t_tree *)(node->content))->head)
			clean_tree(((t_tree *)(node->content))->head);
		free(node->content);
	}
	else if (node->type == SIMPLE)
		free(node->content);
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

t_bool	free_each_turn(t_gc **gc, t_info *exec_in)
{
	t_gc	*tmp;
	t_gc	*head;

	if (!gc)
		return (1);
	head = *gc;
	while (*gc)
	{
		tmp = (*gc)->next;
		if ((*gc)->type != ENV)
			gc_free_node(*gc);
		else
		{
			head = *gc;
			head->next = NULL;
		}
		*gc = tmp;
	}
	if (head)
		*gc = head;
	remove_tmp_file(exec_in->fd_arr_size, exec_in->fd_arr);
	free(exec_in);
	return (1);
}
