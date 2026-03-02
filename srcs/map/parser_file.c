#include "parser_internal.h"

static int	append_chunk(char **dst, size_t *cap, size_t *len,
		const char *chunk, size_t chunk_len)
{
	char	*new_buf;
	size_t	new_cap;
	size_t	i;

	if (*cap <= *len + chunk_len + 1)
	{
		new_cap = *cap;
		while (new_cap <= *len + chunk_len + 1)
			new_cap *= 2;
		new_buf = malloc(new_cap);
		if (!new_buf)
			return (1);
		i = 0;
		while (i < *len)
		{
			new_buf[i] = (*dst)[i];
			i++;
		}
		free(*dst);
		*dst = new_buf;
		*cap = new_cap;
	}
	i = 0;
	while (i < chunk_len)
	{
		(*dst)[*len + i] = chunk[i];
		i++;
	}
	*len += chunk_len;
	(*dst)[*len] = '\0';
	return (0);
}

char	*p_read_all_file(const char *path)
{
	int		fd;
	ssize_t	read_bytes;
	char	chunk[1024];
	char	*buf;
	size_t	cap;
	size_t	len;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	cap = 2048;
	len = 0;
	buf = malloc(cap);
	if (!buf)
		return (close(fd), NULL);
	buf[0] = '\0';
	read_bytes = read(fd, chunk, sizeof(chunk));
	while (read_bytes > 0)
	{
		if (append_chunk(&buf, &cap, &len, chunk, (size_t)read_bytes) == 1)
			return (close(fd), free(buf), NULL);
		read_bytes = read(fd, chunk, sizeof(chunk));
	}
	close(fd);
	if (read_bytes < 0)
		return (free(buf), NULL);
	return (buf);
}

char	**p_split_lines(const char *all, int *line_count)
{
	char	**lines;
	int		count;
	int		i;
	int		start;
	int		idx;

	count = 1;
	i = -1;
	while (all[++i])
		if (all[i] == '\n')
			count++;
	lines = malloc(sizeof(char *) * (count + 1));
	if (!lines)
		return (NULL);
	idx = 0;
	start = 0;
	i = 0;
	while (1)
	{
		if (all[i] == '\n' || all[i] == '\0')
		{
			lines[idx] = p_dup_range(all, start, i);
			if (!lines[idx])
				return (p_free_lines(lines), NULL);
			idx++;
			start = i + 1;
			if (all[i] == '\0')
				break ;
		}
		i++;
	}
	lines[idx] = NULL;
	*line_count = idx;
	return (lines);
}

int	p_is_blank_line(const char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!p_is_space(line[i]))
			return (0);
		i++;
	}
	return (1);
}
