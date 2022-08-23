/* ************************************************************************** */ /*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 12:49:15 by odessein          #+#    #+#             */
/*   Updated: 2022/08/22 16:49:02 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ms_line(char **line)
{
	listen_to_sigs();
	*line = readline("@ShellBasket^$ ");
	if (!(*line))
		free_exit();
	add_history(*line);
	add_to_gc(SIMPLE, *line, get_gc());
}

t_tree	*ms_lex_and_parse(char **line)
{
	t_line	*line_lst;
	t_tree	*tree;

	tree = (t_tree *) malloc(sizeof(t_tree));
	add_to_gc(TREE, tree, get_gc());
	line_lst = fill_line_lst(*line);
	add_to_gc(LINE, line_lst, get_gc());
	tokenization(line_lst);
	fill_ast(line_lst, tree);
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
	//ft_printf(0, "type = %i, PAR = %i\n", leaf->type, leaf->parentheses);
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

int	g_exit_status = 0;

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_tree	*tree;
	t_dict	*env;
	t_info	*exec_info;

	exec_info = (t_info *) malloc(sizeof(t_info));
	exec_info->argv = NULL;
	if (av[1])
		return (1);
	env = double_char_to_lst(envp);
	/*
	t_elem *buff;
	buff = env.head;
	while (buff)
	{
		//ft_printf("%s, %s\n", buff->key, buff->value);
		buff = buff->next;
	}
	*/
	while (ac)
	{
		int	stdou;
		int	stdi;

		exec_info->turn = 0;
		exec_info->pid = NULL;
		exec_info->tmp_fd = -1;
		exec_info->end = FALSE;
		ms_line(&line);
		tree = ms_lex_and_parse(&line);
		malloc_pid_arr(exec_info, tree);
		stdi = dup(STDIN_FILENO);
		stdou = dup(STDOUT_FILENO);
		exec_info->stdou = stdou;
		exec_tree(tree->head, exec_info, env, tree);
//		browse_tree(tree);
		int	i;
		i = 0;
		if (exec_info->tmp_fd != -1)
			close(exec_info->tmp_fd);
		if (dup2(stdi, STDIN_FILENO) == -1)
		{
			perror("sell");
			continue ;
		}
		close(stdi);
		if (exec_info->stdou != -1 && dup2(stdou, STDOUT_FILENO) == -1)
		{
			perror("basket");
			continue ;
		}
		
		close(stdou);
		while (i < exec_info->turn)
		{
			waitpid(exec_info->pid[i], NULL, 0);
			i++;
		}
		//waitpid();
	}
	return (1);
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
