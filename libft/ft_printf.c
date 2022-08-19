/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 14:52:20 by odessein          #+#    #+#             */
/*   Updated: 2022/08/19 13:18:25 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_printf(t_bool err, const char *string, ...)
{
	va_list		ap;
	int			ret_val;
	int			stout;
	int			sterr;

	ret_val = 0;
	stout = dup(STDOUT_FILENO);
	sterr = dup(STDERR_FILENO);
	if (err)
	{
		if (dup2(STDOUT_FILENO, STDERR_FILENO) == -1)
		{
			ft_putstr_fd("erreur from dup2\n", 2);
			return (ret_val);
		}
	}
	va_start(ap, string);
	ft_loop((char *) string, ap, &ret_val);
	va_end(ap);
	if (err)
	{
		if (dup2(stout, STDOUT_FILENO) == -1)
			ft_putstr_fd("erreur from dup2\n", 2);
		if (dup2(sterr, STDERR_FILENO) == -1)
			ft_putstr_fd("erreur from dup2\n", 2);
	}
	return (ret_val);
}

void	ft_sharp(char x, int *r_v)
{
	printf_putchar('0', r_v);
	printf_putchar(x, r_v);
}
