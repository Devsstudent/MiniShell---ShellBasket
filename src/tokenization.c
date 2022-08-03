/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 13:03:13 by odessein          #+#    #+#             */
/*   Updated: 2022/08/03 15:55:28 by odessein         ###   ########.fr       */
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
		attribute_token(buff->word, buff);
		if (!check_symbol(buff))
			return (FALSE);
		//if (!handle_quote(buff))
	//		return (FALSE);
		buff = buff->next;
	}
	return (TRUE);
}

t_bool check_pipe(t_token next, t_token previous)
{
	if (previous == UNDEF)
		return (FALSE);
	if (next == PIPE || next == UNDEF)
		return (FALSE);
	return (TRUE);
}

t_bool check_HERE_DOC(t_token next)
{
	if (next != DELIMITER)
		return (FALSE);
	return (TRUE);
}

t_bool check_RED(t_token next)
{
	if (next != FILES)
		return (FALSE);
	return (TRUE);
}

t_bool check_symbol(t_block *block)
{
	t_token	next_token;
	t_token	previous_token;
	t_token	token;

	next_token = get_next_token(block->next);
	previous_token = get_previous_token(block->prev);
	token = block->token;
	if (token == PIPE)
	{
		return (check_pipe(next_token, previous_token));
	}
	else if (token == HERE_DOC)
		return (check_HERE_DOC(next_token));
	else if (token == RED_IN || token == RED_OUT_APPEND 
		|| token == RED_OUT_TRUNC)
		return (check_RED(next_token));
	return (TRUE);
}

t_token	get_next_token(t_block *next_block)
{
	if (!next_block)
		return (UNDEF);
	else
		attribute_token(next_block->word, next_block);
	return (next_block->token);
}

t_token	get_previous_token(t_block *previous_block)
{
	if (previous_block)
		return (previous_block->token);
	return (UNDEF);
}

void	attribute_token(char *block_content, t_block *block)
{
	t_bool	already_attributed;
	t_token	previous;

	already_attributed = attribute_symbol(block);
	if (!already_attributed)
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
		if (previous == RED_IN || previous == RED_OUT_TRUNC || previous == RED_OUT_APPEND)
		{
			block->token = FILES;
		}
		if (previous == DELIMITER)
			block->token = CMD;
		
	}
	//dans le premier parsing faudrait aussi separer sur les symbols :)
	//genre les espaces et les symbol les mettre dans des blocs seuls
}
/*
t_bool	check_dollar_in_block(t_block *block)
{

	//Maybe in the strcut do something like a double arr with 1 layer for stock the things after the $sign, then 1 layer to a  bool expand or not
	//May should be able to store in 1 array the expand value directly or not if motivated PAS POSSIBLE JE CROIT
	int	i;
	t_type	type;

	i = 0;
	type = NO;
	while (block->word[i])
	{
		if (block->word[i] == '\'' && type == SINGLE)
			type = NO;
		else if (block->word[i] == '\"' && type == DOUBLE)
			type = NO;
		else if (block->word[i] == '\"' && type == NO)
			type = DOUBLE;
		else if (block->word[i] == '\'' && type == NO)
			type = SINGLE;
		else if (block->word[i] == '$' && (type == DOUBLE || type == NO))
			return (TRUE);
		i++;
	}
}
*/
t_bool	attribute_symbol(t_block *block)
{
	if (ft_strncmp(block->word, "<", 2) == 0)
	{
		block->token = RED_IN;
		return (TRUE);
	}
	else if (ft_strncmp(block->word, ">", 2) == 0)
	{
		block->token = RED_OUT_TRUNC;
		return (TRUE);
	}
	else if (ft_strncmp(block->word, ">>", 3) == 0)
	{
		block->token = RED_OUT_APPEND;
		return (TRUE);
	}
	else if (ft_strncmp(block->word, "<<", 3) == 0)
	{
		block->token = HERE_DOC;
		return (TRUE);
	}
	else if (ft_strncmp(block->word, "|", 2) == 0)
	{
		block->token = PIPE;
		return (TRUE);
	}
	return (FALSE);
}

