/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 19:02:58 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/09/08 18:27:28 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

/*
 * we set g_exit_status to -800 to indicate that we are in a here_doc
 * if we are in a heredoc, the function sigint_handler shouldn't be called twice
 * so we ignore one call
*/

void	sigint_handler(int signum)
{
	if (g_exit_status == 120)
	{
		close(STDIN_FILENO);
		g_exit_status = 140;
		write(1, "\n", 1);
		return ;
	}
	if (errno == 2 || errno == 9)
	{
		errno = 89;
		g_exit_status = 140;
		write(0, "\n", 1);
		return ;
	}
	if (signum == SIGINT)
	{
		write(2, "TEST", 4);
		write(0, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
		g_exit_status = 130;
	}
}

t_bool	handle_ctrl_c(char **line, int stdi)
{
	if (g_exit_status == 140)
	{
		free(*line);
		if (dup2(stdi, STDIN_FILENO) == -1)
			return (FALSE);
		*line = NULL;
	}
	return (TRUE);
}

/*
 * In order to ignore the signal ^\, we had to pass it SIG_IGN
 * as handler on the field sa_handler (it is written on the man)
 * This way, the hotkey is not printed on the screen
*/

void	listen_to_sigs(void)
{
	struct sigaction	action_quit;
	struct sigaction	action_int;

	sigemptyset(&action_int.sa_mask);
	action_int.sa_handler = sigint_handler;
	action_int.sa_flags = 0;
	sigemptyset(&action_quit.sa_mask);
	action_quit.sa_handler = SIG_IGN;
	action_quit.sa_flags = 0;
	sigaction(SIGINT, &action_int, NULL);
	sigaction(SIGQUIT, &action_quit, NULL);
}
