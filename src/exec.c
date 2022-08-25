/* ************************************************************************* */
/*                                                                            */ /*                                                        :::      ::::::::   */ /*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 14:37:32 by odessein          #+#    #+#             */
/*   Updated: 2022/08/24 12:53:18 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//Brows le tree

//Geree les lines cmd et arguments + les redirections

//Loop d'exec :
	//while we have redirection : in -> check les permission / l'existance des fichiers etc pour les erreurs and care if its a "" char
	//out trunc or append -> check permission + creation if not created  car if it's a "" char
	//Here_doc Special :)
	//Exec les commandes / built-in
	//With forking and piping if necessary

static void	exec_cmd(t_info *exec_in, t_line *sub, t_dict *env)
{
	char		*cmd_path;
	int			ac;
	int			i;

	i = 0;
	check_redirection(exec_in, sub);
	while (exec_in->argv[i])
	{
		exec_in->argv[i] = handle_quote(exec_in->argv[i]);
		i++;
	}
	cmd_path = check_cmd(exec_in->argv, env);
	if (!cmd_path|| exec_in->open_fd == -2)
	{
		if (exec_in->open_fd != -2 && !cmd_path)
			print_error(exec_in->argv[0], 2);
		return ;
	}
	ac = get_ac(exec_in->argv);
	if (ft_strncmp(cmd_path, "exit", 5) == 0)
		exec_exit(ac, exec_in->argv, env);
	else if (ft_strncmp(cmd_path, "cd", 3) == 0)
		exec_cd(ac, exec_in->argv, env);
	else if (ft_strncmp(cmd_path, "export", 7) == 0)
		exec_export(ac, exec_in->argv, env);
	else if (ft_strncmp(cmd_path, "unset", 6) == 0)
		exec_unset(ac, exec_in->argv, env);
	else if (ft_strncmp(cmd_path, "pwd", 4) == 0)
		exec_pwd(ac, exec_in->argv, env);
	else if (ft_strncmp(cmd_path, "env", 4) == 0)
		exec_env(ac, exec_in->argv, env);
	else if (ft_strncmp(cmd_path, "echo", 5) == 0)
		exec_echo(ac, exec_in->argv, env);
	else
		forking_cmd_alone(cmd_path, exec_in, env);
	if (exec_in->open_fd != -1)
		close(exec_in->open_fd);
	if (exec_in->out_fd != -1)
		close(exec_in->out_fd);
	exec_in->turn++;
}

void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env, t_tree *tree)
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
		//get cmd a gauche puis call a droite again :)
//		exec la gauche puis out dans un pipe ou pas en function d'un param
//		Si c'est le last le fd out sera le out sinon un pipe
	}
	else if (leaf->type == CMD)
	{
		expand(leaf->content, env);
		exec_in->argv = get_cmd_arg(leaf->content);
		exec_cmd(exec_in, leaf->content, env);
		// head = cmd on get cmd puis on exec
	}
	//ft_printf(0, "\n\nEND\n");
}

//On pourrait aussi define une value dans la struct line des quon ajoute un token CMD on ++ (pour moins reparcourir la liste)
//Return un double tableau avec la commandes et arg

void	exec(t_info *exec_in, t_line *sub, t_dict *env)
{
	char		*cmd_path;
	int			pipe_fd[2];
	int		i;


	i = 0;
	if (pipe(pipe_fd) == -1)
		return (perror("pipe"));
	check_redirection(exec_in, sub);
	cmd_path = check_cmd(exec_in->argv, env);
	while (exec_in->argv[i])
	{
		exec_in->argv[i] = handle_quote(exec_in->argv[i]);
		i++;
	}
	if (!cmd_path || exec_in->open_fd == -2)
	{
		close(pipe_fd[1]);
		exec_in->tmp_fd = pipe_fd[0];
		if (exec_in->open_fd != -2 && !cmd_path)
			print_error(exec_in->argv[0], 2);
		return ;
	}
	forking(cmd_path, exec_in, env, pipe_fd);
	close(pipe_fd[1]);
	//if (exec_in->tmp_fd != -1)
	//	close(exec_in->tmp_fd);
	exec_in->tmp_fd = pipe_fd[0];
	if (exec_in->open_fd != -1)
		close(exec_in->open_fd);
	if (exec_in->out_fd != -1)
		close(exec_in->out_fd);
	exec_in->turn++;
}



//Function qui open le file + gere les "" special + retour open + access

//Same for out trunc
//Checker toutes les values du path + la commande si aucune ne fonctionne alors on regarder avec access si c'est un abspath

//On exec a chaque tour la boucle si pas de open open set a 0 et out set a 1 si pas de out

//Funtion special execve pour les built-in aussi : )


