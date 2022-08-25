/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:08:06 by odessein          #+#    #+#             */
/*   Updated: 2022/08/25 13:10:01 by odessein         ###   ########.fr       */
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
	//ft_printf(0, "%i", (int) size);
	return (size);
}

void	get_size_word_in_word(char *word, size_t *size)
{
	int	i;
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
		if (i > 0 && word[i - 1] != ' ' && word[i] == ' ' && word[i + 1] && word[i + 1] != ' ' && !quote && !d_quote)
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

void	loop_get_arg(char *word, char **argv, int *i)
{
	int	j;
	int	last;
	t_bool	quote;
	t_bool	d_quote;

	j = 0;
	last = 0;
	quote = FALSE;
	d_quote = FALSE; while (word[j]) {
		if (word[j] == ' ' && j > 0 && word[j - 1] != ' ' && !d_quote && !quote)
		{
			argv[*i] = ft_substr(word, last, (j - last));
			(*i)++;
			if (word[j + 1])
				last = j + 1;
		}
		while (word[j] && word[j] == ' ')
		{
			j++;
			last = j;
		}
		if (!word[j])
			break ;
		if (word[j] == '\"' && !d_quote)
			d_quote = TRUE;
		if (word[j] == '\'' && !quote)
			quote = TRUE;
		if (word[j] == '\'' && quote)
			quote = FALSE;
		if (word[j] == '\"' && d_quote)
			d_quote = FALSE;
		if (word[j])
			j++;
	}
	if (j != last)
	{
		argv[*i] = ft_substr(word, last, (j - last));
		(*i)++;
	}
}

char	*check_cmd(char **argv, t_dict *env)
{
	char	**path_li;
	int		i;
	char	*res;
	char	*buff;
	struct stat	statbuff;

	if (!argv[0])
		return (NULL);
	if (check_builtins(argv))
		return (argv[0]);
	i = 0;
	res = NULL;
	path_li = ft_split(dict_get_value(env, "PATH"), ':');
	while (path_li && path_li[i])
	{
		buff = ft_strjoin(ft_strdup(path_li[i]), "/");
		buff = ft_strjoin(buff, argv[0]);
		if (access(buff, X_OK) == 0)
		{
			res = buff;
			break;
		}
		free(buff);
		i++;
	}
	if (!res)
	{
		if (access(argv[0], X_OK) == 0)
		{
			if (stat(argv[0], &statbuff) == -1)
			{
				perror("stat");
				return (NULL);
			}
			if ((statbuff.st_mode & S_IFMT) != S_IFREG)
				return (NULL);
			res = ft_strdup(argv[0]);
			if (!res)
				free_exit();
		}
		else
			return (NULL);
	}
	if (path_li)
	{
		int	j;
		j = 0;
		while (path_li && path_li[j])
		{
			free(path_li[j]);
			j++;
		}
		free(path_li);
	}
	return (res);
}
