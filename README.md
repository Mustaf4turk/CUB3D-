# cub3d

Minimal, Linux-focused starter for 42 `cub3d`.

This setup is intentionally simple:
- No Makefile colors or decorative output
- Clear module split similar to common cub3d projects
- Basic raycasting + movement loop to start fast

## Project layout

```
.
├── include/
│   ├── cub3d.h
│   ├── functions.h
│   ├── keycodes.h
│   └── types.h
├── maps/
│   └── demo.cub
├── srcs/
│   ├── main.c
│   ├── map/
│   │   └── parser.c
│   ├── player/
│   │   ├── input.c
│   │   └── move.c
│   ├── raycast/
│   │   └── render.c
│   └── system/
│       ├── cleanup.c
│       ├── errors.c
│       └── init.c
└── Makefile
```

## Linux requirements

Install dependencies:

```bash
sudo apt update
sudo apt install -y build-essential xorg libxext-dev libbsd-dev
```

Clone MiniLibX Linux inside `lib/` (optional, `make` also auto-clones if missing):

```bash
mkdir -p lib
git clone https://github.com/42Paris/minilibx-linux.git lib/mlx_linux
```

## Build and run

```bash
make
./cub3D maps/demo.cub
```

## Current state

- Window + image rendering loop
- Basic WASD movement and arrow rotation
- Basic DDA raycasting with flat wall colors
- Very small parser stub (checks map path is readable, then loads an internal demo map)

## Next mandatory steps

1. Replace parser stub with real `.cub` parsing (`NO/SO/WE/EA/F/C` + map)
2. Add map validation (closed map, one spawn, valid chars)
3. Add texture loading and textured wall rendering
4. Add strict error handling + full free on all failure paths
