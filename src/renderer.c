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

/* Constants */
#define WINDOW_W 800
#define WINDOW_H 600
#define TARGET_FPS 60
#define FOV_FACTOR 500
static const double frame_target_time = 1.0 / (double)TARGET_FPS;
static double rot_angle = 0.0;
static Vec3f camera_position = {0, 0, 0};

// screen space translations
static const int screen_y = WINDOW_H / 2;
static const int screen_x = WINDOW_W / 2;

/* Globals */
static bool running = false;
static Fbuffer * fb = NULL;
uint64_t prev_time;

// just for testing
Model * cube = NULL;
Proj_triangle * cube_triangles = NULL;

Vec2f project(const Vec3f p){
	Vec2f ans = {.x = (p.x * FOV_FACTOR) / p.z, .y = (p.y * FOV_FACTOR) / p.z };
	return ans;
};

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

	Vec3f transformed_vertices[3];
	// 3 projected screen vertices coming out of this process for each face
	for(int i = 0; i < darray_length(cube->faces); i++){

		const Face * face = &cube->faces[i];
		Proj_triangle face_triangle;
		memset(transformed_vertices, 0, sizeof(Vec3f) * 3);
		// transforme and project each face vertex.
		for(int j = 0; j < 3; j++){
			// apply transformations
			Vec3f vertex = cube->vertices[face->indices[j] - 1];
			transformed_vertices[j] = Vec3f_rotate_x(vertex, rot_angle);
			transformed_vertices[j] = Vec3f_rotate_z(transformed_vertices[j], rot_angle);
			transformed_vertices[j].z += 5.0;
		}

		// backface culling test
		Vec3f vec_a = transformed_vertices[0];
		Vec3f vec_b = transformed_vertices[1];
		Vec3f vec_c = transformed_vertices[2];

		Vec3f vec_ab = Vec3f_sub(vec_b, vec_a);
		Vec3f vec_ac = Vec3f_sub(vec_c, vec_a);

		Vec3f normal = Vec3f_normalize(Vec3f_cross(vec_ab, vec_ac));
		Vec3f camera_dir = Vec3f_normalize(Vec3f_sub(camera_position, vec_a));

		if(Vec3f_dot(normal, camera_dir) < 0){
			continue;
		};

		for(int j = 0; j < 3; j++){
			Vec2f proj_point = project(transformed_vertices[j]);

			proj_point.x += screen_x;
			proj_point.y += screen_y;

			face_triangle.projected_points[j] = proj_point;
		}

		darray_push(cube_triangles, face_triangle);
	}
};

static void R_render(){
	Fbuffer_clear(fb, 0);

	int n = darray_length(cube_triangles);
	for(int i = 0; i < n; i++){
		projected_triangle_draw_filled(fb, &cube_triangles[i]);
		projected_triangle_draw_wireframe(fb, &cube_triangles[i]);
	}

	darray_free(cube_triangles);
	D_present_pixels(fb->pixels);
};

void R_cap_fps(){
    uint64_t now = SDL_GetPerformanceCounter();
	uint64_t clock_frequency = SDL_GetPerformanceFrequency();

	double elapsed_secs = (now - prev_time) / (double)(clock_frequency);

	if(elapsed_secs < frame_target_time){
		SDL_Delay((uint32_t)((frame_target_time - elapsed_secs) * 1000));
	}

	prev_time = now;
};

void R_run(int window_w, int window_h){
	D_Init(window_w, window_h);
	fb = Fbuffer_create(window_w, window_h);
	running = true;
	prev_time = SDL_GetPerformanceCounter();
	cube = Obj_load("assets/cube.obj");

	while(running){
		R_input();
		R_cap_fps();
		R_update();
		R_render();
	};

	Model_destroy(cube);
	D_Quit();
	Fbuffer_destroy(fb);
};
