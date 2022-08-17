/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 18:15:06 by odessein          #+#    #+#             */
/*   Updated: 2022/08/17 17:26:58 by odessein         ###   ########.fr       */
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
	int		buff;
	t_bool	d_quote;

	i = 0;
	j = 0;
	d_quote = FALSE;
	while (block->word[i])
	{
		if (d_quote && block->word[i] == '\"')
			d_quote = FALSE;
		else if (!d_quote && block->word[i] == '\"')
			d_quote = TRUE;
		if (!d_quote && block->word[i] == '\'')
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
			buff = i + 1;
			key_arr[j] = ft_substr(block->word, buff, size_doll_val(block->word, &i));
			ft_printf("\nkey arr : %s", key_arr[j]);
			j++;
		}
		else
			i++;
	}
	key_arr[j] = NULL;
}

void	fill_val_arr(char **key_arr, char **val_arr, t_dict *dict)
{
	int		i;
	char	*value;

	i = 0;
	while (key_arr[i])
	{
		value = dict_get_value(dict, key_arr[i]);
		if (!value)
			val_arr[i] = ft_strdup("");
		else
		{
			value = ft_strdup(value);
			if (!value)
				free_exit();
			val_arr[i] = value;
		}
		i++;
	}
	val_arr[i] = NULL;
}

int	total_char_to_remove(char **key_arr)
{
	int	size;
	int	i;

	i = 0;
	size = 0;
	while (key_arr[i])
	{
		// + 1 for $
		size += ft_strlen(key_arr[i]) + 1;
		i++;
	}
	return (size);
}

int	total_char_to_add(char **val_arr)
{
	int	size;
	int	i;

	i = 0;
	size = 0;
	while (val_arr[i])
	{
		size += ft_strlen(val_arr[i]);
		i++;
	}
	return (size);
}

void	replace_key(char *new_word, int *j, char **val_arr)
{
	int	i;

	i = 0;
	if (!(*val_arr))
		return ;
	while ((*val_arr)[i])
	{
		new_word[*j] = (*val_arr)[i];
		i++;
		(*j)++;
	}
}

void	fill_new_word(char *new_word, char *word, char **val_arr)
{
	int		i;
	int		j;
	int		k;
	t_bool	quote;
	t_bool	d_quote;

	i = 0;
	j = 0;
	k = 0;
	quote = FALSE;
	d_quote = FALSE;
	while (word[i])
	{
		if (word[i] == '\"' && !d_quote && !quote)
			d_quote = TRUE;
		else if (word[i] == '\"' && d_quote && !quote)
			d_quote = FALSE;
		else if (word[i] == '\'' && !quote && !d_quote)
			quote = TRUE;
		else if (word[i] == '\'' && quote && !d_quote)
			quote = FALSE;
		if (word[i] == '$' && !quote && (word[i + 1] && word[i + 1] != '$'
			&& word[i + 1] != ' ' && word[i + 1] != '\"' && word[i + 1] != '\''))
		{
			i++;
			while ((word[i] && word[i] != '$' && word[i] != ' ' && word[i] != '\"' && word[i] != '\''))
				i++;
			replace_key(new_word, &j, &val_arr[k++]);
		}
		if  (!quote && word[i] && word[i + 1] && (word[i] == '$' || word[i] == '\'') && word[i + 1] != ' ' && word[i + 1] != '\"' && word[i + 1] != '\'' && word[i + 1] != '$')
			continue ;
		ft_printf("\nnot $ : %c", word[i]);
		new_word[j] = word[i];
		if (!word[i])
			return ;
		i++;
		j++;
	}
}

void	expand_block(t_block *block, char **key_arr, char **val_arr)
{
	//function total_char_to_remove key
	//funtion	total_char_to_add val all_char_statying
	//function all_char_staying
	int		size;
	char	*new_word;

	size = ft_strlen(block->word) + total_char_to_add(val_arr) - total_char_to_remove(key_arr);
	new_word = (char *)malloc(sizeof(*new_word) * (size + 1));
	if (!new_word)
		free_exit();
	fill_new_word(new_word, block->word, val_arr);
	//ft_printf("val_arr[0] = %s\n", val_arr[0]);
	ft_printf("new word: %s\n", new_word);
	add_to_gc(SIMPLE, block->word, get_gc());
	block->word = new_word;
	//browse the string and replace
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
//	ft_printf("key_arr[0] = %s\n", key_arr[0]);
	val_arr = (char **) malloc(sizeof(*val_arr) * (size_double_arr + 1));
	if (!val_arr)
		free_exit();
	add_to_gc(DOUBLE, val_arr, get_gc());
	fill_val_arr(key_arr, val_arr, dict);
	//ft_printf("TETSTSET : %s\n", val_arr[0]);
	expand_block(block, key_arr, val_arr);
}

int	get_nb_of_dollar(t_block *block)
{
	int		i;
	int		dollar;
	t_bool	d_quote;

	i = 0;
	dollar = 0;
	d_quote = FALSE;
	while (block->word[i])
	{
		if (!d_quote && block->word[i] == '\"')
			d_quote = TRUE;
		else if (d_quote && block->word[i] == '\"')
			d_quote = FALSE;
		if (!d_quote && block->word[i] == '\'')
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
	ft_printf("\ndollar : %i", dollar);
	return (dollar);
}

//search for value in our dictionary
//get the value, strlen, and put it in our string

//Function qui va checker dans l'env si la key exist
//Si la key n'existe pas on suprimme le block; 
//(si il n'y a pas de quote sinon le block est egal a 0);

//Else if la value existe on remplace le block
