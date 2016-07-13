# C3D
C3D is a OpenGL 3D Game Engine, written in C, as a learning exercise.

---
* C
* SDL2
* SDL2_image
* OpenGL
* [linmath.h](https://github.com/datenwolf/linmath.h) for linear math.

### Features
***
* A (very bad) OBJ Loader
* AABB collision detection
* Can display AABB wireframe
* Point lights, ambient light
* Can load, compile and attach GLSL shaders
* Helper functions for setting up OpenGL and SDL2 windows
* FPS-style camera
* Bullets

### FAQ
***
Q. Do you know that your code is not very pretty?
A. Yes.

Q. How do I use it?
A. Why would you do that?

`git clone https://github.com/lcthums/C3D`
`cd C3D`
`make`
`./bin/game.o`

You will need OpenGL 3.3+, sdl2 and sdl2_image. There might be examples in the examples directory.
***
### TODO:
- [x] not abandon the project after the first week
- [ ] shut memory leaks
- [ ] weapon.h
- [ ] more lighting stuff
- [ ] decent OBJLoader
- [ ] UI
- [ ] terrain generator
- [ ] A.I.
- [ ] shadows
- [ ] finish a game with it
