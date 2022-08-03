#include "minishell.h"

void	signal_handler(int signum, siginfo_t *info, void *ucontext)
{
	(void)info;
	(void)ucontext;
	if (signum == SIGINT)
	{
		write(0, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

t_bool	read_line(char **line, t_gc **gc)
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
	t_dict			env;
	t_gc			*gc;

	//signal handling
	struct sigaction	action;

	action.sa_sigaction = signal_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	//ends here
	
	if (av[1] && ac)
		return (1);
	gc = NULL;
	double_char_to_lst(envp, &env, &gc);
	while (1)
	{
		t_line	block_lst;

		block_lst.head = NULL;
		//signal handling
		sigaction(SIGQUIT, &action, NULL);
		sigaction(SIGINT, &action, NULL);
		//ends here
		char *line;
		if (!read_line(&line, &gc))
		{
			free_gc(&gc);
			return (1);
		}
		if (!fill_line_lst(&block_lst, line))
			ft_printf("malloc error\n");
		if (!tokenisation(&block_lst))
			ft_printf("syntax error\n");
	}
	return (0);
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
>>>>>>> 9d42c746574d6454e85bfb0c4d2885897d5fb292
	}
	free(arg);
	dict_clear(&env);
	printf("done\n");
	return (0);
}
*/
