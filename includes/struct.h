/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:44:12 by odessein          #+#    #+#             */
/*   Updated: 2022/08/31 18:02:15 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef STRUCT_H
# define STRUCT_H

# include "enum.h"


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
	t_bool			crash;
	char			*word;
}				t_block;

typedef struct	s_line{
	t_block	*head;
	t_block	*last;
	int		size;
}			t_line;

typedef struct	s_leaf{
	t_line			*content;
	t_bool			parentheses;
	t_type_leaf		type;
	struct s_leaf	*right;
	struct s_leaf	*left;
}					t_leaf;

typedef struct	s_dict{
	t_elem	*head;
	t_elem	*last;
	int		size;
}			t_dict;

typedef struct	s_lexeur {
	t_token			token;
	void			*content;
	struct s_lexeur	*next;
}					t_lexeur;

typedef struct	s_tree{
	t_leaf	*head;
}			t_tree;

typedef struct	s_gc{
	void			*content;
	struct	s_gc	*next;
	t_type			type;
}					t_gc;

typedef struct	s_info{
	char	**argv;
	int		open_fd;
	int		out_fd;
	int		*pid;
	int		*fd_arr;
	int		fd_arr_size;
	int		turn;
	int		tmp_fd;
	int		stdou;
	int		stdi;
	t_bool	start;
	t_bool	end;
}			t_info;
#endif
