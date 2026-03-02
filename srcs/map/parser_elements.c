#include "parser_internal.h"

int	p_is_map_char(char c)
{
	return (c == ' ' || c == '0' || c == '1'
		|| c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == '\t');
}

int	p_is_map_line(const char *line)
{
	int	i;
	int	has_map;

	i = 0;
	has_map = 0;
	while (line[i])
	{
		if (!p_is_map_char(line[i]))
			return (0);
		if (line[i] == '0' || line[i] == '1' || line[i] == 'N'
			|| line[i] == 'S' || line[i] == 'E' || line[i] == 'W')
			has_map = 1;
		i++;
	}
	return (has_map);
}

int	p_headers_complete(t_game *game)
{
	if (!game->map.tex_no || !game->map.tex_so || !game->map.tex_we
		|| !game->map.tex_ea)
		return (0);
	if (game->map.floor_rgb[0] < 0 || game->map.ceiling_rgb[0] < 0)
		return (0);
	return (1);
}

int	p_preparse_lines(t_game *game, t_parse *p)
{
	int	i;
	int	map_started;
	int	blank_after_map;

	i = 0;
	map_started = 0;
	blank_after_map = 0;
	while (i < p->count)
	{
		if (!map_started && p_is_blank_line(p->lines[i]))
		{
			i++;
			continue ;
		}
		else if (!map_started && p_is_map_line(p->lines[i]))
			map_started = 1;
		else if (!map_started
			&& p_parse_header_line(game, p->lines[i]) == 1)
			return (1);
		if (map_started)
		{
			if (!p_is_blank_line(p->lines[i])
				&& !p_is_map_line(p->lines[i]))
				return (1);
			if (p_is_blank_line(p->lines[i]))
				blank_after_map = 1;
			else if (blank_after_map)
				return (1);
		}
		i++;
	}
	if (!p_headers_complete(game) || !map_started)
		return (1);
	return (0);
}

int	p_find_map_bounds(t_parse *p)
{
	int	i;

	p->map_start = -1;
	p->map_end = -1;
	i = 0;
	while (i < p->count)
	{
		if (p_is_map_line(p->lines[i]))
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
