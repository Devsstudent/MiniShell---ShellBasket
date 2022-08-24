/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 13:54:45 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/24 12:03:55 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	exec_pwd(int ac, char **argv, t_dict *env)
{
	int		ret;
	char	buff[4097];
	char	*pwd_value;

	(void)ac;
	(void)argv;
	(void)env;
	(void)ret;
	pwd_value = getcwd(buff, 4097);
	if (pwd_value == NULL)
	{
		perror("pwd");
		//Erange 34
		g_exit_status = 34;
		free_exit();
		return ;
	}
	g_exit_status = 0;
	exit(0);
}
