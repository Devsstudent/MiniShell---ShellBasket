/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 16:53:47 by odessein          #+#    #+#             */
/*   Updated: 2022/09/08 16:54:45 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	check_builtins(char **argv)
{
	if (argv[0] == NULL)
		return (FALSE);
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

t_bool	exec_builtin(char **argv, t_dict *env, t_bool fork)
{
	int	ac;

	ac = get_ac(argv);
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		exec_echo(ac, argv, env);
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		exec_pwd(ac, argv, env);
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		exec_env(ac, argv, env);
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		exec_exit(ac, argv, 0);
	else if (ft_strncmp(argv[0], "unset", 5) == 0)
		exec_unset(ac, argv, env);
	else if (ft_strncmp(argv[0], "export", 5) == 0)
		exec_export(ac, argv, env);
	else if (ft_strncmp(argv[0], "cd", 5) == 0)
		exec_cd(ac, argv, env);
	else
		return (FALSE);
	if (fork)
		free_exit();
	return (TRUE);
}
