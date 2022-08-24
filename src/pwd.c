/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 13:54:45 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/24 15:14:28 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

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
		//Erange 34
		errno = 34;
		free_exit();
		return ;
	}
	ft_putstr_fd(buff, 1);
	write(1, "\n", 1);
	errno = 0;
	exit(0);
}
