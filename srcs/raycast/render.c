#include "cub3d.h"

static void	put_pixel(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || y < 0 || x >= WIN_W || y >= WIN_H)
		return ;
	dst = game->img.addr + (y * game->img.line_len + x * (game->img.bpp / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

static void	draw_column(t_game *game, int x, int start, int end)
{
	int	y;

	y = 0;
	while (y < start)
		put_pixel(game, x, y++, 0x1E2A3A);
	while (y <= end)
		put_pixel(game, x, y++, 0x8FA3BF);
	while (y < WIN_H)
		put_pixel(game, x, y++, 0x2B2B2B);
}

static void	trace_ray(t_game *g, int x)
{
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	double	delta_x;
	double	delta_y;
	double	side_x;
	double	side_y;
	int		map_x;
	int		map_y;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	double	perp_dist;
	int		line_h;
	int		draw_start;
	int		draw_end;

	camera_x = 2.0 * x / (double)WIN_W - 1.0;
	ray_dir_x = g->player.dir_x + g->player.plane_x * camera_x;
	ray_dir_y = g->player.dir_y + g->player.plane_y * camera_x;
	map_x = (int)g->player.x;
	map_y = (int)g->player.y;
	if (ray_dir_x == 0)
		delta_x = 1e30;
	else
		delta_x = fabs(1.0 / ray_dir_x);
	if (ray_dir_y == 0)
		delta_y = 1e30;
	else
		delta_y = fabs(1.0 / ray_dir_y);
	if (ray_dir_x < 0)
	{
		step_x = -1;
		side_x = (g->player.x - map_x) * delta_x;
	}
	else
	{
		step_x = 1;
		side_x = (map_x + 1.0 - g->player.x) * delta_x;
	}
	if (ray_dir_y < 0)
	{
		step_y = -1;
		side_y = (g->player.y - map_y) * delta_y;
	}
	else
	{
		step_y = 1;
		side_y = (map_y + 1.0 - g->player.y) * delta_y;
	}
	hit = 0;
	while (!hit)
	{
		if (side_x < side_y)
		{
			side_x += delta_x;
			map_x += step_x;
			side = 0;
		}
		else
		{
			side_y += delta_y;
			map_y += step_y;
			side = 1;
		}
		if (map_y < 0 || map_y >= g->map.height || map_x < 0 || map_x >= g->map.width)
			hit = 1;
		else if (g->map.grid[map_y][map_x] == '1')
			hit = 1;
	}
	if (side == 0)
		perp_dist = side_x - delta_x;
	else
		perp_dist = side_y - delta_y;
	if (perp_dist <= 0.0)
		perp_dist = 0.1;
	line_h = (int)(WIN_H / perp_dist);
	draw_start = -line_h / 2 + WIN_H / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = line_h / 2 + WIN_H / 2;
	if (draw_end >= WIN_H)
		draw_end = WIN_H - 1;
	draw_column(g, x, draw_start, draw_end);
}

void	render_frame(t_game *game)
{
	int	x;

	x = 0;
	while (x < WIN_W)
	{
		trace_ray(game, x);
		x++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->img.ptr, 0, 0);
}
