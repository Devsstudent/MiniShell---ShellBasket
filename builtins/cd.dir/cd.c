/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 21:43:30 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/19 12:23:00 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
#include "sys/types.h"
#include "dirent.h"

void	go_to_path(char *path)
{
	int		ret;
	char	s[100];

	ret = chdir(path);
	if (ret != 0)
		perror("cd");
	set_exit_status(ret);
	//ft_printf("new path = %s\n", getcwd(s, 100));
}

void	exec_cd(int ac, char **argv)
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
		go_to_path(argv[1]);
	}
}
