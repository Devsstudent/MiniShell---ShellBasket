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
//forking.c
void	forking(char *cmd_path, t_info *exec_in, t_dict *env, int pipe_fd[2]);
void	forking_cmd_alone(char *cmd_path, t_info *exec_in, t_dict *env);

//exec.c
void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env, t_tree *tree);
t_bool	command_not_found(int pipe_fd[2], t_info *exec_in, char *cmd_path, t_line *sub);
void	exec(t_info *exec_in, t_line *sub, t_dict *env);

//handle_cmd_utils.c
size_t	get_nb_cmd_arg(t_line *sub);
char	**get_cmd_arg(t_line *sub);
void	get_size_word_in_word(char *word, size_t *size);
void	init_loop_get_arg(int *j, int *last, t_bool *quote, t_bool *d_quote);

//handle_cmd_exec.c
void	loop_get_arg(char *word, char **argv, int *i);
char	*check_cmd(char **argv, t_dict *env);


//redirection.c
void	check_redirection(t_info *exec, t_line *sub);
void	check_red_out(t_block *files, t_info *exec, t_block *red);
void	check_red_in(t_block *files, t_info *exec);
void	browse_line_check_red_in(t_leaf *leaf, t_dict *env);

//handle_quote_before_exec.c
char	*handle_quote(char *word);

//exec_subprocess.c
t_bool	dup_cmd_alone(t_info *exec_in, int pipe_fd[2]);
t_bool	dup_in_pipe(t_info *exec_in, int pipe_fd[2]);
void	close_subprocess_fd(t_info *exec_in, int pipe_fd[2]);

/********************************************/
/*                 execve                   */
/********************************************/
//execve.c
t_bool	check_builtins(char **argv);
size_t	get_ac(char **argv);
void	execve_cmd_alone(char *cmd_path, t_dict *env, t_info *exec_in);
t_bool	exec_builtin(char **argv, t_dict *env, t_bool fork);
t_bool	execve_test(char *pathname, char **argv, t_dict *env, t_bool fork);

//cd.c
void	exec_cd(int ac, char **argv, t_dict *env);
//echo.c
void	exec_echo(int ac, char **argv, t_dict *env);
//exit.c
void	exec_exit(int ac, char **argv, t_bool display_exit);
//unset.c
void	exec_unset(int ac, char **argv, t_dict *env);
//export.c
void	exec_export(int ac, char **argv, t_dict *env);
//export_utils.c
void	handle_key(char *key, t_dict *env);
t_bool	check_sign_in_export(char *arg, int i, t_bool *append);
t_bool	init_export(char *arg, int *i, t_bool *append);
//env.c
void	exec_env(int ac, char **argv, t_dict *env);
//pwd.c
void	exec_pwd(int ac, char **argv, t_dict *env);

#endif
