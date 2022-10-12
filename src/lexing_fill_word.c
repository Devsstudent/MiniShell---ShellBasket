/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_fill_word.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 20:06:59 by odessein          #+#    #+#             */
/*   Updated: 2022/10/11 15:09:09 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

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
