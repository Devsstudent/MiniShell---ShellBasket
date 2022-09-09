/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 20:25:56 by odessein          #+#    #+#             */
/*   Updated: 2022/09/09 20:11:23 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

/*
void	browse_sub_tree(t_leaf *leaf)
{
	t_line	*line;
	t_block	*buff;

	if (leaf->type == CMD)
	{
		line = leaf->content;
		if (line)
		{
			buff = line->head;
			while (buff)
				buff = buff->next;
		}
	}
	if (leaf->left != NULL)
		browse_sub_tree(leaf->left);
	else
		return ;
	if (leaf->right != NULL)
		browse_sub_tree(leaf->right);
	else
		return ;
}

void	browse_tree(t_tree *tree)
{
	t_leaf	*buff;

	buff = tree->head;
	browse_sub_tree(buff);
}
*/

t_info	*init_exec_info(void)
{
	t_info	*exec_info;

	exec_info = (t_info *) malloc(sizeof(t_info));
	if (!exec_info)
		free_exit();
	add_to_gc(EXEC_INFO, exec_info, get_gc());
	exec_info->argv = NULL;
	exec_info->fd_arr = NULL;
	exec_info->fd_arr_size = 0;
	exec_info->turn = 0;
	exec_info->pid = NULL;
	exec_info->tmp_fd = -1;
	exec_info->end = FALSE;
	exec_info->stdi = dup(STDIN_FILENO);
	exec_info->cmd_not_found = FALSE;
	exec_info->stdou = dup(STDOUT_FILENO);
	return (exec_info);
}

void	wait_sub_process(t_info *exec_info);

int	g_exit_status = 0;

static void	main_extension(t_info *exec_info, t_tree *tree, t_dict *env)
{
	malloc_pid_arr(exec_info, tree);
	exec_tree(tree->head, exec_info, env, tree);
	wait_sub_process(exec_info);
	free_each_turn(get_gc());
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_tree	*tree;
	t_dict	*env;
	t_info	*exec_info;

	env = double_char_to_lst(envp);
	dict_modify(env, ft_strdup("SHLVL"),
		ft_itoa(ft_atoi(dict_get_value(env, "SHLVL")) + 1));
	while (ac && av[0])
	{
		exec_info = init_exec_info();
		if (ms_line(&line, exec_info))
			continue ;
		tree = ms_lex_and_parse(&line, exec_info);
		if (tree->head == NULL && free_each_turn(get_gc()))
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
