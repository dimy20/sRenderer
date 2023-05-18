#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <limits>

#include "error.h"
#include "renderer.h"
#include "display.h"
#include "fbuffer.h"
#include "draw.h"
#include "vec.h"
#include "obj.h"
#include "mat.h"
#include "light.h"
#include "texture.h"
#include "perspective.h"
#include "fps.h"
#include "resources.h"
#include "world_obj.h"

// just for testing
SDL_Surface * wall_texture;
/* Constants */
#define M_PI 3.14159265358979323846
#define TO_RAD(d) ((d) * M_PI) / 180.0
#define WINDOW_W 800
#define WINDOW_H 600
static Vec3 camera_position = {0, 0, 0};

// screen space translations
static const int screen_y = WINDOW_H / 2;
static const int screen_x = WINDOW_W / 2;

/* Globals */
static bool running = false;
uint32_t renderer_flags;
std::unique_ptr<mate3d::Fbuffer> fb;
std::unique_ptr<mate3d::display> display;
std::vector<double> zbuffer;
Fps fps;
Resources * resources;
WorldObj obj(std::move(load_obj("assets/cube.obj")), Transform(Vec3(0, 0, 0), Vec3(1, 1, 1), Vec3(0, 0, 0)));

// transformations
Mat4 scale_mat, translate_mat, rotate_y_mat, rotate_x_mat, rotate_z_mat;
Mat4 perspective_proj_mat;

static void R_input(){
	SDL_Event e;
	while(SDL_PollEvent(&e)){
		if(e.type == SDL_QUIT){
			running = false;
		}
	};
};

// applies transformations to a face vertices, and returns an array of 3 transformed vertices
static inline void R_apply_transformations(const Face& face, Vec3 * transformed_vertices, const Mat4& world_matrix){
	std::fill(transformed_vertices, transformed_vertices + 3, Vec3());
	Vec4 augmented_vertices[3];
	for(int j = 0; j < 3; j++){
		augmented_vertices[j] = vec4_from_vec3(obj.model->vertices[face.indices[j] - 1]);
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

static inline void R_apply_projection(const Vec3 * transformed_vertices, Proj_triangle& projected_triangle){

	for(int j = 0; j < 3; j++){
		Vec4 augmented = vec4_from_vec3(transformed_vertices[j]);

		Vec4 proj_point = perspective_project_vertex(perspective_proj_mat, augmented);

		proj_point.x *= screen_x;
		proj_point.y *= screen_y;

		proj_point.x += screen_x;
		proj_point.y += screen_y;

		projected_triangle.projected_points[j] = proj_point;
	}
};

static uint32_t light_color_apply_intensity(uint32_t light_color, float intensity){

	if(intensity > 1) intensity = 1.0;
	if(intensity < 0) intensity = 0;

	uint8_t r, g, b, shaded_r, shaded_g, shaded_b;
	mate3d::unpack_color(light_color, &r, &g, &b);

	shaded_r = static_cast<uint8_t>(intensity * r);
	shaded_g = static_cast<uint8_t>(intensity * g);
	shaded_b = static_cast<uint8_t>(intensity * b);

	return mate3d::pack_color(shaded_r, shaded_g, shaded_b);
}

static void R_update(){
	//cube->rotation.x += 0.01;
	obj.transform.rotation.y += 0.01;
	//cutransformotation.z += 0.01;
	obj.transform.scale.x += 0.002;
	obj.transform.scale.y += 0.001;

	obj.transform.translation.x = 0;
	obj.transform.translation.y = 0;
	obj.transform.translation.z = 5.0;

	scale_mat = Mat4_scale(obj.transform.scale);
	translate_mat  = Mat4_translate(obj.transform.translation);

	rotate_y_mat = Mat4_make_rotate_y(obj.transform.rotation.y);
	rotate_x_mat = Mat4_make_rotate_x(obj.transform.rotation.x);
	rotate_z_mat = Mat4_make_rotate_z(obj.transform.rotation.z);

	// make world matrix
	Mat4 world_matrix = translate_mat * rotate_y_mat * rotate_z_mat * Mat4_id();
	assert(obj.model->faces.size() != 0);

	obj.proj_triangles_num = 0;
	// 3 projected screen vertices coming out of this process for each face
	for(size_t i = 0; i < obj.model->faces.size(); i++){
		Proj_triangle& projected_triangle = obj.projected_triangles[obj.proj_triangles_num];

		Vec3 transformed_vertices[3];
		R_apply_transformations(obj.model->faces[i], transformed_vertices, world_matrix);

		Vec3 normal;
		if(!R_backface_cull_test(transformed_vertices, &normal)){
			continue;
		}

		float light_i = dot(light.direction, normal) * -1.0;
		projected_triangle.color = light_color_apply_intensity(light.color, light_i);
		projected_triangle.face = &(obj.model->faces[i]);

		// project face into a screen triangle
		R_apply_projection(transformed_vertices, projected_triangle);
		obj.proj_triangles_num++;
	}
};

static void R_render(){
	fb->clear(0);
	Tex2_coord uv_coords[3];
	//std::cout << obj.proj_triangles_num << "\n";
	for(size_t i = 0; i < obj.proj_triangles_num; i++){
		Proj_triangle& t = obj.projected_triangles[i];
		//draw_triangle(*fb, t.projected_points, t.color);
		//draw_wireframe_triangle(*fb, t.projected_points, 0x00ff00ff);

		for(size_t j = 0; j < 3; j++){
			uv_coords[j] = obj.model->uv_coords[t.face->uv_indices[j] - 1];
		}
		draw_triangle_tex2mapped(*fb, t.projected_points, uv_coords, wall_texture);
	}

	std::fill(zbuffer.begin(), zbuffer.end(), 1.0);
	display->put_pixels(&fb->pixels[0]);

	auto fps_font = resources->get_font("fps");
	if(fps_font != std::nullopt){
		std::string fps_string = "fps:" + std::to_string(fps.frames());
		display->put_text(*fps_font, fps_string);
	}

	display->present();
};

void renderer::run(){
	running = true;
	while(running){
		fps.counter();
		R_input();
		fps.cap();
		R_update();
		R_render();
	};
};

void renderer::init(int window_w, int window_h){
	//SDL stuff
	DIE(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0, IMG_GetError());
	//display
	display = std::make_unique<mate3d::display>(window_w, window_h);
	fb = std::make_unique<mate3d::Fbuffer>(window_w, window_h);

	//TODO: Move texture to resources:
	wall_texture = load_texture("assets/cube.png");
	zbuffer.resize(window_w * window_h, 1.0);

	// initialize resources
	resources = Resources::get_instance();
	auto fps_font = std::make_unique<mate3d::Font>("assets/Hack-Regular.ttf",
					24,
					display->renderer());

	resources->add_font("fps", std::move(fps_font));

	double aspect_ratio = window_h / static_cast<double>(window_w);
	perspective_proj_mat = Mat4_make_perspective(TO_RAD(60.0), aspect_ratio, 0.1, 100.0);
};


void renderer::quit(){
	IMG_Quit();
};
