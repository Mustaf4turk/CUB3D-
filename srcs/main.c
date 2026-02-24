#include "cub3d.h"

static int	loop_hook(void *param)
{
	return (update_frame((t_game *)param));
}

int	main(int ac, char **av)
{
	t_game	game;

	if (ac != 2)
		return (error_exit("Usage: ./cub3D <path/to/map.cub>"));
	if (init_game(&game, av[1]) != 0)
		return (1);
	handle_input_setup(&game);
	render_frame(&game);
	mlx_loop_hook(game.mlx, loop_hook, &game);
	mlx_loop(game.mlx);
	cleanup_game(&game);
	return (0);
}
