/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 14:19:25 by odessein          #+#    #+#             */
/*   Updated: 2022/07/29 18:27:54 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

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
}
