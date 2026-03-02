#include "parser_internal.h"

static char	**dup_grid(t_game *game)
{
	char	**copy;
	int		y;
	int		x;

	copy = malloc(sizeof(char *) * (game->map.height + 1));
	if (!copy)
		return (NULL);
	y = -1;
	while (++y < game->map.height)
	{
		copy[y] = malloc((size_t)game->map.width + 1);
		if (!copy[y])
		{
			while (--y >= 0)
				free(copy[y]);
			return (free(copy), NULL);
		}
		x = -1;
		while (++x < game->map.width)
			copy[y][x] = game->map.grid[y][x];
		copy[y][x] = '\0';
	}
	copy[y] = NULL;
	return (copy);
}

static int	flood_fill(char **grid, int h, int w, int y, int x)
{
	if (y < 0 || y >= h || x < 0 || x >= w)
		return (1);
	if (grid[y][x] == ' ')
		return (1);
	if (grid[y][x] == '1' || grid[y][x] == 'F')
		return (0);
	grid[y][x] = 'F';
	if (flood_fill(grid, h, w, y - 1, x) == 1)
		return (1);
	if (flood_fill(grid, h, w, y + 1, x) == 1)
		return (1);
	if (flood_fill(grid, h, w, y, x - 1) == 1)
		return (1);
	if (flood_fill(grid, h, w, y, x + 1) == 1)
		return (1);
	return (0);
}

int	p_validate_closed_map(t_game *game)
{
	char	**copy;
	int		y;
	int		x;
	int		ret;

	copy = dup_grid(game);
	if (!copy)
		return (1);
	ret = 0;
	y = -1;
	while (++y < game->map.height && ret == 0)
	{
		x = -1;
		while (++x < game->map.width && ret == 0)
		{
			if (copy[y][x] == '0')
				ret = flood_fill(copy, game->map.height,
						game->map.width, y, x);
		}
	}
	y = 0;
	while (y < game->map.height)
		free(copy[y++]);
	free(copy);
	return (ret);
}

static int	check_tex_file(const char *path)
{
	int		fd;
	size_t	len;

	if (!path)
		return (1);
	len = p_strlen(path);
	if (len < 4 || path[len - 4] != '.' || path[len - 3] != 'x'
		|| path[len - 2] != 'p' || path[len - 1] != 'm')
		return (1);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (1);
	close(fd);
	return (0);
}

int	p_check_tex_files(t_game *game)
{
	if (check_tex_file(game->map.tex_no) != 0)
		return (1);
	if (check_tex_file(game->map.tex_so) != 0)
		return (1);
	if (check_tex_file(game->map.tex_we) != 0)
		return (1);
	if (check_tex_file(game->map.tex_ea) != 0)
		return (1);
	return (0);
}
