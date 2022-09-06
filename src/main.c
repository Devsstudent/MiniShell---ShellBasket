/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 20:25:56 by odessein          #+#    #+#             */
/*   Updated: 2022/09/06 18:59:38 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	ms_line(char **line, t_info *exec_in)
{
	listen_to_sigs();
	*line = readline("@ShellBasket^$ ");
	if (!(*line))
	{
		write(2, "exit\n", 5);
		if (exec_in->stdi != -1)
			close(exec_in->stdi);
		if (exec_in->stdou != -1)
			close(exec_in->stdou);
		free_exit();
	}
	add_history(*line);
	add_to_gc(SIMPLE, *line, get_gc());
	if (*line && !(*line[0]))
		return (TRUE);
	return (FALSE);
}

t_tree	*ms_lex_and_parse(char **line, t_info *exec_in)
{
	t_line	*line_lst;
	t_tree	*tree;

	tree = (t_tree *) malloc(sizeof(t_tree));
	if (!tree)
		free_exit();
	add_to_gc(TREE, tree, get_gc());
	line_lst = fill_line_lst(*line);
	if (line_lst == NULL)
		return (tree->head = NULL, tree);
	add_to_gc(LINE, line_lst, get_gc());
	if (!tokenization(line_lst))
		return (tree->head = NULL, tree);
	fill_ast(line_lst, tree);
	exec_in->fd_arr_size = total_block(tree->head);
	exec_in->fd_arr = malloc(sizeof(int) * exec_in->fd_arr_size);
	if (!exec_in->fd_arr)
		free_exit();
	return (tree);
}

void	malloc_pid_arr(t_info *exec_info, t_tree *tree)
{
	t_leaf	*leaf;
	int		size;

	leaf = NULL;
	size = 0;
	if (tree->head)
		leaf = tree->head;
	if (leaf && leaf->type == CMD)
	{
		exec_info->pid = (int *)malloc(sizeof(int));
		if (!exec_info->pid)
			free_exit();
		(exec_info->pid)[0] = -1;
	}
	else
	{
		while (leaf->type == PIPE_L)
		{
			size++;
			if (leaf->right)
				leaf = leaf->right;
			else
				break ;
		}
		exec_info->pid = (int *)malloc(sizeof(int) * size);
		if (!exec_info->pid)
			free_exit();
	}
	add_to_gc(SIMPLE, exec_info->pid, get_gc());
}

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
	dict_modify(env, ft_strdup("SHLVL"), ft_itoa(ft_atoi(dict_get_value(env, "SHLVL")) + 1));
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

void	wait_sub_process(t_info *exec_info)
{
	int	i;
	int	w_status;

	i = 0;
	if (exec_info->tmp_fd != -1)
		close(exec_info->tmp_fd);
	if (dup2(exec_info->stdi, STDIN_FILENO) == -1)
		perror("sell");
	if (exec_info->stdi > 0)
		close(exec_info->stdi);
	if (exec_info->stdou != -1 && dup2(exec_info->stdou, STDOUT_FILENO) == -1)
		perror("basket");
	if (exec_info->stdou != -1)
		close(exec_info->stdou);
	while (i < exec_info->turn)
	{
		w_status = -81;
		waitpid(exec_info->pid[i], &w_status, WCONTINUED);
		if (exec_info->pid[i] == -1 || w_status == -81)
		{
			if (exec_info->pid[i] != -1 && w_status == -81)
				g_exit_status = 130;
			i++;
			continue ;
		}
		if (WIFEXITED(w_status) && !exec_info->cmd_not_found)
			g_exit_status = WEXITSTATUS(w_status);
		else if (WIFSIGNALED(w_status) && WTERMSIG(w_status) == 2)
			g_exit_status = 130;
		else if (WIFSIGNALED(w_status) && WTERMSIG(w_status) == 3)
		{
			write(2, "Quit (core dumped)\n", 19);
			g_exit_status = 131;
			return ;
		}
		i++;
	}
}
