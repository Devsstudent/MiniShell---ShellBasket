/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <mbelrhaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 16:30:06 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/07/29 18:30:15 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_gc	*gc;
	char	*test0 = ft_strdup("lol");
	char	*test1 = ft_strdup("hoho");
	char	*test2 = ft_strdup("haha");
	char	**split = ft_split("super oui ca marche", ' ');

	(void)ac;
	(void)av;
	(void)envp;

	gc = NULL;
	add_to_gc(SIMPLE, test0, &gc); //pipe is not a type, but a token, that was the problem (face palm)
	printf("%p, %s\n", gc, (char *)gc->content);
	add_to_gc(SIMPLE, test1, &gc);

	printf("\nadding another simple\n\n");

	printf("%p, %s\n", gc, (char *)gc->content);
	printf("%p, %s\n", gc, (char *)gc->next->content);
	add_to_gc(SIMPLE, test2, &gc);

	printf("\nadding another simple\n\n");

	printf("%p, %s\n", gc, (char *)gc->content);
	printf("%p, %s\n", gc, (char *)gc->next->content);
	printf("%p, %s\n", gc, (char *)gc->next->next->content);
	add_to_gc(DOUBLE, split, &gc);

	printf("\nadding a double\n\n");

	gc_free_node_addr(test0, &gc);
	printf("\ndeleting the first one\n\n");
	printf("%p, %s\n", gc, (char *)gc->content);
	printf("%p, %s\n", gc, (char *)gc->next->content);

	gc_free_node_addr(test2, &gc);
	printf("\ndeleting the second one\n\n");
	printf("%p, %s\n", gc, (char *)gc->content);
	printf("%p\n", gc->next);
	printf("%p\n", gc->next->next);

	printf("\ndeleting everything\n\n");
	free_gc(&gc);
	/*
	while (ac)
	{
		//Faire une copie de envp pour avoir notre propre environnement
	}*/
}
