/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 17:16:09 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/09/06 12:59:08 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	exec_unset(int ac, char **argv, t_dict *env)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (strncmp(argv[i], "_", 2) != 0)
			dict_delone(env, argv[i]);
		i++;
	}
	g_exit_status = 0;
}
