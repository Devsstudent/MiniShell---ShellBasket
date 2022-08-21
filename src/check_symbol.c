/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_symbol.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 18:11:48 by odessein          #+#    #+#             */
/*   Updated: 2022/08/21 16:26:25 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool check_pipe(t_token next, t_token previous)
{
	if (previous == UNDEF)
		return (FALSE);
	if (next == PIPE || next == UNDEF)
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
	return (TRUE);
}
