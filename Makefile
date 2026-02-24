NAME = cub3D

CC = cc
CFLAGS = -Wall -Wextra -Werror -O2

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
MINILIBX_DIR = lib/mlx_linux
MINILIBX     = $(MINILIBX_DIR)/libmlx.a
MLX_FLAGS    = -L$(MINILIBX_DIR) -lmlx -lXext -lX11 -lm -lbsd
MLX_REPO     = https://github.com/42Paris/minilibx-linux.git
else ifeq ($(UNAME_S), Darwin)
MINILIBX_DIR = lib/mlx_macos
MINILIBX     = $(MINILIBX_DIR)/libmlx.a
MLX_FLAGS    = -L$(MINILIBX_DIR) -lmlx -framework OpenGL -framework AppKit
MLX_REPO     = https://github.com/42Paris/minilibx-macos.git
else
$(error Unsupported OS: $(UNAME_S))
endif

INCLUDES = -Iinclude -I$(MINILIBX_DIR)

SRCS = \
	srcs/main.c \
	srcs/system/init.c \
	srcs/system/cleanup.c \
	srcs/system/errors.c \
	srcs/map/parser.c \
	srcs/player/input.c \
	srcs/player/move.c \
	srcs/raycast/render.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(MINILIBX) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

$(MINILIBX):
	@if [ ! -d "$(MINILIBX_DIR)" ]; then \
		echo "MiniLibX not found, cloning into $(MINILIBX_DIR)"; \
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
