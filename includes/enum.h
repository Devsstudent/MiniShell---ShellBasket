/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:47:20 by odessein          #+#    #+#             */
/*   Updated: 2022/09/06 17:57:13 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ENUM_H
# define ENUM_H

typedef enum	e_type_leaf{
	CMD,
	PIPE_L,
}	t_type_leaf;

typedef enum	e_token {
	UNDEF,//0
	RED_IN,//1
	RED_OUT_TRUNC,//2
	RED_OUT_APPEND,//3
	HERE_DOC,//4
	PIPE,//5
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
	ENV,
	DICT,
	LINE,
	DOUBLE,
	SIMPLE,
	TREE,
	EXEC_INFO
}	t_type;

#endif
