/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_utils_bis.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 15:28:56 by odessein          #+#    #+#             */
/*   Updated: 2022/10/20 13:27:16 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	is_white_space(char c)
{
	if (c == ' ' || (c > 8 && c < 14))
		return (TRUE);
	return (FALSE);
}

static t_bool	check_dir(struct stat statbuff, char *cmd)
{
	if ((statbuff.st_mode & S_IFMT) == S_IFDIR)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		g_exit_status = 126;
	}
	return (FALSE);
}

t_bool	check_kind_of_abs(struct stat statbuff, char *cmd, char **res)
{
	if (access(cmd, X_OK) == 0)
	{
		if (stat(cmd, &statbuff) == -1)
			return (FALSE);
		if ((statbuff.st_mode & S_IFMT) != S_IFREG)
			return (check_dir(statbuff, cmd));
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
