/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muturk <muturk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 19:07:20 by muturk            #+#    #+#             */
/*   Updated: 2026/03/09 19:07:21 by muturk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	set_dir_ns(t_player *p, char dir)
{
	p->dir_x = 0.0;
	if (dir == 'N')
	{
		p->dir_y = -1.0;
		p->plane_x = 0.66;
	}
	else
	{
		p->dir_y = 1.0;
		p->plane_x = -0.66;
	}
	p->plane_y = 0.0;
}

static void	set_dir_ew(t_player *p, char dir)
{
	p->dir_y = 0.0;
	if (dir == 'E')
	{
		p->dir_x = 1.0;
		p->plane_y = 0.66;
	}
	else
	{
		p->dir_x = -1.0;
		p->plane_y = -0.66;
	}
	p->plane_x = 0.0;
}

void	init_player(t_game *game)
{
	game->player.x = game->map.spawn_x;
	game->player.y = game->map.spawn_y;
	if (game->map.spawn_dir == 'N' || game->map.spawn_dir == 'S')
		set_dir_ns(&game->player, game->map.spawn_dir);
	else
		set_dir_ew(&game->player, game->map.spawn_dir);
	game->player.move_x = 0;
	game->player.move_y = 0;
	game->player.rot = 0;
}
