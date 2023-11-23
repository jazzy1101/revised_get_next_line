/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabae <dabae@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 12:07:09 by dabae             #+#    #+#             */
/*   Updated: 2023/11/23 17:13:19 by dabae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

static	void	clean_keep_rest(t_list **buf_list)
{
	t_list	*last;
	t_list	*new;
	int	i;
	int	j;

	new = malloc(sizeof(t_list));
	if (!buf_list)
		return ;
	if (!new)
	{
		free(new);
		return ;
	}
	new->next = NULL;
	while ((*buf_list)->next)
		*buf_list = (*buf_list)->next;
	last = *buf_list;
	i = 0;
	while (last->str_tmp[i] && last->str_tmp[i] != '\n')
		i++;
	if (last->str_tmp && last->str_tmp[i] == '\n')
		i++;
	new->str_tmp = malloc(sizeof(char) * (ft_strlen(last->str_tmp) - i + 1));
	if (!new->str_tmp)
	{
		free(new->str_tmp);
		return ;
	}
	j = 0;
	while (last->str_tmp[i])
		new->str_tmp[j++] = last->str_tmp[i++];
	new->str_tmp[j] = '\0';
	free_list(*buf_list);
	*buf_list = new;
}

static	void	retrieve_line(t_list *buf_list, char **line)
{
	int	i;
	int	j;

	if (!buf_list)
		return ;
	prep_line(line, buf_list);
	j = 0;
	while (buf_list)
	{
		i = 0;
		while (buf_list->str_tmp[i])
		{
			if (buf_list->str_tmp[i] == '\n')
			{
				(*line)[j++] = buf_list->str_tmp[i];
				break ;
			}
			(*line)[j++] = buf_list->str_tmp[i++];
		}
		buf_list = buf_list->next;
	}
	(*line)[j] = '\0';
}

static void    add_node(t_list **buf_list, char *buffer, int bytes_read)
{
        t_list  *new;
        t_list  *tmp;
        int     i;

        new = malloc(sizeof(t_list));
        if (!new)
	{
		write (1, "b", 1);
	/*	free(new);
		new = NULL;*/
        	return ;
	}
	new->str_tmp = malloc(sizeof(char) * (bytes_read + 1));
	new->next = NULL;
	if (new->str_tmp == NULL)
	{
		free(new);
		return ;
	}
	i = 0;
	while (buffer[i] && i < bytes_read)
	{
		new->str_tmp[i] = buffer[i];
		i++;
	}
	new->str_tmp[i] = '\0';
	if (*buf_list == NULL)
	{
		*buf_list = new;
		return ;
	}
	tmp = *buf_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

static	void	create_list(t_list **buf_list, int fd)
{
	char	*buffer;
	int		bytes_read;

	bytes_read = 1;
	while (!found_n(*buf_list) && bytes_read != 0)
	{
		buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buffer)
		{
			free(buffer);
			buffer = NULL;
			return ;
		}
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if ((*buf_list == NULL && bytes_read == 0) || bytes_read < 0)
		{
			free(buffer);
			buffer = NULL;
			return ;
		}
		buffer[bytes_read] = '\0';
		add_node(buf_list, buffer, bytes_read);
		free(buffer);
		buffer = NULL;
	}
}

char	*get_next_line(int fd)
{
	static t_list	*buf_list;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) == -1)
	{
		free_list(buf_list);
	       	buf_list = NULL;
		return (NULL);
	}
	line = NULL;
	create_list(&buf_list, fd);
	if (!buf_list)
		return (NULL);
	retrieve_line(buf_list, &line);
	clean_keep_rest(&buf_list);
	if (line[0] == '\0')
	{
		free_list(buf_list);
		buf_list = NULL;
		free(line);
		return (NULL);
	}
	return (line);
}
