/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 17:16:09 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/24 19:26:26 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	exec_unset(int ac, char **argv, t_dict *env)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		dict_delone(env, argv[i]);
		i++;
	}
	g_exit_status = 0;
}
