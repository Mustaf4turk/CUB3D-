#ifndef TYPES_H
# define TYPES_H

typedef struct s_img
{
	void	*ptr;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}				t_img;

typedef struct s_tex
{
	void	*img;
	int		*addr;
	int		width;
	int		height;
	int		bpp;
	int		line_len;
	int		endian;
}				t_tex;

typedef struct s_ray
{
	double	dir_x;
	double	dir_y;
	int		step_x;
	int		step_y;
	int		side;
	double	perp_dist;
	double	wall_x;
	int		line_h;
	int		draw_start;
	int		draw_end;
	int		tex_id;
}				t_ray;

typedef struct s_dda
{
	int		map_x;
	int		map_y;
	double	delta_x;
	double	delta_y;
	double	side_x;
	double	side_y;
	int		hit;
}				t_dda;

typedef struct s_player
{
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	int		move_x;
	int		move_y;
	int		rot;
}				t_player;

typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
	char	*tex_no;
	char	*tex_so;
	char	*tex_we;
	char	*tex_ea;
	int		floor_rgb[3];
	int		ceiling_rgb[3];
	double	spawn_x;
	double	spawn_y;
	char	spawn_dir;
}				t_map;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_img		img;
	t_map		map;
	t_player	player;
	t_tex		tex[4];
	int			floor_color;
	int			ceiling_color;
}				t_game;

#endif
