/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 15:13:44 by odessein          #+#    #+#             */
/*   Updated: 2022/08/19 22:29:02 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	loop_copy_env(char **elems_env, char *buff, t_elem	*new_elem)
{
	int	i;

	i = 1;
	new_elem->key = elems_env[0];
	while (elems_env[i])
	{
		if (i != 1)
			buff = ft_strjoin(buff, "=");
		buff = ft_strjoin(buff, elems_env[i]);
		free(elems_env[i]);
		i++;
	}
	new_elem->value = buff;
	free(elems_env[i]);
	free(elems_env);
	new_elem->next = NULL;
	new_elem->prev = NULL;
}

t_elem	*new_elem(char *content)
{
	t_elem	*new_elem;
	char	**elems_env;
	char	*buff;

	new_elem = (t_elem *)malloc(sizeof(t_elem));
	if (!new_elem)
		free_exit();
	buff = (char *)malloc(sizeof(*buff));
	if (!buff)
		free_exit();
	buff[0] = 0;
	if (!new_elem)
		return (NULL);
	elems_env = ft_split(content, '=');
	if (!elems_env)
	{
		free(new_elem);
		return (NULL);
	}
	loop_copy_env(elems_env, buff, new_elem);
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

	int i;
	i = 0;
	buff = dict->head;
	while (buff != NULL)
	{
		i++;
		tmp = buff->next;
		free(buff->key);
		free(buff->value);
		free(buff);
		buff = tmp;
	}
	ft_printf(0, "%i\n", i);
	free(dict);
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
