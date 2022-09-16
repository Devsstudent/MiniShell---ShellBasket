/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:40:53 by odessein          #+#    #+#             */
/*   Updated: 2022/09/16 14:08:29 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

/*static void	exec_cmd(t_info *exec_in, t_line *sub, t_dict *env)
{
	char		*cmd_path;
	int			i;

	i = 0;
	check_redirection(exec_in, sub);
	while (exec_in->argv[i])
	{
		exec_in->argv[i] = handle_quote(exec_in->argv[i]);
		i++;
	}
	cmd_path = check_cmd(exec_in->argv, env);
	add_to_gc(SIMPLE, cmd_path, get_gc());
	if (!cmd_path || exec_in->open_fd == -2)
	{
		if (errno != 13 && exec_in->open_fd != -2 && !cmd_path
			&& check_cmd_in_sub(sub))
			print_error(exec_in->argv[0], 2);
		return ;
	}
	execve_cmd_alone(cmd_path, env, exec_in);
	if (exec_in->open_fd != -1 && exec_in->open_fd != -2)
		close(exec_in->open_fd);
	if (exec_in->out_fd != -1 && exec_in->out_fd != -2)
		close(exec_in->out_fd);
	exec_in->turn++;
}*/

/*void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env, t_tree *tree)
{
	if (!leaf)
		return ;
	if (leaf->type == PIPE_L)
	{
		if (leaf == tree->head)
			exec_in->start = TRUE;
		else
			exec_in->start = FALSE;
		if (!leaf->right)
			exec_in->end = TRUE;
		expand(leaf->left->content, env);
		exec_in->argv = get_cmd_arg(leaf->left->content);
		exec(exec_in, leaf->left->content, env);
		if (leaf->right)
			exec_tree(leaf->right, exec_in, env, tree);
	}
	else if (leaf->type == CMD)
	{
		expand(leaf->content, env);
		exec_in->argv = get_cmd_arg(leaf->content);
		exec_cmd(exec_in, leaf->content, env);
	}
}
*/
//On pourrait aussi define une value dans la struct line des quon ajoute un
//token CMD on ++ (pour moins reparcourir la liste)
//Return un double tableau avec la commandes et arg

void	execute_cmd(t_info *exec_in, t_dict *env, char *cmd_path)
{
	if (exec_in->pipe || !check_builtins(exec_in->argv))
		forking(cmd_path, exec_in, env);
	else
		execve_builtin_alone(cmd_path, env, exec_in);
	if (exec_in->end)
	{
		close(exec_in->pipe_fd[0]);
		close(exec_in->pipe_fd[1]);
	}
	if (exec_in->open_fd != -1 && exec_in->open_fd != -2)
		close(exec_in->open_fd);
	if (exec_in->out_fd != -1 && exec_in->out_fd != -2)
		close(exec_in->out_fd);
	exec_in->turn++;
}

void	exec(t_info *exec_in, t_line *sub, t_dict *env)
{
	char	*cmd_path;
	int		i;

	i = -1;
//	if (exec_in->end)
	//	printf("TEST\n");
	check_redirection(exec_in, sub);
	while (exec_in->argv[++i])
		exec_in->argv[i] = handle_quote(exec_in->argv[i]);
	cmd_path = check_cmd(exec_in->argv, env);
	add_to_gc(SIMPLE, cmd_path, get_gc());
	if (command_not_found(exec_in, cmd_path, sub))
	{
		if (exec_in->end)
			exec_in->cmd_not_found = TRUE;
		if (exec_in->open_fd != -1 && exec_in->open_fd != -2)
			close(exec_in->open_fd);
		if (exec_in->out_fd != -1 && exec_in->out_fd != -2)
			close(exec_in->out_fd);
		return ;
	}
	execute_cmd(exec_in, env, cmd_path);
}
