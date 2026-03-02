# Fractal Tree in C

This project visualizes a dynamic fractal tree using C and the [raylib](https://www.raylib.com/) graphics library. The simulation features animated seasons, weather effects, birds, leaves, rain, snow, stars, and fireflies, all rendered in real time.

## Features
- **Fractal Tree Growth:** Watch the tree grow and branch out dynamically.
- **Seasonal Changes:** Spring, Summer, Autumn, and Winter, each with unique leaf colors and effects.
- **Weather Effects:** Switch between Day, Night, Rain, and Snow for different atmospheres.
- **Animated Elements:** Birds, falling leaves, rain, snowflakes, stars, and glowing fireflies.
- **Camera Controls:** Pan and zoom the view using WASD keys and mouse wheel.

## Controls
- `1` — Night mode
- `2` — Rain mode
- `3` — Snow mode
- `4` — Day mode
- `W/A/S/D` — Move camera
- Mouse Wheel — Zoom in/out
- Arrow Keys — Adjust wind strength

## Requirements
- [raylib](https://www.raylib.com/) library (included as a submodule or folder)
- C compiler (e.g., GCC, MSVC, Clang)

## How to Build
1. Make sure raylib is available in the `raylib/` directory.
2. Compile `tree.c` with your C compiler, linking against raylib. Example (GCC):
	```sh
	gcc tree.c -o tree.exe -Iraylib/src -Lraylib/src -lraylib -lopengl32 -lgdi32 -lwinmm
	```
	Or use the provided build tasks in VS Code.

## File Structure
- `tree.c` — Main source code for the fractal tree simulation
- `raylib/` — Raylib library and dependencies
- `.vscode/` — VS Code configuration files

## Vid

https://github.com/user-attachments/assets/77f1516d-32fa-4b6e-a28c-4278cea6a22e





---
Created by Isha Nayal :)

