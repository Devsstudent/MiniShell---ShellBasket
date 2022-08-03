/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 13:03:13 by odessein          #+#    #+#             */
/*   Updated: 2022/08/03 18:24:02 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	tokenisation(t_line *line)
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
		if (!check_symbol(buff))
			return (FALSE);
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
			block->token = CMD;
		if (previous == CMD || previous == ARG || previous == FILES)
			block->token = ARG;
		if (previous == HERE_DOC)
			block->token = DELIMITER;
		if (previous == PIPE)
			block->token = CMD;
		if (previous == RED_IN || previous == RED_OUT_TRUNC 
			|| previous == RED_OUT_APPEND)
			block->token = FILES;
		if (previous == DELIMITER)
			block->token = CMD;
		
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
