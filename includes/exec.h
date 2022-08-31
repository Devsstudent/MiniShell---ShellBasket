/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:03:12 by odessein          #+#    #+#             */
/*   Updated: 2022/08/31 20:17:12 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXEC_H
# define EXEC_H

# include "enum.h"
# include "struct.h"

/********************************************/
/*                   exec                   */
/********************************************/
void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env, t_tree *tree);
void	exec(t_info *exec_in, t_line *sub, t_dict *env);
size_t	get_nb_cmd_arg(t_line *sub);
char	**get_cmd_arg(t_line *sub);
void	check_redirection(t_info *exec, t_line *sub);
void	check_red_out(t_block *files, t_info *exec, t_block *red);
void	check_red_in(t_block *files, t_info *exec);
char	*check_cmd(char **argv, t_dict *env);
void	forking_cmd_alone(char *cmd_path, t_info *exec_in, t_dict *env);
void	forking(char *cmd_path, t_info *exec_in, t_dict *env, int pipe_fd[2]);
void	get_size_word_in_word(char *word, size_t *size);
void	loop_get_arg(char *word, char **argv, int *i);
void	browse_line_check_red_in(t_leaf *leaf, t_dict *env);
char	*handle_quote(char *word);
int	total_block(t_leaf *leaf);
void	close_subprocess_fd(t_info *exec_in, int pipe_fd[2]);
t_bool	dup_in_pipe(t_info *exec_in, int pipe_fd[2]);
t_bool	dup_cmd_alone(t_info *exec_in, int pipe_fd[2]);
void	init_loop_get_arg(int *j, int *last, t_bool *quote, t_bool *d_quote);

/********************************************/
/*                 execve                   */
/********************************************/
t_bool	check_builtins(char **argv);
size_t	get_ac(char **argv);
t_bool	exec_builtin(char **argv, t_dict *env, t_bool fork);
t_bool	execve_test(char *pathname, char **argv, t_dict *env, t_bool fork);


void	exec_cd(int ac, char **argv, t_dict *env);
void	exec_echo(int ac, char **argv, t_dict *env);
void	exec_exit(int ac, char **argv, t_bool display_exit);
void	exec_unset(int ac, char **argv, t_dict *env);
void	exec_export(int ac, char **argv, t_dict *env);
void	exec_env(int ac, char **argv, t_dict *env);
void	exec_pwd(int ac, char **argv, t_dict *env);
void	execve_cmd_alone(char *cmd_path, t_dict *env, t_info *exec_in);

t_bool	init_export(char *arg, int *i, t_bool *append);
t_bool	check_sign_in_export(char *arg, int i, t_bool *append);
void	handle_key(char *key, t_dict *env);
#endif
