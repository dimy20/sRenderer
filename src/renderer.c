#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <assert.h>

#include "renderer.h"
#include "display.h"
#include "fbuffer.h"
#include "primitives2d.h"
#include "vec.h"
#include "darray.h"
#include "obj.h"
#include "mat.h"

/* Constants */
#define WINDOW_W 800
#define WINDOW_H 600
#define TARGET_FPS 60
#define FOV_FACTOR 500
static const double frame_target_time = 1.0 / (double)TARGET_FPS;
static Vec3 camera_position = {0, 0, 0};

// screen space translations
static const int screen_y = WINDOW_H / 2;
static const int screen_x = WINDOW_W / 2;

/* Globals */
static bool running = false;
static Fbuffer * fb = NULL;
uint64_t prev_time;
uint32_t renderer_flags;

// just for testing
Model * cube = NULL;
Proj_triangle * cube_triangles = NULL;

// transformations
Mat4 scale_mat, translate_mat, rotate_y_mat, rotate_x_mat, rotate_z_mat;

Vec2 project(const Vec3 p){
	Vec2 ans = {.x = (p.x * FOV_FACTOR) / p.z, .y = (p.y * FOV_FACTOR) / p.z };
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

// applies transformations to a face vertices, and returns an array of 3 transformed vertices
static inline void R_apply_transformations(const Face * face, Vec3 * transformed_vertices){
	memset(transformed_vertices, 0, sizeof(Vec3) * 3);

	Vec4 augmented_vertices[3];
	for(int j = 0; j < 3; j++){
		augmented_vertices[j] = Vec4_from_vec3(cube->vertices[face->indices[j] - 1]);

		Mat4 world_matrix = Mat4_id();
		//world_matrix = Mat4_mult_mat4(scale_mat, world_matrix);
		world_matrix = Mat4_mult_mat4(rotate_y_mat, world_matrix);
		world_matrix = Mat4_mult_mat4(rotate_x_mat, world_matrix);
		world_matrix = Mat4_mult_mat4(rotate_z_mat, world_matrix);
		world_matrix = Mat4_mult_mat4(translate_mat, world_matrix);

		augmented_vertices[j] = Mat4_mult_vec4(world_matrix, augmented_vertices[j]);
		transformed_vertices[j] = Vec3_from_vec4(augmented_vertices[j]);
	};
}
// performs backface culling on trnasformed face vertices
static inline bool R_backface_cull_test(const Vec3 * transformed_vertices){
	 //backface culling test
	Vec3 vec_a = transformed_vertices[0];
	Vec3 vec_b = transformed_vertices[1];
	Vec3 vec_c = transformed_vertices[2];

	Vec3 vec_ab = Vec3_sub(vec_b, vec_a);
	Vec3 vec_ac = Vec3_sub(vec_c, vec_a);

	Vec3 normal = Vec3_normalize(Vec3_cross(vec_ab, vec_ac));
	Vec3 camera_dir = Vec3_normalize(Vec3_sub(camera_position, vec_a));

	return Vec3_dot(normal, camera_dir) > 0;
};

static inline void R_apply_projection(const Vec3 * transformed_vertices,
		Proj_triangle * projected_triangle){

	for(int j = 0; j < 3; j++){
		Vec2 proj_point = project(transformed_vertices[j]);
		proj_point.x += screen_x;
		proj_point.y += screen_y;
		projected_triangle->projected_points[j] = proj_point;
	}
};

static void R_update(){
	cube->rotation.x += 0.01;
	cube->rotation.y += 0.01;
	cube->rotation.z += 0.01;
	cube->scale.x += 0.002;
	cube->scale.y += 0.001;

	cube->translation.x += 0.001;
	cube->translation.z = 5.0;

	scale_mat = Mat4_scale(cube->scale.x, cube->scale.y, cube->scale.z);
	translate_mat  = Mat4_translate(cube->translation.x, cube->translation.y, cube->translation.z);

	rotate_y_mat = Mat4_make_rotate_y(cube->rotation.y);
	rotate_x_mat = Mat4_make_rotate_x(cube->rotation.x);
	rotate_z_mat = Mat4_make_rotate_z(cube->rotation.z);

	cube_triangles = NULL;
	assert(darray_length(cube->faces) != 0);

	// 3 projected screen vertices coming out of this process for each face
	for(int i = 0; i < darray_length(cube->faces); i++){
		//printf("Updating\n");
		// init projected_triangle
		Proj_triangle projected_triangle;
		projected_triangle.color = cube->faces[i].color;
		projected_triangle.avg_z = 0;

		Vec3 transformed_vertices[3];
		R_apply_transformations(&cube->faces[i], transformed_vertices);

		// compute avg depth
		for(int j = 0; j < 3; j++){
			projected_triangle.avg_z += transformed_vertices[j].z;
		}
		projected_triangle.avg_z /= 3;

		if(!R_backface_cull_test(transformed_vertices)){
			continue;
		}
		// project face into a screen triangle
		R_apply_projection(transformed_vertices, &projected_triangle);
		darray_push(cube_triangles, projected_triangle);
	}

	// painter's
	qsort(cube_triangles, darray_length(cube_triangles), sizeof(Proj_triangle), compare_proj_triangle);
};

static void R_render(){
	Fbuffer_clear(fb, 0);

	int n = darray_length(cube_triangles);
	for(int i = 0; i < n; i++){
		//projected_triangle_draw_filled(fb, &cube_triangles[i]);
		projected_triangle_draw_wireframe(fb, &cube_triangles[i]);
		for(int j = 0; j < 3; j++){
			Vec2 * p = &cube_triangles[i].projected_points[j];
			draw_rect(fb, p->x, p->y, 3, 3, 0xfffffff);
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
