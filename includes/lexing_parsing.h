/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_parsing.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:41:30 by odessein          #+#    #+#             */
/*   Updated: 2022/08/31 20:05:08 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LEXING_PARSING_H
# define LEXING_PARSING_H

# include "struct.h"
# include "enum.h"
/********************************************/
/*                environment               */
/********************************************/

char	**copy_env(char **env);
t_dict	*double_char_to_lst(char **d_char);
t_elem	*new_elem(char *content);
void	dict_addback(t_dict *dict, t_elem *new);
void	dict_delone(t_dict *dict, char *key);
void	dict_modify(t_dict	*dict, char *key, char *value);
void	dict_clear(t_dict *dict);
char	*dict_get_value(t_dict *dict, char *key);
char	*dict_get_key(t_dict *dict, char *key);
char	**dict_to_double_char_env(t_dict *dict);
char	**dict_to_double_char_export(t_dict *dict);
t_elem	*create_elem(char *key, char *value);
void	loop_copy_env(char **elems_env, char *buff, t_elem	*new_elem);

/********************************************/
/*                parsing                   */
/********************************************/

t_block	*new_block(char *word);
void	line_lst_addback(t_line *line, t_block *new);
void	parse_here_doc(t_leaf *leaf, int *fd_arr, int turn);
void	check_here_doc(t_line *sub, int turn, int *fd_arr);
void	create_tmp(int *fd_arr, int turn); void	fill_here_doc(char *delim, int turn, int *fd_arr);

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

void	fill_word(int *size, t_line *lst, char *line, int i);
void	handle_line(char *line, t_line *lst);
void	analyse_symbol(char *line, int *i, int *size, t_line *lst);
void	handle_pipe(char *line, int *i, int *size, t_line *lst);
void	handle_red_o(char *line, int *i, int *size, t_line *lst);
void	handle_red_i(char *line, int *i, int *size, t_line *lst);
t_line	*fill_line_lst(char *line);
void	handle_space(char *line, int *i, int *size, t_line *lst);
t_bool	not_in_quote(char *line, int i);
void	analyse_word(char *line, int *i, int *size_word, t_line *lst);

/********************************************/
/*          abstract syntax tree            */
/********************************************/

t_leaf	*new_leaf(t_line *cmd, t_type_leaf type);
//BONUS

t_leaf	*new_leaf_bonus(t_line *sub, int par);
t_type_leaf	get_type(t_token token);
t_line	*fill_till_ope(t_block **buff);
void	line_cpy_till_ope(t_block **buff, t_line *sub_lst);
void	test(t_leaf *leaf, t_line *line, int parr);
void	fill_ast_bonus(t_line *line, t_tree *tree);
void		clean_tree(t_leaf *leaf);

void	remove_tmp_file(int file_nb, int *fd_arr);

#endif
