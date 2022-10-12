/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_utils_bis.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 21:11:31 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 21:11:46 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	is_white_space(char c)
{
	if (c == ' ' || (c > 8 && c < 14))
		return (TRUE);
	return (FALSE);
}

//perror("stat");
t_bool	check_kind_of_abs(struct stat statbuff, char *cmd, char **res)
{
	if (access(cmd, X_OK) == 0)
	{
		if (stat(cmd, &statbuff) == -1)
			return (FALSE);
		if ((statbuff.st_mode & S_IFMT) != S_IFREG)
			return (FALSE);
		*res = ft_strdup(cmd);
		if (!(*res))
			free_exit();
	}
	else if (errno == 13)
	{
		perror(cmd);
		g_exit_status = 126;
		return (FALSE);
	}
	else if (errno == 2)
	{
		perror(cmd);
		g_exit_status = errno;
		return (FALSE);
	}
	return (TRUE);
}
