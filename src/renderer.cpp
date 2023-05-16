#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <assert.h>
#include <algorithm>

#include "error.h"
#include "renderer.h"
#include "display.h"
#include "fbuffer.h"
#include "primitives2d.h"
#include "vec.h"
#include "obj.h"
#include "mat.h"
#include "light.h"
#include "texture.h"


SDL_Surface * wall_texture;
std::unique_ptr<Model> load_cube_test();
/* Constants */
#define M_PI 3.14159265358979323846
#define TO_RAD(d) ((d) * M_PI) / 180.0
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
std::unique_ptr<Model> cube = nullptr;
std::vector<Proj_triangle> cube_triangles;

// transformations
Mat4 scale_mat, translate_mat, rotate_y_mat, rotate_x_mat, rotate_z_mat;
Mat4 perspective_proj_mat;

Vec2f weak_project(const Vec3 p){
	return Vec2f((p.x * FOV_FACTOR) / p.z, (p.y * FOV_FACTOR) / p.z);
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
static inline void R_apply_transformations(const Face * face, Vec3 * transformed_vertices, const Mat4& world_matrix){
	std::fill(transformed_vertices, transformed_vertices + 3, Vec3());
	Vec4 augmented_vertices[3];
	for(int j = 0; j < 3; j++){
		augmented_vertices[j] = vec4_from_vec3(cube->vertices[face->indices[j] - 1]);
		augmented_vertices[j] = world_matrix * augmented_vertices[j];
		transformed_vertices[j] = vec3_from_vec4(augmented_vertices[j]);
	};
}
// performs backface culling on trnasformed face vertices
static inline bool R_backface_cull_test(const Vec3 * transformed_vertices, Vec3 * normal){
	 //backface culling test
	Vec3 vec_a = transformed_vertices[0];
	Vec3 vec_b = transformed_vertices[1];
	Vec3 vec_c = transformed_vertices[2];

	Vec3 vec_ab = vec_b - vec_a;
	Vec3 vec_ac = vec_c - vec_a;

	*normal = normalize(cross(vec_ab, vec_ac));
	Vec3 camera_dir = normalize(camera_position - vec_a);

	return dot(*normal, camera_dir) > 0;
};

static inline void R_apply_projection(const Vec3 * transformed_vertices,
		Proj_triangle * projected_triangle){

	for(int j = 0; j < 3; j++){
		Vec4 augmented = vec4_from_vec3(transformed_vertices[j]);
		Vec4 proj_point = Mat4_mult_vec4_project(perspective_proj_mat, augmented);

		proj_point.x *= screen_x;
		proj_point.y *= screen_y;

		proj_point.x += screen_x;
		proj_point.y += screen_y;

		projected_triangle->projected_points[j] = Vec2(proj_point.x, proj_point.y);
	}
};

static uint32_t light_color_apply_intensity(uint32_t light_color, float intensity){

	if(intensity > 1) intensity = 1.0;
	if(intensity < 0) intensity = 0;

	uint8_t r, g, b, shaded_r, shaded_g, shaded_b;
	unpack_color(light_color, &r, &g, &b);

	shaded_r = static_cast<uint8_t>(intensity * r);
	shaded_g = static_cast<uint8_t>(intensity * g);
	shaded_b = static_cast<uint8_t>(intensity * b);

	return pack_color(shaded_r, shaded_g, shaded_b);
}

static void R_update(){
	cube->rotation.x += 0.01;
	cube->rotation.y += 0.01;
	cube->rotation.z += 0.01;
	cube->scale.x += 0.002;
	cube->scale.y += 0.001;

	cube->translation.x = 0;
	cube->translation.y = 0;
	cube->translation.z = 5.0;

	scale_mat = Mat4_scale(cube->scale.x, cube->scale.y, cube->scale.z);
	translate_mat  = Mat4_translate(cube->translation.x, cube->translation.y, cube->translation.z);

	rotate_y_mat = Mat4_make_rotate_y(cube->rotation.y);
	rotate_x_mat = Mat4_make_rotate_x(cube->rotation.x);
	rotate_z_mat = Mat4_make_rotate_z(cube->rotation.z);

	// make world matrix
	Mat4 world_matrix = translate_mat * rotate_y_mat * rotate_z_mat * Mat4_id();
	assert(cube->faces.size() != 0);

	// 3 projected screen vertices coming out of this process for each face
	for(size_t i = 0; i < cube->faces.size(); i++){
		Proj_triangle projected_triangle;
		projected_triangle.avg_z = 0;

		Vec3 transformed_vertices[3];
		R_apply_transformations(&cube->faces[i], transformed_vertices, world_matrix);

		// compute avg depth
		for(int j = 0; j < 3; j++){
			projected_triangle.avg_z += transformed_vertices[j].z;
		}
		projected_triangle.avg_z /= 3;

		Vec3 normal;
		if(!R_backface_cull_test(transformed_vertices, &normal)){
			continue;
		}

		float light_i = dot(light.direction, normal) * -1.0;
		projected_triangle.color = light_color_apply_intensity(light.color, light_i);
		projected_triangle.face = &cube->faces[i];

		// project face into a screen triangle
		R_apply_projection(transformed_vertices, &projected_triangle);

		cube_triangles.push_back(projected_triangle);
	}

	// painter's
	std::sort(cube_triangles.begin(), cube_triangles.end(), comp_proj_triangle);
};

static void R_render(){
	Fbuffer_clear(fb, 0);
	size_t n = cube_triangles.size();
	for(size_t i = 0; i < n; i++){
		Proj_triangle * t = &cube_triangles[i];

		//draw_triangle(fb, t->projected_points, t->color);
		//draw_wireframe_triangle(fb, t->projected_points, 0x00ff00ff);

		uint32_t * pixels = (uint32_t *)wall_texture->pixels;
		Tex2_coord uv_coords[3] = {t->face->a_uv, t->face->b_uv, t->face->c_uv};
		draw_triangle_tex2mapped(fb, t->projected_points, uv_coords, pixels);


	}
	cube_triangles.clear();
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
	DIE(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0, IMG_GetError());
	fb = Fbuffer_create(window_w, window_h);
	running = true;
	prev_time = SDL_GetPerformanceCounter();
	cube = load_cube_test();
	wall_texture = load_texture("assets/wall.png");
	//cube = Obj_load(cube, "assets/f22.obj");

	double aspect_ratio = (double)window_h / (double)window_w;
	perspective_proj_mat = Mat4_make_perspective(TO_RAD(60.0), aspect_ratio, 0.1, 100.0);

	while(running){
		R_input();
		R_cap_fps();
		R_update();
		R_render();
	};

	D_Quit();
	Fbuffer_destroy(fb);
	IMG_Quit();
};