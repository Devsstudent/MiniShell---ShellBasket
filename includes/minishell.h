#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

typedef enum	e_token {
	UNDEF,
	ENV,
	RED_IN,
	RED_OUT_TRUNC,
	RED_OUT_APPEND,
	HERE_DOC,
	PIPE,
	CMD,
	ARG
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
	t_bool			expand;
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

/********************************************/
/*           garbage collector              */
/********************************************/

t_gc	*gc_new_node(t_type type, void *ptr);
void	gc_free_node(t_gc *node);
void	gc_free_node_addr(void *ptr, t_gc **gc);
void	free_gc(t_gc **gc);
t_bool	add_to_gc(t_type type, void *ptr, t_gc **gc);

/********************************************/
/*                environement              */
/********************************************/

char	**copy_env(char **env);
t_bool	double_char_to_lst(char **d_char, t_dict *env, t_gc **gc);
t_elem	*new_elem(char *content);
void	dict_addback(t_dict *dict, t_elem *new);
void	dict_delone(t_dict *dict, char *key);
void	dict_modify(t_dict	*dict, char *key, char *value);
void	dict_clear(t_dict *dict);

#endif
