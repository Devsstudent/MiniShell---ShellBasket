/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 16:21:45 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/19 18:09:48 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
//#include "minishell.h"
#include "unistd.h"
#include "stdio.h"
#include "limits.h"
#include "stdlib.h"
#include "string.h"

int	arg_is_num(char *arg)
{
	int		i;

	i = 0;
	if (arg[0] == '+' || arg[0] == '-')
		i++;
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	arg_is_ll(char *arg)
{
	size_t	num;
	int		i;

	i = 0;
	num = 0;
	if (arg[0] == '+' || arg[0] == '-')
		i++;
	if (strlen(&arg[i]) > 19)
		return (0);
	if (strncmp(arg, "-9223372036854775808", 21) == 0)
		return (1);
	while (arg[i])
	{
		num = num * 10 + (arg[i] - 48);
		i++;
	}
	if (num > LLONG_MAX)
		return (0);
	return (1);
}

long long	ft_atoll(char *arg)
{
	size_t		num;
	int			i;

	i = 0;
	num = 0;
	if (arg[0] == '+' || arg[0] == '-')
		i++;
	while (arg[i])
	{
		num = num * 10 + (arg[i] - 48);
		i++;
	}
	if (arg[0] == '-')
		return ((long long)((long long)(num) * -1));
	else
		return ((long long)(num));
}

void	exec_exit(int ac, char **argv/*, t_dict *env*/)
{
	long long	exit_status;

	printf("exit\n");

	//if too many arguments, the status is 1, exit is printed but not exit is performed
	if (ac > 2)
	{
		printf("shellbasket: exit: too many arguments\n");
		//set_exit_status(1);
		exit_status = 1;
		return ;
	}

	//if not numeric argument, the status is 2, exit is performed
	else if (ac == 2)
	{
		if (!arg_is_num(argv[1]) || !arg_is_ll(argv[1]))
		{
			printf("shellbasket: exit: %s: numeric argument required\n", argv[1]);
			exit_status = 2;
			//set_exit_status(2);
		}
		else
		{
			exit_status = ft_atoll(argv[1]);
			printf("exit status = %lld\n", exit_status);
			exit_status = (u_int8_t)exit_status;
			printf("final exit status = %d\n", exit_status);
			//set_exit_status(exit_status);
		}
	}
	exit(exit_status);
	//free_exit(get_exit_status());
}

int	main(int ac, char **argv)
{
	exec_exit(ac, argv);
	return (0);
}
