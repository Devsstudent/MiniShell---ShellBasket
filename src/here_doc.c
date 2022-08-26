#include "minishell.h"

//Cree un tmp file pour chaque block here_doc

//Si plusieurs heredoc on ecrit dans le premier puis le deuxieme etc

//TRUNC pas APPEND dcp

//ReChecker le content du fd pour expand les value si on doit expand 

//Le fd du file sera le fd du infile dans les redirections

//Ensuite on close tous les tmp file dans la free_each_turn

void	parse_here_doc(t_leaf *leaf)
{
	if (leaf->type == PIPE_L)
	{
		check_here_doc(leaf->left->content);
		if (leaf->right)
			parse_here_doc(leaf->right)
	}
	else if (leaf->type == CMD)
		check_here_doc(leaf->content);
}

void	check_here_doc(t_line *sub)
{
	t_block	*buff;

	buff = sub->head;
	while (buff)
	{
		//Ou on check les delimiter et on envoie pas le next a choix
		if (buff->token == HERE_DOC)
			create_and_fill(buff->next->word)
		buff = buff->next;
	}
}
//Malloc un tableau de fd pour chaque line avec le nombre de sub_line

void	create_and_fill(char *delim)
{
	int	fd;

	join pour le name // checker que il soit pas deja present peut etre sinon modifier son nom jsp / 1 2 3 etc en function du block
	fd = open(".tmp_here_doc", RDW_ONLY | O_CREATE | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("open crash");
		return ;
		//ou free_exit();
	}
	gnl qui write dans ce fd;
	//gnl ou readline pour 
}
