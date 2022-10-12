/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_parsing.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:41:30 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 22:40:08 by odessein         ###   ########.fr       */
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
void	loop_copy_env(char **elems_env, char *buff, t_elem *new_elem);
//dict_to_double.c
char	**dict_to_double_char_env(t_dict *dict);
char	**dict_to_double_char_export(t_dict *dict);
//lst_utils.c
t_elem	*new_elem(char *content);
t_elem	*create_elem(char *key, char *value);
char	*dict_get_key(t_dict *dict, char *key);

/********************************************/
/*                parsing                   */
/********************************************/

//lexing_parsing.c
t_bool	ms_line(char **line, t_info *exec_in);
t_tree	*ms_lex_and_parse(char **line, t_info *exec_in);

//abstract_syntax_tree.c
void	fill_ast_bonus(t_line *line, t_tree *tree);
t_bool	subshell_redir(t_block **buff, t_block **new,
			t_bool *par, t_line **sub);

//ast_bonus_fill.c
void	last_elem(t_line *line, t_leaf *leaf, int lay_par);
t_line	*fill_till_ope(t_block **buff);
void	fill_sub(t_block **buff, t_bool *par, t_line **sub);
int		fill_leaf(t_leaf *leaf, t_block *buff, int lay_par, t_line *sub);

//abstract_syntax_tree_bonus_utils.c
void	handle_last_elem(t_leaf *leaf, t_line *sub, int lay_par, t_bool par);
t_line	*fill_parentheses_block(t_block **buff);
void	remove_parentheses(t_line *line);
/********************************************/
/*                 here_doc                 */
/********************************************/
//here_doc.c
void	parse_here_doc(t_leaf *leaf, int *fd_arr, int turn);
void	check_here_doc(t_line *sub, int turn, int *fd_arr);
void	create_tmp(int *fd_arr, int turn);
void	fill_here_doc(char *delim, int turn, int *fd_arr);
int		total_block(t_leaf *leaf);

/********************************************/
/*              tokenization                */
/********************************************/
//tokenization_bonus.c
t_bool	tokenization(t_line *line);
t_token	get_next_token(t_block *next_block);
void	attribute_token(t_block *block);
t_token	get_previous_token(t_block *block);
t_bool	attribute_symbol(t_block *block);

//check_symbol.c
t_bool	check_pipe(t_token next, t_token previous);
t_bool	check_symbol(t_block *block);
t_bool	check_parentheses(t_token next, t_token previous, t_token token);
t_bool	check_and(t_token next, t_token previous, t_token token);
t_bool	check_or(t_token next, t_token previous, t_token token);

/********************************************/
/*                  lexing                  */
/********************************************/
//lexing_bonus.c
t_line	*fill_line_lst(char *line);

//lexing_utils.c
t_bool	is_symbol(char c);
t_bool	not_in_quote(char *line, int i);
t_bool	check_lines_quotes(char *line);
t_bool	check_lines_parentheses(char *line);

//lexing_bonus_handle.c
void	handle_pipe(char *line, int *i, int *size, t_line *lst);
void	handle_red_o(char *line, int *i, int *size, t_line *lst);
void	handle_and(char *line, int *i, int *size, t_line *lst);
void	handle_par(char *line, int *i, int *size, t_line *lst);
void	handle_red_i(char *line, int *i, int *size, t_line *lst);

//lexing_fill_word.c
void	fill_word(int *size, t_line *lst, char *line, int i);

/********************************************/
/*          abstract syntax tree            */
/********************************************/

//ast_utils.c
t_leaf	*new_leaf(t_line *cmd, t_type_leaf type);
void	clean_tree(t_leaf *leaf);

#endif
