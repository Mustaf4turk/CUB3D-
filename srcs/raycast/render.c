#include "cub3d.h"

static void	put_pixel(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || y < 0 || x >= WIN_W || y >= WIN_H)
		return ;
	dst = game->img.addr + (y * game->img.line_len + x * (game->img.bpp / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

static int	get_tex_x(t_tex *tex, t_ray *ray)
{
	int	tex_x;

	tex_x = (int)(ray->wall_x * tex->width);
	if (tex_x >= tex->width)
		tex_x = tex->width - 1;
	if (ray->side == 0 && ray->dir_x > 0)
		tex_x = tex->width - tex_x - 1;
	if (ray->side == 1 && ray->dir_y < 0)
		tex_x = tex->width - tex_x - 1;
	return (tex_x);
}

static void	draw_wall_strip(t_game *g, int x, t_ray *ray)
{
	t_tex	*tex;
	double	step;
	double	tex_pos;
	int		y;
	int		tex_x;

	tex = &g->tex[ray->tex_id];
	tex_x = get_tex_x(tex, ray);
	step = (double)tex->height / (double)ray->line_h;
	tex_pos = (ray->draw_start - WIN_H / 2.0 + ray->line_h / 2.0) * step;
	y = ray->draw_start;
	while (y <= ray->draw_end)
	{
		put_pixel(g, x, y, 0xFF000000
			| tex->addr[(int)fmin(tex_pos, (double)(tex->height - 1))
			* (tex->line_len / 4) + tex_x]);
		tex_pos += step;
		y++;
	}
}

void	draw_column(t_game *g, int x, t_ray *ray)
{
	int	y;

	y = 0;
	while (y < ray->draw_start)
		put_pixel(g, x, y++, g->ceiling_color);
	draw_wall_strip(g, x, ray);
	y = ray->draw_end + 1;
	while (y < WIN_H)
		put_pixel(g, x, y++, g->floor_color);
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
