/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 12:50:24 by odessein          #+#    #+#             */
/*   Updated: 2022/08/20 12:53:17 by odessein         ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "global.h"

typedef enum	e_type_leaf{
	CMD,
	PIPE_L,
    AND_L,
    OR_L,
    PRTS	
}	t_type_leaf;

typedef enum	e_token {
	UNDEF,//0
	RED_IN,//1
	RED_OUT_TRUNC,//2
	RED_OUT_APPEND,//3
	HERE_DOC,//4
	PIPE,//5
	OR,
	AND,
	P_OPEN,
	P_CLOSE,
	CMD_ARG,//6
	FILES,//7
	DELIMITER//8
}	t_token;

typedef enum	e_quote{
	NO,
	SINGLE,
	DOUBLE_QUOTE
}	t_quote;

typedef enum	e_type{
	DICT,
	LINE,
	DOUBLE,
	SIMPLE,
	TREE
}	t_type;

typedef struct	s_elem{
	struct s_elem	*next;
	struct s_elem	*prev;
	char			*key;
	char			*value;
}					t_elem;

typedef struct	s_block{
	struct s_block	*next;
	struct s_block	*prev;
	t_token			token;
	//Maybe useful to store env_var in a lst for the expand
//	t_env			*env_list;
	char			*word;
}				t_block;

typedef struct	s_dict{
	t_elem	*head;
	t_elem	*last;
	int		size;
}			t_dict;

typedef struct s_line{
	t_block	*head;
	t_block	*last;
	int		size;
}			t_line;

typedef struct	s_gc{
	void			*content;
	struct	s_gc	*next;
	t_type			type;
}					t_gc;

typedef struct	s_lexeur {
	t_token			token;
	void			*content;
	struct s_lexeur	*next;
}					t_lexeur;

typedef struct	s_leaf{
	//head de la sous-list
	t_line			*content;
	t_bool			parentheses;
	t_type_leaf		type;
	struct s_leaf	*right;
	struct s_leaf	*left;
}					t_leaf;

typedef struct	s_tree{
	t_leaf	*head;
}			t_tree;

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
void	dict_clear(t_dict *dict);
char	*dict_get_value(t_dict *dict, char *key);

/********************************************/
/*                parsing                   */
/********************************************/

t_block	*new_block(char *word);
void	line_lst_addback(t_line *line, t_block *new);
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

/********************************************/
/*             remove_quotes                */
/********************************************/

void	remove_useless_quote(t_block *word);
void	str_quote_parse(char *word, char *new_word);
void	str_cp_till_quote(char *word, char *new_word, size_t *i, size_t *j);
size_t	size_new_word_quote(char *word);
size_t	size_till_next_char(char *word, char c, size_t *i);

/********************************************/
/*              tokenization                */
/********************************************/

//check_symbol
t_bool check_pipe(t_token next, t_token previous);
t_bool check_symbol(t_block *block);

//tokenization
void	tokenization(t_line *line);
t_token	get_next_token(t_block *next_block);
void	attribute_token(t_block *block);
t_token	get_previous_token(t_block *block);
t_bool	attribute_symbol(t_block *block);

/********************************************/
/*             signal handling              */
/********************************************/

void	sigint_handler(int signum);
void	listen_to_sigs(void);


void	fill_ast(t_line *line, t_tree *tree);
void	line_cpy_till_pipe(t_block **buff, t_line *sub_lst);

/********************************************/
/*          abstract syntax tree            */
/********************************************/

t_leaf	*new_leaf(t_line *cmd, t_type_leaf type);
//BONUS

t_bool	    check_parentheses(t_token next, t_token previous, t_token token);
t_bool	    check_and(t_token next, t_token previous, t_token token);
t_bool	    check_or(t_token next, t_token previous, t_token token);
t_bool	    handle_and(char *line, int *i, int *size, t_line *lst);
t_bool	    handle_par(char *line, int *i, int *size, t_line *lst);
t_leaf      *new_leaf_bonus(t_line *sub, int par);
void        remove_parentheses(t_line *line);
t_line      *fill_parentheses_block(t_block **buff);
t_type_leaf get_type(t_token token);
t_line      *fill_till_ope(t_block **buff);
void        line_cpy_till_ope(t_block **buff, t_line *sub_lst);
void        test(t_leaf *leaf, t_line *line, int parr);
void        fill_ast_bonus(t_line *line, t_tree *tree);
void        line_cpy_till_pend(t_block **buff, t_line *sub_lst);

int	total_char_to_remove(char **key_arr);
int	total_char_to_add(char **val_arr);
t_bool	check_char(char c);
int	size_doll_val(char *word, int i);
void	advance_if_in_s_quote(t_bool *d_quote, char *word, int *i);
void	fill_key_arr(t_block *block, char **key_arr, int *indexes);
int	*get_indexes_expandables(t_block *block, int dollar);

void	browse_ast_apply_expand(t_leaf *leaf, t_dict *env);
void	expand(t_line *line, t_dict *env);
void	check_dollar_in_block(t_block *block);

int	get_nb_of_dollar(t_block *block);
void	fill_val_arr(char **key_arr, char **val_arr, t_dict *dict);
void	replace_key(char *new_word, int *j, char **val_arr);
t_bool	handle_simple_word(char *new_word, char *word, int *indexes);
void	fill_new_word(char *new_word, char *word, char **val_arr, int *indexes);
void	expand_block(t_block *block, char **key_arr, char **val_arr, int *indexes);
void	handle_dollar_in_block(t_block *block, t_dict *dict);
void	browse_ast_apply_expand(t_leaf *leaf, t_dict *env);

char	*get_exit_status(void);
void	clean_tree(t_leaf *leaf);

#endif
