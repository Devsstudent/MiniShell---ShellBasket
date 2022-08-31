/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 12:50:24 by odessein          #+#    #+#             */
/*   Updated: 2022/08/31 18:08:35 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <dirent.h>
# include <signal.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "global.h"
# include "lexing_parsing.h"
# include "exec.h"
# include "utils.h"
# include "expand.h"

/********************************************/
/*             remove_quotes                */
/********************************************/

//void	remove_useless_quote(t_block *word);
//void	str_quote_parse(char *word, char *new_word);
//void	str_cp_till_quote(char *word, char *new_word, size_t *i, size_t *j);
//size_t	size_new_word_quote(char *word);
#endif
