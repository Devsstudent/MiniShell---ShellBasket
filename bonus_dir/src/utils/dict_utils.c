/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dict_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:42:24 by odessein          #+#    #+#             */
/*   Updated: 2022/09/07 13:48:02 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	dict_addback(t_dict *dict, t_elem *new)
{
	t_elem	*buff;

	if (!dict)
		return ;
	if (!new)
		return ;
	if (!(dict->head))
	{
		dict->head = new;
		dict->size = 1;
		dict->last = dict->head;
		return ;
	}
	buff = dict->head;
	while (buff->next != NULL)
		buff = buff->next;
	(dict->size)++;
	new->prev = buff;
	buff->next = new;
	dict->last = new;
}

void	dict_delone(t_dict *dict, char *key)
{
	t_elem	*buff;

	buff = dict->head;
	while (buff && ft_strncmp(buff->key, key, ft_strlen(key) + 1) != 0)
		buff = buff->next;
	if (buff)
	{
		(dict->size)--;
		if (buff->prev)
			buff->prev->next = buff->next;
		else
			dict->head = buff->next;
		if (buff->next)
			buff->next->prev = buff->prev;
		free(buff->key);
		free(buff->value);
		free(buff);
	}
}

void	dict_modify(t_dict	*dict, char *key, char *value)
{
	t_elem	*buff;

	if (strncmp(key, "_", 2) == 0)
		return (free(value));
	buff = dict->head;
	while (dict && buff && ft_strncmp(buff->key, key, ft_strlen(key) + 1) != 0)
		buff = buff->next;
	if (buff)
	{
		free(buff->value);
		free(key);
		buff->value = value;
	}
	else
	{
		free(key);
		free(value);
	}
}

void	dict_clear(t_dict *dict)
{
	t_elem	*buff;
	t_elem	*tmp;

	buff = dict->head;
	while (buff != NULL)
	{
		tmp = buff->next;
		free(buff->key);
		free(buff->value);
		free(buff);
		buff = tmp;
	}
	free(dict);
}

char	*dict_get_value(t_dict *dict, char *key)
{
	t_elem	*buff;

	if (!dict)
		return (NULL);
	buff = dict->head;
	while (buff && ft_strncmp(buff->key, key, ft_strlen(key) + 1) != 0)
		buff = buff->next;
	if (buff)
		return (buff->value);
	else
		return (NULL);
}
