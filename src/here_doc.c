#include "minishell.h"

//Cree un tmp file pour chaque block qui contient des here_doc

//Malloc un tableau de fd pour chaque line avec le nombre de sub_line

//Si plusieurs heredoc on ecrit dans le premier puis le deuxieme etc

//TRUNC pas APPEND dcp

//ReChecker le content du fd pour expand les value si on doit expand 

//Le fd du file sera le fd du infile dans les redirections

//Ensuite on close tous les tmp file dans la free_each_turn

//a Checker
void	check_here_doc(t_line *sub, int turn, int *fd_arr);
void	create_tmp(int *fd_arr, int turn);
void	fill_here_doc(char *delim, int turn, int *fd_arr);

int	total_block(t_leaf *leaf)
{
	int	i;

	i = 0;
	if (leaf->type == CMD)
		return (1);
	else if (leaf->type == PIPE_L)
	{
		while (leaf != NULL)
		{
			i++;
			leaf = leaf->right;
		}
	}
	return (i);
}

void	parse_here_doc(t_leaf *leaf, int *fd_arr, int turn)
{
	if (!leaf)
		return ;
	if (leaf->type == PIPE_L)
	{
		check_here_doc(leaf->left->content, turn, fd_arr);
		if (leaf->right)
			parse_here_doc(leaf->right, fd_arr, turn + 1);
	}
	else if (leaf->type == CMD)
		check_here_doc(leaf->content, turn, fd_arr);
}

//Ou on check les delimiter et on envoie pas le next au choix
void	check_here_doc(t_line *sub, int turn, int *fd_arr)
{
	t_block	*buff;

	buff = sub->head;
	while (buff)
	{
		if (buff->token == HERE_DOC)
		{
			create_tmp(fd_arr, turn);
			fill_here_doc(buff->next->word, turn, fd_arr);
		}
		buff = buff->next;
	}
}

void	create_tmp(int *fd_arr, int turn)
{
	char	*num;
	char	*name;

	num = ft_itoa(turn);
	name = ft_strjoin(ft_strdup(".tmp_here_doc_"), num);
	fd_arr[turn] = open(name, O_RDWR | O_CREAT | O_TRUNC, 0600);
	free(num);
	free(name);
	if (fd_arr[turn] == -1)
	{
		perror("open crash");
		return ;
	}
}
//freeexit si open crash maybe

//A revoir pck faut aussi geree les quotes au milieux du mot mdrr
void	fill_here_doc(char *delim, int turn, int *fd_arr)
{
	char	*new_delim;
	char	*line;
	char	*num;
	char	*name;

	errno = 140;
	new_delim = ft_strjoin(ft_strdup(delim), "\n");
	add_to_gc(SIMPLE, new_delim, get_gc());
	write(1, "> ", 2);
	line = get_next_line(0);
	while (line && ft_strncmp(line, new_delim, ft_strlen(new_delim)) != 0)
	{
		if (line)
			write(fd_arr[turn], line, ft_strlen(line));
		write(1, "> ", 2);
		free(line);
		line = get_next_line(0);
	}
	if (!line)
		write(2, "warning: here-document at some line delimited by end-of-file\n", 61);
	g_exit_status = 0;
	free(line);
	close(fd_arr[turn]);
	num = ft_itoa(turn);
	name = ft_strjoin(ft_strdup(".tmp_here_doc_"), num);
	fd_arr[turn] = open(name, O_RDONLY, 0600);
	free(num);
	free(name);
}
