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
	line_lst = fill_line_lst(*line);
	tokenization(line_lst);
	fill_ast(line_lst, tree);
	return (tree);
}

void	browse_sub_tree(t_leaf *leaf)
{
	ft_printf(0, "type = %i, PAR = %i\n", leaf->type, leaf->parentheses);
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
				ft_printf(0, "content = %s\n", buff->word);
				buff = buff->next;
			}
		}
	}
	if (leaf->left != NULL)
	{
		ft_printf(0, "left\n");
		browse_sub_tree(leaf->left);
	}
	else
		return ;
	if (leaf->right != NULL)
	{
		ft_printf(0, "right\n");
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


int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_tree	*tree;
	t_dict	env;

	if (av[1])
		return (1);
	double_char_to_lst(envp, &env);
	/*
	t_elem *buff;
	buff = env.head;
	while (buff)
	{
		ft_printf("%s, %s\n", buff->key, buff->value);
		buff = buff->next;
	}
	*/
	while (ac)
	{
		ms_line(&line);
		tree = ms_lex_and_parse(&line);
		browse_tree(tree);
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
