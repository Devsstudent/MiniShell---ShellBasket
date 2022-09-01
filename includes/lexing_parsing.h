/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_parsing.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:41:30 by odessein          #+#    #+#             */
/*   Updated: 2022/09/01 17:21:57 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LEXING_PARSING_H
# define LEXING_PARSING_H

# include "struct.h"
# include "enum.h"
/********************************************/
/*                environment               */
/********************************************/
//dict_utils.c
void	dict_addback(t_dict *dict, t_elem *new);
void	dict_delone(t_dict *dict, char *key);
void	dict_modify(t_dict	*dict, char *key, char *value);
void	dict_clear(t_dict *dict);
char	*dict_get_value(t_dict *dict, char *key);
//copy_env.c
t_dict	*double_char_to_lst(char **d_char);
//dict_to_double.c
char	**dict_to_double_char_env(t_dict *dict);
char	**dict_to_double_char_export(t_dict *dict);
//lst_utils.c
void	loop_copy_env(char **elems_env, char *buff, t_elem *new_elem);
t_elem	*new_elem(char *content);
t_elem	*create_elem(char *key, char *value);
char	*dict_get_key(t_dict *dict, char *key);
//

/********************************************/
/*                parsing                   */
/********************************************/
//line_lst_utils.c
t_block	*new_block(char *word);
void	line_lst_addback(t_line *line, t_block *new);
void	line_cpy_till_pipe(t_block **buff, t_line *sub_lst);

/********************************************/
/*                 here_doc                 */
/********************************************/
//here_doc.c
void	parse_here_doc(t_leaf *leaf, int *fd_arr, int turn);
void	check_here_doc(t_line *sub, int turn, int *fd_arr);
void	create_tmp(int *fd_arr, int turn); void	fill_here_doc(char *delim, int turn, int *fd_arr);
void	fill_here_doc(char *delim, int turn, int *fd_arr);
int	total_block(t_leaf *leaf);
//here_doc_utils.c
void	remove_tmp_file(int file_nb, int *fd_arr);
char	*get_delim(char *delim);
void	close_reopen_here_doc(int turn, int *fd_arr, char *line);

/********************************************/
/*              tokenization                */
/********************************************/

//check_symbol
t_bool check_pipe(t_token next, t_token previous);
t_bool check_symbol(t_block *block);

//tokenization
t_bool	tokenization(t_line *line);
t_token	get_next_token(t_block *next_block);
void	attribute_token(t_block *block);
t_token	get_previous_token(t_block *block);
t_bool	attribute_symbol(t_block *block);

void	line_clear(t_line *line);

/********************************************/
/*                  lexing                  */
/********************************************/
//lexing.c
t_bool	not_in_quote(char *line, int i);
void	analyse_symbol(char *line, int *i, int *size, t_line *lst);
void	analyse_word(char *line, int *i, int *size_word, t_line *lst);
t_line	*fill_line_lst(char *line);

//lexing_fill_word.c
void	fill_word(int *size, t_line *lst, char *line, int i);

//lexing_handle.c
void	handle_line(char *line, t_line *lst);
void	handle_pipe(char *line, int *i, int *size, t_line *lst);
void	handle_red_o(char *line, int *i, int *size, t_line *lst);
void	handle_red_i(char *line, int *i, int *size, t_line *lst);
void	handle_space(char *line, int *i, int *size, t_line *lst);

/********************************************/
/*          abstract syntax tree            */
/********************************************/

//abstract_syntax_tree.c
t_line	*fill_till_pipe(t_block **buff);
void	fill_tree_while_pipe(t_tree **tree, t_line *cmd);
void	fill_ast(t_line *line, t_tree *tree);
//ast_utils.c
t_leaf	*new_leaf(t_line *cmd, t_type_leaf type);
void	clean_tree(t_leaf *leaf);

#endif
