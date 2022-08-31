/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:06:58 by odessein          #+#    #+#             */
/*   Updated: 2022/08/31 18:39:17 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXPAND_H
# define EXPAND_H

/********************************************/
/*                 expand                   */
/********************************************/
//A SPLIT EN PLUSIEURS FICHIER : ) 
void	expand(t_line *line, t_dict *env);
int	total_char_to_remove(char **key_arr);
int	total_char_to_add(char **val_arr);
t_bool	check_char(char c);
int	size_doll_val(char *word, int i);
void	advance_if_in_s_quote(t_bool *d_quote, char *word, int *i);
void	fill_key_arr(t_block *block, char **key_arr, int *indexes);
int	*get_indexes_expandables(t_block *block, int dollar);
void	browse_ast_apply_expand(t_leaf *leaf, t_dict *env);
void	check_dollar_in_block(t_block *block);
int	get_nb_of_dollar(t_block *block);
void	fill_val_arr(char **key_arr, char **val_arr, t_dict *dict);
void	replace_key(char *new_word, int *j, char **val_arr);
t_bool	handle_simple_word(char *new_word, char *word, int *indexes);
void	fill_new_word(char *new_word, char *word, char **val_arr, int *indexes);
void	expand_block(t_block *block, char **key_arr, char **val_arr, int *indexes);
void	handle_dollar_in_block(t_block *block, t_dict *dict);
void	check_null_val(char *val);
t_bool	check_char(char c);
t_bool	char_is_num(char c, int *i);

#endif
