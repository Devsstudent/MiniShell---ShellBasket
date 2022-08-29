#include "minishell.h"

//Cree un tmp file pour chaque block qui contient des here_doc

//Malloc un tableau de fd pour chaque line avec le nombre de sub_line

//Si plusieurs heredoc on ecrit dans le premier puis le deuxieme etc

//TRUNC pas APPEND dcp

//ReChecker le content du fd pour expand les value si on doit expand 

//Le fd du file sera le fd du infile dans les redirections

//Ensuite on close tous les tmp file dans la free_each_turn

//a Checker
int	total_block(t_leaf *leaf)
{
	int	i;

	i = 0;
	if (leaf->type == CMD)
		return (1);
	else if (leaf->type = PIPE_L)
	{
		while (leaf != NULL)
		{
			i++;
			leaf = leaf->right;
		}
		return (i);
	}
}

fd_arr = malloc(sizeof(*fd_arr) * total_block(tree->head) + 1); ou pas

void	parse_here_doc(t_leaf *leaf, int *fd_arr, int turn)
{
	if (leaf->type == PIPE_L)
	{
		check_here_doc(leaf->left->content, turn);
		if (leaf->right)
			parse_here_doc(leaf->right, turn + 1)
	}
	else if (leaf->type == CMD)
		check_here_doc(leaf->content);
}

void	check_here_doc(t_line *sub, int turn, int *fd_arr)
{
	t_block	*buff;

	buff = sub->head;
	while (buff)
	{
		//Ou on check les delimiter et on envoie pas le next a choix
		if (buff->token == HERE_DOC)
			create_and_fill(buff->next->word, turn, fd_arr)
		buff = buff->next;
	}
}



void	create_and_fill(char *delim, int turn, int *fd_arr)
{
	char	*new_delim;

	join pour le name // checker que il soit pas deja present peut etre sinon modifier son nom jsp / 1 2 3 etc en function du block
	fd_arr[turn] = open(".tmp_here_doc", RDW_ONLY | O_CREATE | O_TRUNC, 0600);
	if (fd_arr[turn] == -1)
	{
		perror("open crash");
		return ;
		//ou free_exit();
	}
	if (delim[0] == '\"')
		new_delim = ft_strtrim(delim, '\"');
	else
		new_delim = delim;
	char	*line;
	line = get_next_line(0);
	while (line && ft_strncmp(line, new_delim, ft_strlen(new_delim)) != 0)
	{
		if (line)
			write(fd_arr[turn], line, ft_strlen(line));
		line = get_next_line(0);
	}
	if (line)
		write(fd_arr[turn], line, ft_strlen(line));
	write(fd_arr[turn], 0, 1);
}
