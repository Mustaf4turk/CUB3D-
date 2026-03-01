#include "cub3d.h"

static void	init_player(t_game *game)
{
	game->player.x = game->map.spawn_x;
	game->player.y = game->map.spawn_y;
	if (game->map.spawn_dir == 'N')
	{
		game->player.dir_x = 0.0;
		game->player.dir_y = -1.0;
		game->player.plane_x = 0.66;
		game->player.plane_y = 0.0;
	}
	else if (game->map.spawn_dir == 'S')
	{
		game->player.dir_x = 0.0;
		game->player.dir_y = 1.0;
		game->player.plane_x = -0.66;
		game->player.plane_y = 0.0;
	}
	else if (game->map.spawn_dir == 'E')
	{
		game->player.dir_x = 1.0;
		game->player.dir_y = 0.0;
		game->player.plane_x = 0.0;
		game->player.plane_y = 0.66;
	}
	else
	{
		game->player.dir_x = -1.0;
		game->player.dir_y = 0.0;
		game->player.plane_x = 0.0;
		game->player.plane_y = -0.66;
	}
	game->player.move_x = 0;
	game->player.move_y = 0;
	game->player.rot = 0;
}

int	init_game(t_game *game, const char *map_path)
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
	if (load_map_from_cub(game, map_path) != 0)
		return (cleanup_game(game), 1);
	init_player(game);
	game->floor_color = 0xFF000000 | (game->map.floor_rgb[0] << 16)
		| (game->map.floor_rgb[1] << 8) | game->map.floor_rgb[2];
	game->ceiling_color = 0xFF000000 | (game->map.ceiling_rgb[0] << 16)
		| (game->map.ceiling_rgb[1] << 8) | game->map.ceiling_rgb[2];
	game->mlx = mlx_init();
	if (!game->mlx)
		return (cleanup_game(game), error_exit("mlx_init failed"));
	game->win = mlx_new_window(game->mlx, WIN_W, WIN_H, "cub3D");
	if (!game->win)
		return (cleanup_game(game), error_exit("mlx_new_window failed"));
	game->img.ptr = mlx_new_image(game->mlx, WIN_W, WIN_H);
	if (!game->img.ptr)
		return (cleanup_game(game), error_exit("mlx_new_image failed"));
	game->img.addr = mlx_get_data_addr(game->img.ptr,
			&game->img.bpp, &game->img.line_len, &game->img.endian);
	if (load_textures(game) != 0)
		return (cleanup_game(game), 1);
	return (0);
}
