/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trial.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 19:40:39 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/09/13 20:32:35 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env, t_tree *tree)
{
	if (!leaf)
		return ;
	if (leaf->type == cmd)
		exec_cmd(exec_in, leaf->content, env);
	if (leaf->type == OR)
	{
		exec_tree leaf->left
		if (g_exit_status != 0)
			exec_tree leaf->right
	}
	if (leaf->type == AND)
	{
		exec_tree leaf->left
		if (g_exit_status == 0)
			exec_tree leaf->right
	}
	if (leaf->type == PIPE)
	{
		exec_tree leaf->left
		exec_tree leaf->right
	}
}



//waitpid des qu'on finit la pipeline, ou des qu'on finit une commande

The struct exec_in contains : 

start = tells us if it is the start
end = tells us if it is the end
the exit status is really important and should be handled well

