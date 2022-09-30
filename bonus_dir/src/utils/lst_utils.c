/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 15:13:44 by odessein          #+#    #+#             */
/*   Updated: 2022/09/30 18:41:10 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	loop_copy_env(char **elems_env, char *buff, t_elem	*new_elem)
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

void	line_cpy_till_ope(t_block **buff, t_line *sub_lst)
{
	t_block *new;

	while (*buff && !((*buff)->token == PIPE
			|| (*buff)->token == OR || (*buff)->token == AND))
	{
		new = new_block(ft_strdup((*buff)->word));
		if (!new)
			return (free_exit());
		new->token = (*buff)->token;
		line_lst_addback(sub_lst, new);
		*buff = (*buff)->next;
	}
}

void	line_cpy(t_line *line, t_line *sub_lst)
{
	t_block *new;
	t_block *buff;

	buff = line->head;
	while (buff)
	{
		new = new_block(ft_strdup(buff->word));
		if (!new)
			return (free_exit());
		new->token = buff->token;
		line_lst_addback(sub_lst, new);
		buff = buff->next;
	}
}

void	line_cpy_till_pend(t_block **buff, t_line *sub_lst)
{
	t_block	*new;
	int		count;

	count = -12;
	while (*buff && count != 0)
	{
		if ((*buff)->token == P_OPEN)
		{
			if (count == -12)
				count = 0;
			count++;
		}
		if ((*buff)->token == P_CLOSE)
			count--;
		new = new_block(ft_strdup((*buff)->word));
		if (!new)
			return (free_exit());
		new->token = (*buff)->token;
		line_lst_addback(sub_lst, new);
		*buff = (*buff)->next;
	}
}
