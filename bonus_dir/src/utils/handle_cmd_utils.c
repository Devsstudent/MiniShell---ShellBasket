/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 20:08:59 by odessein          #+#    #+#             */
/*   Updated: 2022/09/08 16:50:36 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

size_t	get_nb_cmd_arg(t_line *sub)
{
	t_block	*buff;
	size_t	size;

	buff = sub->head;
	size = 0;
	while (buff)
	{
		if (buff->token == CMD_ARG)
			get_size_word_in_word(buff->word, &size);
		buff = buff->next;
	}
	return (size);
}

void	get_size_word_in_word(char *word, size_t *size)
{
	int		i;
	t_bool	quote;
	t_bool	d_quote;

	i = 0;
	quote = FALSE;
	d_quote = FALSE;
	while (word[i])
	{
		if (word[i] == '\"' && !d_quote)
			d_quote = TRUE;
		else if (word[i] == '\"' && d_quote)
			d_quote = FALSE;
		if (word[i] == '\'' && !quote)
			quote = TRUE;
		else if (word[i] == '\'' && quote)
			quote = FALSE;
		if (i > 0 && word[i - 1] != ' ' && word[i] == ' '
			&& word[i + 1] && word[i + 1] != ' ' && !quote && !d_quote)
			(*size)++;
		i++;
	}
	(*size)++;
}

char	**get_cmd_arg(t_line *sub)
{
	t_block		*buff;
	char		**argv;
	int			i;

	i = 0;
	argv = (char **) malloc(sizeof(*argv) * (get_nb_cmd_arg(sub) + 1));
	add_to_gc(DOUBLE, argv, get_gc());
	buff = sub->head;
	while (buff)
	{
		if (buff->token == CMD_ARG)
			loop_get_arg(buff->word, argv, &i);
		buff = buff->next;
	}
	argv[i] = 0;
	return (argv);
}

void	init_loop_get_arg(int *j, int *last, t_bool *quote, t_bool *d_quote)
{
	*j = 0;
	*last = 0;
	*quote = FALSE;
	*d_quote = FALSE;
}

t_bool	check_quote(t_bool *d_quote, t_bool *quote, char word)
{
	if (word == '\"' && !(*d_quote))
		*d_quote = TRUE;
	else if (word == '\"' && *d_quote)
		*d_quote = FALSE;
	if (word == '\'' && !(*quote))
		*quote = TRUE;
	else if (word == '\'' && *quote)
		*quote = FALSE;
	return (TRUE);
}
