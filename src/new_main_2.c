/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_main_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 19:30:02 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/15 16:26:01 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	read_line(char **line)
{	
	*line = readline("@ShellBasket^$ ");
	if (!(*line))
		return (FALSE);
	add_history(*line);
	add_to_gc(SIMPLE, *line, get_gc());
	return (TRUE);
}

void	browse_sub_tree(t_leaf *leaf)
{
	ft_printf("type = %i, PAR = %i\n", leaf->type, leaf->parentheses);
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
				ft_printf("content = %s\n", buff->word);
				buff = buff->next;
			}
		}
	}
	if (leaf->left != NULL)
	{
		ft_printf("left\n");
		browse_sub_tree(leaf->left);
	}
	else
		return ;
	if (leaf->right != NULL)
	{
		ft_printf("right\n");
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

/*
void	browse_tree(t_tree *tree)
{
	t_leaf	*buff;
	t_leaf	*buff_left;
	
	buff = tree->head;
	while (buff)
	{
		ft_printf("type = %i\n", buff->type);
		if (buff->left != NULL)
		{
			buff_left = buff->left;
			while (buff_left != NULL)
			{
				ft_printf("left type = %i\n", buff_left->type);
				if (buff_left->left != NULL)
					ft_printf("left type = %i\n", buff_left->left->type);
				if (buff_left->right)
					ft_printf("right type %i\n", buff_left->right->type);
				buff_left = buff_left->left;
			}
			
		}
			
		buff = buff->right;
	}
}
*/
int	main(int ac, char **av, char **envp)
{
	t_dict			env;
	t_line			block_lst;
	char			*line;
	t_tree			tree;
	
	listen_to_sigs();
	if (av[1] && ac)
		return (1);
	double_char_to_lst(envp, &env);
	while (1)
	{
		block_lst.head = NULL;
		if (!read_line(&line))
			free_exit();
		if (!fill_line_lst(&block_lst, line))
		{
			ft_printf("malloc error\n");
			return (0);
		}
		tokenization(&block_lst);
		t_block	*buff;
		buff = block_lst.head;
		while (buff)
		{
//			ft_printf("word = %s ; token = %i\n", buff->word, buff->token);
			buff = buff->next;
		}
		fill_ast_bonus(&block_lst, &tree);
		browse_tree(&tree);
		ft_printf("\n\nhead = %i\n", tree.head->type);
		ft_printf("right leaf = %i\n", tree.head->right->type);
	}
	free_exit();
}

//testing the dict env

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
