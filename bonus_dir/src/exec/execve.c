/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:12:26 by odessein          #+#    #+#             */
/*   Updated: 2022/10/14 13:43:48 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

size_t	get_ac(char **argv)
{
	int	ac;

	ac = 0;
	while (argv[ac])
		ac++;
	return (ac);
}

t_bool	execve_cmd(char *pathname, t_info *exec_info, t_dict *env)
{
	char	**env_bis;
	int		i;
	char	**argv;

	argv = exec_info->argv;
	env_bis = dict_to_double_char_env(env);
	if (!exec_builtin(env, 1, exec_info))
	{
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

void	execve_builtin_alone(char *cmd_path, t_dict *env, t_info *exec_in)
{
	int	ac;

	if (!dup_cmd_alone(exec_in))
		return ;
	ac = get_ac(exec_in->argv);
	if (ft_strncmp(cmd_path, "exit", 5) == 0)
		exec_exit(ac, exec_in, 1);
	else if (ft_strncmp(cmd_path, "cd", 3) == 0)
		exec_cd(ac, exec_in->argv, env);
	else if (ft_strncmp(cmd_path, "export", 7) == 0)
		exec_export(ac, exec_in->argv, env);
	else if (ft_strncmp(cmd_path, "unset", 6) == 0)
		exec_unset(ac, exec_in->argv, env);
	else if (ft_strncmp(cmd_path, "pwd", 4) == 0)
		exec_pwd(ac, exec_in->argv, env);
	else if (ft_strncmp(cmd_path, "env", 4) == 0)
		exec_env(ac, exec_in->argv, env);
	else if (ft_strncmp(cmd_path, "echo", 5) == 0)
		exec_echo(ac, exec_in->argv, env);
}
