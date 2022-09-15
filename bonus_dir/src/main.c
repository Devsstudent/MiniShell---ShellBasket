#include "minishell.h"

void	browse_sub_tree(t_leaf *leaf)
{
//	printf(0, "type = %i, PAR = %i\n", leaf->type, leaf->parentheses);
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

//Function recursive d'exec
//Function recursive de parcours de l'ast
//Reset les init_info apres chaque exec + wait les pid
//One thing by function !

int	g_exit_status = 0;

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_tree	*tree;
	t_info	*exec_in;
	//t_dict	*env;

	(void) envp;
	exec_in = NULL;
	if (av[1])
		return (1);
	//env = double_char_to_lst(envp);
	while (ac)
	{
		ms_line(&line, exec_in);
		tree = ms_lex_and_parse(&line, exec_in);
		browse_sub_tree(tree->head);
	//	browse_ast_apply_expand(tree->head, env);
	}
	return (1);
}
