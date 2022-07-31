#include "minishell.h"

t_bool	handle_line(char **line, t_gc **gc)
{
	*line = readline("@ShellBasket^$ ");
	if (!(*line))
		return (FALSE);
	add_history(*line);
	add_to_gc(SIMPLE, *line, gc);
	return (TRUE);
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_gc	*gc;
	t_dict	env;

	if (av[1])
		return (1);
	gc = NULL;
	double_char_to_lst(envp, &env, &gc);
	while (ac)
	{
		if (!handle_line(&line, &gc))
		{
			free_gc(&gc);
			return (0);
		}
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