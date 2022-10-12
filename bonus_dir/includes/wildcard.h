/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 20:22:10 by odessein          #+#    #+#             */
/*   Updated: 2022/10/12 20:29:18 by odessein         ###   ########.fr       */
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

//wildcard_handler.c
void	order_filenames(char **filenames);
t_bool	check_match(char *filename, char **patterns, char *word);
void	select_filenames(char **filenames, char **patterns, char **matches,
	char *word);
char	**handle_wildcards(char *word);
void	wildcard(t_line *sub);

#endif
