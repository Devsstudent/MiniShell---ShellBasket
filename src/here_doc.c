/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 20:04:23 by odessein          #+#    #+#             */
/*   Updated: 2022/09/01 18:37:00 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	total_block(t_leaf *leaf)
{
	int	i;

	i = 0;
	if (leaf->type == CMD)
		return (1);
	else if (leaf->type == PIPE_L)
	{
		while (leaf != NULL)
		{
			i++;
			leaf = leaf->right;
		}
	}
	return (i);
}

void	parse_here_doc(t_leaf *leaf, int *fd_arr, int turn)
{
	if (!leaf)
		return ;
	if (g_exit_status == 140)
		return ;
	if (leaf->type == PIPE_L)
	{
		check_here_doc(leaf->left->content, turn, fd_arr);
		if (leaf->right)
			parse_here_doc(leaf->right, fd_arr, turn + 1);
	}
	else if (leaf->type == CMD)
		check_here_doc(leaf->content, turn, fd_arr);
}

//Ou on check les delimiter et on envoie pas le next au choix
void	check_here_doc(t_line *sub, int turn, int *fd_arr)
{
	t_block	*buff;

	buff = sub->head;
	while (buff)
	{
		if (g_exit_status == 140)
			return ;
		if (buff->token == HERE_DOC)
		{
			create_tmp(fd_arr, turn);
			fill_here_doc(buff->next->word, turn, fd_arr);
		}
		buff = buff->next;
	}
}

void	create_tmp(int *fd_arr, int turn)
{
	char	*num;
	char	*name;

	num = ft_itoa(turn);
	name = ft_strjoin(ft_strdup(".tmp_here_doc_"), num);
	fd_arr[turn] = open(name, O_RDWR | O_CREAT | O_TRUNC, 0600);
	free(num);
	free(name);
	if (fd_arr[turn] == -1)
	{
		perror("open crash");
		return ;
	}
}

//freeexit si open crash maybe

//A revoir pck faut aussi geree les quotes au milieux du mot mdrr
void	fill_here_doc(char *delim, int turn, int *fd_arr)
{
	char	*new_delim;
	char	*line;

	errno = 140;
	new_delim = get_delim(delim);
	add_to_gc(SIMPLE, new_delim, get_gc());
	write(1, "> ", 2);
	line = get_next_line(0);
	while (line && ft_strncmp(line, new_delim, ft_strlen(new_delim)) != 0)
	{
		if (line)
			write(fd_arr[turn], line, ft_strlen(line));
		write(1, "> ", 2);
		free(line);
		line = get_next_line(0);
	}
	if (!line && g_exit_status != 140)
		write(2, "warning: here-document delimited by end-of-file\n", 48);
	close_reopen_here_doc(turn, fd_arr, line);
}
