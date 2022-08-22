/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 17:58:23 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/20 17:06:51 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

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

t_bool	check_char(char c)
{
	if (c > -1 && c < 48)
		return (FALSE);
	return (TRUE);
}

int	size_doll_val(char *word,int i)
{
	int	size;

	size = 0;
	i++;
	if ((word[i] >= '0' && word[i] <= '9') || word[i] == '?')
	{
		size++;
		return (size);
	}
	while (word[i] && check_char(word[i]))
	{
		size++;
		i++;
	}
	return (size);
}


void	advance_if_in_s_quote(t_bool *d_quote, char *word, int *i)
{
	char	c;

	c = word[*i];
	if (!(*d_quote) && c == '\"')
		*d_quote = TRUE;
	else if (*d_quote && c == '\"')
		*d_quote = FALSE;
	if (!(*d_quote) && c == '\'')
	{
		(*i) += 1;
		while (word[*i] && word[*i] != '\'')
			(*i) += 1;
	}
}

void	fill_key_arr(t_block *block, char **key_arr, int *indexes) { int	i;
	int	j;
	int	size_val;

	i = 0;
	j = 0;
	while (indexes[i] != -1)
	{
		size_val = size_doll_val(block->word, indexes[i]);
		key_arr[j] = ft_substr(block->word, indexes[i] + 1, size_val);
		//ft_printf(0, "key = %s\n", key_arr[j]);
		//KEYS OK
		j++;
		i++;
	}
	key_arr[j] = NULL;
}

int	*get_indexes_expandables(t_block *block, int dollar)
{
	int		i;
	int		j;
	int		*indexes;
	t_bool	d_quote;

	i = 0;
	j = 0;
	indexes = malloc(sizeof(int) * (dollar + 1));
	if (!indexes)
		return (NULL);
	d_quote = FALSE;
	while (block->word[i])
	{
		advance_if_in_s_quote(&d_quote, block->word, &i);
		if (block->word[i + 1] && block->word[i] == '$'
				&& check_char(block->word[i + 1]))
			indexes[j++] = i;
		i++;
	}
	indexes[j] = -1;
	return (indexes);
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
		advance_if_in_s_quote(&d_quote, block->word, &i);
		if (block->word[i] == '$' && block->word[i + 1]
				&& check_char(block->word[i + 1]))
			dollar++;
		i++;
	}
	return (dollar);
}

void	fill_val_arr(char **key_arr, char **val_arr, t_dict *dict)
{
	int		i;
	char	*value;

	i = 0;
	while (key_arr[i])
	{
		if (key_arr[i][0] == '?' && !key_arr[i][1])
		{
			value = get_exit_status();
			val_arr[i++] = value;
			continue ;
		}
		value = dict_get_value(dict, key_arr[i]);
		if (!value)
			val_arr[i] = ft_strdup("");
		else
		{
			value = ft_strdup(value);
			if (!value)
				free_exit();
			val_arr[i] = value;
			////ft_printf("value = %s\n", val_arr[i]);
			//values OK
		}
		i++;
	}
	val_arr[i] = NULL;
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

t_bool	handle_simple_word(char *new_word, char *word, int *indexes)
{
	if (indexes[0] == -1)
	{
		ft_strlcpy(new_word, word, ft_strlen(word) + 1);
		return (TRUE);
	}
	return (FALSE);
}

t_bool	char_is_num(char c, int *i)
{
	if ((c >= '0' && c <= '9') || c == '?')
	{
		(*i)++;
		return (TRUE);
	}
	return (FALSE);
}

void	init_i_j_k(int *i, int *j, int *k)
{
	*i = 0;
	*j = 0;
	*k = 0;
}

void	fill_new_word(char *new_word, char *word, char **val_arr, int *indexes)
{
	int		i;
	int		j;
	int		k;

	init_i_j_k(&i, &j, &k);
	if (handle_simple_word(new_word, word, indexes))
		return ;
	while (word[i])
	{
		while (i != indexes[k] && word[i])
			new_word[j++] = word[i++];
		if (word[i] && i == indexes[k])
		{
			i++;
			while (word[i] && check_char(word[i]))
			{
				if (word[i - 1] && word[i - 1] == '$' && char_is_num(word[i], &i) && word[i] != '?')
					break ;
				i++;
			}
			replace_key(new_word, &j, &val_arr[k++]);
		}
	}
	new_word[j] = '\0';
}

void	expand_block(t_block *block, char **key_arr, char **val_arr, int *indexes)
{
	int		size;
	char	*new_word;

	size = ft_strlen(block->word) + total_char_to_add(val_arr) - total_char_to_remove(key_arr);
	////ft_printf("SIZE = %i\n", size);
	//SIZE OK
	new_word = (char *) malloc(sizeof(*new_word) * (size + 1));
	if (!new_word)
		free_exit();
	fill_new_word(new_word, block->word, val_arr, indexes);
	free(block->word);
	block->word = new_word;
	//ft_printf(0, "From l:267 of expand.c\n last value of block : %s\n", block->word);
}

void	handle_dollar_in_block(t_block *block, t_dict *dict)
{
	char	**key_arr;
	char	**val_arr;
	int		size_double_arr;
	int		*indexes;

	size_double_arr = get_nb_of_dollar(block);
	//ft_printf(0, "***nb expandable = %i***\n", size_double_arr);
	indexes = get_indexes_expandables(block, size_double_arr);
	if (!indexes)
		free_exit();
	add_to_gc(SIMPLE, indexes, get_gc());
	key_arr = (char **) malloc(sizeof(*key_arr) * (size_double_arr + 1));
	if (!key_arr)
		free_exit();
	add_to_gc(DOUBLE, key_arr, get_gc());
	fill_key_arr(block, key_arr, indexes);
	val_arr = (char **) malloc(sizeof(*val_arr) * (size_double_arr + 1));
	if (!val_arr)
		free_exit();
	add_to_gc(DOUBLE, val_arr, get_gc());
	fill_val_arr(key_arr, val_arr, dict);
	expand_block(block, key_arr, val_arr, indexes);
}

void	expand(t_line *line, t_dict *env)
{
	t_block	*buff;

	buff = line->head;
	while (buff)
	{
		if (buff->token == CMD_ARG || buff->token == FILES)
			handle_dollar_in_block(buff, env);
		buff = buff->next;
	}
}

void	browse_ast_apply_expand(t_leaf *leaf, t_dict *env)
{
	t_line *line;

	if (!leaf || !env)
		return ;
	if (leaf->type == CMD)
	{
		line = leaf->content;
		expand(line, env);
	}
	if (leaf->left != NULL)
		browse_ast_apply_expand(leaf->left, env);
	else
		return ;
	if (leaf->right != NULL)
		browse_ast_apply_expand(leaf->right, env);
	else
		return ;
}
