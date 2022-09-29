/* ************************************************************************** */
/*                                                                            */ /*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 15:14:15 by odessein          #+#    #+#             */
/*   Updated: 2022/09/29 15:38:49 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
/*
int	exec_tree(t_leaf *leaf, t_info *exec_in)
{
	if (leaf->type == PIPE_L || CMD_ARG)
	{
		//if pipe fork()
		//if cmd exec exec_pipe_line_cmd(leaf);
	}
	else if (leaf->type == OR_L)
	{
		exec_tree(leaf->left, exec_in);
		if (check_exit_status)
			exec_tree(leaf->right, exec_in);
	}
	else if (leaf->type == AND_L)
	{
		exec_tree(leaf->left, exec_in);
		if (check_exit_status)
			exec_tree(leaf->right, exec_in);
	}
	//Check la type de leaf
	//On regarge toujours a gauche tant qu'on a pas de pipe ou une cmd
	//A partir du moment ou on a un pipe : on execc a gauche puis on check a droite
	//En gros quand on a pipe on descend et on recupere le tous de l'exec avec la fonction pipe qui s'executer
	//on recuper l'exit status on remonte sur la feuille precedente
	//en function si on a un retour positif ou pas si c'est un et ou un ou
	//on va voir a droite
	//etc ..
}
*/

//Connexion between pipe

void	exec_cmd(t_info *exec_info, t_leaf *leaf, t_dict *env, t_leaf *prev)
{
	expand(leaf->content, env);
	wildcard(leaf->content);
	exec_info->argv = get_cmd_arg(leaf->content);
	if (exec_info->open_fd != -2)
		exec(exec_info, leaf, env, prev);
}

static void	leaf_type_or(t_leaf *leaf, t_info *exec_in, t_dict *env)
{
	exec_tree(leaf->left, exec_in, env, leaf);
	wait_sub_process(exec_in);
	if (g_exit_status != 0)
		exec_tree(leaf->right, exec_in, env, leaf);
}

static void	leaf_type_and(t_leaf *leaf, t_info *exec_in, t_dict *env)
{
	exec_tree(leaf->left, exec_in, env, leaf);
	wait_sub_process(exec_in);
	if (g_exit_status == 0)
		exec_tree(leaf->right, exec_in, env, leaf);
}



//Checker si on a pipe avant

//Comment stocker les info du pipe precedent ?

//Quand es-ce-qu'on les closes ?

//Si on pipe a gauche et a droite on peux close le pipe quand on aura exec celui de droite


//Si oui lire sur pipe precedent
	//et que deux cmd apres : ecrire sur le fd du premier pipe (celui qu'on aura garder)
//Sinon stdin
//Ecrire sur le pipe precedent ou precedent ++;

//Lire sur le precredent ecrire sur le precedent

//Lire sur le precedent pipe ou ecrire sur le suivant ? Au choix

//Checker si a gauche il y a un pipe
//Si oui : Lire sur le precedent /// Ecrire sur le prochain / Save ou quoi
//Si non : exec la cmd sur le pipe
	//Si pas de pipe a droite : Ecrit sur stdout : read sur le pipe

//A voir comment on rempli la exec_info pour reussir l'exec :)


static void	leaf_type_cmd_pipe(t_leaf *leaf, t_info *exec_in, t_dict *env, t_leaf *prev)
{
	if (leaf->type == PIPE_L)
	{
		exec_in->pipe = TRUE;
		if (prev && prev->type == PIPE_L)
		{
			if (prev->head && exec_in->right)
				leaf->head = TRUE;
			exec_in->prev_pipe = TRUE;
		}
		else
		{
			leaf->head = TRUE;
			exec_in->prev_pipe = FALSE;
			//Read sur stdin :)
		}
		exec_in->left = TRUE;
		exec_in->right = FALSE;
		exec_tree(leaf->left, exec_in, env, leaf);
		exec_in->right = TRUE;
		exec_in->left = FALSE;
		if ((leaf->head && leaf->right->type != PIPE_L && exec_in->right))
			exec_in->end = TRUE;
		exec_tree(leaf->right, exec_in, env, leaf);
	}
	else
	{
		if (!prev)
			exec_in->end = TRUE;
		//ft_putstr_fd("just checking\n", 2);
		exec_cmd(exec_in, leaf, env, prev);
	}
}
/*
static void	leaf_type_parenthese(t_leaf *leaf, t_info *exec_in, t_dict *env)
{
	int		pid;
	t_info	*fork_info;
//	int		yo;
//	int		i;

	pid = 1;
	fork_info = init_exec_info();
	init_pid_lst(fork_info);
//	i = 1;
	ft_putstr_fd("okay how many ?\n", 2);
//	yo = leaf->parentheses - exec_in->par_lvl;
	while (i < yo)
	{
		if (pid > 0)
		{
			pid = fork();
			if (pid < 0)
				return (perror("Error on fork:"));
			pid_li_addback(exec_in->pid_li, new_pid(pid));
		}
		i++;
	}

	if (pid > 0)
	{
		exec_in->par_lvl = leaf->parentheses;
		fork_info->par_lvl = leaf->parentheses;
		pid = fork();
		if (pid < 0)
			return (perror("Error on fork:"));
		pid_li_addback(exec_in->pid_li, new_pid(pid));
	}
	if (pid == 0)
	{
		exec_tree(leaf, fork_info, env, leaf);
		exec_in->par_lvl--;
		free_exit();
	}
//	exec_tree(leaf->left, exec_in, env, leaf);
//	exec_tree(leaf->right, exec_in, env, leaf);
}*/

void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env, t_leaf *prev)
{
	if (leaf->parentheses > exec_in->par_lvl)
	{
		exec_subshell(leaf, exec_in, env, prev);
		exec_in->fork = FALSE;
		return ;
	}
	else if (leaf->type == PIPE_L || leaf->type == CMD)
		leaf_type_cmd_pipe(leaf, exec_in, env, prev);
	else if (leaf->type == OR_L)
		leaf_type_or(leaf, exec_in, env);
	else if (leaf->type == AND_L)
		leaf_type_and(leaf, exec_in, env);
	else if (leaf->type == RED_IN_L)
		exec_tree(leaf->left, exec_in, env, prev);
	else if (leaf->type == RED_OUT_TRUNC_L || leaf->type == RED_OUT_APPEND_L)
		exec_tree(leaf->left, exec_in, env, prev);
}
