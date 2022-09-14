/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 14:49:21 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/09/14 18:24:59 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

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
			if (write(STDOUT_FILENO, " ", 1) < 0)
				perror("");
		i++;
	}
}

t_bool	check_options(char *options)
{
	int	i;

	i = 2;
	while (options[i])
	{
		if (options[i] != 'n')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

void	find_first_arg(char **argv, int *i)
{
	*i = 2;
	while (argv[*i])
	{
		if (!(ft_strncmp(argv[*i], "-n", 2) == 0 && check_options(argv[*i])))
			return ;
		(*i)++;
	}
}

void	exec_echo(int ac, char **argv, t_dict *env)
{
	int	i;

	(void)env;
	if (ac == 1)
	{
		if (write(STDOUT_FILENO, "\n", 1) < 0)
			perror("");
		return ;
	}
	if (ac > 1 && ft_strncmp(argv[1], "-n", 2) == 0 && check_options(argv[1]))
	{
		find_first_arg(argv, &i);
		display_args(&argv[i]);
	}
	else if (ac > 1)
	{
		display_args(&argv[1]);
		if (write(STDOUT_FILENO, "\n", 1) < 0)
			perror("");
	}
	g_exit_status = 0;
}
