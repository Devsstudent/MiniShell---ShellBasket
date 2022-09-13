/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 14:19:25 by odessein          #+#    #+#             */
/*   Updated: 2022/08/19 21:48:59 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
int	**size_env(char **env)
{
	int		**xy;
	int		i;
	int		j;

	xy = (int **) malloc(sizeof(*xy) * 2);
	if (!xy)
		return (NULL);
	i = 0;
	while (env[i] != 0)
		i++;
	*xy = malloc(sizeof(**xy));
	if (!(*xy))
		return (NULL);
	**xy = i;
	*(xy + 1) = malloc(sizeof(**xy) * i);
	if (!*(xy + 1))
		return (NULL);
	i = -1;
	while (++i < xy[0][0])
	{
		j = 0;
		while (env[i][j])
			j++;
		xy[1][i] = j;
	}
	return (xy);
}

char	**copy_env(char **env)
{
	int		i;
	int		j;
	char	**new_env;
	int		**size_envp;

	size_envp = size_env(env);
	new_env = malloc(sizeof(*new_env) * (size_envp[0][0] + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < size_envp[0][0])
	{
		j = 0;
		new_env[i] = malloc(sizeof(**new_env) * size_envp[1][i]);
		if (!new_env[i])
			return (NULL);
		while (env[i][j] != 0)
		{
			new_env[i][j] = env[i][j];
			j++;
		}
		i++;
	}
	env[i] = NULL;
	return(new_env);
}*/

t_dict	*double_char_to_lst(char **d_char)
{
	t_elem	*new;
	t_dict	*env;

	env = (t_dict *)malloc(sizeof(t_dict));
	env->size = 0;
	env->head = NULL;
	env->last = NULL;
	add_to_gc(ENV, env, get_gc());
	while (*d_char != 0)
	{
		new = new_elem(*d_char);
		if (!new)
			free_exit();
		dict_addback(env, new);
		d_char++;
	}
	return (env);
}
