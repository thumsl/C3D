#include "../include/engine.h"
#include <math.h>

int main(int argc, char* argv[]) {
	bool running = true, mouseGrab = true, drawBoundingBox = false;
	unsigned int timePassed = 0, frames = 0, pastTime, currentTime = 0, frameTime;
	int x = -1, y = -1, deltax = 0, deltay = 0, i;
	float angle = 0.0f, verticalAngle = 0.0f, horizontalAngle = PI, factor = 0;
	SDL_Event e;

	/* Create Window2 */
	SDL_Window* window = window_create(WIDTH, HEIGHT, "C3D Game Engine");
	if (window == NULL) {
		engine_quit();
		return 1;
	}

	/* Initialize OpenGL */
	if (!initOpenGL()) {
		engine_quit();
		return 1;
	}

	/* Attach and compile shaders */
    shader *S = shader_loadFromFile("src/glsl/shader.vert", "src/glsl/shader.frag");
    // After being loaded with shader_loadFromFile, the shader is also applied (shader_use())
	if (S == NULL) {
		engine_quit();
		return 1;
	}

	vec4 pastDirection;
	vec3 lightColor = {0.9f, 0.8f, 0.7f}, lightPosition = {0.0f, 35.0f, 0.0f}, center, nextPosition, pastPosition; 
	ambientLight *ambient = initAmbientLight(lightColor, 0.07f);
	setAmbientLight(ambient, S);

	lightColor[0] = 1.0f; lightColor[1] = 1.0f; lightColor[2] = 1.0f;
	pointLight *point = initPointLight(lightColor, lightPosition, 0.5f, 3.0f);
	setPointLight(point, S);

	/* Set the view matrix (camera) */
	camera* C = camera_init();

		/* Set the projection matrix */
	mat4x4 model_view_projection, projection;
	mat4x4_perspective(projection, FOV, (float)WIDTH/(float)HEIGHT, 0.001f, 1000.f);

	vec3_mul_cross(C->up, C->right, C->direction);
	vec3_add(center, C->eye, C->direction);
	mat4x4_look_at(C->view, C->eye, center, C->up);

	/* Initialize all meshes */
	linkedList* meshList = list_create();
	mesh_loadFromFileToList("res/obj/raptor.obj", "res/textures/raptor.png", meshList);
	mesh_loadFromFileToList("res/obj/R2-D2.obj", "res/textures/R2-D2.tga", meshList);
	mesh_loadFromFileToList("res/obj/jax.obj", "res/textures/jax.tga", meshList);

	mesh_translate(meshList->head->data, -2.0f, 0.0f, 0.0f);
	mesh_translate(meshList->head->next->data, 2.0f, 0.0f, 0.0f);

	/* Define the player */
	player* P = player_init(C->eye);

	//terrain *mainTerrain = terrain_genDiamondSquare(129, 10, 2, "res/textures/grass.jpg");
	// TODO: check for errors

	level* mainLevel = level_loadMeshes("res/maps/map.bmp");
	list_insert(mainLevel->meshList, mesh_genFlatFloor(mainLevel->size, "res/textures/grass.jpg")); // CHECK FOR ERRORS
	// TODO: WEAPON //

	/* Bullet List */
	linkedList* bulletList = list_create();
	bulletType* defaultBullet  = bullet_createType(0.05f, 1, 200, "res/obj/bullet.obj", "res/textures/steel.jpg");

	SDL_WarpMouseInWindow(window, WIDTH/2, HEIGHT/2);
	while (running) {
		pastTime = currentTime;
		currentTime = SDL_GetTicks();
		timePassed += (currentTime - pastTime);
		frameTime = (currentTime - pastTime);
		frames++;
		if (timePassed >= 1000) {
			timePassed = 0;
			printf("%d FPS\n", frames);
			frames = 0;
		}

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_WINDOWEVENT)
				if (e.window.event == SDL_WINDOWEVENT_CLOSE)
					running = false;
			if (e.type == SDL_KEYDOWN)
				switch(e.key.keysym.sym) {
					case SDLK_ESCAPE:
						mouseGrab = !mouseGrab;
						window_grabCursor(window, mouseGrab);
						break;
					case SDLK_q:
						running = false;
						break;
					case SDLK_a:
						P->movement.left = true;
						break;
					case SDLK_d:
						P->movement.right = true;
						break;
					case SDLK_w:
						P->movement.forward = true;
						break;
					case SDLK_s:
						P->movement.backward = true;
						break;
					case SDLK_h:
						drawBoundingBox = !drawBoundingBox;
						break;
					case SDLK_j:
						ambient->intensity -= 0.05;
						setAmbientLight(ambient, S);
						break;
					case SDLK_k:
						ambient->intensity += 0.05;
						setAmbientLight(ambient, S);
						break;
					case SDLK_LSHIFT:
						DEBUG_PRINT(("Running\n"));
						P->movement.run = 1;
						break;

				}
			if (e.type == SDL_KEYUP)
				switch(e.key.keysym.sym) {
					case SDLK_a:
						P->movement.left = 0;
						break;
					case SDLK_d:
						P->movement.right = 0;
						break;
					case SDLK_w:
						P->movement.forward = 0;
						break;
					case SDLK_s:
						P->movement.backward = 0;
						break;
					case SDLK_LSHIFT:
						P->movement.run = 0;
						break;
				}
			if (e.type == SDL_MOUSEBUTTONDOWN)
				if (e.button.button == SDL_BUTTON_LEFT) {
					list_insert(bulletList, bullet_create(C->eye, C->direction, defaultBullet));
				}
		}

	    SDL_GetMouseState(&x, &y);
		
		/* Movement */
		vec3_copy(pastPosition, C->eye);
		vec3_copy(nextPosition, C->eye);
		camera_fps_movement_simulate(nextPosition, C, P->movement, frameTime);
		player_updateHitbox(P, nextPosition); // this way the player has the be invisible
		if (aabb_collision(P->hitbox, ((mesh*)meshList->head->next->data)->hitbox)) {
			DEBUG_PRINT(("Collision!\n"));
			vec3_copy(C->eye, pastPosition);
			player_updateHitbox(P, C->eye);
		}
		else {
			camera_fps_movement(C, P->movement, frameTime);
			player_updateHitbox(P, C->eye);
		}

		/* FPS camera control */
	    deltax = x - WIDTH/2;
	    deltay = y - HEIGHT/2;

	    if ((deltax != 0 || deltay != 0) && mouseGrab) {
			horizontalAngle += (float)(WIDTH/2 - x) * SENSITIVITY;
			verticalAngle += (float)(HEIGHT/2 - y) * SENSITIVITY;

			if (verticalAngle > PI/2.0f)
				verticalAngle = PI/2.0f;
			else if (verticalAngle < -PI/2.0f)
				verticalAngle = -PI/2.0f;

			camera_fps_mouse_look(C, horizontalAngle, verticalAngle);
			SDL_WarpMouseInWindow(window, WIDTH/2, HEIGHT/2);
	    }

		vec3_add(center, C->eye, C->direction);
		mat4x4_look_at(C->view, C->eye, center, C->up);		
		
		// Update point light
		point->position[0] = C->eye[0];
		point->position[1] = C->eye[1];
		point->position[2] = C->eye[2];
		setPointLight(point, S);
	    factor += 0.0005 * frameTime;

		/* Rendering */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    // Terrain pieces
		node* aux = mainLevel->meshList->head;
		while (aux != NULL) {
			mesh_draw((mesh*)aux->data, C, projection, S, drawBoundingBox);
			aux = aux->next;
		}
		/*while (aux != NULL) {
			mesh_draw((mesh*)aux->data, C, projection, S, drawBoundingBox);
			aux = aux->next;
		}*/

	    // MeshList
		aux = meshList->head;
		while (aux != NULL) {
			mesh_rotate_from_ident(aux->data, 0, factor, 0);
			mesh_draw((mesh*)aux->data, C, projection, S, drawBoundingBox);
			aux = aux->next;
		}

		// Bullets
		aux = bulletList->head;
		while (aux != NULL) {
			mesh* currentBullet = ((bullet*) aux->data) -> model;
			mesh_draw(currentBullet, C, projection, S, drawBoundingBox);
			if (!bullet_updatePosition((bullet*)aux->data, frameTime))
				aux = list_delete_node(bulletList, aux);
			else
				aux = aux->next;
		}

		// Update framebuffer
		SDL_GL_SwapWindow(window);
		SDL_Delay(1);
	}
	
	// TODO: CLEAN UP;
	engine_quit();

	return 0;
}
