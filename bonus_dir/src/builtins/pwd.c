/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 13:54:45 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/24 19:26:10 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//Erange 34
void	exec_pwd(int ac, char **argv, t_dict *env)
{
	char	buff[4097];
	char	*pwd_value;

	(void)ac;
	(void)argv;
	(void)env;
	pwd_value = getcwd(buff, 4097);
	if (pwd_value == NULL)
	{
		perror("pwd");
		g_exit_status = 34;
		return ;
	}
	ft_putstr_fd(buff, 1);
	write(1, "\n", 1);
	g_exit_status = 0;
}
