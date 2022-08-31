/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:05:37 by odessein          #+#    #+#             */
/*   Updated: 2022/08/31 18:12:03 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef UTILS_H
# define UTILS_H
/********************************************/
/*           garbage collector              */
/********************************************/
//garabage_collector.c
t_gc	*gc_new_node(t_type type, void *ptr);
t_bool	add_to_gc(t_type type, void *ptr, t_gc **gc);

//garbage_collector_free.c
t_gc	**get_gc(void);
void	gc_free_node(t_gc *node);
void	gc_free_node_addr(void *ptr, t_gc **gc);
void	free_gc(t_gc **gc);
t_bool	free_each_turn(t_gc **gc, t_info *exec_in);
//free_exit.c
void	free_exit(void);

/********************************************/
/*             signal handling              */
/********************************************/
//signal_handling.c
void	sigint_handler(int signum);
void	listen_to_sigs(void);

/********************************************/
/*                   errror                 */
/********************************************/
//error.c
void	print_syntax_error(char *ope, int type);
void	print_error(char *ope, int type);
char	*get_exit_status(void);
t_bool	perror_false(char *str);

#endif
