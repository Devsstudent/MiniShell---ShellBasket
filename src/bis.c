/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bis.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 19:45:07 by odessein          #+#    #+#             */
/*   Updated: 2022/09/06 20:30:01 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	free_double_arr(t_gc *node)
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
	else if (node->type == SIMPLE || node->type == EXEC_INFO)
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
	free_each_turn(gc);
	while (*gc)
	{
		tmp = (*gc)->next;
		gc_free_node(*gc);
		*gc = tmp;
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
	tmp = *gc;
	while (tmp)
	{
		if (tmp->type == EXEC_INFO)
		{
			if (tmp->content && ((t_info *)(tmp->content))->fd_arr)
			{
				remove_tmp_file(((t_info *)(tmp->content))->fd_arr_size, ((t_info *)(tmp->content))->fd_arr);
			}
			gc_free_one_node(tmp, gc);
			if (((t_info *)(tmp->content))->fd_arr)
				((t_info *)(tmp->content))->fd_arr = NULL;
		}
		tmp = tmp->next;
	}
	return (1);
}

void	gc_free_one_node(t_gc *node, t_gc **gc)
{
	t_gc	*tmp;

	if (!gc || !(*gc))
		return ;
	tmp = *gc;
	if (node == *gc)
	{
		*gc = node->next;
		gc_free_node(node);
		return ;
	}
	while (tmp->next && tmp->next != node)
	{
		if (tmp->next == node)
		{
			tmp->next = tmp->next->next;
			gc_free_node(node);
		}
		tmp = tmp->next;
	}
}
