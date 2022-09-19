/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:47:15 by odessein          #+#    #+#             */
/*   Updated: 2022/09/07 18:33:44 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	remove_tmp_file(int file_nb, int *fd_arr)
{
	int		i;
	char	*num;
	char	*name;

	i = 0;
	while (i < file_nb)
	{
		num = ft_itoa(i);
		name = ft_strjoin(ft_strdup(".tmp_here_doc_"), num);
		unlink(name);
		free(name);
		free(num);
		i++;
	}
	free(fd_arr);
}

static void	replace_quote(char *delim)
{
	int	i;

	i = 0;
	while (delim[i])
	{
		if (delim[i] == '\"' || delim[i] == '\'')
			delim[i] = 21;
		i++;
	}
}

static int	get_size_delim(char *delim)
{
	int	size;
	int	i;

	i = 0;
	size = 0;
	while (delim[i])
	{
		if ((delim[i + 1] && (delim[i] == '$' && delim[i + 1] != 21))
			|| (delim[i] != 21 && delim[i] != '$'))
			size++;
		i++;
	}
	return (size);
}

char	*get_delim(char *delim)
{
	int		i;
	int		size;
	int		j;
	char	*new_delim;

	replace_quote(delim);
	size = get_size_delim(delim);
	new_delim = malloc(sizeof(*new_delim) * size + 1);
	if (!new_delim)
		free_exit();
	j = 0;
	i = 0;
	while (delim[i])
	{
		if (delim[i] == '$' && delim[i + 1] != 21)
			new_delim[j++] = delim[i];
		else if (delim[i] != 21 && delim[i] != '$')
			new_delim[j++] = delim[i];
		i++;
	}
	new_delim[j] = 0;
	return (new_delim);
}

void	close_reopen_here_doc(int turn, int *fd_arr, char *line)
{
	char	*num;
	char	*name;

	if (!line && g_exit_status != 140)
		write(2, "warning: here-document delimited by end-of-file\n", 48);
	if (g_exit_status != 140)
		g_exit_status = 0;
	free(line);
	close(fd_arr[turn]);
	num = ft_itoa(turn);
	name = ft_strjoin(ft_strdup(".tmp_here_doc_"), num);
	fd_arr[turn] = open(name, O_RDONLY, 0600);
	free(num);
	free(name);
}
