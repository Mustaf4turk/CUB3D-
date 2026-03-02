NAME = cub3D

CC = cc
CFLAGS = -Wall -Wextra -Werror -O2

MINILIBX_DIR = lib/mlx_linux
MINILIBX = $(MINILIBX_DIR)/libmlx.a
INCLUDES = -Iinclude -I$(MINILIBX_DIR)
MLX_FLAGS = -L$(MINILIBX_DIR) -lmlx -lXext -lX11 -lm
MLX_REPO = https://github.com/42Paris/minilibx-linux.git

SRCS = \
	srcs/main.c \
	srcs/system/init.c \
	srcs/system/cleanup.c \
	srcs/system/errors.c \
	srcs/map/parser.c \
	srcs/map/parser_utils.c \
	srcs/map/parser_file.c \
	srcs/map/parser_header.c \
	srcs/map/parser_elements.c \
	srcs/map/parser_map.c \
	srcs/map/parser_validate.c \
	srcs/player/input.c \
	srcs/player/move.c \
	srcs/raycast/render.c \
	srcs/raycast/texture.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(MINILIBX) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

$(MINILIBX):
	@if [ ! -f "$(MINILIBX_DIR)/Makefile" ]; then \
		echo "MiniLibX not found, cloning into $(MINILIBX_DIR)"; \
		rm -rf $(MINILIBX_DIR); \
		mkdir -p lib; \
		git clone $(MLX_REPO) $(MINILIBX_DIR); \
	fi
	$(MAKE) -C $(MINILIBX_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	@if [ -d "$(MINILIBX_DIR)" ]; then $(MAKE) -C $(MINILIBX_DIR) clean; fi

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
