/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 18:15:06 by odessein          #+#    #+#             */
/*   Updated: 2022/08/03 18:15:13 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

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
