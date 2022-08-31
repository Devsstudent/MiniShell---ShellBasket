/* ************************************************************************** */ /*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 12:49:15 by odessein          #+#    #+#             */
/*   Updated: 2022/08/22 16:49:02 by odessein         ###   ########.fr       */
/*                                                                            */ /* ************************************************************************** */
#include "minishell.h"

t_bool	ms_line(char **line)
{
	listen_to_sigs();
	*line = readline("@ShellBasket^$ ");
	if (!(*line))
		free_exit();
	if (*line &&  !(*line[0]))
		return (TRUE);
	add_history(*line);
	add_to_gc(SIMPLE, *line, get_gc());
	return (FALSE);
}


t_tree	*ms_lex_and_parse(char **line, t_info *exec_in)
{
	t_line	*line_lst;
	t_tree	*tree;

	tree = (t_tree *) malloc(sizeof(t_tree));
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
	return (tree);
}

void	malloc_pid_arr(t_info *exec_info, t_tree *tree)
{
	t_leaf	*leaf;
	int	size;

	size = 0;
	if (tree->head)
		leaf = tree->head;
	if (leaf && leaf->type == CMD)
		exec_info->pid = (int *)malloc(sizeof(int));
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
	}
	add_to_gc(SIMPLE, exec_info->pid, get_gc());
	//ft_printf(0, "HEREE %i\n", size);
}

void	browse_sub_tree(t_leaf *leaf)
{
	//ft_printf(0, "(TRUE)type = %i, PAR = %i\n", leaf->type, leaf->parentheses);
	if (leaf->type == CMD)
	{
		t_line *line;
		line = leaf->content;
		t_block	*buff;
		if (line)
		{
			buff = line->head;
			while (buff)
			{
				//ft_printf(0, "content = %s\n", buff->word);
				buff = buff->next;
			}
		}
	}
	if (leaf->left != NULL)
	{
		//ft_printf(0, "left\n");
		browse_sub_tree(leaf->left);
	}
	else
		return ;
	if (leaf->right != NULL)
	{
		//ft_printf(0, "right\n");
		browse_sub_tree(leaf->right);
	}
	else
		return ;
}

void	browse_tree(t_tree *tree)
{
	t_leaf  *buff;

	buff = tree->head;
	browse_sub_tree(buff);
}

t_info	*init_exec_info(void)
{
	t_info	*exec_info;

	exec_info = (t_info *) malloc(sizeof(t_info));
	//sometimes exec_info isn't freed
	//Free exec_info
	exec_info->argv = NULL;
	exec_info->fd_arr = NULL;
	exec_info->fd_arr_size = 0;
	exec_info->turn = 0;
	exec_info->pid = NULL;
	exec_info->tmp_fd = -1;
	exec_info->end = FALSE;
	exec_info->stdi = dup(STDIN_FILENO);
	exec_info->stdou = dup(STDOUT_FILENO);
	return (exec_info);
	//penser a closes et a reset a chaque tour
}

void	wait_sub_process(t_info *exec_info);

int	g_exit_status = 0;

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_tree	*tree;
	t_dict	*env;
	t_info	*exec_info;

	if (av[1])
		return (1);
	env = double_char_to_lst(envp);
	while (ac)
	{
		exec_info = init_exec_info();
		if (ms_line(&line))
			continue ;
		tree = ms_lex_and_parse(&line, exec_info);
		if (tree->head == NULL)
		{
			free_each_turn(get_gc(), exec_info);
			continue ;
		}
		parse_here_doc(tree->head, exec_info->fd_arr, 0);
		malloc_pid_arr(exec_info, tree);
		browse_line_check_red_in(tree->head, env);
		exec_tree(tree->head, exec_info, env, tree);
		wait_sub_process(exec_info);
		free_each_turn(get_gc(), exec_info);
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
	close(exec_info->stdi);
	if (exec_info->stdou != -1 && dup2(exec_info->stdou, STDOUT_FILENO) == -1)
		perror("basket");
	if (exec_info->stdou != -1)
		close(exec_info->stdou);
	if (check_builtins(exec_info->argv) && exec_info->turn == 1)
		i++;
	while (i < exec_info->turn)
	{
		waitpid(exec_info->pid[i], &w_status, 0);
		if (WIFSIGNALED(w_status))
			g_exit_status = 130;
		i++;
	}
}


/*
int	main(int ac, char **av, char **envp)
{
	t_dict	env;
	t_elem	*buff;
	t_elem	*new;
	char	**arg;

	arg = ft_split("COCO=COCO", '=');
	if (!double_char_to_lst(envp, &env))
		return (1);
	printf("here\n");
	new = new_elem("COCO=PASCOCO");
	if (!new)
		return (3);
	dict_addback(&env, new);
	dict_modify(&env, arg[0], arg[1]);
	buff = env.head;
	while (buff)
	{
		printf("%s=%s\n", buff->key, buff->value);
		buff = buff->next;
	}
	free(arg);
	dict_clear(&env);
	printf("done\n");
	return (0);
}
*/
