#include "cub3d.h"

static int	is_walkable(t_game *game, double x, double y)
{
	int	mx;
	int	my;

	mx = (int)x;
	my = (int)y;
	if (mx < 0 || my < 0 || my >= game->map.height || mx >= game->map.width)
		return (0);
	return (game->map.grid[my][mx] != '1');
}

static void	move_player(t_game *game)
{
	double	nx;
	double	ny;

	nx = game->player.x;
	ny = game->player.y;
	if (game->player.move_y != 0)
	{
		nx += game->player.dir_x * MOVE_SPEED * game->player.move_y;
		ny += game->player.dir_y * MOVE_SPEED * game->player.move_y;
	}
	if (game->player.move_x != 0)
	{
		nx += game->player.plane_x * MOVE_SPEED * game->player.move_x;
		ny += game->player.plane_y * MOVE_SPEED * game->player.move_x;
	}
	if (is_walkable(game, nx, game->player.y))
		game->player.x = nx;
	if (is_walkable(game, game->player.x, ny))
		game->player.y = ny;
}

static void	rotate_player(t_game *game)
{
	double	old_dir_x;
	double	old_plane_x;
	double	a;

	if (game->player.rot == 0)
		return ;
	a = ROT_SPEED * game->player.rot;
	old_dir_x = game->player.dir_x;
	game->player.dir_x = game->player.dir_x * cos(a) - game->player.dir_y * sin(a);
	game->player.dir_y = old_dir_x * sin(a) + game->player.dir_y * cos(a);
	old_plane_x = game->player.plane_x;
	game->player.plane_x = game->player.plane_x * cos(a)
		- game->player.plane_y * sin(a);
	game->player.plane_y = old_plane_x * sin(a)
		+ game->player.plane_y * cos(a);
}

int	update_frame(t_game *game)
{
	move_player(game);
	rotate_player(game);
	render_frame(game);
	return (0);
}
