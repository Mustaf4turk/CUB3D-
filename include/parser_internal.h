/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muturk <muturk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 19:10:05 by muturk            #+#    #+#             */
/*   Updated: 2026/03/09 19:10:28 by muturk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_INTERNAL_H
# define PARSER_INTERNAL_H

# include "cub3d.h"

typedef struct s_parse
{
	char	**lines;
	int		count;
	int		map_start;
	int		map_end;
}				t_parse;

typedef struct s_buf
{
	char	*data;
	size_t	cap;
	size_t	len;
}				t_buf;

typedef struct s_flood
{
	char	**grid;
	int		h;
	int		w;
}				t_flood;

size_t	p_strlen(const char *s);
int		p_is_space(char c);
int		p_is_digit(char c);
char	*p_dup_range(const char *s, int start, int end);
void	p_free_lines(char **lines);

char	*p_read_all_file(const char *path);
char	**p_split_lines(const char *all, int *line_count);
int		p_is_blank_line(const char *line);

int		p_parse_header_line(t_game *game, const char *line);

int		p_is_map_char(char c);
int		p_is_map_line(const char *line);
int		p_headers_complete(t_game *game);
int		p_preparse_lines(t_game *game, t_parse *p);
int		p_find_map_bounds(t_parse *p);

int		p_build_map_grid(t_game *game, t_parse *p);
int		p_validate_char_and_spawn(t_game *game);

int		p_validate_closed_map(t_game *game);
int		p_check_tex_files(t_game *game);

#endif
