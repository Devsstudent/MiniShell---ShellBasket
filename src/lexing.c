/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 14:15:02 by odessein          #+#    #+#             */
/*   Updated: 2022/08/01 19:14:24 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Fill the line list; block by block
//First check the line quotes

static t_bool	check_lines_quotes(char *line)
{
	int		i;
	t_quote	quote;

	i = 0;
	quote = NO;
	while (line[i] != 0)
	{
		if (line[i] == '\"' && quote == DOUBLE_QUOTE)
			quote = NO;
		else if (line[i] == '\'' && quote == SINGLE)
			quote = NO;
		else if (line[i] == '\'' && quote == NO)
			quote = SINGLE;
		else if (line[i] == '\"' && quote == NO)
			quote = DOUBLE;
		i++;
	}
	if (quote != NO)
		return (FALSE);
	return (TRUE);
}

static t_bool	not_in_quote(char *line, int *i)
{
	static t_quote	quote;
	
	if (line[*i] == '\"' && quote == DOUBLE_QUOTE)
		quote = NO;
	else if (line[*i] == '\'' && quote == SINGLE)
		quote = NO;
	else if (line[*i] == '\'' && quote == NO)
		quote = SINGLE;
	else if (line [*i] == '\"' && quote == NO)
		quote = DOUBLE;
	if (quote == NO)
		return (TRUE);
	return (FALSE);
}

static int	*int_dup(int size)
{
	int	*dup;

	dup = (int *) malloc(sizeof(*dup));
	if (!dup)
		return (NULL);
	*dup = size;
	return (dup);
}

t_bool	get_size_word(int *size, t_list **len_lst, char c)
{
	int		*cp_size;
	int		tmp;
	t_list	*new;

	cp_size = int_dup(*size);
	new = ft_lstnew(cp_size);
	if (!new)
		return (FALSE);
	ft_lstadd_back(len_lst, new);
	*size = 0;
	if (c == '|' || c == '>' || c == '<')
	{ 
		tmp = 1;
		if (!get_size_word(&tmp, len_lst, 0))
			return (FALSE);
	}
	if (c == 'H' || c == 'R')
	{
		tmp = 2;
		if (!get_size_word(&tmp, len_lst, 0))
			return (FALSE);
	}
	return (TRUE);
}

t_bool	split(char *line, int *i, int *size, t_list **len_lst)
{
	if (line[*i] == ' ' || line[*i] == '|')
		if (!get_size_word(size, len_lst, line[*i]))
			return (FALSE);
	if (line[*i] == '>' || line[*i] == '<')
	{
		if (line[*i + 1] == line[*i])
		{
			(*i)++;
			if (line[*i] == '<')
				if (!get_size_word(size, len_lst, 'H'))
					return (FALSE);
			if (line[*i] == '>')
				if (!get_size_word(size, len_lst, 'R'))
					return (FALSE);
		}
		else 
			if (!get_size_word(size, len_lst, line[*i]))
				return (FALSE);
	}
	return(TRUE);
}

t_bool	get_size_line_words(char *line, t_list **len_lst)
{
	int		i;
	int		size_word;
	t_bool	not_quote;

	i = 0;
	size_word = 0;
	while (line[i] != 0)
	{
		not_quote = not_in_quote(line, &i);
		if (not_quote && (line[i] == '\'' || line[i] == '\"'))
		{
			size_word++;
			i++;
			continue ;
		}
		if (not_quote && line[i] != 0)
		{
			if (line[i] == ' ' && i - 1 >= 0 && line[i - 1] == ' ')
			{
				i++;
				continue;
			}
			printf("size_word = %i\n", size_word);
			if (!split(line, &i, &size_word, len_lst))
				return (FALSE);
		}
		if (line[i] != 0)
		{
			size_word++;
			i++;
		}
	}
	if (!get_size_word(&size_word, len_lst, 0))
		return (FALSE);
	return (TRUE);
}

t_bool	fill_line_lst(t_line *block_lst, char *line)
{
	int		i;
	t_list	*len_lst;

	i = 0;
	len_lst = NULL;
	if (!check_lines_quotes(line))
		return (FALSE);
	get_size_line_words(line, &len_lst);
	t_list	*buff;

	buff = len_lst;
	while (buff != NULL)
	{
		ft_printf("ici : %i\n", *(int *)(buff->content));
		buff = buff->next;
	}
	//If quote on compte rien / sinon 
	return (TRUE);
}
