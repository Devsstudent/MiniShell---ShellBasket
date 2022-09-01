/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:08:06 by odessein          #+#    #+#             */
/*   Updated: 2022/09/01 17:05:04 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

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
		if (check_quote(&d_quote, &quote, word[j]) && !word[j])
			break ;
		else
			j++;
	}
	if (j != last)
		argv[((*i)++)] = ft_substr(word, last, (j - last));
}

static t_bool	check_abs_path(char *cmd, char **res)
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

static void	check_cmd_path(char **res, char ***path_li, char **argv, t_dict *env)
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
