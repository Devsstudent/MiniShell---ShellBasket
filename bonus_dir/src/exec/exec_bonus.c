#include "minishell.h"

void	exec_cmd(t_info *exec_info, t_line *sub, t_dict *env)
{
	expand(sub, env);
	wildcard(sub);
	exec_info->argv = get_cmd_arg(sub);
	if (exec_info->final_out != -2 && exec_info->open_fd != -2)
		exec(exec_info, sub, env);
}

static void	leaf_type_or(t_leaf *leaf, t_info *exec_in, t_dict *env)
{
	exec_tree(leaf->left, exec_in, env);
	wait_sub_process(exec_in);
	if (g_exit_status != 0)
		exec_tree(leaf->right, exec_in, env);
}

static void	leaf_type_and(t_leaf *leaf, t_info *exec_in, t_dict *env)
{
	exec_tree(leaf->left, exec_in, env);
	wait_sub_process(exec_in);
	if (g_exit_status == 0)
		exec_tree(leaf->right, exec_in, env);
}

//imagine we have a subshell
//we fork, then in the fork, we execute the rest of the tree
//when we fork, we have to have a clean slate, a new exec_in, that can be filled with new instructions, pipes, and everything, once it is over, we wait then go back to reality, ooh there goes gravity ooh

static void	leaf_type_cmd_pipe(t_leaf *leaf, t_info *exec_in, t_dict *env)
{
	if (leaf->type == CMD)
		return (exec_cmd(exec_in, leaf->content, env));
	if (pipe(exec_in->pipe_fd) == -1)
		return (perror("CRASH PIPE EXEC"));
	exec_tree(leaf->left, exec_in, env);
	exec_tree(leaf->right, exec_in, env);
}

void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env)
{
	static int	PAR;

	if (leaf->parentheses > PAR)
	{
		exec_subshell(leaf, exec_in, env, &PAR);
		return ;
	}
	if (leaf->type == PIPE_L || leaf->type == CMD)
		leaf_type_cmd_pipe(leaf, exec_in, env);
	else if (leaf->type == OR_L)
		leaf_type_or(leaf, exec_in, env);
	else if (leaf->type == AND_L)
		leaf_type_and(leaf, exec_in, env);
}
