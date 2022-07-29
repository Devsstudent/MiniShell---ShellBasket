#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_gc	*gc;
	char	*test = "lol";

	gc = NULL;
	add_to_gc(PIPE, test, &gc);
	printf("%p, %s\n", gc, (char *)gc->content);
	/*
	while (ac)
	{
		//Faire une copie de envp pour avoir notre propre environnement
	}*/
}

//function to malloc like each malloc store in a list the adress of the malloced value
//Maybe tokenise it with a token or something
