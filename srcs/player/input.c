#include "cub3d.h"

static int	on_destroy(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	cleanup_game(game);
	exit(0);
}

static int	on_key_press(int key, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (key == KEY_ESC)
		return (on_destroy(game));
	if (key == KEY_W)
		game->player.move_y = 1;
	if (key == KEY_S)
		game->player.move_y = -1;
	if (key == KEY_D)
		game->player.move_x = 1;
	if (key == KEY_A)
		game->player.move_x = -1;
	if (key == KEY_LEFT)
		game->player.rot = -1;
	if (key == KEY_RIGHT)
		game->player.rot = 1;
	return (0);
}

static int	on_key_release(int key, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (key == KEY_W && game->player.move_y == 1)
		game->player.move_y = 0;
	if (key == KEY_S && game->player.move_y == -1)
		game->player.move_y = 0;
	if (key == KEY_D && game->player.move_x == 1)
		game->player.move_x = 0;
	if (key == KEY_A && game->player.move_x == -1)
		game->player.move_x = 0;
	if ((key == KEY_LEFT && game->player.rot == -1)
		|| (key == KEY_RIGHT && game->player.rot == 1))
		game->player.rot = 0;
	return (0);
}

void	handle_input_setup(t_game *game)
{
	mlx_hook(game->win, DestroyNotify, NoEventMask, on_destroy, game);
	mlx_hook(game->win, KeyPress, KeyPressMask, on_key_press, game);
	mlx_hook(game->win, KeyRelease, KeyReleaseMask, on_key_release, game);
}
