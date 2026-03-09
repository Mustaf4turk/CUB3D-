/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muturk <muturk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 19:08:26 by muturk            #+#    #+#             */
/*   Updated: 2026/03/09 19:08:27 by muturk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

static void	free_grid(char **grid, int h)
{
	int	y;

	y = 0;
	while (y < h)
		free(grid[y++]);
	free(grid);
}

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

static int	flood_fill(t_flood *f, int y, int x)
{
	if (y < 0 || y >= f->h || x < 0 || x >= f->w)
		return (1);
	if (f->grid[y][x] == ' ')
		return (1);
	if (f->grid[y][x] == '1' || f->grid[y][x] == 'F')
		return (0);
	f->grid[y][x] = 'F';
	if (flood_fill(f, y - 1, x) == 1)
		return (1);
	if (flood_fill(f, y + 1, x) == 1)
		return (1);
	if (flood_fill(f, y, x - 1) == 1)
		return (1);
	if (flood_fill(f, y, x + 1) == 1)
		return (1);
	return (0);
}

int	p_validate_closed_map(t_game *game)
{
	t_flood	f;
	int		y;
	int		x;
	int		ret;

	f.grid = dup_grid(game);
	if (!f.grid)
		return (1);
	f.h = game->map.height;
	f.w = game->map.width;
	ret = 0;
	y = -1;
	while (++y < f.h && ret == 0)
	{
		x = -1;
		while (++x < f.w && ret == 0)
		{
			if (f.grid[y][x] == '0')
				ret = flood_fill(&f, y, x);
		}
	}
	free_grid(f.grid, f.h);
	return (ret);
}
