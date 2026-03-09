*This project has been created as part of the 42 curriculum by muturk, talgan.*

# cub3D

## Description

cub3D is a 42 school project inspired by the classic game Wolfenstein 3D. It uses a raycasting engine to render a first-person 3D perspective from a 2D map. The program reads a `.cub` scene file containing wall textures, floor/ceiling colors, and a grid-based map, then displays an interactive 3D view using the MiniLibX graphics library.

The core goal is to explore raycasting techniques — casting a ray for each column of the screen, detecting wall intersections via the DDA (Digital Differential Analyzer) algorithm, and projecting textured walls with correct perspective. The player can move through the maze and look around in real time.

## Instructions

### Prerequisites

This project runs on Linux. Install the required packages:

```bash
sudo apt update
sudo apt install -y build-essential xorg libxext-dev libbsd-dev
```

### Building

MiniLibX is expected under `lib/mlx_linux`. If it is not present, `make` will clone it automatically.

```bash
make
```

Other make targets:

| Target | Effect |
|--------|--------|
| `make clean` | Remove object files |
| `make fclean` | Remove object files and the binary |
| `make re` | Full rebuild |

### Running

```bash
./cub3D maps/demo.cub
```

You can try a different map:

```bash
./cub3D maps/maze.cub
```

### Controls

| Key | Action |
|-----|--------|
| W | Move forward |
| S | Move backward |
| A | Strafe left |
| D | Strafe right |
| ← | Rotate camera left |
| → | Rotate camera right |
| ESC | Quit |

## Map Format (.cub)

A valid `.cub` file contains a header section followed by a map grid:

```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm
F 60,60,60
C 30,60,90

111111111111
100000000001
101111011101
1000N0010001
101011010101
100000000001
111111111111
```

- **NO / SO / WE / EA** — File paths to wall textures (XPM format), one per cardinal direction.
- **F** — Floor color as `R,G,B` (each 0–255).
- **C** — Ceiling color as `R,G,B` (each 0–255).
- **1** — Wall · **0** — Empty space · **N / S / E / W** — Player spawn position and facing direction.
- The map must be fully enclosed by walls.

## Project Structure

```
srcs/
├── main.c                 # Entry point, starts mlx loop
├── map/
│   ├── parser.c           # .cub file reading and validation
│   ├── parser_file.c      # File I/O, line splitting
│   ├── parser_header.c    # NO/SO/WE/EA/F/C parsing
│   ├── parser_elements.c  # Map line detection, preliminary checks
│   ├── parser_map.c       # Grid construction, spawn detection
│   ├── parser_validate.c  # Flood fill wall enclosure check
│   └── parser_utils.c     # Helper functions
├── player/
│   ├── input.c            # Key press/release events
│   └── move.c             # Movement and collision detection
├── raycast/
│   ├── render.c           # DDA raycasting and column drawing
│   └── texture.c          # XPM texture loading
└── system/
    ├── init.c             # Game struct and MLX initialization
    ├── cleanup.c          # Memory cleanup
    └── errors.c           # Error message output
```

## Features

- **DDA raycasting** — Efficient wall-hit detection with textured rendering.
- **Smooth movement** — WASD movement with arrow-key camera rotation.
- **Collision detection** — Bounding-box check prevents walking through walls.
- **Full scene parser** — Validates header fields, RGB values (including overflow), file extension, and map integrity.
- **Flood fill validation** — Ensures the map is properly enclosed by walls before the game starts.

## Resources

- [Lode's Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html) — The primary reference for understanding and implementing the DDA raycasting algorithm.
- [42 Docs – MiniLibX](https://harm-smits.github.io/42docs/libs/minilibx) — Documentation for the MiniLibX graphics library.
- [MiniLibX Linux repository](https://github.com/42Paris/minilibx-linux) — Source code for MiniLibX on Linux.
- [Wolfenstein 3D (Wikipedia)](https://en.wikipedia.org/wiki/Wolfenstein_3D) — Background on the game that inspired this project.
- [Permadi – Ray-Casting Tutorial](https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/) — Another classic raycasting walkthrough with detailed diagrams.

### AI Usage

AI tools (GitHub Copilot) were used during development for the following tasks:
- Generating boilerplate code and repetitive structure definitions.
- Debugging parser edge cases and memory leaks.
- Drafting and formatting this README.

All AI-generated output was reviewed, tested, and adapted manually. The core raycasting logic, DDA implementation, and map validation were developed and understood by the authors.