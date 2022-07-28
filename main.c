#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/wait.h>

int	main(void)
{
	char	*line;
	char	*cmd;
	int		pid;
	char	*arg[2] = {"ls", NULL};

	while (1)
	{
		line = readline("@ShellBasket^$ ");
		add_history(line);
		if (!line)
		{
			rl_clear_history();
			return (1);
		}
		pid = fork();
		if (pid == 0)
		{
			if (execve(line, arg, NULL) == -1)
			{
				perror(line);
				return (2);
			}
		}
		waitpid(pid, NULL, 0);
		free(line);
	}
}
