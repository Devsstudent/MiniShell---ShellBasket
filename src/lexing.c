/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <mbelrhaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 14:15:02 by odessein          #+#    #+#             */
/*   Updated: 2022/08/31 20:07:11 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//Fill the line list; block by block
//First check the line quotes

static t_bool	check_lines_quotes(char *line)
{
	int		i;
	t_quote	quote;

	i = 0;
	quote = NO;
	while (line[i] != 0)
	{
		if (line[i] == '\"' && quote == DOUBLE_QUOTE)
			quote = NO;
		else if (line[i] == '\'' && quote == SINGLE)
			quote = NO;
		else if (line[i] == '\'' && quote == NO)
			quote = SINGLE;
		else if (line[i] == '\"' && quote == NO)
			quote = DOUBLE_QUOTE;
		i++;
	}
	if (quote != NO)
		return (FALSE);
	return (TRUE);
}

t_bool	not_in_quote(char *line, int i)
{
	static t_quote	quote;

	if (line[i] == '\"' && quote == DOUBLE_QUOTE)
		quote = NO;
	else if (line[i] == '\'' && quote == SINGLE)
		quote = NO;
	else if (line[i] == '\'' && quote == NO)
		quote = SINGLE;
	else if (line [i] == '\"' && quote == NO)
		quote = DOUBLE_QUOTE;
	if (quote == NO)
		return (TRUE);
	return (FALSE);
}


//Not possible to have single &
//Taking care ()
//Check || && ();

void	analyse_symbol(char *line, int *i, int *size, t_line *lst)
{
	if (line[*i] == '|')
		handle_pipe(line, i, size, lst);
	else if (line[*i] == '>')
		handle_red_o(line, i, size, lst);
	else if (line[*i] == '<')
		handle_red_i(line, i, size, lst);
	else
		(*size)++;
}

void	analyse_word(char *line, int *i, int *size_word, t_line *lst)
{
	if (line[*i] == ' ')
		handle_space(line, i, size_word, lst);
	else
		analyse_symbol(line, i, size_word, lst);
}

t_line	*fill_line_lst(char *line)
{
	t_line	*block_lst;

	block_lst = (t_line *) malloc(sizeof(t_line));
	if (!block_lst)
		free_exit();
	if (!check_lines_quotes(line))
		return (free(block_lst), print_syntax_error(NULL, 1), NULL);
	block_lst->head = NULL;
	handle_line(line, block_lst);
	return (block_lst);
}
