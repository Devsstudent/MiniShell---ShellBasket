/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dict_to_double.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:43:10 by odessein          #+#    #+#             */
/*   Updated: 2022/09/04 19:29:38 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	init_dict_to_double(int *i, t_elem **buff, char ***arr,
			t_dict *dict)
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
			arr[i] = ft_strjoin(arr[i], buff->value);
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
