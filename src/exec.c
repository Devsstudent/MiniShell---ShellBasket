/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 14:37:32 by odessein          #+#    #+#             */
/*   Updated: 2022/08/20 14:42:52 by odessein         ###   ########.fr       */
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

void	exec_tree(t_leaf *leaf)
{
	if (leaf->type == PIPE_L)
	{
		//get cmd a gauche puis call a droite again :)
//		exec la gauche puis out dans un pipe ou pas en function d'un param
//		Si c'est le last le fd out sera le out sinon un pipe
	}
	else if (leaf->type == CMD)
	{
		// head = cmd on get cmd puis on exec
	}
}

//Return un double tableau avec la commandes et arg
char	*get_cmd_arg(t_line *sub)
{
	t_block		*buff;
	t_cmd_arg	*li;
	t_bool		cmd;

	li = malloc(sizeof(t_cmd_arg));
	li->head = NULL;
	cmd = FALSE;
	buff = sub->head;
	while (buff)
	{
		if (buff->token == CMD && !cmd)
			fill_cmd(buff, li);
		else if (buff->token == CMD && cmd)
			fill_arr(buff, li);
		buff = buff->next;
	}
	return (argv);
}

void	fill_arr(t_block *buff, t_cmd_arg *li)
{
	t_li_exec	*buffer;

	buffer = li->head;
	while (buffer->next)
		buffer = buffer->next;
	buffer->next = ft_strdup(buff->word);
	if (!buffer->next)
		free_exit();
}

void	fill_cmd(t_block *buff, t_cmd_arg *li)
{
	li->head = ft_strdup(buff->word);
	if (!li->head)
		free_exit();
	li->first = li->head;
}

//Function qui open le file + gere les "" special + retour open + access

//Same for out trunc etc

//Checker toutes les values du path + la commande si aucune ne fonctionne alors on regarder avec access si c'est un abspath

//On exec a chaque tour la boucle si pas de open open set a 0 et out set a 1 si pas de out

//Funtion special execve pour les built-in aussi : ) 


