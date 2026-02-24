#include "cub3d.h"

static void	init_player(t_game *game)
{
	game->player.x = 3.5;
	game->player.y = 3.5;
	game->player.dir_x = -1.0;
	game->player.dir_y = 0.0;
	game->player.plane_x = 0.0;
	game->player.plane_y = 0.66;
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
	if (load_map_from_cub(game, map_path) != 0)
		return (1);
	init_player(game);
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
	return (0);
}
