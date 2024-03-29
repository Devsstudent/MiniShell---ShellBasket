/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 18:48:33 by odessein          #+#    #+#             */
/*   Updated: 2022/10/11 19:29:07 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static t_bool	line_only_has_white_space(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!((line[i] >= 9 && line[i] <= 13) || line[i] == 32))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

t_bool	ms_line(char **line, t_info *exec_in)
{
	listen_to_sigs();
	rl_outstream = stderr;
	*line = readline("@ShellBasket^$ ");
	if (!(*line))
	{
		if (exec_in->stdou != -1)
			close(exec_in->stdou);
		close(exec_in->stdi);
		write(2, "exit\n", 5);
		free_exit();
	}
	add_history(*line);
	add_to_gc(SIMPLE, *line, get_gc());
	if (*line && (!(*line[0]) || line_only_has_white_space(*line)))
		return (close(exec_in->stdi), close(exec_in->stdou), TRUE);
	return (FALSE);
}

t_tree	*ms_lex_and_parse(char **line, t_info *exec_in)
{
	t_line	*line_lst;
	t_tree	*tree;

	tree = (t_tree *) malloc(sizeof(t_tree));
	if (!tree)
		free_exit();
	add_to_gc(TREE, tree, get_gc());
	line_lst = fill_line_lst(*line);
	if (line_lst == NULL)
		return (tree->head = NULL, tree);
	add_to_gc(LINE, line_lst, get_gc());
	if (!tokenization(line_lst))
		return (tree->head = NULL, tree);
	fill_ast(line_lst, tree);
	exec_in->fd_arr_size = total_block(tree->head);
	exec_in->fd_arr = malloc(sizeof(int) * exec_in->fd_arr_size);
	if (!exec_in->fd_arr)
		free_exit();
	return (tree);
}
