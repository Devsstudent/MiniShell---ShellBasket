/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 14:49:21 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/19 15:10:29 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	display_args(char **argv)
{
	int	i;

	i = 0;
	while (argv[i] != NULL)
	{
		ft_printf("%s", argv[i]);
		if (argv[i + 1] != NULL)
			ft_printf(" ");
		i++;
	}
}

void	exec_echo(int ac, char **argv, t_dict *env)
{
	(void)env;
	if (ac == 1)
	{
		ft_printf("\n");
		return ;
	}
	if (ac > 1 && ft_strncmp(argv[1], "-n", 3) == 0)
	{
		display_args(&argv[2]);
	}
	else if (ac > 1 && ft_strncmp(argv[1], "-n", 3) != 0)
	{
		display_args(&argv[1]);
		ft_printf("\n");
	}
}

