#include "minishell.h"
/*
int	exec_tree(t_leaf *leaf, t_info *exec_in)
{
	if (leaf->type == PIPE_L || CMD_ARG)
	{
		//if pipe fork()
		//if cmd exec
		exec_pipe_line_cmd(leaf);
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
