#include "cub3d.h"

typedef struct s_parse
{
	char	**lines;
	int		count;
	int		map_start;
	int		map_end;
}t_parse;

static size_t	str_len(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\r' || c == '\v' || c == '\f');
}

static int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static int	is_cub_path(const char *path)
{
	size_t	len;

	len = str_len(path);
	if (len < 4)
		return (0);
	if (path[len - 4] != '.' || path[len - 3] != 'c'
		|| path[len - 2] != 'u' || path[len - 1] != 'b')
		return (0);
	return (1);
}

static void	free_lines(char **lines)
{
	int	i;

	if (!lines)
		return ;
	i = 0;
	while (lines[i])
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}

static char	*dup_range(const char *s, int start, int end)
{
	char	*dup;
	int		i;

	if (end < start)
		end = start;
	dup = malloc((size_t)(end - start + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (start + i < end)
	{
		dup[i] = s[start + i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

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

static char	*read_all_file(const char *path)
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

static char	**split_lines(const char *all, int *line_count)
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
			lines[idx] = dup_range(all, start, i);
			if (!lines[idx])
				return (free_lines(lines), NULL);
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

static int	is_blank_line(const char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!is_space(line[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	starts_with_token(const char *line, const char *token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (line[i] != token[i])
			return (0);
		i++;
	}
	if (line[i] && !is_space(line[i]))
		return (0);
	return (1);
}

static char	*extract_value_after_key(const char *line, int key_len)
{
	int	start;
	int	end;

	start = key_len;
	while (line[start] && is_space(line[start]))
		start++;
	end = (int)str_len(line);
	while (end > start && is_space(line[end - 1]))
		end--;
	if (end <= start)
		return (NULL);
	return (dup_range(line, start, end));
}

static int	parse_rgb_value(const char *line, int rgb[3])
{
	int	i;
	int	j;
	int	value;

	i = 0;
	j = 0;
	while (j < 3)
	{
		while (line[i] && is_space(line[i]))
			i++;
		if (!is_digit(line[i]))
			return (1);
		value = 0;
		while (is_digit(line[i]))
			value = value * 10 + (line[i++] - '0');
		if (value < 0 || value > 255)
			return (1);
		rgb[j++] = value;
		while (line[i] && is_space(line[i]))
			i++;
		if (j < 3)
		{
			if (line[i] != ',')
				return (1);
			i++;
		}
	}
	while (line[i])
	{
		if (!is_space(line[i]))
			return (1);
		i++;
	}
	return (0);
}

static int	is_map_char(char c)
{
	return (c == ' ' || c == '0' || c == '1'
		|| c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == '\t');
}

static int	is_map_line(const char *line)
{
	int	i;
	int	has_map;

	i = 0;
	has_map = 0;
	while (line[i])
	{
		if (!is_map_char(line[i]))
			return (0);
		if (line[i] == '0' || line[i] == '1' || line[i] == 'N'
			|| line[i] == 'S' || line[i] == 'E' || line[i] == 'W')
			has_map = 1;
		i++;
	}
	return (has_map);
}

static int	set_header_value(char **dst, const char *line, int key_len)
{
	char	*value;

	if (*dst)
		return (1);
	value = extract_value_after_key(line, key_len);
	if (!value)
		return (1);
	*dst = value;
	return (0);
}

static int	parse_header_line(t_game *game, const char *line)
{
	if (starts_with_token(line, "NO"))
		return (set_header_value(&game->map.tex_no, line, 2));
	if (starts_with_token(line, "SO"))
		return (set_header_value(&game->map.tex_so, line, 2));
	if (starts_with_token(line, "WE"))
		return (set_header_value(&game->map.tex_we, line, 2));
	if (starts_with_token(line, "EA"))
		return (set_header_value(&game->map.tex_ea, line, 2));
	if (starts_with_token(line, "F"))
	{
		if (game->map.floor_rgb[0] != -1)
			return (1);
		return (parse_rgb_value(line + 1, game->map.floor_rgb));
	}
	if (starts_with_token(line, "C"))
	{
		if (game->map.ceiling_rgb[0] != -1)
			return (1);
		return (parse_rgb_value(line + 1, game->map.ceiling_rgb));
	}
	return (1);
}

static int	headers_complete(t_game *game)
{
	if (!game->map.tex_no || !game->map.tex_so || !game->map.tex_we
		|| !game->map.tex_ea)
		return (0);
	if (game->map.floor_rgb[0] < 0 || game->map.ceiling_rgb[0] < 0)
		return (0);
	return (1);
}

static int	preparse_lines(t_game *game, t_parse *p)
{
	int	i;
	int	map_started;
	int	blank_after_map;

	i = 0;
	map_started = 0;
	blank_after_map = 0;
	while (i < p->count)
	{
		if (!map_started && is_blank_line(p->lines[i]))
		{
			i++;
			continue ;
		}
		else if (!map_started && is_map_line(p->lines[i]))
			map_started = 1;
		else if (!map_started && parse_header_line(game, p->lines[i]) == 1)
			return (1);
		if (map_started)
		{
			if (!is_blank_line(p->lines[i]) && !is_map_line(p->lines[i]))
				return (1);
			if (is_blank_line(p->lines[i]))
				blank_after_map = 1;
			else if (blank_after_map)
				return (1);
		}
		i++;
	}
	if (!headers_complete(game) || !map_started)
		return (1);
	return (0);
}

static int	find_map_bounds(t_parse *p)
{
	int	i;

	p->map_start = -1;
	p->map_end = -1;
	i = 0;
	while (i < p->count)
	{
		if (is_map_line(p->lines[i]))
		{
			if (p->map_start < 0)
				p->map_start = i;
			p->map_end = i;
		}
		i++;
	}
	if (p->map_start < 0)
		return (1);
	return (0);
}

static int	max_map_width(t_parse *p)
{
	int	i;
	int	w;
	int	max;

	i = p->map_start;
	max = 0;
	while (i <= p->map_end)
	{
		w = (int)str_len(p->lines[i]);
		if (w > max)
			max = w;
		i++;
	}
	return (max);
}

static int	build_map_grid(t_game *game, t_parse *p)
{
	int	row;
	int	col;
	int	line_len;

	game->map.height = p->map_end - p->map_start + 1;
	game->map.width = max_map_width(p);
	game->map.grid = malloc(sizeof(char *) * (game->map.height + 1));
	if (!game->map.grid)
		return (1);
	row = 0;
	while (row < game->map.height)
	{
		game->map.grid[row] = malloc((size_t)game->map.width + 1);
		if (!game->map.grid[row])
			return (1);
		col = 0;
		while (col < game->map.width)
			game->map.grid[row][col++] = ' ';
		game->map.grid[row][col] = '\0';
		line_len = (int)str_len(p->lines[p->map_start + row]);
		col = 0;
		while (col < line_len)
		{
			if (p->lines[p->map_start + row][col] == '\t')
				game->map.grid[row][col] = ' ';
			else
				game->map.grid[row][col] = p->lines[p->map_start + row][col];
			col++;
		}
		row++;
	}
	game->map.grid[row] = NULL;
	return (0);
}

static int	set_spawn(t_game *game, int y, int x, char dir)
{
	if (game->map.spawn_dir != '\0')
		return (1);
	game->map.spawn_dir = dir;
	game->map.spawn_x = (double)x + 0.5;
	game->map.spawn_y = (double)y + 0.5;
	game->map.grid[y][x] = '0';
	return (0);
}

static int	validate_char_and_spawn(t_game *game)
{
	int	y;
	int	x;
	char	c;

	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width)
		{
			c = game->map.grid[y][x];
			if (!(c == ' ' || c == '0' || c == '1' || c == 'N'
					|| c == 'S' || c == 'E' || c == 'W'))
				return (1);
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
				if (set_spawn(game, y, x, c) == 1)
					return (1);
			x++;
		}
		y++;
	}
	if (game->map.spawn_dir == '\0')
		return (1);
	return (0);
}

static int	is_open_cell(char c)
{
	return (c == '0');
}

static int	validate_closed_map(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width)
		{
			if (is_open_cell(game->map.grid[y][x]))
			{
				if (y == 0 || x == 0 || y == game->map.height - 1
					|| x == game->map.width - 1)
					return (1);
				if (game->map.grid[y - 1][x] == ' ' || game->map.grid[y + 1][x] == ' '
					|| game->map.grid[y][x - 1] == ' ' || game->map.grid[y][x + 1] == ' ')
					return (1);
			}
			x++;
		}
		y++;
	}
	return (0);
}

int	load_map_from_cub(t_game *game, const char *path)
{
	char	*all;
	t_parse	p;

	if (!is_cub_path(path))
		return (error_exit("Expected a .cub file"));
	game->map.floor_rgb[0] = -1;
	game->map.ceiling_rgb[0] = -1;
	all = read_all_file(path);
	if (!all)
		return (error_exit("Cannot read map file"));
	p.lines = split_lines(all, &p.count);
	free(all);
	if (!p.lines)
		return (error_exit("Memory allocation failed"));
	if (preparse_lines(game, &p) == 1 || find_map_bounds(&p) == 1
		|| build_map_grid(game, &p) == 1 || validate_char_and_spawn(game) == 1
		|| validate_closed_map(game) == 1)
		return (free_lines(p.lines), error_exit("Invalid .cub content"));
	free_lines(p.lines);
	return (0);
}
