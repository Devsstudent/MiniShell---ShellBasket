/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_main_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 19:30:02 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/13 23:34:00 by mbelrhaz         ###   ########.fr       */
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

//essai qui ne marche pas
/*
void    browse_sub_tree(t_leaf *leaf)
{
    t_leaf  *buff;
    
    buff = leaf;
    while (buff)
    {
        ft_printf("type = %i\n", buff->type);
        if (buff->left != NULL)
            browse_sub_tree(buff->left);
        else
            return ;
        if (buff->right != NULL)
            browse_sub_tree(buff->right);
        else
            return ;
    }
}

void    browse_tree(t_tree *tree)
{
    t_leaf  *buff;
    
    buff = tree->head;
    browse_sub_tree(buff);
}
*/


void	browse_tree(t_tree *tree)
{
	t_leaf	*buff;
	
	buff = tree->head;
	while (buff)
	{
		ft_printf("type = %i\n", buff->type);
		if (buff->left != NULL)
			ft_printf("left type = %i\n", buff->left->type);
		buff = buff->right;
	}
}

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
			ft_printf("malloc error\n");
		tokenization(&block_lst);
		t_block	*buff;
		buff = block_lst.head;
		while (buff)
		{
			ft_printf("word = %s ; token = %i\n", buff->word, buff->token);
			buff = buff->next;
		}
		fill_ast_bonus(&block_lst, &tree);
		browse_tree(&tree);
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
