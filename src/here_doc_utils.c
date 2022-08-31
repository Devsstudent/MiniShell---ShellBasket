/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:47:15 by odessein          #+#    #+#             */
/*   Updated: 2022/08/31 19:43:39 by odessein         ###   ########.fr       */
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
