#include "cub3d.h"

static int	is_cub_path(const char *path)
{
	int	len;

	len = 0;
	while (path[len])
		len++;
	if (len < 4)
		return (0);
	if (path[len - 4] != '.' || path[len - 3] != 'c'
		|| path[len - 2] != 'u' || path[len - 1] != 'b')
		return (0);
	return (1);
}

static char	*dup_line(const char *s)
{
	int		len;
	int		i;
	char	*dup;

	len = 0;
	while (s[len])
		len++;
	dup = malloc((size_t)len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int	load_map_from_cub(t_game *game, const char *path)
{
	int		fd;
	char	*demo[] = {
		"111111111111",
		"100000000001",
		"101111011101",
		"100000010001",
		"101011010101",
		"100000000001",
		"111111111111",
		NULL
	};
	int		i;

	if (!is_cub_path(path))
		return (error_exit("Expected a .cub file"));
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (error_exit("Cannot open map file"));
	close(fd);
	game->map.height = 7;
	game->map.width = 12;
	game->map.grid = malloc(sizeof(char *) * 8);
	if (!game->map.grid)
		return (error_exit("Map allocation failed"));
	i = 0;
	while (i < game->map.height)
	{
		game->map.grid[i] = dup_line(demo[i]);
		if (!game->map.grid[i])
			return (error_exit("Map row allocation failed"));
		i++;
	}
	game->map.grid[i] = NULL;
	return (0);
}
