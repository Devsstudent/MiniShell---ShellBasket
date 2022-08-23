/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:59:38 by odessein          #+#    #+#             */
/*   Updated: 2022/08/23 20:21:48 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static t_bool	check_ambiguous(char *word, t_info *exec_in, t_bool type)
{
	int	i;
	t_bool	quote;
	t_bool	d_quote;

	i = 0;
	quote = FALSE;
	d_quote = FALSE;
	while (word[i])
	{
		ft_putchar_fd(word[i], 2);
		if (word[i] == '\"' && !d_quote)
			d_quote = TRUE;
		else if (word[i] == '\"' && d_quote)
			d_quote = FALSE;
		if (word[i] == '\'' && !quote)
			quote = TRUE;
		else if (word[i] == '\'' && quote)
			quote = FALSE;
		if (i > 0 && word[i - 1] != ' ' && word[i] == ' ' && !quote && !d_quote)
		{
			if (type)
				exec_in->out_fd = -2;
			if (!type)
				exec_in->open_fd = -2;
			write(2, "ambiguous redirect :)", 21);
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}
void	check_redirection(t_info *exec, t_line *sub)
{
	t_block	*buff;

	if (sub->head)
		buff = sub->head;
	else
		return ;
	exec->open_fd = -1;
	exec->out_fd = -1;
	while (buff)
	{
		if (buff->token == RED_IN)
		{
			if (!check_ambiguous(buff->next->word, exec, FALSE))
				check_red_in(buff->next, exec);
		}
		else if (buff->token == RED_OUT_TRUNC || buff->token == RED_OUT_APPEND)
		{
			if (!check_ambiguous(buff->next->word, exec, TRUE))
				check_red_out(buff->next, exec, buff);
		}
		/*
		else if (buff->token == HERE_DOC)
			*/
		buff = buff->next;
	}
}

void	check_red_in(t_block *files, t_info *exec)
{
	int	f_open;

	//before it was :
	// if (exec->out_fd != STDIN_FILENO)
		//close (exec->out_fd);
	//so one fd was left open
	if (exec->open_fd != -1)
		close(exec->open_fd);
	exec->open_fd = -1;
	//Va falloir revoir a fond l'expand xd ca reste la meme idee mais bon pck dans l'ideal pour l'error on a besoin de la value avant l'expand (a voir apres)
	if (ft_strncmp(files->word, "", 2) == 0)
		print_error(NULL, 1);
	else if (ft_strncmp(files->word, "\"\"", 3) == 0 || ft_strncmp(files->word, "\'\'", 3) == 0)
	{
		files->word[0] = ' ';
		ft_bzero(files->word + 1, ft_strlen(files->word) - 1);
	}
	//soit on check avec access
//->	//soit on check juste le retour de open
	f_open = open(files->word, O_RDONLY);
	if (f_open == -1)
		perror(files->word);
	else
		exec->open_fd = f_open;
	if (errno == 13)
		free_exit();
}

void	check_red_out(t_block *files, t_info *exec, t_block *red)
{
	int	f_open_out;

	if (exec->out_fd != -1)
		close(exec->out_fd);
	exec->out_fd = -1;
	if (ft_strncmp(files->word, "", 2) == 0)
	{
		print_error(NULL, 1);
		return ;
	}
	else if (ft_strncmp(files->word, "\"\"", 3) == 0 || ft_strncmp(files->word, "\'\'", 3) == 0)
	{
		write(2, " :", 2);
		//ft_printf(1, " :");
		ft_bzero(files->word, ft_strlen(files->word));
	}
	if (red->token == RED_OUT_TRUNC)
		f_open_out = open(files->word, O_CREAT | O_RDWR | O_TRUNC, 0600);
	else
		f_open_out = open(files->word, O_CREAT | O_RDWR | O_APPEND, 0600);
	if (f_open_out == -1)
		perror(files->word);
	else
		exec->out_fd = f_open_out;
	if (errno == 13)
		free_exit();
}
