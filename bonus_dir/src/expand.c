/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 18:15:06 by odessein          #+#    #+#             */
/*   Updated: 2022/08/16 21:17:22 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//Gerer le cas particulier du "" et '' <- a remplacer par zeroooooo 0
//$?
//expansion dans le here_doc, ne pas gerer, ca change en fonction du nb de dollars, paire ou impaire, ne pas s'embeter avec ca

void	expand(t_line *line, t_dict *dict)
{
	t_block	*buff;

	buff = line->head;
	while (buff)
	{
		if (buff->token == CMD_ARG || buff->token == FILES)
			check_dollar_in_block(buff, dict);
		buff = buff->next;
	}
}

int	size_doll_val(char *word, int *i)
{
	int	size;

	size = 0;
	(*i)++;
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
		{
			i++;
			while (block->word[i] && block->word[i] != '\'')
				i++;
			if (!block->word[i])
				break ;
		}
		if (block->word[i + 1] && block->word[i] == '$' 
			&& block->word[i + 1] != '$' && block->word[i + 1] != ' ' 
			&& block->word[i + 1] != '\'' && block->word[i + 1] != '\"')
		{
			key_arr[j] = ft_substr(block->word, i + 1, size_doll_val(block->word, &i));
			ft_printf("\n%s %i\n", key_arr[j], j);
			j++;
		}
		else
			i++;
	}
	key_arr[j] = NULL;
}

void	fill_val_arr(char **key_arr, char **val_arr, dict *dict)
{
	int		i;
	char	*value;

	i = 0;
	while (key_arr[i])
	{
		value = ft_strdup(dict_get_value(dict, key_arr[i]);
		if (!value)
			free_exit();
		val_arr[i] = value;
		i++;
	}
	val_arr[i] = NULL;
}

void	check_dollar_in_block(t_block *block, t_dict *dict)
{
	char	**key_arr;
	char	**val_arr;
	int		size_double_arr;

	size_double_arr = get_nb_of_dollar(block);
	key_arr = (char **) malloc(sizeof(*key_arr) * (size_double_arr + 1));
	if (!key_arr)
		free_exit();
	add_to_gc(DOUBLE, key_arr, get_gc());
	fill_key_arr(block, key_arr);
	val_arr = (char **) malloc(sizeof(*val_arr) * (size_double_arr + 1));
	if (!val_arr)
		free_exit();
	add_to_gc(DOUBLE, key_arr, get_gc());
	fill_val_arr(key_arr, val_arr, dict);
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
		{
			i++;
			while (block->word[i] && block->word[i] != '\'')
				i++;
		}
		if (block->word[i + 1] && block->word[i] == '$' 
				&& block->word[i + 1] != '$' && block->word[i + 1] != ' ' 
				&& block->word[i + 1] != '\'' && block->word[i + 1] != '\"')
			dollar++;
		i++;
	}
	return (dollar);
}

//en meme temps que la creation de key_arr, on peut creer un tableau d'int qui prendra les indices de debut de chaque variable a expand
//search for value in our dictionary
//get the value, strlen, and put it in our string

//Function qui va checker dans l'env si la key exist
//Si la key n'existe pas on suprimme le block; 
//(si il n'y a pas de quote sinon le block est egal a 0);

//Else if la value existe on remplace le block

