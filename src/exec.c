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

void	exec_tree(t_leaf *leaf, t_info *exec_in)
{
	char	**argv;

	if (leaf->type == PIPE_L)
	{
		//EXPANNNNND OHOOOOOO
		exec_in->argv = get_cmd_arg(leaf->left->content);
		exec(exec_in, leaf->left->content, pos);
		if (leaf->right)
			exec_tree(leaf->right)
		//get cmd a gauche puis call a droite again :)
//		exec la gauche puis out dans un pipe ou pas en function d'un param
//		Si c'est le last le fd out sera le out sinon un pipe
	}
	else if (leaf->type == CMD)
	{
		// head = cmd on get cmd puis on exec
	}
}
//On pourrait aussi define une value dans la struct line des quon ajoute un token CMD on ++ (pour moins reparcourir la liste)
size_t	get_nb_cmd_arg(t_line *sub)
{
	t_block	*buff;
	size_t	size;

	buff = sub->head;
	size = 0;
	while (buff)
	{
		if (buff->token == CMD)
			size++;
		buff = buff->next;
	}
	return (size);
}

//Return un double tableau avec la commandes et arg
char	*get_cmd_arg(t_line *sub)
{
	t_block		*buff;
	char		**argv;
	int			i;

	argv = (char **) malloc(sizeof(*argv) * (get_nb_cmd_arg(sub) + 1));
	add_to_gc(DOUBLE, argv, get_gc());
	buff = sub->head;
	while (buff)
	{
		if (buff->token == CMD)
		{
			argv[i] = ft_strdup(buff->word);
			if (!argv[i])
				free_exit();
			i++;
		}
	}
	argv[i] = 0;
	return (argv);
}

void	exec(t_info *exec_in, t_line *sub)
{
	check_redirection(exec_in, sub);
	pipe_fork //checking inside abs path or not etc
}

void	check_redirection(t_info *exec, t_line *sub)
{
	t_block	*buff;

	buff = sub->head;
	exec->open_fd = STDIN_FILENO;
	exec->out_fd = STDOUT_FILENO;
	while (buff)
	{
		if (buff->token == RED_IN)
			check_red_in(buff->next, exec);
		else if (buff->token == RED_OUT_TRUNC || buff->token == BUFF_OUT_APPEND)
			check_red_out(buff->next, exec, buff);
		/*
		else if (buff->token == HERE_DOC)
			*/
		buff = buff->next;
	}
}

void	check_red_in(t_block *files, t_info *exec)
{
	int	f_open;

	exec->open_fd = STDIN_FILENO;
	if (ft_strncmp(files->word, "", 2) == 0)
		print_syntax_error();
	//soit on check avec access
//->	//soit on check juste le retour de open
	f_open = open(files->word, O_RDONLY);
	if (f_open == -1)
		p_error(files->word);
	else
		exec->open_fd = f_open;
}

void	check_red_out(t_block *files, t_info *exec, t_block *red)
{
	int	f_open_out;

	exec->out_fd = STDOUT_FILENO;
	if (ft_strncmp(files->word, "", 2) == 0)
		print_syntax_error();
	if (red->token == RED_OUT_TRUNC)
		f_open_out = open(files->word, O_CREAT | O_RDWR | O_TRUNC, 0600);
	else
		f_open_out = open(files->word, O_CREAT | O_RDWR | O_APPEND, 0600);
	if (f_open_out == -1)
		p_error(files->word);
	else
		exec->out_fd = f_open_out;
}

//Function qui open le file + gere les "" special + retour open + access

//Same for out trunc 
//Checker toutes les values du path + la commande si aucune ne fonctionne alors on regarder avec access si c'est un abspath

//On exec a chaque tour la boucle si pas de open open set a 0 et out set a 1 si pas de out

//Funtion special execve pour les built-in aussi : ) 


