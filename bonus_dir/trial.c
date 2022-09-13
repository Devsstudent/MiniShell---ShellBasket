/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trial.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 19:40:39 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/09/13 21:15:12 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	exec_cmd(t_info *exec_info, t_line *sub, t_dict *env)
{
	expand(sub, env);
	handle_wildcards(sub, env);
	exec_info->argv = get_cmd_arg(sub);
	exec(exec_info, sub, env);
}

void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env)
{
	if (!leaf)
		return ;
	if (leaf->type == CMD)
		exec_cmd(exec_in, leaf->content, env);
	if (leaf->type == OR)
	{
		exec_tree(leaf->left, exec_in, env);
		if (g_exit_status != 0)
			exec_tree(leaf->right, exec_in, env);
	}
	if (leaf->type == AND)
	{
		exec_tree(leaf->left, exec_in, env);
		if (g_exit_status == 0)
			exec_tree(leaf->right, exec_in, env);
	}
	if (leaf->type == PIPE_L)
	{
		exec_tree(leaf->left, exec_in, env);
		exec_tree(leaf->right, exec_in, env);
	}
}

/*
	when should we wait ? we wait at the end of each command
	let's imagine (a && b) | super
	can we wait after each command ?
	after each fork, we wait for the processes to finish
	can we wait at the end for all processes to finish, just like in no bonus ?
	why do we have to wait to the end
	because we just want it to finish all the children before we can go on to another line
	or not ?
*/

//waitpid des qu'on finit la pipeline, ou des qu'on finit une commande
/*
The struct exec_in contains : 

start = tells us if it is the start
end = tells us if it is the end
the exit status is really important and should be handled well
*/
/*
	Ce serait bien de faire le menage dans les fonctions de la 
	partie obligatoire, ca nous permettrait de 
	savoir ce dont on a reellement besoin
	Essayer de fonctionner sans avoir besoin de start et de end
	dans la struct exec_in serait bien
*/
