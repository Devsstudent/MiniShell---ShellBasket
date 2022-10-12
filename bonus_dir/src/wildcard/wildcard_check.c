/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 20:44:27 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 20:45:40 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	check_special_match(char *filename, char *word)
{
	if ((filename[0] == '.' && word[0] != '.')
		|| (filename[ft_strlen(filename) - 1] == '/'
			&& word[ft_strlen(word) - 1] != '/'))
		return (FALSE);
	return (TRUE);
}

t_bool	check_end_word_match(char *filename, char *word, char **patterns, int i)
{
	size_t	last_pattern_len;

	last_pattern_len = ft_strlen(patterns[i]);
	if (patterns[i + 1] == NULL && word[ft_strlen(word) - 1] != '*')
		if (ft_strncmp(filename + ft_strlen(filename) - last_pattern_len,
				patterns[i], last_pattern_len + 1) != 0)
			return (FALSE);
	return (TRUE);
}

t_bool	check_match(char *filename, char **patterns, char *word)
{
	int		i;
	char	*ptr;

	i = 0;
	ptr = filename;
	if (!check_special_match(filename, word))
		return (FALSE);
	while (patterns[i])
	{
		if (i == 0 && word[0] != '*'
			&& ft_strncmp(filename, patterns[0], ft_strlen(patterns[0])) != 0)
			return (FALSE);
		ptr = ft_strnstr(ptr, patterns[i], ft_strlen(filename));
		if (ptr == NULL)
			return (FALSE);
		ptr += ft_strlen(patterns[i]);
		if (!check_end_word_match(filename, word, patterns, i))
			return (FALSE);
		i++;
	}
	return (TRUE);
}
