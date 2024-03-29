/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 16:21:45 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/09/12 14:40:16 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
#include "limits.h"

static int	arg_is_num(char *arg)
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

static int	arg_is_ll(char *arg)
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

static long long	ft_atoll(char *arg)
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
		return ((long long)(((long long)(num)) * -1));
	else
		return ((long long)(num));
}

void	exec_exit(int ac, t_info *exec_info, t_bool display_exit)
{
	if (display_exit)
		ft_putstr_fd("exit\n", 1);
	if (ac >= 2)
	{
		if (!arg_is_num(exec_info->argv[1]) || !arg_is_ll(exec_info->argv[1]))
		{
			ft_putstr_fd("shellbasket: exit: numeric argument required\n", 2);
			g_exit_status = 2;
		}
		else if (ac > 2)
		{
			ft_putstr_fd("shellbasket: exit: too many arguments\n", 2);
			if (g_exit_status != 127)
				g_exit_status = 1;
			return ;
		}
		else
			g_exit_status = (u_int8_t)ft_atoll(exec_info->argv[1]);
	}
	close(exec_info->stdi);
	if (exec_info->stdou != -1)
		close(exec_info->stdou);
	free_exit();
}
