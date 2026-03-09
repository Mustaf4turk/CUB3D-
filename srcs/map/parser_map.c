/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muturk <muturk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 19:08:13 by muturk            #+#    #+#             */
/*   Updated: 2026/03/09 19:08:14 by muturk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

static int	max_map_width(t_parse *p)
{
	int	i;
	int	w;
	int	max;

	i = p->map_start;
	max = 0;
	while (i <= p->map_end)
	{
		w = (int)p_strlen(p->lines[i]);
		if (w > max)
			max = w;
		i++;
	}
	return (max);
}

static void	fill_row(t_game *game, t_parse *p, int row)
{
	int		col;
	int		line_len;
	char	*src;

	col = 0;
	while (col < game->map.width)
		game->map.grid[row][col++] = ' ';
	game->map.grid[row][col] = '\0';
	src = p->lines[p->map_start + row];
	line_len = (int)p_strlen(src);
	col = 0;
	while (col < line_len)
	{
		if (src[col] == '\t')
			game->map.grid[row][col] = ' ';
		else
			game->map.grid[row][col] = src[col];
		col++;
	}
}

int	p_build_map_grid(t_game *game, t_parse *p)
{
	int	row;

	game->map.height = p->map_end - p->map_start + 1;
	game->map.width = max_map_width(p);
	game->map.grid = malloc(sizeof(char *) * (game->map.height + 1));
	if (!game->map.grid)
		return (1);
	row = 0;
	while (row <= game->map.height)
		game->map.grid[row++] = NULL;
	row = 0;
	while (row < game->map.height)
	{
		game->map.grid[row] = malloc((size_t)game->map.width + 1);
		if (!game->map.grid[row])
			return (1);
		fill_row(game, p, row);
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

int	p_validate_char_and_spawn(t_game *game)
{
	int		y;
	int		x;
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
