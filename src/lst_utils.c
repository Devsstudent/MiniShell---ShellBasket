/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 15:13:44 by odessein          #+#    #+#             */
/*   Updated: 2022/08/30 15:13:47 by odessein         ###   ########.fr       */
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

t_elem	*create_elem(char *key, char *value)
{
	t_elem	*new_elem;

	new_elem = (t_elem *)malloc(sizeof(t_elem));
	if (!new_elem)
		free_exit();
	new_elem->next = NULL;
	new_elem->prev = NULL;
	new_elem->key = key;
	new_elem->value = value;
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

char	*dict_get_key(t_dict *dict, char *key)
{
	t_elem	*buff;

	buff = dict->head;
	while (buff && ft_strncmp(buff->key, key, ft_strlen(key) + 1) != 0)
		buff = buff->next;
	if (buff)
		return (buff->key);
	else
		return (NULL);
}

void	init_dict_to_double(int *i, t_elem **buff, char ***arr, t_dict *dict)
{
	*i = 0;
	*buff = dict->head;
	*arr = (char **) malloc(sizeof(*arr) * (dict->size + 1));
	if (!(*arr))
		free_exit();
	add_to_gc(DOUBLE, *arr, get_gc());
}

char	**dict_to_double_char_export(t_dict *dict)
{
	char	**arr;
	t_elem	*buff;
	int		i;

	init_dict_to_double(&i, &buff, &arr, dict);
	while (i < dict->size)
	{
		if (buff->value)
		{
			arr[i] = ft_strjoin(ft_strdup(buff->key), "=");
			if (!arr[i])
				free_exit();
			arr[i] = ft_strjoin(arr[i], ft_strdup(buff->value));
			if (!arr[i])
				free_exit();
		}
		else
			arr[i] = ft_strdup(buff->key);
		buff = buff->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

char	**dict_to_double_char_env(t_dict *dict)
{
	char	**arr;
	t_elem	*buff;
	int		i;

	init_dict_to_double(&i, &buff, &arr, dict);
	while (buff)
	{
		if (!buff->value)
		{
			buff = buff->next;
			continue ;
		}
		arr[i] = ft_strjoin(ft_strdup(buff->key), "=");
		if (!arr[i])
			free_exit();
		arr[i] = ft_strjoin(arr[i], buff->value);
		if (!arr[i])
			free_exit();
		buff = buff->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
