/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 13:03:13 by odessein          #+#    #+#             */
/*   Updated: 2022/07/31 20:19:22 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	tokenisation(t_line *line, t_gc *gc)
{
	t_block	*buff;

	buff = line->head;
	while (buff != NULL)
	{
		handle_quote()
		attribute_token()
		if (check_symbol())
			return (FALSE);
		buff = buff->next;
	}
}

void	handle_quote(t_block *block)
{
	//need the size to malloc the new word
	//str_copy modifier qui va copier tous sauf les quotes out quoi
	int		i;
	int		j;
	char	*new_word;
	t_type	quote;
	t_bool	on;
	int		size;

	while (block->word[i])
	{
		size++;
		if (block->word[i] == "\"" && !on)
		{
			size--;
			on = TRUE;
			quote = DOUBLE;
		}
		else if (block->word[i] == "\'" && !on)
		{
			size--;
			on = TRUE;
			quote = SIMPLE;
		}
		if (quote == SIMPLE && on)
			size++ while not find another \'
		else if (quote == DOUBLE && on)
			size++ while not find another \"
		//faire des check de size
		//function qui prend un char * et qui retourne une size sans les quotes "inutile"
		//ensuite function qui copy dans une char * le previous char * sans les quotes "inutile"
		
	}
	get_next_quote_addr() // or index
	
	//trouver la premiere quote
	//run juste qu'a la sortie 
	//Remove les deux quotes et continuee
	//tt le temps ca
}

int	size_new_word_quote(char *word)
{
	t_bool	on;
	t_type	quote;
	int		size;

	size = 0;
	while (*word)
	{
		if (*word == "\'")
		{
			word++;
			size += size_till_next_char(word, '\'');
		}
		else if (*word == "\"")
		{
			word++;
			size += size_till_next_char(word, '\"');
		}
		size++;
		word++;
	}
	ft_printf("%i", size);
	return (size);
}

int		size_till_next_char(char *word, char c)
{
	int	size;

	size = 0;
	while (*word != c)
	{
		word++;
		size++;
	}
	return (size);
}

void	attribute_token(t_line *line, char *block_content, t_block *block)
{
	atrribut_symbol(block);
}

void	attribute_symbol(t_block *block)
{
	if (ft_strncmp(block->word, "<", 2) == 0)
		buff->token = RED_IN;
	else if (ft_strncmp(block->word, ">", 2) == 0)
		buff->token = RED_OUT_TRUNC;
	else if (ft_strncmp(block->word, ">>", 3) == 0)
		buff->token = RED_OUT_APPEND;
	else if (ft_strncmp(block->word, "<<", 3) == 0)
		buff->token = HERE_DOC
	else if (ft_strncmp(block->word, "|", 2) == 0)
		buff->token = PIPE;
}
