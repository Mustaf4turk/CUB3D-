/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muturk <muturk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 19:08:17 by muturk            #+#    #+#             */
/*   Updated: 2026/03/09 19:08:18 by muturk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

static int	check_tex_file(const char *path)
{
	int		fd;
	size_t	len;

	if (!path)
		return (1);
	len = p_strlen(path);
	if (len < 4 || path[len - 4] != '.' || path[len - 3] != 'x'
		|| path[len - 2] != 'p' || path[len - 1] != 'm')
		return (1);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (1);
	close(fd);
	return (0);
}

int	p_check_tex_files(t_game *game)
{
	if (check_tex_file(game->map.tex_no) != 0)
		return (1);
	if (check_tex_file(game->map.tex_so) != 0)
		return (1);
	if (check_tex_file(game->map.tex_we) != 0)
		return (1);
	if (check_tex_file(game->map.tex_ea) != 0)
		return (1);
	return (0);
}
