/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:08:06 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 21:16:43 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	advance_if_space(char *word, int *j, int *last, int quote_status)
{
	if (is_white_space(word[*j]))
	{
		while (word[*j])
		{
			(*j)++;
			if (!is_white_space(word[*j]) && !quote_status)
			{
				*last = *j;
				(*j)--;
				break ;
			}
		}
	}
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
		if (is_white_space(word[j]) && j > 0 && !is_white_space(word[j - 1])
			&& !d_quote && !quote)
		{
			argv[*i] = ft_substr(word, last, (j - last));
			(*i)++;
			if (word[j + 1])
				last = j + 1;
		}
		advance_if_space(word, &j, &last, quote + d_quote);
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

	if (cmd && (!cmd[0] || !strncmp(cmd, ".", 2) || !strncmp(cmd, "..", 3)))
		return (FALSE);
	if (!(*res) && (cmd && ft_strrchr(cmd, '/')))
		if (!check_kind_of_abs(statbuff, cmd, res))
			return (FALSE);
	return (TRUE);
}

static void	check_cmd_path(char **res, char ***path_li, char **argv,
			t_dict *env)
{
	int		i;
	char	*buff;

	i = 0;
	*path_li = ft_split(dict_get_value(env, "PATH"), ':');
	while (*path_li && (*path_li)[i])
	{
		buff = ft_strjoin(ft_strdup((*path_li)[i]), "/");
		buff = ft_strjoin(buff, argv[0]);
		if (ft_strnstr(buff, "//", ft_strlen(buff)))
			return (free(buff));
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

	path_li = NULL;
	if (!argv[0])
		return (NULL);
	if (check_builtins(argv))
		return (ft_strdup(argv[0]));
	res = NULL;
	if (argv[0] && !ft_strrchr(argv[0], '/'))
		check_cmd_path(&res, &path_li, argv, env);
	if (path_li)
	{
		j = 0;
		while (path_li && path_li[j])
			free(path_li[j++]);
		free(path_li);
	}
	errno = 0;
	if (!check_abs_path(argv[0], &res))
		return (free(res), NULL);
	return (res);
}
