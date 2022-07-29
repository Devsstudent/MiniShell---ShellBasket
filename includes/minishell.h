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

typedef enum	e_type{
	LST,
	DOUBLE,
	SIMPLE
}	t_type;

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

t_bool	add_to_gc(t_type type, void *ptr, t_gc **gc);
char	**copy_env(char **env);

#endif
