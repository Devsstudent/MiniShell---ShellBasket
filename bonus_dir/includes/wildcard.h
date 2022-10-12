/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 20:22:10 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 22:26:43 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef WILDCARD_H
# define WILDCARD_H

/********************************************/
/*                 wildcard                 */
/********************************************/

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
