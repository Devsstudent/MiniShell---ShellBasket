#include "minishell.h"

t_bool	check_ambiguous_bis(t_block *buff)
{
	int		i;
	t_bool	quote;
	t_bool	d_quote;
	char	*word;

	word = buff->word;
	i = -1;
	quote = FALSE;
	d_quote = FALSE;
	if (!word[0])
	{
		write(2, "ambiguous redirect :)", 21);
		return (TRUE);
	}
	while (word[++i])
	{
		check_quote_redir(&d_quote, &quote, word[i]);
		if (i > 0 && word[i - 1] != ' ' && word[i] == ' ' && !quote && !d_quote)
		{
			write(2, "ambiguous redirect :)", 21);
			return (TRUE);
		}
	}
	return (FALSE);
}

void	ambiguous_case(t_bool type, t_info *exec_in)
{
	if (type)
		exec_in->out_fd = -2;
	if (!type)
		exec_in->open_fd = -2;
	write(2, "ambiguous redirect :)", 21);
}

t_bool	check_ambiguous(char *word, t_info *exec_in, t_bool type,
			t_bool crash)
{
	int		i;
	t_bool	quote;
	t_bool	d_quote;

	quote = FALSE;
	d_quote = FALSE;
	if (crash)
	{
		exec_in->open_fd = -2;
		return (TRUE);
	}
	i = -1;
	while (word[++i])
	{
		check_quote_redir(&d_quote, &quote, word[i]);
		if (i > 0 && word[i - 1] != ' ' && word[i] == ' ' && !quote && !d_quote)
		{
			ambiguous_case(type, exec_in);
			return (TRUE);
		}
	}
	return (FALSE);
}
