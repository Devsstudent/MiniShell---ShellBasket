/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 20:12:21 by odessein          #+#    #+#             */
/*   Updated: 2022/09/04 20:09:45 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	init_export(char *arg, int *i, t_bool *append)
{
	*i = 0;
	*append = FALSE;
	if (!(ft_isalpha(arg[0]) || arg[0] == '_'))
	{
		ft_putstr_fd("hellbasket: export: not a valid identifier\n", 2);
		g_exit_status = 1;
		return (FALSE);
	}
	return (TRUE);
}

t_bool	check_sign_in_export(char *arg, int i, t_bool *append)
{
	if (!ft_isalnum(arg[i]) && arg[i] != '+' && arg[i] != '_')
	{
		ft_putstr_fd("ellbasket: export: not a valid identifier\n", 2);
		g_exit_status = 1;
		return (FALSE);
	}
	if (arg[i] == '+' && arg[i + 1] != '=')
	{
		ft_putstr_fd("basket: export: not a valid identifier\n", 2);
		g_exit_status = 1;
		return (FALSE);
	}
	else if (arg[i] == '+')
		*append = TRUE;
	return (TRUE);
}

void	handle_key(char *key, t_dict *env)
{
	t_elem	*elem;

	if (dict_get_key(env, key) == NULL)
	{
		elem = create_elem(key, NULL);
		dict_addback(env, elem);
	}
	else
		free(key);
}
