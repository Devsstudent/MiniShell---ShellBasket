/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 20:22:10 by odessein          #+#    #+#             */
/*   Updated: 2022/10/19 18:39:31 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef WILDCARD_H
# define WILDCARD_H

/********************************************/
/*                 wildcard                 */
/********************************************/

//wildcard_handle_quotes.c
char	**ft_split_wildcard(char *word);

//wildcard_handle_quotes_utils.c
char	**ft_free(char **splitos, int size);
char	**handle_quotes_in_blocks(char **splitos);
void	str_advance_till_quote(char *word, int *i);
void	str_adv_count_till_quote(char *word, int *i, int *k);
void	str_fill_till_quote(char *word, int *i, int *k, char *block);

//wildcard_fill.c
void	fill_filenames(char **filenames, DIR **dir, struct dirent **ent);
void	fill_matches(char *word, char **filenames, char **patterns,
			char **matches);
char	*fill_final_string(char **items, char *word);

//wildcard_handler.c
void	order_filenames(char **filenames);
t_bool	check_match(char *filename, char **patterns, char *word);
void	select_filenames(char **filenames, char **patterns, char **matches,
			char *word);
char	**handle_wildcards(char *word);
void	wildcard(t_line *sub);

//wildcard_check.c
t_bool	check_special_match(char *filename, char *word);
t_bool	check_end_word_match(char *filename, char *word,
			char **patterns, int i);
t_bool	check_match(char *filename, char **patterns, char *word);
#endif
