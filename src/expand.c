/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 18:15:06 by odessein          #+#    #+#             */
/*   Updated: 2022/08/16 17:30:50 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//Geree le cas particulier du "" et '' <- a remplacer par zeroooooo 0
//$?

void	expand(t_line *line)
{
	t_block	*buff;

	buff = line->head;
	while (buff)
	{
		if (buff->token == CMD_ARG || buff->token == FILES)
			check_dollar_in_block(buff);
		buff = buff->next;
	}
}

int	size_doll_val(char *word, int *i)
{
	int	size;

	size = 1;
	while (word[*i] && word[*i] != ' ' && word[*i] != '\'' && word[*i] != '\"'
			&& word[*i] != '$')
	{
		size++;
		(*i)++;
	}
	return (size);
}

void	fill_key_arr(t_block *block, char **key_arr)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (block->word[i])
	{
		if (block->word[i] == '\'')
			while (block->word[i] != '\'')
				i++;
		if (!block->word[i])
			return ;
		if (block->word[i + 1] && block->word[i] == '$' && block->word[i + 1] != '$' && block->word[i + 1] != ' ' && block->word[i + 1] != '\'' && block->word[i + 1] != '\"')
		{
			key_arr[j] = ft_substr(block->word, i, size_doll_val(block->word, &(i + 1)));
			j++;
		}
		else
			i++;
	}
	key_arr[j] = NULL;
	return (dollar);
}

void	check_dollar_in_block(t_block *block)
{
	char	**key_arr;
	int		size_double_arr;

	size_double_arr = get_nb_of_dollar(block);
	key_arr = (char **) malloc(sizeof(*key_arr) * (size_double_arr + 1));
	if (!key_arr)
		free_exit();
	add_to_gc(DOUBLE, key_arr, get_gc());
	fill_key_arr(block, key_arr);
}
// a tester
int	get_nb_of_dollar(t_block *block)
{
	int		i;
	int		dollar;

	i = 0;
	dollar = 0;
	while (block->word[i])
	{
		if (block->word[i] == '\'')
			while (block->word[i] != '\'')
				i++;
		if (!block->word[i])
			return ;
		if (block->word[i + 1] && block->word[i] == '$' && block->word[i + 1] != '$' && block->word[i + 1] != ' ' && block->word[i + 1] != '\'' && block->word[i + 1] != '\"')
			dollar++;
		i++;
	}
	return (dollar);
}


//Function qui va checker dans l'env si la key exist
//Si la key n'existe pas on suprimme le block; 
//(si il n'y a pas de quote sinon le block est egal a 0);

//Else if la value existe on remplace le block

