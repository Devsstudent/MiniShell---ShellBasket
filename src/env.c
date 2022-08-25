/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 17:38:52 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/24 22:15:27 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	exec_env(int ac, char **argv, t_dict *env)
{
	int		i;
	char	**double_arr_env;

	(void)ac;
	(void)argv;
	i = 0;
	double_arr_env = dict_to_double_char_env(env);
	while (double_arr_env[i])
	{
		ft_putstr_fd(double_arr_env[i], 1);
		write(1, "\n", 1);
		i++;
	}
	g_exit_status = 0;
}
