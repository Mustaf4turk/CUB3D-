#include "parser_internal.h"

static int	is_cub_path(const char *path)
{
	size_t	len;

	len = p_strlen(path);
	if (len < 4)
		return (0);
	if (path[len - 4] != '.' || path[len - 3] != 'c'
		|| path[len - 2] != 'u' || path[len - 1] != 'b')
		return (0);
	return (1);
}

int	load_map_from_cub(t_game *game, const char *path)
{
	char	*all;
	t_parse	p;

	if (!is_cub_path(path))
		return (error_exit("Expected a .cub file"));
	game->map.floor_rgb[0] = -1;
	game->map.ceiling_rgb[0] = -1;
	all = p_read_all_file(path);
	if (!all)
		return (error_exit("Cannot read map file"));
	p.lines = p_split_lines(all, &p.count);
	free(all);
	if (!p.lines)
		return (error_exit("Memory allocation failed"));
	if (p_preparse_lines(game, &p) == 1)
		return (p_free_lines(p.lines), error_exit("Invalid header or map data"));
	if (p_find_map_bounds(&p) == 1)
		return (p_free_lines(p.lines), error_exit("Map not found in file"));
	if (p_build_map_grid(game, &p) == 1)
		return (p_free_lines(p.lines), error_exit("Failed to build map grid"));
	if (p_validate_char_and_spawn(game) == 1)
		return (p_free_lines(p.lines), error_exit("Invalid char or missing spawn"));
	if (p_validate_closed_map(game) == 1)
		return (p_free_lines(p.lines), error_exit("Map is not closed by walls"));
	if (p_check_tex_files(game) == 1)
		return (p_free_lines(p.lines), error_exit("Invalid texture file path"));
	p_free_lines(p.lines);
	return (0);
}
