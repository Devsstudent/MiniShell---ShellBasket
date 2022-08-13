************************************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abstract_syntax_tree_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 15:47:32 by odessein          #+#    #+#             */
/*   Updated: 2022/08/13 18:46:38 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//Function qui cree des block pour les parentheses -> sub line

//Linear priority so left operator at the top

//Check if block on left if "parentheses"

	//if it is so redo 1 2 3

	//else check the right part 


t_line	*fill_parentheses_block(t_block **buff)
{
	t_line	*new_sub;

	new_sub = (t_line *) malloc(sizeof(t_line));
	if (!new_sub)
		free_exit();
	else
		add_to_gc(LINE, new_sub, get_gc());
	new_sub->head = NULL;
	line_cpy_till_pend(buff, new_sub);
	return (new_sub);
}

void	test(t_leaf *leaf, t_line *line)
{
	t_block	*buff;
	t_line	*sub;

	//Read les opearator de la ligne au fur et a mesure
	//Si on a des parentheses on recurs sur le sous block 
	//puis on pass au prochians operators de la ligne de base etc etc 
	//*1*remove les parentheses start et end ;
	remove_parenthese(line); -> delete les block parenthese si au debut et fin
	buff = line->head;
	while (buff)
	{
		if (buff->token == P_OPEN)
			sub = fill_parentheses(&buff);
		else
			sub = fill_till_ope(&buff);
		if (buff == NULL)
		{
			last_elem(sub, leaf);
			return ;
		}
		leaf->type = get_type(buff->token);
		leaf->left = new_leaf(sub);
		if (leaf->left->type == P)
			test(leaf->left, sub);
		leaf->right = new_leaf(NULL);
		leaf = leaf->right;
		//Cree les sous_lignes
		//create node 1 and put it to the left
		//place operator on top
		//if node 1 with parentheses -> recurse
		//put node 2 in the right position depending on whether or not we have another operator 
		//when a node placed has parentheses, recurse
		buff = buff->next;
	}
	
}

//recursion !!!
//first, if parentheses at the beignning and end of block, discard them
//second, parse the subline, make nodes and put them in place,
//browse the tree
//if parentheses, recurse
//when is the end ?
//when we browse the tree and don't find parentheses, it is the end

void	fill_ast_bonus(t_line *line, t_tree *tree)
{
	t_block	*buff;

	buff = line->head;
	//Browser 
}
