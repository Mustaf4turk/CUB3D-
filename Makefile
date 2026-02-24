NAME = cub3D

CC = cc
CFLAGS = -Wall -Wextra -Werror -O2

UNAME_S := $(shell uname -s)

MINILIBX_DIR = lib/mlx_linux
MINILIBX = $(MINILIBX_DIR)/libmlx.a

INCLUDES = -Iinclude -I$(MINILIBX_DIR)

ifeq ($(UNAME_S), Linux)
MLX_FLAGS = -L$(MINILIBX_DIR) -lmlx -lXext -lX11 -lm -lbsd
else
$(error This starter is configured for Linux only)
endif

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
		echo "MiniLibX not found at $(MINILIBX_DIR)"; \
		echo "Run: git clone https://github.com/42Paris/minilibx-linux.git lib/mlx_linux"; \
		exit 1; \
	fi
	$(MAKE) -sC $(MINILIBX_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	@if [ -d "$(MINILIBX_DIR)" ]; then $(MAKE) -sC $(MINILIBX_DIR) clean; fi

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
