/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 20:12:21 by odessein          #+#    #+#             */
/*   Updated: 2022/09/10 16:29:40 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	put_error_export(char *arg)
{
	ft_putstr_fd("shellbasket: export: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
}

t_bool	init_export(char *arg, int *i, t_bool *append)
{
	*i = 0;
	*append = FALSE;
	if (!(ft_isalpha(arg[0]) || arg[0] == '_'))
	{
		put_error_export(arg);
		g_exit_status = 1;
		return (FALSE);
	}
	return (TRUE);
}

t_bool	check_sign_in_export(char *arg, int i, t_bool *append)
{
	if (!ft_isalnum(arg[i]) && arg[i] != '+' && arg[i] != '_')
	{
		put_error_export(arg);
		g_exit_status = 1;
		return (FALSE);
	}
	if (arg[i] == '+' && arg[i + 1] != '=')
	{
		put_error_export(arg);
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
