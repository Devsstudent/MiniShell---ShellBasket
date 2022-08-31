/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:08:06 by odessein          #+#    #+#             */
/*   Updated: 2022/08/30 16:23:01 by mbelrhaz         ###   ########.fr       */
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

void	init_loop_get_arg(int *j, int *last, t_bool *quote, t_bool *d_quote)
{
	*j = 0;
	*last = 0;
	*quote = FALSE;
	*d_quote = FALSE;
}

void	loop_get_arg(char *word, char **argv, int *i)
{
	int		j;
	int		last;
	t_bool	quote;
	t_bool	d_quote;

	init_loop_get_arg(&j, &last, &quote, &d_quote);
	while (word[j])
	{
		if (word[j] == ' ' && j > 0 && word[j - 1] != ' ' && !d_quote && !quote)
		{
			argv[*i] = ft_substr(word, last, (j - last));
			(*i)++;
			if (word[j + 1])
				last = j + 1;
		}
		while (word[j] && word[j] == ' ')
			j++;
		if (!word[j] && check_quote(&d_quote, &quote, word[j]))
			break ;
		else
			j++;
	}
	if (j != last)
		argv[((*i)++)] = ft_substr(word, last, (j - last));
}

t_bool	check_abs_path(char *cmd, char **res)
{
	struct stat	statbuff;

	if (!(*res))
	{
		if (access(cmd, X_OK) == 0)
		{
			if (stat(cmd, &statbuff) == -1)
			{
				perror("stat");
				return (FALSE);
			}
			if ((statbuff.st_mode & S_IFMT) != S_IFREG)
				return (FALSE);
			*res = ft_strdup(cmd);
			if (!(*res))
				free_exit();
		}
		else
			return (FALSE);
	}
	return (TRUE);
}

void	check_cmd_path(char **res, char ***path_li, char **argv, t_dict *env)
{
	int		i;
	char	*buff;

	i = 0;
	*path_li = ft_split(dict_get_value(env, "PATH"), ':');
	while (*path_li && (*path_li)[i])
	{
		buff = ft_strjoin(ft_strdup((*path_li)[i]), "/");
		buff = ft_strjoin(buff, argv[0]);
		if (access(buff, X_OK) == 0)
		{
			*res = buff;
			break ;
		}
		free(buff);
		i++;
	}
}

char	*check_cmd(char **argv, t_dict *env)
{
	char	**path_li;
	int		j;
	char	*res;

	if (!argv[0])
		return (NULL);
	if (check_builtins(argv))
		return (ft_strdup(argv[0]));
	res = NULL;
	check_cmd_path(&res, &path_li, argv, env);
	if (path_li)
	{
		j = 0;
		while (path_li && path_li[j])
		{
			free(path_li[j]);
			j++;
		}
		free(path_li);
	}
	if (!check_abs_path(argv[0], &res))
		return (NULL);
	return (res);
}
