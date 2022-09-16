/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_symbol.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 18:11:48 by odessein          #+#    #+#             */
/*   Updated: 2022/08/20 13:24:33 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool check_pipe(t_token next, t_token previous)
{
	if (previous == UNDEF)
		return (FALSE);
	if (next == PIPE || next == UNDEF || next == OR || next == AND
			|| next == P_CLOSE)
		return (FALSE);
	return (TRUE);
}

t_bool	check_parentheses(t_token next, t_token previous, t_token token)
{
	if (token == P_CLOSE)
	{
		if (next == UNDEF || next == OR || next == AND || next == PIPE
			|| next == P_CLOSE || next == RED_IN || next == RED_OUT_TRUNC || next == RED_OUT_APPEND)
			return (TRUE);
	}
	else if (token == P_OPEN)
	{
		if (next == P_CLOSE)
		{
			print_syntax_error(NULL, 1);
			return (FALSE);
		}
		if (previous == UNDEF || previous == OR || previous == AND
			|| previous == PIPE || previous == P_OPEN)
			return (TRUE);
	}
	return (FALSE);
}

t_bool	check_and(t_token next, t_token previous, t_token token)
{
	(void)token;
	if (previous == UNDEF || next == UNDEF)
		return (FALSE);
	if (next == AND || next == OR || next == PIPE || next == P_CLOSE)
		return (FALSE);
	return (TRUE);
}

t_bool	check_or(t_token next, t_token previous, t_token token)
{
	(void)token;
	if (previous == UNDEF || next == UNDEF)
		return (FALSE);
	if (next == OR || next == PIPE || next == AND || next == P_CLOSE)
		return (FALSE);
	return (TRUE);
}

t_bool check_symbol(t_block *block)
{
	t_token	next_token;
	t_token	previous_token;
	t_token	token;

	//if close parenthese not possible to have not a symbol after (a part EOF)
	next_token = get_next_token(block->next);
	previous_token = get_previous_token(block->prev);
	token = block->token;
	if (token == PIPE)
		return (check_pipe(next_token, previous_token));
	else if (token == HERE_DOC)
		return (next_token == DELIMITER);
	else if (token == RED_IN || token == RED_OUT_APPEND 
		|| token == RED_OUT_TRUNC)
		return (next_token == FILES);
	else if (token == OR && !check_or(next_token, previous_token, token))
		return (FALSE);
	else if (token == AND && !check_and(next_token, previous_token, token))
		return (FALSE);
	else if ((token == P_OPEN || token == P_CLOSE) 
			&& !check_parentheses(next_token, previous_token, token))
		return (FALSE);
	return (TRUE);
}
