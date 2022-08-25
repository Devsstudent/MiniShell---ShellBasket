/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 19:59:38 by odessein          #+#    #+#             */
/*   Updated: 2022/08/25 12:23:13 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static t_bool	check_ambiguous_bis(t_block *buff)
{
	int	i;
	t_bool	quote;
	t_bool	d_quote;
	char	*word;

	word = buff->word;
	i = 0;
	quote = FALSE;
	d_quote = FALSE;
	if (!word[i])
	{
		write(2, "ambiguous redirect :)", 21);
		return (TRUE);
	}
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
			write(2, "ambiguous redirect :)", 21);
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}
static t_bool	check_ambiguous(char *word, t_info *exec_in, t_bool type, t_bool crash)
{
	int	i;
	t_bool	quote;
	t_bool	d_quote;

	i = 0;
	if (crash)
	{
	//	ft_putstr_fd("testest", 2);
		exec_in->open_fd = -2;
		return (TRUE);
	}
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
			if (!check_ambiguous(buff->next->word, exec, FALSE, buff->next->crash))
				check_red_in(buff->next, exec);
		}
		else if (buff->token == RED_OUT_TRUNC || buff->token == RED_OUT_APPEND)
		{
			if (!check_ambiguous(buff->next->word, exec, TRUE, FALSE))
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
	if (!files->crash)
	{
		f_open = open(files->word, O_RDONLY);
		if (f_open == -1)
			perror(files->word);
		else
			exec->open_fd = f_open;
	}
	else
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

static void	check_red_in_sub(t_line *sub_line, t_dict *env)
{
	t_block	*buff;
	struct stat	statbuff;

	buff = sub_line->head;
	while (buff)
	{
		if (buff->token == RED_IN)
		{
			expand(sub_line, env);
			if (check_ambiguous_bis(buff->next))
			{
				buff->next->crash = TRUE;
				buff = buff->next;
				continue ;
			}
			if (access(buff->next->word, R_OK) == 0)
			{
				if (stat(buff->next->word, &statbuff) == -1)
					perror("stat broslinecheck");
				if ((statbuff.st_mode & S_IFMT) != S_IFREG)
				{
					ft_putstr_fd(buff->next->word, 2);
					ft_putstr_fd(": Not a file\n", 2);
					buff->next->crash = TRUE;
				}
			}
			else
			{
				ft_putstr_fd(buff->next->word, 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				buff->next->crash = TRUE;
			}
		}
		buff = buff->next;
	}
}

void	browse_line_check_red_in(t_leaf *leaf, t_dict *env)
{
	if (!leaf)
		return ;
	if (leaf->type == PIPE_L)
	{
		if (leaf->content)
			check_red_in_sub(leaf->content, env);
		if (leaf->right)
			browse_line_check_red_in(leaf->right, env);
	}
	else if (leaf->type == CMD)
		if (leaf->content)
			check_red_in_sub(leaf->content, env);
}
