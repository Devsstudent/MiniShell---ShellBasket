/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 13:03:13 by odessein          #+#    #+#             */
/*   Updated: 2022/08/19 13:43:59 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	tokenization(t_line *line)
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
		{
			if (buff->next)
				ft_printf(0, "syntax error: unexpected token near field `%s`", buff->next->word); //ft_printf_put_error
			else
				ft_printf(0, "syntax error: unexpected token near field `newline`");
			free_exit();
		}
		buff = buff->next;
	}
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
		if (previous == UNDEF || previous == AND || previous == OR)
			block->token = CMD_ARG;
		if (previous == CMD_ARG || previous == FILES || previous == P_OPEN)
			block->token = CMD_ARG;
		if (previous == PIPE)
			block->token = CMD_ARG;
		if (previous == HERE_DOC)
			block->token = DELIMITER;
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
	else if (ft_strncmp(block->word, "||", 3) == 0)
		block->token = OR;
	else if (ft_strncmp(block->word, "&&", 3) == 0)
		block->token = AND;
	else if (ft_strncmp(block->word, "|", 2) == 0)
		block->token = PIPE;
	else if (ft_strncmp(block->word, ")", 2) == 0)
		block->token = P_CLOSE;
	else if (ft_strncmp(block->word, "(", 2) == 0)
		block->token = P_OPEN;
	if (block->token != UNDEF)
		return (TRUE);
	return (FALSE);
}
