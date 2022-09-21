/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:44:12 by odessein          #+#    #+#             */
/*   Updated: 2022/09/21 13:08:26 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef STRUCT_H
# define STRUCT_H

# include "enum.h"

typedef struct s_elem{
	struct s_elem	*next;
	struct s_elem	*prev;
	char			*key;
	char			*value;
}					t_elem;

typedef struct s_block{
	struct s_block	*next;
	struct s_block	*prev;
	t_token			token;
	t_bool			crash;
	char			*word;
}				t_block;

typedef struct s_line{
	t_block	*head;
	t_block	*last;
	int		size;
}			t_line;

typedef struct s_leaf{
	t_line			*content;
	int				parentheses;
	t_type_leaf		type;
	struct s_leaf	*right;
	struct s_leaf	*left;
}					t_leaf;

typedef struct s_dict{
	t_elem	*head;
	t_elem	*last;
	int		size;
}			t_dict;

typedef struct s_lexeur {
	t_token			token;
	void			*content;
	struct s_lexeur	*next;
}					t_lexeur;

typedef struct s_tree{
	t_leaf	*head;
}			t_tree;

typedef struct s_pid{
	int				pid;
	struct s_pid	*next;
}					t_pid;

typedef struct s_pid_li{
	t_pid	*head;
	int		size;
	t_pid	*last;
}			t_pid_li;

typedef struct s_gc{
	void			*content;
	struct s_gc		*next;
	t_type			type;
}					t_gc;

typedef struct s_info{
	t_pid_li	*pid_li;
	char		**argv;
	int			open_fd;
	int			out_fd;
	int			*fd_arr;
	int			fd_arr_size;
	int			turn;
	int			pipe_fd[2];
	int			tmp_fd;
	int			stdou;
	int			final_out;
	int			stdi;
	t_bool		cmd_not_found;
	t_bool		pipe;
	t_bool		end;
}				t_info;
#endif
