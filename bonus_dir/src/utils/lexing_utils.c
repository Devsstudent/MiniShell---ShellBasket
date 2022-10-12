/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 22:00:44 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 22:04:50 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	is_symbol(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '&')
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

t_bool	check_lines_quotes(char *line)
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

t_bool	check_lines_parentheses(char *line)
{
	int	i;
	int	not_quote;
	int	close;
	int	open;

	i = 0;
	close = 0;
	open = 0;
	while (line[i])
	{
		not_quote = not_in_quote(line, i);
		if (not_quote && line[i] == '(')
			open++;
		else if (not_quote && line[i] == ')')
			close++;
		i++;
	}
	if (close != open)
		return (FALSE);
	return (TRUE);
}
