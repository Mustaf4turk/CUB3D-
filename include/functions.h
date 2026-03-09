#ifndef FUNCTIONS_H
# define FUNCTIONS_H

# include "types.h"

int		init_game(t_game *game, const char *map_path);
void	init_player(t_game *game);
void	cleanup_game(t_game *game);
int		error_exit(const char *msg);

int		load_map_from_cub(t_game *game, const char *path);

void	handle_input_setup(t_game *game);
int		update_frame(t_game *game);

void	render_frame(t_game *game);
void	draw_column(t_game *g, int x, t_ray *ray);
void	calc_wall_tex(t_game *g, t_ray *ray);
void	trace_ray(t_game *g, int x);

int		load_textures(t_game *game);

#endif
