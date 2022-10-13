/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 13:05:42 by odessein          #+#    #+#             */
/*   Updated: 2022/10/13 17:16:26 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//Function recursive d'exec
//Function recursive de parcours de l'ast
//Reset les init_info apres chaque exec + wait les pid
//One thing by function !

int	g_exit_status = 0;
//Same parsing, not same waiting pid 
//reset de l'exec dans la recursion + les check redirection etc
//Juste les here_doc au parsing enfaite (et encore)
//Objectif : setup clean pour l'exec

void	init_pid_lst(t_info *exec_info)
{
	exec_info->pid_li = malloc(sizeof(t_pid_li));
	if (!(exec_info->pid_li))
		free_exit();
	add_to_gc(SIMPLE, exec_info->pid_li, get_gc());
	exec_info->pid_li->head = NULL;
	exec_info->pid_li->last = NULL;
	exec_info->pid_li->size = 0;
}

static void	main_extension(t_info *exec_info, t_tree *tree, t_dict *env)
{
	init_pid_lst(exec_info);
	exec_tree(tree->head, exec_info, env, NULL);
	wait_sub_process(exec_info);
	if (exec_info->stdi > -1)
		close(exec_info->stdi);
	if (exec_info->stdou > -1)
		close(exec_info->stdou);
	free_each_turn(get_gc());
}

static void	setup_shlvl(t_dict *env)
{
	dict_modify(env, ft_strdup("SHLVL"),
		ft_itoa(ft_atoi(dict_get_value(env, "SHLVL")) + 1));
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_tree	*tree;
	t_dict	*env;
	t_info	*exec_info;

	env = double_char_to_lst(envp);
	setup_shlvl(env);
	while (ac && av[0])
	{
		exec_info = init_exec_info();
		if (ms_line(&line, exec_info))
			continue ;
		tree = ms_lex_and_parse(&line, exec_info);
		if (!tree->head && free_each_turn(get_gc()))
			continue ;
		parse_here_doc(tree->head, exec_info->fd_arr, 0);
		if (g_exit_status == 140 && free_each_turn(get_gc()))
		{
			g_exit_status = 130;
			continue ;
		}
		main_extension(exec_info, tree, env);
	}
	return (1);
}
