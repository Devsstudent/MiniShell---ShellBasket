/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 21:43:30 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/19 14:47:15 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

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
		set_exit_status(ret);
		return ;
	}
	set_exit_status(ret);
	pwd_key = ft_strdup("PWD");
	if (pwd_key == NULL)
		free_exit();
	//see malloc in dict functions, which should we free or not
	pwd_value = ft_strdup(getcwd(buff, 4097));
	dict_modify(env, pwd_key, pwd_value);
	if (pwd_value == NULL)
		free_exit();
	//ft_printf("new path = %s\n", getcwd(buff, 4097));
}

void	exec_cd(int ac, char **argv, t_dict *env)
{
	if (ac == 1)
	{
		return (0);
		set_exit_status(0);
	}
	if (ac > 2)
	{
		ft_printf("cd: I only accept a relative or absolute path\n");
		set_exit_status(200);
	}
	else
	{
		go_to_path(argv[1], env);
	}
}
