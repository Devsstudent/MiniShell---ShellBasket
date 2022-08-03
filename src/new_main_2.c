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

t_bool	read_line(char **line)
{	
	*line = readline("@ShellBasket^$ ");
	if (!(*line))
		return (FALSE);
	listen_to_sigs();
	add_history(*line);
	add_to_gc(SIMPLE, *line, get_gc());
	return (TRUE);
}

void	listen_to_sigs(void)
{
	struct sigaction	action;

	action.sa_sigaction = signal_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGQUIT, &action, NULL);
	sigaction(SIGINT, &action, NULL);
}

int	main(int ac, char **av, char **envp)
{
	t_dict			env;
	t_line			block_lst;
	char			*line;

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
		tokenisation(&block_lst);
		t_block	*buff;
		buff = block_lst.head;
		while (buff)
		{
			ft_printf("word = %s ; token = %i\n", buff->word, buff->token);
			buff = buff->next;
		}
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
>>>>>>> 9d42c746574d6454e85bfb0c4d2885897d5fb292
	}
	free(arg);
	dict_clear(&env);
	printf("done\n");
	return (0);
}
*/
