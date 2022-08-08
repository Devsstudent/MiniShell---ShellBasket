/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pick_me_a_color.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbelrhaz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 19:38:40 by mbelrhaz          #+#    #+#             */
/*   Updated: 2022/08/08 20:21:51 by mbelrhaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../libft/libft.h"

//for the builtins, does the makefile have to compile them ? 
//Then we'll have the executables, and we'll be able to use them ?

int	main(void)
{
	//random color !!
	//We'll use the miguel formula
	const int	a = 0;
	size_t		i;
	
	i = 0;
	//unimportant chitchat
	ft_printf("\nHow are you doing today ?\n\n\033[38;5;33m> \033[m");
	get_next_line(0);
	ft_printf("\nWhat did you eat this morning ?\n\n\033[38;5;33m> \033[m");
	get_next_line(0);
	ft_printf("\nHmm, interesting...\n");
	while (i < 1000000000)
		i++;
	printf("Your color is...\n");
	i = 0;
	while (i < 1000000000)
		i++;
	if (((size_t)&a / 10) % 2 == 0)
		ft_printf("\n\033[38;5;%dmWhite !!\033[m\n\n", (((size_t)&a / 10)) % 10 + 30);
	else
		ft_printf("\nBlack !!\n\n");
	return 0;
}

