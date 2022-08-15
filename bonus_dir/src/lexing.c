/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <mbelrhaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 14:15:02 by odessein          #+#    #+#             */
/*   Updated: 2022/08/12 20:15:22 by odessein         ###   ########.fr       */
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
			quote = DOUBLE_QUOTE;
		i++;
	}
	if (quote != NO)
		return (FALSE);
	return (TRUE);
}

static t_bool	not_in_quote(char *line, int i)
{
	static t_quote	quote;

	if (line[i] == '\"' && quote == DOUBLE_QUOTE)
		quote = NO;
	else if (line[i] == '\'' && quote == SINGLE)
		quote = NO;
	else if (line[i] == '\'' && quote == NO)
		quote = SINGLE;
	else if (line [i] == '\"' && quote == NO)
		quote = DOUBLE_QUOTE;
	if (quote == NO)
		return (TRUE);
	return (FALSE);
}

t_bool	fill_word(int *size, t_line *lst, char *line, int i)
{
	char	*word;
	t_block	*buff;
	int		j;

	j = *size - 1;
	if (*size == 0)
		return (TRUE);
	word = malloc(sizeof(*word) * (*size + 1));
	if (!word)
		return (FALSE);
	while (j >= 0)
		word[j--] = line[i--];
	word[*size] = '\0';
	buff = new_block(word);
	if (!buff)
		return (FALSE);
	line_lst_addback(lst, buff);
	*size = 0;
	return (TRUE);
}

t_bool	handle_pipe(char *line, int *i, int *size, t_line *lst)
{
	if (*i >= 1 && line[*i - 1] && (ft_isalnum(line[*i - 1]) 
			|| line[*i - 1] == '\"' || line[*i - 1] == '\''))
		if (!fill_word(size, lst, line, *i - 1))
			return (FALSE);
	(*size) += 1;
	if (!fill_word(size, lst, line, *i))
		return (FALSE);
	return (TRUE);
}

t_bool	handle_red_o(char *line, int *i, int *size, t_line *lst)
{
	if (line[*i - 1] && (ft_isalnum(line[*i - 1]) || line[*i - 1] == '\"'
			|| line[*i - 1] == '\''))
		if (!fill_word(size, lst, line, *i - 1))
			return (FALSE);
	if (line[*i + 1] && line[*i + 1] == '>')
	{
		(*i)++;
		(*size) += 2;
		if (!fill_word(size, lst, line, *i))
			return (FALSE);
	}
	else
	{
		(*size) += 1;
		if (!fill_word(size, lst, line, *i))
			return (FALSE);
	}
	return (TRUE);
}

t_bool	handle_red_i(char *line, int *i, int *size, t_line *lst)
{
	if (line[*i - 1] && (ft_isalnum(line[*i - 1]) || line[*i - 1] == '\"'
			|| line[*i - 1] == '\''))
		if (!fill_word(size, lst, line, *i - 1))
			return (FALSE);
	if (line[*i + 1] && line[*i + 1] == '<')
	{
		(*i)++;
		(*size) += 2;
		if (!fill_word(size, lst, line, *i))
			return (FALSE);
	}
	else
	{
		(*size) += 1;
		if (!fill_word(size, lst, line, *i))
			return (FALSE);
	}
	return (TRUE);
}
//Not possible to have single &
//Taking care ()
//Check || && ();
t_bool	analyse_symbol(char *line, int *i, int *size, t_line *lst)
{
	if (line[*i] == '|')
	{
		if (!handle_pipe(line, i, size, lst))
			return (FALSE);
	}
	else if (line[*i] == '>')
	{
		if (!handle_red_o(line, i, size, lst))
			return (FALSE);
	}
	else if (line[*i] == '<')
	{
		if (!handle_red_i(line, i, size, lst))
			return (FALSE);
	}
	else
		(*size)++;
	return (TRUE);
}

static t_bool	handle_space(char *line, int *i, int *size, t_line *lst)
{
	 if (*i > 0 && (ft_isalnum(line[*i - 1])
			|| line[*i - 1] == '\"' || line[*i - 1] == '\''))
	{
		if (!fill_word(size, lst, line, *i - 1))
			return (FALSE);
	}
	*size = 0;
	return (TRUE);
}

t_bool	analyse_word(char *line, int *i, int *size_word, t_line *lst)
{
	if (line[*i] == ' ')
	{
		if (!handle_space(line, i, size_word, lst))
			return (FALSE);
	}
	else if (!analyse_symbol(line, i, size_word, lst))
		return (FALSE);
	return (TRUE);
}

t_bool	handle_line(char *line, t_line *lst)
{
	int		i;
	int		size_word;
	t_bool	not_quote;

	i = 0;
	size_word = 0;
	while (line[i] != 0)
	{
		not_quote = not_in_quote(line, i);
		if (!not_quote)
			size_word++;
		if (not_quote && (line[i] == '\'' || line[i] == '\"')
			&& size_word++ && i++)
			continue ;
		if (not_quote && line[i] != 0)
			if (!analyse_word(line, &i, &size_word, lst))
				return (FALSE);
		if (line[i] != 0)
		       	i++;
	}
	if (!fill_word(&size_word, lst, line, i - 1))
		return (FALSE);
	return (TRUE);
}

t_bool	fill_line_lst(t_line *block_lst, char *line)
{
	int		i;
	t_block		*buff;

	i = 0;
	if (!check_lines_quotes(line))
		return (FALSE);
	handle_line(line, block_lst);
	add_to_gc(LINE, block_lst, get_gc());
	buff = block_lst->head;
	while (buff)
	{
		//ft_printf("|%s|\n", buff->word);
		buff = buff->next;
	}
	//If quote on compte rien / sinon
	return (TRUE);
}
