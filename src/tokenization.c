/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 13:03:13 by odessein          #+#    #+#             */
/*   Updated: 2022/08/01 16:58:46 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	tokenisation(t_line *line, t_gc *gc)
{
	t_block	*buff;

	buff = line->head;
	while (buff != NULL)
	{
		attribute_token()
		if (check_symbol())
			return (FALSE);
		if (!handle_quote(buff))
			return (FALSE);
		buff = buff->next;
	}
}

void	attribute_token(t_line *line, char *block_content, t_block *block)
{
	t_bool	already_attributed;
	t_token	previous;

	already_attributed = atrribut_symbol(block);
	block->expand = check_dollar_in_block(block);
	if (!already_attributed)
	{
		previous = get_previous_token(block);
		if (previous == UNDEF)
			block->token = CMD;
		if (previous == CMD)
			block->token = ARG;
		if (previous == HERE_DOC)
			block->token = DELIMITER;
		if (previous == PIPE)
			block->token = CMD;
		if (previous == RED_IN || previous == RED_OUT_TRUNC || previous == RED_OUT_APPEND)
			block->token = FILES;
		
	}
	//dans le premier parsing faudrait aussi separer sur les symbols :)
	//genre les espaces et les symbol les mettre dans des blocs seuls
}

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

t_bool	attribute_symbol(t_block *block)
{
	if (ft_strncmp(block->word, "<", 2) == 0)
	{
		buff->token = RED_IN;
		return (TRUE);
	}
	else if (ft_strncmp(block->word, ">", 2) == 0)
	{
		buff->token = RED_OUT_TRUNC;
		return (TRUE);
	}
	else if (ft_strncmp(block->word, ">>", 3) == 0)
	{
		buff->token = RED_OUT_APPEND;
		return (TRUE);
	}
	else if (ft_strncmp(block->word, "<<", 3) == 0)
	{
		buff->token = HERE_DOC
		return (TRUE);
	}
	else if (ft_strncmp(block->word, "|", 2) == 0)
	{
		buff->token = PIPE;
		return (TRUE);
	}
	return (FALSE);
}

t_token	get_previous_token(t_block *block)
{
	if (block->prev)
		return (block->prev->token);
	return (UNDEF);
}
