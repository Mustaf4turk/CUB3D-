#ifndef FUNCTIONS_H
# define FUNCTIONS_H

# include "types.h"

int		init_game(t_game *game, const char *map_path);
void	cleanup_game(t_game *game);
int		error_exit(const char *msg);

int		load_map_from_cub(t_game *game, const char *path);

void	handle_input_setup(t_game *game);
int		update_frame(t_game *game);

void	render_frame(t_game *game);

#endif
