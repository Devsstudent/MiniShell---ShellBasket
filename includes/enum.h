/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:47:20 by odessein          #+#    #+#             */
/*   Updated: 2022/09/08 18:39:54 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ENUM_H
# define ENUM_H

typedef enum e_type_leaf {
	CMD,
	PIPE_L,
	AND_L,
	OR_L,
	PRTS
}	t_type_leaf;

typedef enum e_token {
	UNDEF,
	RED_IN,
	RED_OUT_TRUNC,
	RED_OUT_APPEND,
	HERE_DOC,
	PIPE,
	CMD_ARG,
	FILES,
	DELIMITER
}	t_token;

typedef enum e_quote {
	NO,
	SINGLE,
	DOUBLE_QUOTE
}	t_quote;

typedef enum e_type{
	ENV,
	DICT,
	LINE,
	DOUBLE,
	SIMPLE,
	TREE,
	EXEC_INFO
}	t_type;

#endif
