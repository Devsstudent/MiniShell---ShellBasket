/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 14:49:21 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/19 18:15:49 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//without -e, the escape sequences are not interpreted
//to do that, use write, char by char, not ft_printf, it does it alone, it erases \ alone

void	display_single_arg(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		write(1, &arg[i], 1);
		i++;
	}
}

void	display_args(char **argv)
{
	int	i;

	i = 0;
	while (argv[i] != NULL)
	{
		display_single_arg(argv[i]);
		if (argv[i + 1] != NULL)
			write(1, " ", 1);
		i++;
	}
}

void	exec_echo(int ac, char **argv, t_dict *env)
{
	(void)env;
	if (ac == 1)
	{
		write(1, "\n", 1);
		return ;
	}
	if (ac > 1 && ft_strncmp(argv[1], "-n", 3) == 0)
	{
		display_args(&argv[2]);
	}
	else if (ac > 1 && ft_strncmp(argv[1], "-n", 3) != 0)
	{
		display_args(&argv[1]);
		write(1, "\n", 1);
	}
	set_exit_status(0);
}

/*
int	main(int ac, char **argv)
{
	exec_echo(ac, argv);
	return (0);
}
*/

