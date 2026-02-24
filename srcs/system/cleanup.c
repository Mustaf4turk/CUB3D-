#include "cub3d.h"

static void	free_map(char **grid)
{
	int	i;

	if (!grid)
		return ;
	i = 0;
	while (grid[i])
	{
		free(grid[i]);
		i++;
	}
	free(grid);
}

void	cleanup_game(t_game *game)
{
	if (game->img.ptr)
		mlx_destroy_image(game->mlx, game->img.ptr);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
# ifdef __linux__
		mlx_destroy_display(game->mlx);
# endif
		free(game->mlx);
	}
	free_map(game->map.grid);
	free(game->map.tex_no);
	free(game->map.tex_so);
	free(game->map.tex_we);
	free(game->map.tex_ea);
}
