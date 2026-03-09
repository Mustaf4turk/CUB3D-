/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muturk <muturk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 19:07:34 by muturk            #+#    #+#             */
/*   Updated: 2026/03/09 19:07:35 by muturk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	load_single_tex(t_game *game, t_tex *tex, char *path)
{
	tex->img = mlx_xpm_file_to_image(game->mlx, path,
			&tex->width, &tex->height);
	if (!tex->img)
		return (1);
	tex->addr = (int *)mlx_get_data_addr(tex->img,
			&tex->bpp, &tex->line_len, &tex->endian);
	if (!tex->addr)
		return (1);
	return (0);
}

void	calc_wall_tex(t_game *g, t_ray *ray)
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

int	load_textures(t_game *game)
{
	if (load_single_tex(game, &game->tex[TEX_NO], game->map.tex_no) != 0)
		return (error_exit("Failed to load north texture"));
	if (load_single_tex(game, &game->tex[TEX_SO], game->map.tex_so) != 0)
		return (error_exit("Failed to load south texture"));
	if (load_single_tex(game, &game->tex[TEX_WE], game->map.tex_we) != 0)
		return (error_exit("Failed to load west texture"));
	if (load_single_tex(game, &game->tex[TEX_EA], game->map.tex_ea) != 0)
		return (error_exit("Failed to load east texture"));
	return (0);
}
