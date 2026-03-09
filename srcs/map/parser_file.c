/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muturk <muturk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 19:07:55 by muturk            #+#    #+#             */
/*   Updated: 2026/03/09 19:07:56 by muturk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

static int	grow_buf(t_buf *buf, size_t need)
{
	char	*new_data;
	size_t	new_cap;
	size_t	i;

	new_cap = buf->cap;
	while (new_cap <= need)
		new_cap *= 2;
	new_data = malloc(new_cap);
	if (!new_data)
		return (1);
	i = 0;
	while (i < buf->len)
	{
		new_data[i] = buf->data[i];
		i++;
	}
	free(buf->data);
	buf->data = new_data;
	buf->cap = new_cap;
	return (0);
}

static int	append_chunk(t_buf *buf, const char *chunk, size_t chunk_len)
{
	size_t	i;

	if (buf->cap <= buf->len + chunk_len + 1)
	{
		if (grow_buf(buf, buf->len + chunk_len + 1) != 0)
			return (1);
	}
	i = 0;
	while (i < chunk_len)
	{
		buf->data[buf->len + i] = chunk[i];
		i++;
	}
	buf->len += chunk_len;
	buf->data[buf->len] = '\0';
	return (0);
}

static char	*read_loop(int fd, t_buf *buf)
{
	ssize_t	rd;
	char	chunk[1024];

	rd = read(fd, chunk, sizeof(chunk));
	while (rd > 0)
	{
		if (append_chunk(buf, chunk, (size_t)rd) == 1)
		{
			free(buf->data);
			return (NULL);
		}
		rd = read(fd, chunk, sizeof(chunk));
	}
	if (rd < 0)
	{
		free(buf->data);
		return (NULL);
	}
	return (buf->data);
}

char	*p_read_all_file(const char *path)
{
	int		fd;
	t_buf	buf;
	char	*result;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	buf.cap = 2048;
	buf.len = 0;
	buf.data = malloc(buf.cap);
	if (!buf.data)
	{
		close(fd);
		return (NULL);
	}
	buf.data[0] = '\0';
	result = read_loop(fd, &buf);
	close(fd);
	return (result);
}
