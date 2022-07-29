#include "ms.h"

int	main(int ac, char **av, char **envp)
{
	int i;
	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	/*
	while (ac)
	{
		//Faire une copie de envp pour avoir notre propre environnement
	}*/
}

//function to malloc like each malloc store in a list the adress of the malloced value
//Maybe tokenise it with a token or something
