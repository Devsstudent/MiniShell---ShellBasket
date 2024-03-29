/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:03:12 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 22:27:53 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXEC_H
# define EXEC_H

# include "enum.h"
# include "struct.h"

void	close_fds_in_subshell(t_info *exec_in);
void	exec_subshell(t_leaf *laef, t_info *exec_in, t_dict *env);
/********************************************/
/*                   exec                   */
/********************************************/
//exec_bonus.c
void	exec_cmd(t_info *exec_info, t_leaf *leaf, t_dict *env);
t_bool	parentheses_pipe(t_leaf *leaf, t_info *exec_in);
t_bool	exec_left_right_pipe(t_leaf *leaf, t_info *exec_in, t_dict *env);
void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env, t_leaf *prev);

//exec_bonus_leaf_type.c
t_bool	leaf_red_in(t_leaf *leaf, t_info *exec_in, t_dict *env);
void	leaf_type_cmd(t_leaf *leaf, t_info *exec_in, t_dict *env, t_leaf *prev);
void	leaf_type_pipe(t_leaf *leaf, t_info *exec_in,
			t_dict *env, t_leaf *prev);
void	leaf_type_or(t_leaf *leaf, t_info *exec_in, t_dict *env);
void	leaf_type_and(t_leaf *leaf, t_info *exec_in, t_dict *env,
			t_leaf *prev);

//forking.c
void	forking(char *cmd_path, t_info *exec_in, t_dict *env);
void	forking_cmd_alone(char *cmd_path, t_info *exec_in, t_dict *env);

//exec.c
//void	exec_tree(t_leaf *leaf, t_info *exec_in, t_dict *env, t_tree *tree);
void	exec(t_info *exec_in, t_leaf *leaf, t_dict *env);

//cmd_not_found.c
t_bool	command_not_found(t_info *exec_in, char *cmd_path,
			t_line *sub);
t_bool	check_cmd_in_sub(t_line *sub);

//handle_cmd_utils.c
size_t	get_nb_cmd_arg(t_line *sub);
char	**get_cmd_arg(t_line *sub);
void	get_size_word_in_word(char *word, size_t *size);
void	init_loop_get_arg(int *j, int *last, t_bool *quote, t_bool *d_quote);
t_bool	check_quote(t_bool *d_quote, t_bool *quote, char word);

//handle_cmd_utils_bis.c
t_bool	is_white_space(char c);
t_bool	check_kind_of_abs(struct stat statbuff, char *cmd, char **res);

//handle_cmd_exec.c
void	loop_get_arg(char *word, char **argv, int *i);
char	*check_cmd(char **argv, t_dict *env);

//redirection.c
void	check_quote_redir(t_bool *d_quote, t_bool *quote, char word);
void	browse_line_check_red_in(t_leaf *leaf, t_dict *env);
t_bool	check_file_permission(char *file, t_info *exec, int type);

//redir_ambiguous.c
t_bool	check_ambiguous_bis(t_block *buff);
void	ambiguous_case(t_bool type, t_info *exec_in);
t_bool	check_ambiguous(char *word, t_info *exec_in, t_bool type);

//check_redir.c
t_bool	check_red_in(t_block *files, t_info *exec);
t_bool	check_red_out(t_block *files, t_info *exec, t_block *red);
void	check_redirection(t_info *exec, t_line *sub);
void	check_red_in_sub(t_line *sub_line, t_dict *env);

//handle_quote_before_exec.c
char	*handle_quote(char *word);

//exec_subprocess.c
t_bool	dup_cmd_alone(t_info *exec_in);
t_bool	dup_in_pipe(t_info *exec_in);
void	close_subprocess_fd(t_info *exec_in);

/********************************************/
/*                 execve                   */
/********************************************/
//execve.c
size_t	get_ac(char **argv);
t_bool	execve_cmd(char *pathname, t_info *exec_info, t_dict *env);
void	execve_builtin_alone(char *cmd_path, t_dict *env, t_info *exec_in);

//builtins.c
t_bool	exec_builtin(t_dict *env, t_bool fork, t_info *exec_in);
t_bool	check_builtins(char **argv);

//cd.c
void	exec_cd(int ac, char **argv, t_dict *env);
//echo.c
void	exec_echo(int ac, char **argv, t_dict *env);
//exit.c
void	exec_exit(int ac, t_info *exec_info, t_bool display_exit);
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
