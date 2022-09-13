/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 18:05:37 by odessein          #+#    #+#             */
/*   Updated: 2022/09/08 19:48:26 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef UTILS_H
# define UTILS_H



void	line_cpy_till_pend(t_block **buff, t_line *sub_lst);
void    line_cpy_till_ope(t_block **buff, t_line *sub_lst);
/********************************************/
/*           garbage collector              */
/********************************************/
//garabage_collector.c
t_gc	*gc_new_node(t_type type, void *ptr);
void	gc_free_node_addr(void *ptr, t_gc **gc);
t_bool	add_to_gc(t_type type, void *ptr, t_gc **gc);
void	gc_free_one_node(t_gc *node, t_gc **gc);
t_bool	free_each_turn(t_gc **gc);

//garbage_collector_free.c
t_gc	**get_gc(void);
void	gc_free_node(t_gc *node);
void	free_gc(t_gc **gc);
void	free_exec_info(t_gc **gc);
//free_exit.c
void	free_exit(void);

/********************************************/
/*             signal handling              */
/********************************************/
//signal_handling.c
void	sigint_handler(int signum);
void	sigint_handler_exec(int signum);
void	listen_to_sigs(void);
t_bool	handle_ctrl_c(char **line, int stdi);

/********************************************/
/*                   errror                 */
/********************************************/
//error.c
void	print_syntax_error(char *ope, int type);
void	print_error(char *ope, int type);
char	*get_exit_status(void);
t_bool	perror_false(char *str);

/********************************************/
/*                 wait_pid                 */
/********************************************/
//wait_processs
void	wait_sub_process(t_info *exec_info);

/********************************************/
/*               malloc_pid                 */
/********************************************/
//malloc_pid_func.c
void	malloc_pid_arr(t_info *exec_info, t_tree *tree);

#endif
