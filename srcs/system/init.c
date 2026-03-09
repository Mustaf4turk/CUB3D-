/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muturk <muturk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 19:09:04 by muturk            #+#    #+#             */
/*   Updated: 2026/03/09 19:09:05 by muturk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_game_null(t_game *game)
{
	game->mlx = NULL;
	game->win = NULL;
	game->img.ptr = NULL;
	game->map.grid = NULL;
	game->map.tex_no = NULL;
	game->map.tex_so = NULL;
	game->map.tex_we = NULL;
	game->map.tex_ea = NULL;
	game->map.spawn_dir = '\0';
	game->tex[0].img = NULL;
	game->tex[1].img = NULL;
	game->tex[2].img = NULL;
	game->tex[3].img = NULL;
	game->floor_color = 0;
	game->ceiling_color = 0;
}

static int	init_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (error_exit("mlx_init failed"));
	game->win = mlx_new_window(game->mlx, WIN_W, WIN_H, "cub3D");
	if (!game->win)
		return (error_exit("mlx_new_window failed"));
	game->img.ptr = mlx_new_image(game->mlx, WIN_W, WIN_H);
	if (!game->img.ptr)
		return (error_exit("mlx_new_image failed"));
	game->img.addr = mlx_get_data_addr(game->img.ptr,
			&game->img.bpp, &game->img.line_len, &game->img.endian);
	if (load_textures(game) != 0)
		return (1);
	return (0);
}

int	init_game(t_game *game, const char *map_path)
{
	init_game_null(game);
	if (load_map_from_cub(game, map_path) != 0)
		return (cleanup_game(game), 1);
	init_player(game);
	game->floor_color = 0xFF000000 | (game->map.floor_rgb[0] << 16)
		| (game->map.floor_rgb[1] << 8) | game->map.floor_rgb[2];
	game->ceiling_color = 0xFF000000 | (game->map.ceiling_rgb[0] << 16)
		| (game->map.ceiling_rgb[1] << 8) | game->map.ceiling_rgb[2];
	if (init_mlx(game) != 0)
		return (cleanup_game(game), 1);
	return (0);
}
