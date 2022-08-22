/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 14:37:32 by odessein          #+#    #+#             */
/*   Updated: 2022/08/22 19:55:05 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_bool	exec_builtin(char **argv, t_dict *env);
t_bool	execve_test(char *pathname, char **argv, t_dict *env);
size_t	get_ac(char **argv);
t_bool	check_builtins(char **argv);
void	forking(char *cmd_path, t_info *exec_in, t_dict *env, int pipe_fd[2]);
//Brows le tree

//Geree les lines cmd et arguments + les redirections

//Loop d'exec :
	//while we have redirection : in -> check les permission / l'existance des fichiers etc pour les erreurs and care if its a "" char
	//out trunc or append -> check permission + creation if not created  car if it's a "" char
	//Here_doc Special :)
	//Exec les commandes / built-in
	//With forking and piping if necessary
void	forking_cmd_alone(char *cmd_path, t_info *exec_in, t_dict *env);
void	exec_cmd(t_info *exec_in, t_line *sub, t_dict *env);

void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env, t_tree *tree)
{
	if (!leaf)
		return ;
	if (leaf->type == PIPE_L)
	{
		//EXPANNNNND OHOOOOOO
		if (leaf == tree->head)
			exec_in->start = TRUE;
		else
			exec_in->start = FALSE;
		if (!leaf->right)
			exec_in->end = TRUE;
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
		exec_in->argv = get_cmd_arg(leaf->content);
		exec_cmd(exec_in, leaf->content, env);
		// head = cmd on get cmd puis on exec
	}
	//ft_printf(0, "\n\nEND\n");
}

void	exec_cmd(t_info *exec_in, t_line *sub, t_dict *env)
{
	char		*cmd_path;

	check_redirection(exec_in, sub);
	cmd_path = check_cmd(exec_in->argv, env);
	if (!cmd_path)
	{
		print_error(exec_in->argv[0], 2);
		return ;
	}
	forking_cmd_alone(cmd_path, exec_in, env);
	if (exec_in->open_fd != -1)
		close(exec_in->open_fd);
	if (exec_in->out_fd != -1)
		close(exec_in->out_fd);
	exec_in->turn++;
}

void	forking_cmd_alone(char *cmd_path, t_info *exec_in, t_dict *env)
{
	exec_in->pid[exec_in->turn] = fork();
	if (exec_in->pid[exec_in->turn] < 0)
		return (perror("shellbasket"));
	if (exec_in->open_fd != -1)
		if (dup2(exec_in->open_fd, STDIN_FILENO) == -1)
			return (perror("shellbasket"));
	if (exec_in->out_fd != -1)
		if (dup2(exec_in->out_fd, STDOUT_FILENO) == -1)
			return (perror("shellbasket"));
	if (exec_in->pid[exec_in->turn] == 0)
	{
		if (exec_in->open_fd != -1)
			close(exec_in->open_fd);
		if (exec_in->out_fd != -1)
			close(exec_in->out_fd);
		if (!execve_test(cmd_path, exec_in->argv, env))
			return (perror(exec_in->argv[0]));
	}
	free(cmd_path);
}
//On pourrait aussi define une value dans la struct line des quon ajoute un token CMD on ++ (pour moins reparcourir la liste)
size_t	get_nb_cmd_arg(t_line *sub)
{
	t_block	*buff;
	size_t	size;
	int		j;

	buff = sub->head;
	j = 0;
	size = 0;
	while (buff)
	{
		if (buff->token == CMD_ARG)
		{
			while (buff->word[i])
			{
				if (buff->word[i] == ' ' && buff->word[i + 1] && buff->word[i + 1] != ' ')
					size++;
				i++;
			}
			size++;
		}
		buff = buff->next;
	}
	//ft_printf(0, "%i", (int) size);
	return (size);
}

//Return un double tableau avec la commandes et arg
char	**get_cmd_arg(t_line *sub)
{
	t_block		*buff;
	char		**argv;
	int			i;
	int			j;
	char		**space_or_not;

	i = 0;
	argv = (char **) malloc(sizeof(*argv) * (get_nb_cmd_arg(sub) + 1));
	add_to_gc(DOUBLE, argv, get_gc());
	buff = sub->head;
	while (buff)
	{
		if (buff->token == CMD_ARG)
		{
			argv[i] = NULL;
			j = 0;
			//Split a la manooo
			space_or_not = ft_split(buff->word, ' ');
			while (space_or_not && space_or_not[j])
			{
				argv[i] = sapce_or_not[j];
				j++;
				i++;
			}
			if (!argv[i])
			{
				free(space_or_not);
				free_exit();
			}
			free(space_or_not);
			i++;
		}
		buff = buff->next;
	}
	argv[i] = 0;
	return (argv);
}

void	exec(t_info *exec_in, t_line *sub, t_dict *env)
{
	char		*cmd_path;
	int			pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (perror("shellbasket"));
	check_redirection(exec_in, sub);
	cmd_path = check_cmd(exec_in->argv, env);
	if (!cmd_path)
	{
		print_error(exec_in->argv[0], 2);
		if (exec_in->tmp_fd != -1)
			close(exec_in->tmp_fd);
		exec_in->tmp_fd = pipe_fd[0];
		close(pipe_fd[1]);
		return ;
	}
	forking(cmd_path, exec_in, env, pipe_fd);
	if (exec_in->open_fd != -1)
		close(exec_in->open_fd);
	if (exec_in->out_fd != -1)
		close(exec_in->out_fd);
	exec_in->turn++;
}

void	forking(char *cmd_path, t_info *exec_in, t_dict *env, int pipe_fd[2])
{
	exec_in->pid[exec_in->turn] = fork();
	if (exec_in->pid[exec_in->turn] < 0)
		return (perror("shellbasket"));
	if (exec_in->end)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (exec_in->out_fd != -1)
	{
		if (dup2(exec_in->out_fd, STDOUT_FILENO) == -1)
			return (perror("basket"));
	}
	else if (!exec_in->end)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			return (perror("shell"));
	}
	else
	{
		if (dup2(exec_in->stdou, STDOUT_FILENO) == -1)
			return (perror("sheet"));
		close(exec_in->stdou);
		exec_in->stdou = -1;
	}
	if (exec_in->open_fd != -1)
	{
		if (dup2(exec_in->open_fd, STDIN_FILENO) == -1)
			return (perror("set"));
	}
	else if (exec_in->tmp_fd != -1 && !exec_in->start)
	{
		if (dup2(exec_in->tmp_fd, STDIN_FILENO) == -1)
			return (perror("lasy"));
	}
	if (exec_in->pid[exec_in->turn] == 0)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		if (exec_in->open_fd != -1)
			close(exec_in->open_fd);
		if (exec_in->out_fd != -1)
			close(exec_in->out_fd);
		if (exec_in->tmp_fd != -1)
			close(exec_in->tmp_fd);
		if (!execve_test(cmd_path, exec_in->argv, env))
			return (perror(exec_in->argv[0]));
	}
	if (exec_in->tmp_fd != -1)
		close(exec_in->tmp_fd);
	exec_in->tmp_fd = pipe_fd[0];
	//find a wait all pid after the exec so need to store each pid;
}

char	*check_cmd(char **argv, t_dict *env)
{
	char	**path_li;
	int		i;
	char	*res;
	char	*buff;

	//builtins
	i = 0;
	res = NULL;
	path_li = ft_split(dict_get_value(env, "PATH"), ':');
	while (path_li[i])
	{
		buff = ft_strjoin(ft_strdup(path_li[i]), "/");
		buff = ft_strjoin(buff, argv[0]);
		if (access(buff, X_OK) == 0)
		{
			res = buff;
			break;
		}
		free(buff);
		i++;
	}
	if (!res)
	{
		if (access(argv[0], X_OK) == 0)
		{
			res = ft_strdup(argv[0]);
			if (!res)
				free_exit();
		}
	}
	int	j;
	j = 0;
	while (path_li && path_li[j])
	{
		free(path_li[j]);
		j++;
	}
	free(path_li);
	add_to_gc(SIMPLE, res, get_gc());
	return (res);
}

void	check_redirection(t_info *exec, t_line *sub)
{
	t_block	*buff;

	if (sub->head)
		buff = sub->head;
	else
		return ;
	exec->open_fd = -1;
	exec->out_fd = -1;
	while (buff)
	{
		if (buff->token == RED_IN)
			check_red_in(buff->next, exec);
		else if (buff->token == RED_OUT_TRUNC || buff->token == RED_OUT_APPEND)
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

	//before it was :
	// if (exec->out_fd != STDIN_FILENO)
		//close (exec->out_fd);
	//so one fd was left open
	if (exec->open_fd != -1)
		close(exec->open_fd);
	exec->open_fd = -1;
	//Va falloir revoir a fond l'expand xd ca reste la meme idee mais bon pck dans l'ideal pour l'error on a besoin de la value avant l'expand (a voir apres)
	if (ft_strncmp(files->word, "", 2) == 0)
		print_error(NULL, 1);
	else if (ft_strncmp(files->word, "\"\"", 3) == 0 || ft_strncmp(files->word, "\'\'", 3) == 0)
	{
		files->word[0] = ' ';
		ft_bzero(files->word + 1, ft_strlen(files->word) - 1);
	}
	//soit on check avec access
//->	//soit on check juste le retour de open
	f_open = open(files->word, O_RDONLY);
	if (f_open == -1)
		perror(files->word);
	else
		exec->open_fd = f_open;
	if (errno == 13)
		free_exit();
}

void	check_red_out(t_block *files, t_info *exec, t_block *red)
{
	int	f_open_out;

	if (exec->out_fd != -1)
		close(exec->out_fd);
	exec->out_fd = -1;
	if (ft_strncmp(files->word, "", 2) == 0)
	{
		print_error(NULL, 1);
		return ;
	}
	else if (ft_strncmp(files->word, "\"\"", 3) == 0 || ft_strncmp(files->word, "\'\'", 3) == 0)
	{
		//ft_printf(1, " :");
		ft_bzero(files->word, ft_strlen(files->word));
	}
	if (red->token == RED_OUT_TRUNC)
		f_open_out = open(files->word, O_CREAT | O_RDWR | O_TRUNC, 0600);
	else
		f_open_out = open(files->word, O_CREAT | O_RDWR | O_APPEND, 0600);
	if (f_open_out == -1)
		perror(files->word);
	else
		exec->out_fd = f_open_out;
	if (errno == 13)
		free_exit();
}

t_bool	check_builtins(char **argv)
{
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "cd", 3) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "export", 7) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		return (TRUE);
	else if (ft_strncmp(argv[0], "unset", 6) == 0)
		return (TRUE);
	return (FALSE);
}

size_t	get_ac(char **argv)
{
	int	ac;

	ac = 0;
	while (argv[ac])
		ac++;
	return (ac);
}

t_bool	exec_builtin(char **argv, t_dict *env)
{
	int	ac;

	(void) env;
	ac = get_ac(argv);
	(void) ac;
	/*
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		exec_echo(ac, argv, env);
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		exec_exit(ac, argv, env);
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		exec_pwd(ac, argv, env);
	else if (ft_strncmp(argv[0], "cd", 3) == 0)
		exec_cd(ac, argv, env);
	else if (ft_strncmp(argv[0], "export", 7) == 0)
		exec_export(ac, argv, env);
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		exec_env(ac, argv, env);
	else if (ft_strncmp(argv[0], "unset", 6) == 0)
		exec_unset(ac, argv, env);
	*/
	return (FALSE);
}
//exit dans les builr in
t_bool	execve_test(char *pathname, char **argv, t_dict *env)
{
	char	**env_bis;

	env_bis = dict_to_double_char(env);
	if (!exec_builtin(argv, env))
	{
		if (execve(pathname, argv, env_bis) == -1)
		{
			return (FALSE);
		}
	}
	return (TRUE);
			
}

//Function qui open le file + gere les "" special + retour open + access

//Same for out trunc
//Checker toutes les values du path + la commande si aucune ne fonctionne alors on regarder avec access si c'est un abspath

//On exec a chaque tour la boucle si pas de open open set a 0 et out set a 1 si pas de out

//Funtion special execve pour les built-in aussi : )


