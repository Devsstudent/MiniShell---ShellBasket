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
	if (node->type == DOUBLE)
	{
		while (((char **)(node->content))[i])
		{
			free(((char **)(node->content))[i]);
			i++;
		}
		free(node->content);
	}
	else if (node->type == ENV)
		dict_clear(node->content);
	else if (node->type == LINE)
		line_clear(node->content);
	else if (node->type == TREE)
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

void	remove_tmp_file(int file_nb, int *fd_arr)
{
	int i;
	char	*num;
	char	*name;

	i = 0;
	while (i < file_nb)
	{
		num = ft_itoa(i);
		name = ft_strjoin(ft_strdup(".tmp_here_doc_"), num);
		unlink(name);
		free(name);
		free(num);
		i++;
	}
	free(fd_arr);
}

t_bool	free_each_turn(t_gc **gc, t_info *exec_in)
{
	t_gc	*tmp;
	t_gc	*head;

//Clean tous sauf l'environnemnt
	if (!gc)
		return 1;
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
	return 1;
}
