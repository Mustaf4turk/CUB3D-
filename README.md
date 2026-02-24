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
- Real `.cub` parser for `NO/SO/WE/EA/F/C` + map section
- Map validation: one player spawn, allowed chars, map closure check

## Next mandatory steps

1. Add texture file validation (`.xpm` exists and readable)
2. Add textured wall rendering using parsed `NO/SO/WE/EA` paths
3. Tighten error handling + full free on all failure paths
4. Add more valid/invalid map test cases
