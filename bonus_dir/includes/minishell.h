/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 13:02:02 by odessein          #+#    #+#             */
/*   Updated: 2022/09/12 17:02:56 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <signal.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "global.h"
# include "struct.h"
# include "exec.h"
# include "lexing_parsing.h"
# include "utils.h"
# include "enum.h"
# include "expand.h"

void	free_exit(void);
void	listen_to_sigs(void);

/********************************************/
/*           garbage collector              */
/********************************************/

t_gc	**get_gc(void);
t_gc	*gc_new_node(t_type type, void *ptr);
void	gc_free_node(t_gc *node);
void	gc_free_node_addr(void *ptr, t_gc **gc);
void	free_gc(t_gc **gc);
t_bool	add_to_gc(t_type type, void *ptr, t_gc **gc);
/********************************************/
/*                environment               */
/********************************************/

char	**copy_env(char **env);
t_dict	*double_char_to_lst(char **d_char);
t_elem	*new_elem(char *content);
void	dict_addback(t_dict *dict, t_elem *new);
void	dict_delone(t_dict *dict, char *key);
void	dict_modify(t_dict	*dict, char *key, char *value);
char	*dict_get_value(t_dict *dict, char *key);
void	dict_clear(t_dict *dict);

/********************************************/
/*                parsing                   */
/********************************************/

t_block		*new_block(char *word);
void		line_lst_addback(t_line *line, t_block *new);
void		line_clear(t_line *line);
void		line_cpy_till_pipe(t_block **buff, t_line *sub_lst);
void		line_cpy_till_ope(t_block **buff, t_line *sub_lst);
void		line_cpy_till_pend(t_block **buff, t_line *sub_lst);

/********************************************/
/*                  lexing                  */
/********************************************/

void		analyse_symbol(char *line, int *i, int *size, t_line *lst);
void		fill_word(int *size, t_line *lst, char *line, int i);
void		handle_pipe(char *line, int *i, int *size, t_line *lst);
void		handle_red_o(char *line, int *i, int *size, t_line *lst);
void		handle_red_i(char *line, int *i, int *size, t_line *lst);
void		handle_and(char *line, int *i, int *size, t_line *lst);
void		handle_par(char *line, int *i, int *size, t_line *lst);
void		handle_line(char *line, t_line *lst);
t_line		*fill_line_lst(char *line);

/********************************************/
/*             remove_quotes                */
/********************************************/

void	remove_useless_quote(t_block *word);
//void	str_quote_parse(char *word, char *new_word);
//void	str_cp_till_quote(char *word, char *new_word, size_t *i, size_t *j);
//size_t	size_new_word_quote(char *word);
//size_t	size_till_next_char(char *word, char c, size_t *i);

/********************************************/
/*              tokenization                */
/********************************************/

//check_symbol
t_bool		check_pipe(t_token next, t_token previous);
t_bool		check_symbol(t_block *block);
t_bool		check_parentheses(t_token next, t_token previous, t_token token);
t_bool		check_and(t_token next, t_token previous, t_token token);
t_bool		check_or(t_token next, t_token previous, t_token token);

//tokenization
t_bool	tokenization(t_line *line);
t_token	get_next_token(t_block *next_block);
void	attribute_token(t_block *block);
t_token	get_previous_token(t_block *block);
t_bool	attribute_symbol(t_block *block);

/********************************************/
/*             signal handling              */
/********************************************/

void	sigint_handler(int signum);
void	listen_to_sigs(void);

/********************************************/
/*          abstract syntax tree            */
/********************************************/

void		clean_tree(t_leaf *leaf);
void		fill_ast_bonus(t_line *line, t_tree *tree);
t_leaf		*new_leaf_bonus(t_line *sub, int lay_par);
void		remove_parentheses(t_line *line);
t_line		*fill_parentheses_block(t_block **buff);
t_type_leaf	get_type(t_token token);
t_line		*fill_till_ope(t_block **buff);
//Recusrion fill tree :
void		test(t_leaf *leaf, t_line *line, int lay_par);

int	total_char_to_remove(char **key_arr);
int	total_char_to_add(char **val_arr);
t_bool	check_char(char c);
int	size_doll_val(char *word, int i);
void	advance_if_in_s_quote(t_bool *d_quote, char *word, int *i);
void	fill_key_arr(t_block *block, char **key_arr, int *indexes);
int	*get_indexes_expandables(t_block *block, int dollar);
int	get_nb_of_dollar(t_block *block);
void	fill_val_arr(char **key_arr, char **val_arr, t_dict *dict);
void	replace_key(char *new_word, int *j, char **val_arr);
t_bool	handle_simple_word(char *new_word, char *word, int *indexes);
void	fill_new_word(char *new_word, char *word, char **val_arr, int *indexes);
void	expand_block(t_block *block, char **key_arr, char **val_arr, int *indexes);
void	handle_dollar_in_block(t_block *block, t_dict *dict);
void	expand(t_line *line, t_dict *dict);
void	browse_ast_apply_expand(t_leaf *leaf, t_dict *env);

char	*get_exit_status(void);
void	print_syntax_error(char *ope, int type);

#endif
