/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_bonus_handle.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 22:03:55 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 22:04:13 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	handle_pipe(char *line, int *i, int *size, t_line *lst)
{
	if (*i >= 1 && line[*i - 1])
		fill_word(size, lst, line, *i - 1);
	if (line[*i + 1] && line[*i + 1] == '|')
	{
		(*i)++;
		(*size) += 2;
		fill_word(size, lst, line, *i);
	}
	else
	{
		(*size) += 1;
		fill_word(size, lst, line, *i);
	}
}

void	handle_red_o(char *line, int *i, int *size, t_line *lst)
{
	if (*i > 1 && line[*i - 1])
		fill_word(size, lst, line, *i - 1);
	if (line[*i + 1] && line[*i + 1] == '>')
	{
		(*i)++;
		(*size) += 2;
		fill_word(size, lst, line, *i);
	}
	else
	{
		(*size) += 1;
		fill_word(size, lst, line, *i);
	}
}

void	handle_and(char *line, int *i, int *size, t_line *lst)
{
	if (*i > 1 && line[*i - 1])
		fill_word(size, lst, line, *i - 1);
	if (line[*i + 1] && line[*i + 1] == '&')
	{
		(*i)++;
		(*size) += 2;
		fill_word(size, lst, line, *i);
	}
	else
		print_syntax_error("&", 0);
}

void	handle_par(char *line, int *i, int *size, t_line *lst)
{
	if (*i > 1 && line[*i - 1])
		fill_word(size, lst, line, *i - 1);
	if (line[*i] == ')' || line[*i] == '(')
	{
		(*size) += 1;
		fill_word(size, lst, line, *i);
	}
}

void	handle_red_i(char *line, int *i, int *size, t_line *lst)
{
	if (*i > 1 && line[*i - 1])
		fill_word(size, lst, line, *i - 1);
	if (line[*i + 1] && line[*i + 1] == '<')
	{
		(*i)++;
		(*size) += 2;
		fill_word(size, lst, line, *i);
	}
	else
	{
		(*size) += 1;
		fill_word(size, lst, line, *i);
	}
}
