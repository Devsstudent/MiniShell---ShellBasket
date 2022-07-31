#include "minishell.h"

int		size_till_next_char(char *word, char c, int *i)
{
	int	size;

	size = 0;
	while (word[*i] && word[*i] != c)
	{
		(*i)++;
		size++;
	}
	if ((size_t) ((*i) + 1) < ft_strlen(word))
		(*i)++;
	return (size);
}

int	size_new_word_quote(char *word)
{
	int		size;
	int		i;

	size = 0;
	i = 0;
	while (i < ft_strlen(word))
	{
		if (word[i] == '\'')
		{
			i++;
			size += size_till_next_char(word, '\'', &i);
		}
		else if (word[i] == '\"')
		{
			i++;
			size += size_till_next_char(word, '\"', &i);
		}
		else
		{
			size++;
			i++;
		}
	}
	ft_printf("size : %i\n", size);
	return (size);
}

void	str_cp_till_char(char *word, char *new_word, int *i, int *j)
{
	char	c;

	if (i > 0)
		c = word[*i - 1];
	while (*i < ft_strlen(word) && word[*i] != c)
	{
		new_word[*j] = word[*i];
	//	ft_printf("%i %i %c\n", *i, *j, word[*i]);
		(*j)++;
		(*i)++;
	}
	if ((size_t) (*i) + 1 < ft_strlen(word))
		(*i)++;
}

void	str_quote_parse(char *word, char *new_word)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < ft_strlen(word))
	{
		if (word[i] == '\'')
		{
			i++;
			str_cp_till_char(word, new_word, &i, &j);
		}
		else if (word[i] == '\"')
		{
			str_cp_till_char(word, new_word, &i, &j);
			i++;
		}
		else
		{
			new_word[j] = word[i];
			j++;
			i++;
		}
	}
	new_word[j] = 0;
	//ft_printf("%s", new_word);
}

t_bool	handle_quote(char *word)
{
	char	*new_word;
	int		size;

	size = size_new_word_quote(word);
	new_word = malloc(sizeof(*new_word) * size + 1);
	if (!new_word)
		return (FALSE);
	str_quote_parse(word, new_word);
	
	return (TRUE);
}

int	main(void)
{
	char	*line;
	int		size;

	size = 0;
	line = readline("word :");
	handle_quote(line);
}
