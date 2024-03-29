/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 21:43:30 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/09/10 15:30:47 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

	/*
	ret = access(path, F_OK);
	if (ret == -1)
	{
		perror("shellbasket");
		g_exit_status = ret;
		return ;
	}
	*/
void	go_to_path(char *path, t_dict *env)
{
	int		ret;
	char	buff[4097];
	char	*pwd_key;
	char	*pwd_value;

	ret = chdir(path);
	if (ret != 0)
	{
		perror("cd");
		g_exit_status = 1;
		return ;
	}
	dict_modify(env, ft_strdup("OLDPWD"),
		ft_strdup(dict_get_value(env, "PWD")));
	g_exit_status = 0;
	pwd_key = ft_strdup("PWD");
	pwd_value = ft_strdup(getcwd(buff, 4097));
	dict_modify(env, pwd_key, pwd_value);
}

void	exec_cd(int ac, char **argv, t_dict *env)
{
	if (ac == 1)
	{
		g_exit_status = 0;
		return ;
	}
	if (ac > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		g_exit_status = 1;
	}
	else
		go_to_path(argv[1], env);
}
