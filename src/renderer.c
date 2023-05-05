#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "renderer.h"
#include "display.h"
#include "fbuffer.h"
#include "primitives2d.h"
#include "vec.h"
#include "darray.h"
#include "obj.h"

// for testing
Triangle * cube_triangles = NULL;

/* Constants */
#define WINDOW_W 800
#define WINDOW_H 600
#define TARGET_FPS 60
#define FOV_FACTOR 500
static const double frame_target_time = 1.0 / (double)TARGET_FPS;
static double rot_angle = 0.0;

// screen space translations
static const int screen_y = WINDOW_H / 2;
static const int screen_x = WINDOW_W / 2;

/* Globals */
static bool running = false;
static Fbuffer * fb = NULL;
uint64_t prev_time;
const Model * cube = NULL;

Vec2f project(const Vec3f p){
	Vec2f ans = {.x = (p.x * FOV_FACTOR) / p.z, .y = (p.y * FOV_FACTOR) / p.z };
	return ans;
}

static void R_input(){
	SDL_Event e;
	while(SDL_PollEvent(&e)){
		if(e.type == SDL_QUIT){
			running = false;
		}
	};
};

static void R_update(){
	rot_angle += 0.01;

	cube_triangles = NULL;

	//memset(cube_triangles, 0, sizeof(Triangle) * CUBE_NUM_FACES);
	// 3 projected screen vertices coming out of this process for each face
	for(int i = 0; i < darray_length(cube->faces); i++){

		const Face * face = &cube->faces[i];
		Triangle face_triangle;
		//Triangle * face_triangle = &cube_triangles[i];

		// transforme and project each face vertex.
		for(int j = 0; j < 3; j++){
			// apply transformations
			Vec3f vertex = cube->vertices[face->indices[j] - 1];

			Vec3f transformed_vertex = Vec3f_rotate_x(vertex, rot_angle);
			//transformed_vertex = Vec3f_rotate_z(transformed_vertex, rot_angle);
			transformed_vertex.z -= -5.0;

			//face_triangle->projected_points[j] = project(transformed_vertex);
			face_triangle.projected_points[j] = project(transformed_vertex);
		}

		darray_push(cube_triangles, face_triangle);
	}
};

static void R_render(){
	Fbuffer_clear(fb, 0);

	int n = darray_length(cube_triangles);
	for(int i = 0; i < n; i++){
		for(int j = 0; j < 3; j++){
			const Vec2f * v1 = &cube_triangles[i].projected_points[j];
			const Vec2f * v2 = &cube_triangles[i].projected_points[(j + 1) % 3];

			line(fb, v1->x + screen_x,
					 v1->y + screen_y,
					 v2->x + screen_x,
					 v2->y + screen_y,
					 0xff00ffff);
		}
	}

	darray_free(cube_triangles);
	D_present_pixels(fb->pixels);
};

void R_cap_fps(){
    uint64_t now = SDL_GetPerformanceCounter();
	uint64_t clock_frequency = SDL_GetPerformanceFrequency();

	double elapsed_secs = (now - prev_time) / (double)(clock_frequency);

	if(elapsed_secs < frame_target_time){
	//	printf("%f\n", elapsed_secs);
		SDL_Delay((uint32_t)((frame_target_time - elapsed_secs) * 1000));
	}

	prev_time = now;
};

void R_run(int window_w, int window_h){
	D_Init(window_w, window_h);
	fb = Fbuffer_create(window_w, window_h);
	running = true;
	prev_time = SDL_GetPerformanceCounter();
	cube = Obj_load("assets/f22.obj");

	while(running){
		R_input();
		R_cap_fps();
		R_update();
		R_render();
	};

	darray_free(cube->vertices);
	darray_free(cube->faces);
	D_Quit();
	Fbuffer_destroy(fb);
};
