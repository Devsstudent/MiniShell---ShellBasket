/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_pid_func.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 18:50:24 by odessein          #+#    #+#             */
/*   Updated: 2022/09/08 19:06:52 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static t_bool	malloc_pid_arr_no_pipe(t_info *exec_info, t_tree *tree)
{
	t_leaf	*leaf;

	leaf = NULL;
	if (tree->head)
		leaf = tree->head;
	if (leaf && leaf->type == CMD)
	{
		exec_info->pid = (int *)malloc(sizeof(int));
		if (!exec_info->pid)
			free_exit();
		(exec_info->pid)[0] = -1;
		return (TRUE);
	}
	return (FALSE);
}

void	malloc_pid_arr(t_info *exec_info, t_tree *tree)
{
	t_leaf	*leaf;
	int		size;

	leaf = NULL;
	size = 0;
	if (tree->head)
		leaf = tree->head;
	if (!malloc_pid_arr_no_pipe(exec_info, tree))
	{
		while (leaf && leaf->type == PIPE_L)
		{
			size++;
			if (leaf->right)
				leaf = leaf->right;
			else
				break ;
		}
		exec_info->pid = (int *)malloc(sizeof(int) * size);
		if (!exec_info->pid)
			free_exit();
	}
	add_to_gc(SIMPLE, exec_info->pid, get_gc());
}
