#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <cassert>
#include <iostream>
#include <limits>

#include "engine.h"
#include "error.h"
#include "world_obj.h"
#include "draw.h"
#include "obj.h"
#include "light.h"
#include "geometry.h"

std::vector<double> zbuffer;
WorldObj obj(std::move(load_obj("assets/f22.obj")),
			Transform(Vec3(0, 0, 0), Vec3(1, 1, 1), Vec3(0, 0, 0)));

#define WINDOW_W 800
#define WINDOW_H 600

// just for testing
SDL_Surface * wall_texture;

// transformations
Mat4 scale_mat, translate_mat, rotate_y_mat, rotate_x_mat, rotate_z_mat;
Mat4 perspective_proj_mat;
Mat4 view_matrix;

Engine::Engine(int window_w, int window_h) : m_camera(Vec3(0, 0, -5.0)){
	DIE(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0, IMG_GetError());
	//display
	m_display = std::make_unique<mate3d::display>(window_w, window_h);
	m_fb = std::make_unique<mate3d::Fbuffer>(window_w, window_h);

	//TODO: Move texture to resources:
	wall_texture = load_texture("assets/f22.png");
	zbuffer.resize(window_w * window_h, 1.0);

	// initialize resources
	m_resources = Resources::get_instance();
	auto fps_font = std::make_unique<mate3d::Font>("assets/Hack-Regular.ttf",
					24,
					m_display->renderer());

	m_resources->add_font("fps", std::move(fps_font));

	double aspect_ratio = window_h / static_cast<double>(window_w);
	perspective_proj_mat = Mat4_make_perspective(to_rad(60.0), aspect_ratio, 0.1, 100.0);

}

Engine::~Engine(){
	IMG_Quit();
}

void Engine::draw(){
	m_fb->clear(0);
	Tex2_coord uv_coords[3];
	//std::cout << obj.proj_triangles_num << "\n";
	for(size_t i = 0; i < obj.proj_triangles_num; i++){
		Proj_triangle& t = obj.projected_triangles[i];
		//draw_triangle(*fb, t.projected_points, t.color);
		//draw_wireframe_triangle(*fb, t.projected_points, 0x00ff00ff);

		for(size_t j = 0; j < 3; j++){
			uv_coords[j] = obj.model->uv_coords[t.face->uv_indices[j] - 1];
		}
		draw_triangle_tex2mapped(*m_fb, t.projected_points, uv_coords, wall_texture);
	}

	std::fill(zbuffer.begin(), zbuffer.end(), 1.0);
	m_display->put_pixels(&m_fb->pixels[0]);

	auto fps_font = m_resources->get_font("fps");
	if(fps_font != std::nullopt){
		std::string fps_string = "fps:" + std::to_string(m_fps.frames());
		m_display->put_text(*fps_font, fps_string);
	}

	m_display->present();
};


void Engine::update(){
	obj.transform.rotation.y += 0.01;

	obj.transform.translation.x = 0;
	obj.transform.translation.y = 0;
	obj.transform.translation.z = 5.0;

	scale_mat = Mat4_scale(obj.transform.scale);
	translate_mat  = Mat4_translate(obj.transform.translation);

	rotate_y_mat = Mat4_make_rotate_y(obj.transform.rotation.y);
	rotate_x_mat = Mat4_make_rotate_x(obj.transform.rotation.x);
	rotate_z_mat = Mat4_make_rotate_z(obj.transform.rotation.z);

	m_camera.update(m_input);
	// make world matrix
	Mat4 world_matrix = translate_mat * rotate_y_mat * rotate_z_mat * Mat4_id();
	view_matrix = m_camera.view_matrix();
	assert(obj.model->faces.size() != 0);

	obj.proj_triangles_num = 0;
	// 3 projected screen vertices coming out of this process for each face
	for(size_t i = 0; i < obj.model->faces.size(); i++){
		Proj_triangle& projected_triangle = obj.projected_triangles[obj.proj_triangles_num];

		Vec3 transformed_vertices[3];
		pipeline::transformations(obj.model->faces[i], transformed_vertices, world_matrix);

		Vec3 normal;
		if(pipeline::backface_culled(transformed_vertices, &normal)){
			continue;
		}

		float light_i = dot(light.direction, normal) * -1.0;
		projected_triangle.color = light_color_apply_intensity(light.color, light_i);
		projected_triangle.face = &(obj.model->faces[i]);

		// project face into a screen triangle
		pipeline::projection(transformed_vertices, projected_triangle);
		obj.proj_triangles_num++;
	}
};

void Engine::run(){
	while(!m_input.quit){
		m_fps.counter();
		m_input.update();
		m_fps.cap();
		update();
		draw();
	};
}
