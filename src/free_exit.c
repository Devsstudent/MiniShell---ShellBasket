/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 13:59:27 by odessein          #+#    #+#             */
/*   Updated: 2022/09/01 19:26:27 by odessein         ###   ########.fr       */
#include "minishell.h"

void	free_exit()
{
	rl_clear_history();
	free_gc(get_gc());
	exit(g_exit_status);
}
