/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 14:11:56 by odessein          #+#    #+#             */
/*   Updated: 2022/09/06 13:15:35 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <signal.h>
#include <errno.h>
#include "libft.h"

void	listen_sigs(void)
{
	struct sigaction	action_quit;

	sigemptyset(&action_quit.sa_mask);
	action_quit.sa_handler = SIG_IGN;
	action_quit.sa_flags = 0;
	sigaction(SIGQUIT, &action_quit, NULL);
}

char	*get_next_line(int fd)
{
	static char	buff[FOPEN_MAX][BUFFER_SIZE + 1];
	char		*line;

	if (fd < 0 || fd > FOPEN_MAX - 1)
		return (NULL);
	line = (char *) malloc(sizeof(*line));
	if (!line)
		return (NULL);
	*line = 0;
	if (*buff != 0)
		line = gnl_strjoin(line, buff[fd]);
	if (ft_check_line(line))
		ft_reset_buff(buff[fd]);
	listen_sigs();
	line = gnl_loop(line, buff[fd], fd);
	return (line);
}

char	*gnl_loop(char *line, char *buff, int fd)
{
	while (!ft_check_line(line))
	{
		if (!ft_fill_buff(buff, fd))
		{
			if (*line == 0)
				free(line);
			else
			{
				ft_reset_buff(buff);
				return (line);
			}
			return (NULL);
		}
		line = gnl_strjoin(line, buff);
		ft_reset_buff(buff);
	}
	return (line);
}

t_bool	ft_fill_buff(char *buff, int fd)
{
	int	read_val;

	read_val = read(fd, buff, BUFFER_SIZE);
	if (read_val > 0 && errno != EINTR)
	{
		buff[read_val] = 0;
		return (TRUE);
	}
	else if (errno == EINTR)
	{
		write(2, "YO", 2);
	}
	return (FALSE);
}

t_bool	ft_check_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\n')
			return (TRUE);
		i++;
	}
	return (FALSE);
}

char	*ft_reset_buff(char *buff)
{
	int		i;
	char	*new_buff;

	i = 0;
	while (buff[i] != '\n' && buff[i] != '\0')
		i++;
	if (buff[i] == '\n')
		i++;
	new_buff = gnl_strdup(&(buff[i]));
	if (!new_buff)
		return (NULL);
	ft_bzero(buff, BUFFER_SIZE);
	i = 0;
	while (new_buff[i])
	{
		buff[i] = new_buff[i];
		i++;
	}
	buff[i] = new_buff[i];
	free(new_buff);
	return (buff);
}
