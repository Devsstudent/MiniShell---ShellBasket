/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector_free.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 19:45:07 by odessein          #+#    #+#             */
/*   Updated: 2022/10/05 16:53:29 by mbelrhaz         ###   ########.fr       */
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

void	free_exec_info(t_gc **gc)
{
	t_gc	*tmp;

	tmp = *gc;
	while (tmp)
	{
		if (tmp->type == EXEC_INFO)
		{
			if (tmp->content && ((t_info *)(tmp->content))->fd_arr)
				remove_tmp_file(((t_info *)(tmp->content))->fd_arr_size,
					((t_info *)(tmp->content))->fd_arr);
			gc_free_one_node(tmp, gc);
			if (((t_info *)(tmp->content))->fd_arr)
				((t_info *)(tmp->content))->fd_arr = NULL;
		}
		tmp = tmp->next;
	}
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
