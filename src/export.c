/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 17:50:29 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/09/06 19:16:08 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//export can have multiple arguments //we have to parse each argument 
//to understand what we have to do, only = and +=
//errors if invalid name
//no errors if += on an unexisting variable
//after the =, + are possible and are characters
//when between quotes "", works like when not betwwen quotes
//remove quote before, and interpret it the right way

void	fill_export_env(char *key, char *value, t_bool append, t_dict *env)
{
	t_elem	*elem;
	char	*new_value;
	char	*old_value;

	if (dict_get_key(env, key) == NULL)
	{
		elem = create_elem(key, value);
		dict_addback(env, elem);
	}
	else
	{
		old_value = dict_get_value(env, key);
		if (append && old_value)
		{
			new_value = ft_strjoin(ft_strdup(old_value), value);
			free(value);
			dict_modify(env, key, new_value);
		}
		else
			dict_modify(env, key, value);
	}
}

void	get_key_value(char *arg, t_bool append, t_dict *env)
{
	char	*ptr;
	char	*key;
	char	*value;

	if (append)
		ptr = ft_strnstr(arg, "+=", ft_strlen(arg));
	else
		ptr = ft_strnstr(arg, "=", ft_strlen(arg));
	if (ptr == NULL)
		return ;
	key = ft_substr(arg, 0, ptr - arg);
	if (!key)
		free_exit();
	if (append)
		value = ft_substr(ptr, 2, ft_strlen(ptr));
	else
		value = ft_substr(ptr, 1, ft_strlen(ptr));
	if (value == NULL)
		return ;
	fill_export_env(key, value, append, env);
}

void	export_arg(char *arg, t_dict *env)
{
	int		i;
	t_bool	append;

	if (!init_export(arg, &i, &append))
		return ;
	while (arg[i])
	{
		if (arg[i] == '=')
			break ;
		if (!check_sign_in_export(arg, i, &append))
			return ;
		i++;
	}
	if (arg[i] == '\0')
		handle_key(ft_strdup(arg), env);
	else
		get_key_value(arg, append, env);
	g_exit_status = 0;
}

void	display_export_env(t_dict *env)
{
	int		i;
	char	**vars;

	i = 0;
	vars = dict_to_double_char_export(env);
	while (vars[i])
	{
		ft_putstr_fd(vars[i], 1);
		write(1, "\n", 1);
		i++;
	}
}

void	exec_export(int ac, char **argv, t_dict *env)
{
	int		i;

	printf("argv[1] = %s\n", argv[1]);
	if (ac == 1)
	{
		display_export_env(env);
		g_exit_status = 0;
		return ;
	}
	i = 1;
	while (i < ac)
	{
		export_arg(argv[i], env);
		i++;
	}
}
