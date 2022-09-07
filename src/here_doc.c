/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 20:04:23 by odessein          #+#    #+#             */
/*   Updated: 2022/09/07 18:33:49 by odessein         ###   ########.fr       */
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
	return ;
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
	return ;
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

t_bool	handle_ctrl_c(char **line, int stdi)
{
	if (g_exit_status == 140)
	{
		free(*line);
		if (dup2(stdi, STDIN_FILENO) == -1)
			return (FALSE);
		*line = NULL;
	}
	return (TRUE);
}

//freeexit si open crash maybe

//A revoir pck faut aussi geree les quotes au milieux du mot mdrr

void	fill_here_doc(char *delim, int turn, int *fd_arr)
{
	char	*new_delim;
	char	*line;
	int		stdi;

	stdi = dup(STDIN_FILENO);
	g_exit_status = 120;
	new_delim = get_delim(delim);
	add_to_gc(SIMPLE, new_delim, get_gc());
	line = readline("> ");
	if (!handle_ctrl_c(&line, stdi))
		return ;
	while (line)
	{
		write(fd_arr[turn], line, ft_strlen(line));
		write(fd_arr[turn], "\n", 1);
		if (ft_strncmp(new_delim, line, ft_strlen(new_delim) + 1) == 0)
			break ;
		free(line);
		line = readline("> ");
		if (!handle_ctrl_c(&line, stdi))
			return ;
	}
	close_reopen_here_doc(turn, fd_arr, line);
}
