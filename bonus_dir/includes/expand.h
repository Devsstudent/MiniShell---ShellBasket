/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:06:58 by odessein          #+#    #+#             */
/*   Updated: 2022/09/08 19:10:52 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXPAND_H
# define EXPAND_H

/********************************************/
/*                 expand                   */
/********************************************/
//A SPLIT EN PLUSIEURS FICHIER : ) 

//expand.c
void	expand(t_line *line, t_dict *env);
t_bool	handle_simple_word(char *new_word, char *word, int *indexes);
void	expand_block(t_block *block, char **key_arr, char **val_arr,
			int *indexes);
void	handle_dollar_in_block(t_block *block, t_dict *dict);

//expand_check_key.c
void	fill_key_arr(t_block *block, char **key_arr, int *indexes);
int		*get_indexes_expandables(t_block *block, int dollar);
t_bool	char_is_num(char c, int *i);
void	replace_key(char *new_word, int *j, char **val_arr);
int		get_nb_of_dollar(t_block *block);

//expand_fill_word_val.c
void	fill_val_arr(char **key_arr, char **val_arr, t_dict *dict);
void	fill_new_word(char *new_word, char *word, char **val_arr, int *indexes);
int		size_doll_val(char *word, int i);

//expand_utils.c
int		total_char_to_remove(char **key_arr);
int		total_char_to_add(char **val_arr);
t_bool	check_char(char c);
void	check_null_val(char *val);
void	advance_if_in_s_quote(t_bool *d_quote, char *word, int *i);

#endif
