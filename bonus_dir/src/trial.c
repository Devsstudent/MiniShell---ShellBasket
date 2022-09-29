/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trial.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 19:40:39 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/09/22 16:54:04 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
/*

static t_bool	check_red_out_tree(t_block *files, t_info *exec, t_type_leaf redir_type)
{
	if (ft_strncmp(files->word, "", 2) == 0)
	{
		print_error(NULL, 1);
		return (FALSE);
	}
	else if (ft_strncmp(files->word, "\"\"", 3) == 0
		|| ft_strncmp(files->word, "\'\'", 3) == 0)
	{
		write(2, " :", 2);
		ft_bzero(files->word, ft_strlen(files->word));
	}
	if (redir_type == RED_OUT_TRUNC_L)
		exec->std = open(files->word, O_CREAT | O_RDWR | O_TRUNC, 0600);
	else
		exec->stdou = open(files->word, O_CREAT | O_RDWR | O_APPEND, 0600);
	if (exec->stdou == -1)
	{
		perror(files->word);
		exec->stdou = dup(STDOUT_FILENO);
		return (FALSE);
	}
	return (TRUE);
}
*/
/*
t_bool	check_red_out_tree(t_block *files, t_info *exec, t_type_leaf red)
{
	if (exec->final_out != -1 && exec->final_out != -2)
		close(exec->final_out);
	exec->final_out = -1;
	if (ft_strncmp(files->word, "", 2) == 0)
	{
		print_error(NULL, 1);
		return (FALSE);
	}
	else if (ft_strncmp(files->word, "\"\"", 3) == 0
		|| ft_strncmp(files->word, "\'\'", 3) == 0)
	{
		write(2, " :", 2);
		ft_bzero(files->word, ft_strlen(files->word));
	}
	if (red == RED_OUT_TRUNC_L)
		exec->final_out = open(files->word, O_CREAT | O_RDWR | O_TRUNC, 0600);
	else
		exec->final_out = open(files->word, O_CREAT | O_RDWR | O_APPEND, 0600);
	if (exec->final_out == -1)
	{
		perror(files->word);
		exec->final_out = -2;
		return (FALSE);
	}
	return (TRUE);
}

void	check_redir_tree(t_type_leaf redir_type, t_block *buff, t_info *exec_in)
{
	if (redir_type == RED_IN_L)
	{
		if (!check_file_permission(buff->word, exec_in, 1))
			return ;
		else if (!check_ambiguous(buff->word, exec_in, TRUE)
			&& !check_red_in(buff, exec_in))
			return ;
		exec_in->first_in = 1;
	}
	if (redir_type == RED_OUT_TRUNC_L || redir_type == RED_OUT_APPEND_L)
	{
		if (!check_file_permission(buff->word, exec_in, 0))
			return ;
		else if (!check_ambiguous(buff->word, exec_in, TRUE)
			&& !check_red_out_tree(buff, exec_in, redir_type))
			return ;
	}
}

void	exec_cmd(t_info *exec_info, t_line *sub, t_dict *env)
{
	expand(sub, env);
	wildcard(sub);
	exec_info->argv = get_cmd_arg(sub);
	if (exec_info->final_out != -2 && exec_info->open_fd != -2)
		exec(exec_info, sub, env);
}

//Reparcourir le tree et verifier qu'on a soit pas de pipe du coter droit, soit qu'on est bien sur un des pipes de droite quoi
t_bool	check_end(t_leaf *leaf, t_info *exec_in, t_leaf *pos)
{
	t_leaf	*buff;
	t_bool	check;

	(void) exec_in;
	buff = pos;
	check = FALSE;
	while (pos)
	{
		if (pos->type == PIPE_L)
			check = TRUE;
		if (pos->right && pos->right->type == CMD)
		{
			if (pos->right == leaf && check)
			{
				printf("YES ASAP\n");
				printf("%s\n", pos->right->content->head->word);
				return (TRUE);
			}
			else
				return (FALSE);
		}
		pos = pos->right;
	}
	check = FALSE;
	while (buff)
	{
		if (buff->type == PIPE_L)
			check = TRUE;
		if (buff->right && buff->right->type == CMD)
		{
			if (buff->right == leaf && check)
			{
				printf("YES ASAF\n");
				printf("%s", buff->right->content->head->word);
				return (TRUE);
			}
		}
		buff = buff->left;
	}
	return (FALSE);
}

void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env, t_tree *tree)
{
	if (!leaf)
		return ;
	t_block *buff;
	if (leaf->content)
	{
		buff = leaf->content->head;
		while (buff)
		{
			printf("%s \n", buff->word);
			buff = buff->next;
		}
	}
	else
		printf("??\n");
	if (leaf->type == CMD)
	{
		if (exec_in->pipe == FALSE)
			exec_in->end = TRUE;
		exec_cmd(exec_in, leaf->content, env);
	}
	if (leaf->type == OR_L)
	{
		exec_tree(leaf->left, exec_in, env, tree);
		wait_sub_process(exec_in);
		if (g_exit_status != 0)
			exec_tree(leaf->right, exec_in, env, tree);
	}
	if (leaf->type == AND_L)
	{
		exec_tree(leaf->left, exec_in, env, tree);
		wait_sub_process(exec_in);
		if (g_exit_status == 0)
			exec_tree(leaf->right, exec_in, env, tree);
	}
	if (leaf->type == PIPE_L)
	{
		if (pipe(exec_in->pipe_fd) < 0 && !exec_in->end)
			return (perror("pipe_exec_tree CRASH"));
		exec_in->pipe = TRUE;
		exec_tree(leaf->left, exec_in, env, tree);
		printf("CMD :%s\n", leaf->right->content->head->word);
		if (check_end(leaf->right, exec_in, tree->head))
			exec_in->end = TRUE;
		exec_tree(leaf->right, exec_in, env, tree);
	}
	if (leaf->type == RED_IN_L || leaf->type == RED_OUT_TRUNC_L
		|| leaf->type == RED_OUT_APPEND_L)
	{
		check_redir_tree(leaf->type, leaf->right->content->head, exec_in);
		printf("exec final out = %i\n", exec_in->final_out);
		exec_tree(leaf->left, exec_in, env, tree);
		exec_in->final_out = -1;
	}
}
//pipe on check a gauche du suivant si on a une cmd ou pas
*/
/*
t_bool	check_end_pipe_line(t_leaf *leaf, t_info *exec_in)
{
	if ((leaf->type == OR_L || leaf->type == AND_L) && exec_in->turn != 0)
	{
		//wait les pid + free  les block de pid 
		exec_in->turn = 0;
		//dup back stdout stdin
		return (TRUE);
	}
	return (FALSE);
}*/


//tant que pipeline on turn++ et on ajoute les pid a une liste chainee

//function qui va checker si on fini la pipe_line ou pas

//des que c'est fini on waiit tous les pid + free la liste / reset turn etc

//Si on a un pipe on fork // Pareille pour les parentheses



//pipe on check a gauche du suivant si on a une cmd ou pas


//the last wait_processes has to be at the end of main like in mandatory part*/
