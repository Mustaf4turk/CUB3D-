#include "cub3d.h"

static void	init_ray(t_game *g, int x, t_ray *ray, t_dda *dda)
{
	double	camera_x;

	camera_x = 2.0 * x / (double)WIN_W - 1.0;
	ray->dir_x = g->player.dir_x + g->player.plane_x * camera_x;
	ray->dir_y = g->player.dir_y + g->player.plane_y * camera_x;
	dda->map_x = (int)g->player.x;
	dda->map_y = (int)g->player.y;
	dda->delta_x = (ray->dir_x == 0) ? 1e30 : fabs(1.0 / ray->dir_x);
	dda->delta_y = (ray->dir_y == 0) ? 1e30 : fabs(1.0 / ray->dir_y);
	ray->step_x = (ray->dir_x < 0) ? -1 : 1;
	if (ray->dir_x < 0)
		dda->side_x = (g->player.x - dda->map_x) * dda->delta_x;
	else
		dda->side_x = (dda->map_x + 1.0 - g->player.x) * dda->delta_x;
	ray->step_y = (ray->dir_y < 0) ? -1 : 1;
	if (ray->dir_y < 0)
		dda->side_y = (g->player.y - dda->map_y) * dda->delta_y;
	else
		dda->side_y = (dda->map_y + 1.0 - g->player.y) * dda->delta_y;
}

static void	run_dda(t_game *g, t_ray *ray, t_dda *dda)
{
	dda->hit = 0;
	while (!dda->hit)
	{
		if (dda->side_x < dda->side_y)
		{
			dda->side_x += dda->delta_x;
			dda->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			dda->side_y += dda->delta_y;
			dda->map_y += ray->step_y;
			ray->side = 1;
		}
		if (dda->map_y < 0 || dda->map_y >= g->map.height
			|| dda->map_x < 0 || dda->map_x >= g->map.width)
			dda->hit = 1;
		else if (g->map.grid[dda->map_y][dda->map_x] == '1')
			dda->hit = 1;
	}
}

static void	calc_ray_dist(t_ray *ray, t_dda *dda)
{
	if (ray->side == 0)
		ray->perp_dist = dda->side_x - dda->delta_x;
	else
		ray->perp_dist = dda->side_y - dda->delta_y;
	if (ray->perp_dist <= 0.0)
		ray->perp_dist = 0.1;
	ray->line_h = (int)(WIN_H / ray->perp_dist);
	ray->draw_start = -ray->line_h / 2 + WIN_H / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_h / 2 + WIN_H / 2;
	if (ray->draw_end >= WIN_H)
		ray->draw_end = WIN_H - 1;
}

static void	calc_wall_tex(t_game *g, t_ray *ray)
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

void	trace_ray(t_game *g, int x)
{
	t_ray	ray;
	t_dda	dda;

	init_ray(g, x, &ray, &dda);
	run_dda(g, &ray, &dda);
	calc_ray_dist(&ray, &dda);
	calc_wall_tex(g, &ray);
	draw_column(g, x, &ray);
}
