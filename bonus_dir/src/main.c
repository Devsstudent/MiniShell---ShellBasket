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

//Function recursive d'exec
//Function recursive de parcours de l'ast
//Reset les init_info apres chaque exec + wait les pid
//One thing by function !

int	g_exit_status = 0;
//Same parsing, not same waiting pid 
//reset de l'exec dans la recursion + les check redirection etc
//Juste les here_doc au parsing enfaite (et encore)
//Objectif : setup clean pour l'exec
int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_tree	*tree;
	t_info	*exec_in;
	t_dict	*env;

	(void) envp;
	if (av[1])
		return (1);
	env = double_char_to_lst(envp);
	while (ac)
	{
		exec_in = init_exec_info();
		ms_line(&line, exec_in);
		tree = ms_lex_and_parse(&line, exec_in);
		browse_sub_tree(tree->head);
		exec_tree(tree->head, exec_in, env);
	}
	return (1);
}
