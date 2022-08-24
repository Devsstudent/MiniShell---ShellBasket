/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:12:26 by odessein          #+#    #+#             */
/*   Updated: 2022/08/24 12:42:16 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	check_builtins(char **argv)
{
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "cd", 3) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "export", 7) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "unset", 6) == 0)
		return (TRUE);
	return (FALSE);
}

size_t	get_ac(char **argv)
{
	int	ac;

	ac = 0;
	while (argv[ac])
		ac++;
	return (ac);
}

t_bool	exec_builtin(char **argv, t_dict *env)
{
	int	ac;

	ac = get_ac(argv);
	if (ft_strncmp(argv[0], "echo", 5) == 0)
	{
		exec_echo(ac, argv, env);
		return (TRUE);
	}
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
	{
		exec_pwd(ac, argv, env);
		return (TRUE);
	}
	else if (ft_strncmp(argv[0], "env", 4) == 0)
	{
		exec_env(ac, argv, env);
		return (TRUE);
	}
	return (FALSE);
}
//exit dans les builr in
t_bool	execve_test(char *pathname, char **argv, t_dict *env)
{
	char	**env_bis;

	env_bis = dict_to_double_char(env);
	if (!exec_builtin(argv, env))
	{
		int	i;
		i = 0;
		while (argv[i])
			i++;
		if (execve(pathname, argv, env_bis) == -1)
		{
			return (FALSE);
		}
	}
	return (TRUE);
}
