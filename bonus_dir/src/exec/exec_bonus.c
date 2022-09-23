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

void	exec_cmd(t_info *exec_info, t_line *sub, t_dict *env)
{
	expand(sub, env);
	wildcard(sub);
	exec_info->argv = get_cmd_arg(sub);
	if (exec_info->final_out != -2 && exec_info->open_fd != -2)
		exec(exec_info, sub, env);
}

static void	leaf_type_or(t_leaf *leaf, t_info *exec_in, t_dict *env, t_side side)
{
	if (side == NONE)
	{
		side = LEFT;
		exec_tree(leaf->left, exec_in, env, TRUE);
		wait_sub_process(exec_in);
		if (g_exit_status != 0)
		{
			side = RIGHT;
			exec_tree(leaf->right, exec_in, env, FALSE);
		}
	}
	else
	{
		exec_tree(leaf->left, exec_in, env, TRUE);
		wait_sub_process(exec_in);
		if (g_exit_status != 0)
			exec_tree(leaf->right, exec_in, env, FALSE);

	}
}

static void	leaf_type_and(t_leaf *leaf, t_info *exec_in, t_dict *env, t_side side)
{
	if (side == NONE)
	{
		side = LEFT;
		exec_tree(leaf->left, exec_in, env, TRUE);
		wait_sub_process(exec_in);
		if (g_exit_status == 0)
		{
			side = RIGHT;
			exec_tree(leaf->right, exec_in, env, FALSE);
		}
	}
	else
	{
		exec_tree(leaf->left, exec_in, env, side);
		wait_sub_process(exec_in);
		if (g_exit_status == 0)
		exec_tree(leaf->right, exec_in, env, side);
	}
}

static void	leaf_type_cmd_pipe(t_leaf *leaf, t_info *exec_in, t_dict *env, t_side side)
{
	if (side == NONE)
	{
		side = LEFT;
		if (leaf->type == PIPE_L)
		{
			if (pipe(exec_in->pipe_fd) == -1)
				return (perror("CRASH PIPE EXEC"));
			exec_tree(leaf->left, exec_in, env, side);
			side = RIGHT;
			if (leaf->right->type == CMD && side == RIGHT)
			{
				ft_putstr_fd("END ??\n", 2);
				exec_in->end = TRUE;
			}
			exec_tree(leaf->right, exec_in, env, side);
		}
		else
		{
			side = RIGHT;
			exec_cmd(exec_in, leaf->content, env);
		}
	}
	else
	{
		if (leaf->type == PIPE_L)
		{
			if (pipe(exec_in->pipe_fd) == -1)
				return (perror("CRASH PIPE EXEC"));
			exec_tree(leaf->left, exec_in, env, side);
			if (leaf->right->type == CMD || (leaf->right->type == CMD && leaf->parent_type != PIPE_L))
			{
				ft_putstr_fd("END ??\n", 2);
				exec_in->end = TRUE;
			}
			exec_tree(leaf->right, exec_in, env, side);
			exec_in->pipe = TRUE;
		}
		else
			exec_cmd(exec_in, leaf->content, env);

	}
}

void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env, t_side side)
{
	if (!leaf)
		return ;
	if (leaf->type == PIPE_L || leaf->type == CMD)
	{
		leaf_type_cmd_pipe(leaf, exec_in, env, side);
	}
	else if (leaf->type == OR_L)
	{
		leaf_type_or(leaf, exec_in, env, side);
	}
	else if (leaf->type == AND_L)
	{
		leaf_type_and(leaf, exec_in, env, side);
	}
}
