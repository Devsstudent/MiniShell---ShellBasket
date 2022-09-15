/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 18:50:24 by odessein          #+#    #+#             */
/*   Updated: 2022/09/15 18:59:00 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_pid	*new_pid(int pid)
{
	t_pid	*new_pid;

	new_pid = malloc(sizeof(t_pid));
	if (!new_pid)
		free_exit();
	new_pid->pid = pid;
	new_pid->next = NULL;
	return (new_pid);
}

void	pid_li_addback(t_pid_li *pid_lst, t_pid *new)
{
	t_pid	*buff;

	if (!pid_lst)
		return (free_exit());
	if (!new)
		return (free_exit());
	if (!(pid_lst->head))
	{
		pid_lst->head = new;
		pid_lst->last = new;
		(pid_lst->size) += 1;
		return ;
	}
	buff = pid_lst->head;
	while (buff->next != NULL)
		buff = buff->next;
	buff->next = new;
	pid_lst->last = new;
	(pid_lst->size) += 1;
}

void	pid_li_clear(t_pid_li *pid_lst)
{
	t_pid	*buff;

	if (!pid_lst)
		return ;
	while (pid_lst->head)
	{
		buff = pid_lst->head;
		pid_lst->head = pid_lst->head->next;
		free(buff);
	}
	pid_lst->size = 0;
}
