/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:12:26 by odessein          #+#    #+#             */
/*   Updated: 2022/09/03 15:54:25 by mbelrhaz         ###   ########.fr       */
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

size_t	get_ac(char **argv)
{
	int	ac;

	ac = 0;
	while (argv[ac])
		ac++;
	return (ac);
}

void	execve_cmd_alone(char *cmd_path, t_dict *env, t_info *exec_in)
{
	int	ac;
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (perror("pipe in forking cmd alone"));
	if (!dup_cmd_alone(exec_in, pipe_fd)
		|| !exec_cmd_alone_not_builtin(exec_in, env, cmd_path))
		return ;
	ac = get_ac(exec_in->argv);
	if (ft_strncmp(cmd_path, "exit", 5) == 0)
		exec_exit(ac, exec_in->argv, 1);
	else if (ft_strncmp(cmd_path, "cd", 3) == 0)
		exec_cd(ac, exec_in->argv, env);
	else if (ft_strncmp(cmd_path, "export", 7) == 0)
		exec_export(ac, exec_in->argv, env);
	else if (ft_strncmp(cmd_path, "unset", 6) == 0)
		exec_unset(ac, exec_in->argv, env);
	else if (ft_strncmp(cmd_path, "pwd", 4) == 0)
		exec_pwd(ac, exec_in->argv, env);
	else if (ft_strncmp(cmd_path, "env", 4) == 0)
		exec_env(ac, exec_in->argv, env);
	else if (ft_strncmp(cmd_path, "echo", 5) == 0)
		exec_echo(ac, exec_in->argv, env);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

t_bool	exec_cmd_alone_not_builtin(t_info *exec_in, t_dict *env, char *cmd_path)
{
	if (!check_builtins(exec_in->argv))
	{
		exec_in->pid[exec_in->turn] = fork();
		if (exec_in->pid[exec_in->turn] < 0)
		{
			perror("shellbasket");
			return (FALSE);
		}
		else if (exec_in->pid[exec_in->turn] == 0)
		{
			if (exec_in->open_fd != -1)
				close(exec_in->open_fd);
			if (exec_in->out_fd != -1)
				close(exec_in->out_fd);
			if (!execve_test(cmd_path, exec_in->argv, env, 1))
			{
				perror(exec_in->argv[0]);
				free_exit();
			}
		}
	}
	return (TRUE);
}

t_bool	exec_builtin(char **argv, t_dict *env, t_bool fork)
{
	int	ac;

	ac = get_ac(argv);
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		exec_echo(ac, argv, env);
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		exec_pwd(ac, argv, env);
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		exec_env(ac, argv, env);
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		exec_exit(ac, argv, 0);
	else if (ft_strncmp(argv[0], "unset", 5) == 0)
		exec_unset(ac, argv, env);
	else if (ft_strncmp(argv[0], "export", 5) == 0)
		exec_export(ac, argv, env);
	else if (ft_strncmp(argv[0], "cd", 5) == 0)
		exec_cd(ac, argv, env);
	else
		return (FALSE);
	if (fork)
		free_exit();
	return (TRUE);
}

//exit dans les builr in
t_bool	execve_test(char *pathname, char **argv, t_dict *env, t_bool fork)
{
	char	**env_bis;
	int		i;

	env_bis = dict_to_double_char_env(env);
	if (!exec_builtin(argv, env, fork))
	{
		i = 0;
		while (argv[i])
			i++;
		if (execve(pathname, argv, env_bis) == -1)
			return (FALSE);
	}
	return (TRUE);
}
