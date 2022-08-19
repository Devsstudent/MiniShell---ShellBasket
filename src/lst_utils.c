/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 15:13:44 by odessein          #+#    #+#             */
/*   Updated: 2022/07/30 19:35:47 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_elem	*new_elem(char *content)
{
	t_elem	*new_elem;
	char	**elems_env;

	new_elem = malloc(sizeof(t_elem));
	if (!new_elem)
		return (NULL);
	elems_env = ft_split(content, '=');
	if (!elems_env)
	{
		free(new_elem);
		return (NULL);
	}
	new_elem->key = elems_env[0];
	new_elem->value = elems_env[1];
	free(elems_env[2]);
	free(elems_env);
	new_elem->next = NULL;
	new_elem->prev = NULL;
	return (new_elem);
}

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

	buff = dict->head;
	while (buff && ft_strncmp(buff->key, key, ft_strlen(key) + 1) != 0)
		buff = buff->next;
	if (buff)
	{
		free(buff->value);
		free(key);
		buff->value = value;
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
}
char	*dict_get_value(t_dict *dict, char *key)
{
	t_elem	*buff;

	buff = dict->head;
	while (buff && ft_strncmp(buff->key, key, ft_strlen(key) + 1) != 0)
		buff = buff->next;
	if (buff)
		return (buff->value);
	else
		return (NULL);
}
