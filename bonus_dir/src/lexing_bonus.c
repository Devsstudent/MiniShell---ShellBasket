/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <mbelrhaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 14:15:02 by odessein          #+#    #+#             */
/*   Updated: 2022/08/16 15:12:53 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//Fill the line list; block by block
//First check the line quotes

t_bool	is_symbol(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '&')
		return (FALSE);
	return (TRUE);
}

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

static t_bool	check_lines_parentheses(char *line)
{
	int	i;
	int	not_quote;
	int	close;
	int	open;

	i = 0;
	close = 0;
	open = 0;
	while (line[i])
	{
		not_quote = not_in_quote(line, i);
		if (not_quote && line[i] == '(')
			open++;
		else if (not_quote && line[i] == ')')
			close++;
		i++;
	}
	if (close != open)
		return (FALSE);
	return (TRUE);
}

void	fill_word(int *size, t_line *lst, char *line, int i)
{
	char	*word;
	t_block	*buff;
	int		j;

	j = *size - 1;
	if (*size == 0)
		return ;
	word = malloc(sizeof(*word) * (*size + 1));
	if (!word)
		free_exit();
	while (j >= 0)
		word[j--] = line[i--];
	word[*size] = '\0';
	buff = new_block(word);
	line_lst_addback(lst, buff);
	*size = 0;
}

void	handle_pipe(char *line, int *i, int *size, t_line *lst)
{
	if (*i >= 1 && line[*i - 1])
		fill_word(size, lst, line, *i - 1);
	if (line[*i + 1] && line[*i + 1] == '|')
	{
		(*i)++;
		(*size) += 2;
		fill_word(size, lst, line, *i);
	}
	else
	{
		(*size) += 1;
		fill_word(size, lst, line, *i);
	}
}

void	handle_red_o(char *line, int *i, int *size, t_line *lst)
{
	if (*i > 1 && line[*i - 1])
		fill_word(size, lst, line, *i - 1);
	if (line[*i + 1] && line[*i + 1] == '>')
	{
		(*i)++;
		(*size) += 2;
		fill_word(size, lst, line, *i);
	}
	else
	{
		(*size) += 1;
		fill_word(size, lst, line, *i);
	}
}

void	handle_and(char *line, int *i, int *size, t_line *lst)
{
	if (*i > 1 && line[*i - 1])
		fill_word(size, lst, line, *i - 1);
	if (line[*i + 1] && line[*i + 1] == '&')
	{
		(*i)++;
		(*size) += 2;
		fill_word(size, lst, line, *i);
	}
}

void	handle_par(char *line, int *i, int *size, t_line *lst)
{
	if (*i > 1 && line[*i - 1])
		fill_word(size, lst, line, *i - 1);
	if (line[*i] == ')' || line[*i] == '(')
	{
		(*size) += 1;
		fill_word(size, lst, line, *i);
	}
}

void	handle_red_i(char *line, int *i, int *size, t_line *lst)
{
	if (*i > 1 && line[*i - 1])
		fill_word(size, lst, line, *i - 1);
	if (line[*i + 1] && line[*i + 1] == '<')
	{
		(*i)++;
		(*size) += 2;
		fill_word(size, lst, line, *i);
	}
	else
	{
		(*size) += 1;
		fill_word(size, lst, line, *i);
	}
}

void	analyse_symbol(char *line, int *i, int *size, t_line *lst)
{
	if (line[*i] == '|')
		handle_pipe(line, i, size, lst);
	else if (line[*i] == '>')
		handle_red_o(line, i, size, lst);
	else if (line[*i] == '<')
		handle_red_i(line, i, size, lst);
	else if (line[*i] == '&')
		handle_and(line, i, size, lst);
	else if (line[*i] == '(' || line[*i] == ')')
		handle_par(line, i, size, lst);
	else
		(*size)++;
}

static void	handle_space(char *line, int *i, int *size, t_line *lst)
{
	if (*i > 0 && (line[*i - 1]))
		fill_word(size, lst, line, *i - 1);
	*size = 0;
} 

void	analyse_word(char *line, int *i, int *size_word, t_line *lst)
{
	if (line[*i] == ' ')
		handle_space(line, i, size_word, lst);
	else
		analyse_symbol(line, i, size_word, lst);
}

void	handle_line(char *line, t_line *lst)
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
			analyse_word(line, &i, &size_word, lst);
		if (line[i] != 0)
		       	i++;
	}
	fill_word(&size_word, lst, line, i - 1);
}

t_line	*fill_line_lst(char *line)
{
	t_line	*block_lst;
	
	block_lst = (t_line *) malloc(sizeof(t_line));
	if (!block_lst || !check_lines_quotes(line) 
		|| !check_lines_parentheses(line))
	{
		//mettre message d'erreur
		free_exit();
	}
	block_lst->head = NULL;
	add_to_gc(LINE, block_lst, get_gc());
	handle_line(line, block_lst);
	return (block_lst);
}
