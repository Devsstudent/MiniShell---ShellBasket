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

//Connexion between pipe

void	exec_cmd(t_info *exec_info, t_line *sub, t_dict *env)
{
	expand(sub, env);
	wildcard(sub);
	exec_info->argv = get_cmd_arg(sub);
	if (exec_info->final_out != -2 && exec_info->open_fd != -2)
		exec(exec_info, sub, env);
}

static void	leaf_type_or(t_leaf *leaf, t_info *exec_in, t_dict *env, t_type_leaf prev)
{
	(void) prev;
	exec_tree(leaf->left, exec_in, env, leaf);
	wait_sub_process(exec_in);
	if (g_exit_status != 0)
		exec_tree(leaf->right, exec_in, env, leaf);
}

static void	leaf_type_and(t_leaf *leaf, t_info *exec_in, t_dict *env, t_type_leaf prev)
{
	(void) prev;
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
	(void) prev;
	if (leaf->type == PIPE_L)
	{
		if (prev == PIPE_L && exec_in->left == TRUE)
			exec_in->stdout_pipe = exec_in->pipe_fd[1];
		if (pipe(leaf->pipe_fd) == -1)
			return (perror("CRASH PIPE EXEC"));
		if (prev->type == PIPE_L)
		{
			//On va faire un dup sur prev->pipe_fd[0] pour la cmd de gauche
			//Ecrire sur le pipe actuel pour la cmd de gauche
			//Lire sur le pipe actuel pour la cmd de droite
			//On va faire un dup sur prev->pipe_fd[1] pour la cmd de droite 
		}
		else
			//Read sur stdin :)
		if (leaf->left->type == PIPE_L)
		{
			//
		}
		else
			//exec left sur le pipe
		exec_in->left = TRUE;
		exec_in->right = FALSE;
		exec_tree(leaf->left, exec_in, env, leaf);
		exec_in->right = TRUE;
		exec_in->left = FALSE;
		exec_tree(leaf->right, exec_in, env, leaf);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		exec_in->stdout_pipe = -1;
	}
	else
		exec_cmd(exec_in, leaf->content, env);
}

void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env, t_leaf *prev)
{
	if (!leaf)
		return ;
	if (leaf->type == PIPE_L || leaf->type == CMD)
		leaf_type_cmd_pipe(leaf, exec_in, env, prev);
	else if (leaf->type == OR_L)
		leaf_type_or(leaf, exec_in, env, prev);
	else if (leaf->type == AND_L)
		leaf_type_and(leaf, exec_in, env, prev);
}
