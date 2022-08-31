/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 19:56:57 by odessein          #+#    #+#             */
/*   Updated: 2022/08/31 19:57:49 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
void	handle_pipe(char *line, int *i, int *size, t_line *lst)
{
	if (*i >= 1 && line[*i - 1])
		fill_word(size, lst, line, *i - 1);
	(*size) += 1;
	fill_word(size, lst, line, *i);
}

void	handle_red_o(char *line, int *i, int *size, t_line *lst)
{
	if (*i >= 1 && line[*i - 1])
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

void	handle_red_i(char *line, int *i, int *size, t_line *lst)
{
	if (*i >= 1 && line[*i - 1])
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

void	handle_line(char *line, t_line *lst)
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

void	handle_space(char *line, int *i, int *size, t_line *lst)
{
	if (*i > 0 && (line[*i - 1]))
		fill_word(size, lst, line, *i - 1);
	*size = 0;
}
