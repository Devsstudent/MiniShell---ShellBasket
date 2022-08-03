#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <signal.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

typedef enum	e_token {
	UNDEF,//0
	RED_IN,//1
	RED_OUT_TRUNC,//2
	RED_OUT_APPEND,//3
	HERE_DOC,//4
	PIPE,//5
	CMD,//6
	ARG,//7
	FILES,//8
	DELIMITER//9
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
	SIMPLE
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
}		t_gc;

typedef struct	s_lexeur {
	t_token		token;
	void		*content;
	struct s_lexeur	*next;
}			t_lexeur;

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
/*                environement              */
/********************************************/

char	**copy_env(char **env);
t_bool	double_char_to_lst(char **d_char, t_dict *env);
t_elem	*new_elem(char *content);
void	dict_addback(t_dict *dict, t_elem *new);
void	dict_delone(t_dict *dict, char *key);
void	dict_modify(t_dict	*dict, char *key, char *value);
void	dict_clear(t_dict *dict);

/********************************************/
/*                parsing                   */
/********************************************/
t_block	*new_block(char *word);
void	line_lst_addback(t_line *line, t_block *new);
void	line_clear(t_line *line);


/********************************************/
/*                  lexing                  */
/********************************************/

t_bool	handle_line(char *line, t_line *lst);
t_bool	analyse_symbol(char *line, int *i, int *size, t_line *lst);
t_bool	fill_word(int *size, t_line *lst, char *line, int i);
t_bool	handle_pipe(char *line, int *i, int *size, t_line *lst);
t_bool	handle_red_o(char *line, int *i, int *size, t_line *lst);
t_bool	handle_red_i(char *line, int *i, int *size, t_line *lst);
t_bool	fill_line_lst(t_line *block_lst, char *line);

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
void	tokenisation(t_line *line);
t_token	get_next_token(t_block *next_block);
void	attribute_token(t_block *block);
t_token	get_previous_token(t_block *block);
t_bool	attribute_symbol(t_block *block);

#endif
