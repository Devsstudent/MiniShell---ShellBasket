#include "minishell.h"

void	browse_sub_tree(t_leaf *leaf)
{
	printf("type = %i, PAR = %i\n", leaf->type, leaf->parentheses);
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
				printf("content = %s\n", buff->word);
				buff = buff->next;
			}
		}
	}
	if (leaf->left != NULL)
	{
		printf("left\n");
		browse_sub_tree(leaf->left);
	}
	else
		return ;
	if (leaf->right != NULL)
	{
		printf("right\n");
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
	if (!exec_info)
		free_exit();
	add_to_gc(EXEC_INFO, exec_info, get_gc());
	exec_info->pid_li = NULL;
	exec_info->pipe = FALSE;
	exec_info->argv = NULL;
	exec_info->fd_arr = NULL;
	exec_info->fd_arr_size = 0;
	exec_info->turn = 0;
	exec_info->pipe_fd[0] = -1;
	exec_info->pipe_fd[1] = -1;
	exec_info->tmp_fd = -1;
	exec_info->end = FALSE;
	exec_info->stdi = dup(STDIN_FILENO);
	exec_info->cmd_not_found = FALSE;
	exec_info->stdou = dup(STDOUT_FILENO);
	return (exec_info);
}

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
	//browse_tree(tree);
	init_pid_lst(exec_info);
	exec_tree(tree->head, exec_info, env);
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
		browse_tree(tree);
		if (tree->head == NULL && free_each_turn(get_gc()))
		{
			close(exec_info->stdou);
			close(exec_info->stdi);
			continue ;
		}
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
