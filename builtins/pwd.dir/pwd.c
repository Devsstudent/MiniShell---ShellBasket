/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 13:54:45 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/20 17:34:05 by odessein         ###   ########.fr       */
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
	pwd_value = getcwd(buff, 4097);
	if (pwd_value == NULL)
	{
		perror("pwd");
		//Erange 34
		set_exit_status(34);
		return ;
	}
	ft_printf(0, "%s\n", pwd_value);
	set_exit_status(0);
}
