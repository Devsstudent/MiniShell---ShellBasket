/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 17:16:09 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/10/20 10:52:05 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	put_error_unset(char *arg)
{
	ft_putstr_fd("shellbasket: unset: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
}

static void	check_valid_identifier(char *arg)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(arg[0]) || arg[0] == '_'))
	{
		put_error_unset(arg);
		return ;
	}
	while (arg[i])
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '+' && arg[i] != '_')
		{
			put_error_unset(arg);
			return ;
		}
		if (arg[i] == '+' && arg[i + 1] != '=')
		{
			put_error_unset(arg);
			return ;
		}
		i++;
	}
}

void	exec_unset(int ac, char **argv, t_dict *env)
{
	int		i;

	i = 1;
	while (i < ac)
	{
		check_valid_identifier(argv[i]);
		if (strncmp(argv[i], "_", 2) != 0)
			dict_delone(env, argv[i]);
		i++;
	}
	g_exit_status = 0;
}
