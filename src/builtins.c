/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 16:53:47 by odessein          #+#    #+#             */
/*   Updated: 2022/10/13 20:33:55 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	check_builtins(char **argv)
{
	if (argv[0] == NULL)
		return (FALSE);
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "cd", 3) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "export", 7) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "unset", 6) == 0)
		return (TRUE);
	return (FALSE);
}

static void	exit_builtin(t_info *exec_in, t_bool fork, int pipe_fd[2])
{
	close_subprocess_fd(exec_in, pipe_fd);
	if (fork)
		free_exit();
}

static void	sig_pipe_handler(t_bool fork)
{
	if (fork)
		signal(SIGPIPE, sigpipe_handler);
}

t_bool	exec_builtin(t_dict *env, t_bool fork, t_info *exec_in, int pipe_fd[2])
{
	int		ac;
	char	**argv;

	sig_pipe_handler(fork);
	argv = exec_in->argv;
	ac = get_ac(argv);
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		exec_echo(ac, argv, env);
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		exec_pwd(ac, argv, env);
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		exec_env(ac, argv, env);
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		exec_exit(ac, exec_in, 0);
	else if (ft_strncmp(argv[0], "unset", 5) == 0)
		exec_unset(ac, argv, env);
	else if (ft_strncmp(argv[0], "export", 5) == 0)
		exec_export(ac, argv, env);
	else if (ft_strncmp(argv[0], "cd", 5) == 0)
		exec_cd(ac, argv, env);
	else
		return (FALSE);
	exit_builtin(exec_in, fork, pipe_fd);
	return (TRUE);
}
