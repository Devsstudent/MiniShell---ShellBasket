#include "minishell.h"

void	signal_handler(int signum)
{
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
	add_history(*line);
	add_to_gc(SIMPLE, *line, get_gc());
	return (TRUE);
}

void	listen_to_sigs(void)
{
	struct sigaction	action_quit;
	struct sigaction	action_int;
	
	sigemptyset(&action_quit.sa_mask);
	sigemptyset(&action_int.sa_mask);
	action_int.sa_handler = signal_handler;
	action_quit.sa_handler = SIG_IGN;
	action_quit.sa_flags = 0;
	action_int.sa_flags = 0;
	sigaction(SIGQUIT, &action_quit, NULL);
	sigaction(SIGINT, &action_int, NULL);
}

int	main(int ac, char **av, char **envp)
{
	t_dict			env;
	t_line			block_lst;
	char			*line;
	
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
