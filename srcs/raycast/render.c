#include "cub3d.h"

static void	put_pixel(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || y < 0 || x >= WIN_W || y >= WIN_H)
		return ;
	dst = game->img.addr + (y * game->img.line_len + x * (game->img.bpp / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

static void	draw_column(t_game *g, int x, t_ray *ray)
{
	int		tex_x;
	int		tex_y;
	double	step;
	double	tex_pos;
	t_tex	*tex;
	int		y;

	tex = &g->tex[ray->tex_id];
	tex_x = (int)(ray->wall_x * tex->width);
	if (tex_x >= tex->width)
		tex_x = tex->width - 1;
	if (ray->side == 0 && ray->dir_x > 0)
		tex_x = tex->width - tex_x - 1;
	if (ray->side == 1 && ray->dir_y < 0)
		tex_x = tex->width - tex_x - 1;
	step = (double)tex->height / (double)ray->line_h;
	tex_pos = (ray->draw_start - WIN_H / 2.0 + ray->line_h / 2.0) * step;
	y = 0;
	while (y < ray->draw_start)
		put_pixel(g, x, y++, g->ceiling_color);
	while (y <= ray->draw_end)
	{
		tex_y = (int)tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= tex->height)
			tex_y = tex->height - 1;
		put_pixel(g, x, y, tex->addr[tex_y * (tex->line_len / 4) + tex_x]);
		tex_pos += step;
		y++;
	}
	while (y < WIN_H)
		put_pixel(g, x, y++, g->floor_color);
}

static void	calc_tex_info(t_ray *ray, t_game *g)
{
	if (ray->side == 0)
	{
		ray->wall_x = g->player.y + ray->perp_dist * ray->dir_y;
		if (ray->step_x < 0)
			ray->tex_id = TEX_EA;
		else
			ray->tex_id = TEX_WE;
	}
	else
	{
		ray->wall_x = g->player.x + ray->perp_dist * ray->dir_x;
		if (ray->step_y < 0)
			ray->tex_id = TEX_NO;
		else
			ray->tex_id = TEX_SO;
	}
	ray->wall_x -= floor(ray->wall_x);
}

static void	trace_ray(t_game *g, int x)
{
	t_ray	ray;
	double	camera_x;
	double	delta_x;
	double	delta_y;
	double	side_x;
	double	side_y;
	int		map_x;
	int		map_y;
	int		hit;

	camera_x = 2.0 * x / (double)WIN_W - 1.0;
	ray.dir_x = g->player.dir_x + g->player.plane_x * camera_x;
	ray.dir_y = g->player.dir_y + g->player.plane_y * camera_x;
	map_x = (int)g->player.x;
	map_y = (int)g->player.y;
	if (ray.dir_x == 0)
		delta_x = 1e30;
	else
		delta_x = fabs(1.0 / ray.dir_x);
	if (ray.dir_y == 0)
		delta_y = 1e30;
	else
		delta_y = fabs(1.0 / ray.dir_y);
	if (ray.dir_x < 0)
	{
		ray.step_x = -1;
		side_x = (g->player.x - map_x) * delta_x;
	}
	else
	{
		ray.step_x = 1;
		side_x = (map_x + 1.0 - g->player.x) * delta_x;
	}
	if (ray.dir_y < 0)
	{
		ray.step_y = -1;
		side_y = (g->player.y - map_y) * delta_y;
	}
	else
	{
		ray.step_y = 1;
		side_y = (map_y + 1.0 - g->player.y) * delta_y;
	}
	hit = 0;
	while (!hit)
	{
		if (side_x < side_y)
		{
			side_x += delta_x;
			map_x += ray.step_x;
			ray.side = 0;
		}
		else
		{
			side_y += delta_y;
			map_y += ray.step_y;
			ray.side = 1;
		}
		if (map_y < 0 || map_y >= g->map.height
			|| map_x < 0 || map_x >= g->map.width)
			hit = 1;
		else if (g->map.grid[map_y][map_x] == '1')
			hit = 1;
	}
	if (ray.side == 0)
		ray.perp_dist = side_x - delta_x;
	else
		ray.perp_dist = side_y - delta_y;
	if (ray.perp_dist <= 0.0)
		ray.perp_dist = 0.1;
	ray.line_h = (int)(WIN_H / ray.perp_dist);
	ray.draw_start = -ray.line_h / 2 + WIN_H / 2;
	if (ray.draw_start < 0)
		ray.draw_start = 0;
	ray.draw_end = ray.line_h / 2 + WIN_H / 2;
	if (ray.draw_end >= WIN_H)
		ray.draw_end = WIN_H - 1;
	calc_tex_info(&ray, g);
	draw_column(g, x, &ray);
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
