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

char	*p_read_all_file(const char *path)
{
	int		fd;
	ssize_t	rd;
	char	chunk[1024];
	t_buf	buf;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	buf.cap = 2048;
	buf.len = 0;
	buf.data = malloc(buf.cap);
	if (!buf.data)
		return (close(fd), NULL);
	buf.data[0] = '\0';
	rd = read(fd, chunk, sizeof(chunk));
	while (rd > 0)
	{
		if (append_chunk(&buf, chunk, (size_t)rd) == 1)
			return (close(fd), free(buf.data), NULL);
		rd = read(fd, chunk, sizeof(chunk));
	}
	close(fd);
	if (rd < 0)
		return (free(buf.data), NULL);
	return (buf.data);
}
