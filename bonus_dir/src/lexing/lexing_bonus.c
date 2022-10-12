/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <mbelrhaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 14:15:02 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 22:07:43 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//Fill the line list; block by block
//First check the line quotes

static void	analyse_symbol(char *line, int *i, int *size, t_line *lst)
{
	if (line[*i] == '|')
		handle_pipe(line, i, size, lst);
	else if (line[*i] == '>')
		handle_red_o(line, i, size, lst);
	else if (line[*i] == '<')
		handle_red_i(line, i, size, lst);
	else if (line[*i] == '&')
		handle_and(line, i, size, lst);
	else if (line[*i] == '(' || line[*i] == ')')
		handle_par(line, i, size, lst);
	else
		(*size)++;
}

static void	handle_space(char *line, int *i, int *size, t_line *lst)
{
	if (*i > 0 && (line[*i - 1]))
		fill_word(size, lst, line, *i - 1);
	*size = 0;
}

static void	analyse_word(char *line, int *i, int *size_word, t_line *lst)
{
	if (line[*i] == ' ' || (line[*i] >= 9 && line[*i] <= 13))
		handle_space(line, i, size_word, lst);
	else
		analyse_symbol(line, i, size_word, lst);
}

static void	handle_line(char *line, t_line *lst)
{
	int		i;
	int		size_word;
	t_bool	not_quote;

	i = 0;
	size_word = 0;
	while (line[i] != 0)
	{
		not_quote = not_in_quote(line, i);
		if (!not_quote)
			size_word++;
		if (not_quote && (line[i] == '\'' || line[i] == '\"')
			&& size_word++ && i++)
			continue ;
		if (not_quote && line[i] != 0)
			analyse_word(line, &i, &size_word, lst);
		if (line[i] != 0)
			i++;
	}
	fill_word(&size_word, lst, line, i - 1);
}

t_line	*fill_line_lst(char *line)
{
	t_line	*block_lst;

	block_lst = (t_line *) malloc(sizeof(t_line));
	if (!block_lst)
		free_exit();
	if (!check_lines_quotes(line))
	{
		print_syntax_error(NULL, 2);
		return (free(block_lst), NULL);
	}
	if (!check_lines_parentheses(line))
	{
		print_syntax_error(NULL, 1);
		return (free(block_lst), NULL);
	}
	block_lst->head = NULL;
	handle_line(line, block_lst);
	return (block_lst);
}
