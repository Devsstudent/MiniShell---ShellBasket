#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_gc	*gc;
	char	**our_env;
	char	*test = "lol";

	//gc = NULL;
	our_env = copy_env(envp);
	int		i;
	int		j;
	i = 0;
	while (our_env[i])
	{
		printf("%s", our_env[i]);
		i++;
	}
	//add_to_gc(PIPE, test, &gc);
	//printf("%p, %s\n", gc, (char *)gc->content);
	/*
	while (ac)
	{
		//Faire une copie de envp pour avoir notre propre environnement
	}*/
}

//function to malloc like each malloc store in a list the adress of the malloced value
//Maybe tokenise it with a token or something
