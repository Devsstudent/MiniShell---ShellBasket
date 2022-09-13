/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 19:02:58 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/08 19:28:53 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	sigint_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(0, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
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
