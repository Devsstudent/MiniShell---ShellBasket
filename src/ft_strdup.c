/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 17:13:01 by odessein          #+#    #+#             */
/*   Updated: 2022/05/13 15:29:49 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	*ft_strdup(const char *s)
{
	char	*res;
	size_t	len;
	int		i;

	i = 0;
	len = ft_strlen(s);
	res = (char *) malloc(sizeof(char) * (len + 1));
	if (!res)
		free_exit();
	while (s[i] != '\0')
	{
		res[i] = s[i];
		i++;
	}
	res[i] = s[i];
	return (res);
}
