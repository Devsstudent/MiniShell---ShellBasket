/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 13:03:13 by odessein          #+#    #+#             */
/*   Updated: 2022/08/24 18:37:13 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	tokenization(t_line *line)
{
	t_block	*buff;

	buff = line->head;
	while (buff)
	{
		buff->token = UNDEF;
		buff = buff->next;
	}
	buff = line->head;
	while (buff != NULL)
	{
		attribute_token(buff);
		//ft_printf(0, "%i\n", buff->token);
		if (!check_symbol(buff))
		{
			if (buff->next)
				print_syntax_error(buff->next->word, 0);
			else
				print_syntax_error("newline", 0);
			return (FALSE);
		}
		buff = buff->next;
	}
	return (TRUE);
}

t_token	get_next_token(t_block *next_block)
{
	if (!next_block)
		return (UNDEF);
	else
		attribute_token(next_block);
	return (next_block->token);
}

t_token	get_previous_token(t_block *previous_block)
{
	if (previous_block)
		return (previous_block->token);
	return (UNDEF);
}

void	attribute_token(t_block *block)
{
	t_bool	is_symbol;
	t_token	previous;

	is_symbol = attribute_symbol(block);
	if (!is_symbol)
	{
		previous = get_previous_token(block->prev);
		if (previous == UNDEF)
			block->token = CMD_ARG;
		if (previous == CMD_ARG || previous == FILES)
			block->token = CMD_ARG;
		if (previous == HERE_DOC)
			block->token = DELIMITER;
		if (previous == PIPE)
			block->token = CMD_ARG;
		if (previous == RED_IN || previous == RED_OUT_TRUNC
			|| previous == RED_OUT_APPEND)
			block->token = FILES;
		if (previous == DELIMITER)
			block->token = CMD_ARG;
	}
}

t_bool	attribute_symbol(t_block *block)
{
	if (ft_strncmp(block->word, "<", 2) == 0)
		block->token = RED_IN;
	else if (ft_strncmp(block->word, ">", 2) == 0)
		block->token = RED_OUT_TRUNC;
	else if (ft_strncmp(block->word, ">>", 3) == 0)
		block->token = RED_OUT_APPEND;
	else if (ft_strncmp(block->word, "<<", 3) == 0)
		block->token = HERE_DOC;
	else if (ft_strncmp(block->word, "|", 2) == 0)
		block->token = PIPE;
	if (block->token != UNDEF)
		return (TRUE);
	return (FALSE);
}
