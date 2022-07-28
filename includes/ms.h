#ifndef MS_H
# define MS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

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

typedef struct	s_lexeur {
	t_token		token;
	void		*content;
	struct s_lexeur	*next;
}			t_lexeur;


#endif
