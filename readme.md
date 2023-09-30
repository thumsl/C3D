# C3D
C3D is a OpenGL 3D Game Engine, written in C, as a learning exercise.

[YouTube videos showcasing features](https://www.youtube.com/watch?v=a591XlVhg2s&t=0s&index=2&list=PLjRugBWdnpzGRwpwNgbzrJWdNYixQwfZE)

---
* [linmath.h](https://github.com/datenwolf/linmath.h) is used for linear math

### Features
***
* helper functions for setting up OpenGL and SDL2 windows
* can load, compile and attach GLSL shaders
* rudimentary font and text display system
* Assimp for model loading
* bad AABB collision detection
* can display AABB wireframe
* point lights, ambient light
* FPS-style camera
* bullets

### FAQ
***
Q. Do you know that your code is not very pretty?

A. Yes.

Q. How do I use it?

A. Why would you do that?

clone this repository

`cd C3D`

`cp examples/basic.c ./main.c`

`make`

`./game.out`

You will need OpenGL 3.3+ capable drivers and hardware, SDL2, SDL2_image, Assimp and GLEW. Check the examples directory for samples.
***

### TODO:
- [x] not abandon the project after the first week
- [x] decent OBJLoader (Assimp!)
- [ ] UI
- [ ] shadows
- [ ] weapon.h
- [ ] terrain generator
- [ ] shut memory leaks
- [ ] more lighting stuff
- [ ] finish a game with it
