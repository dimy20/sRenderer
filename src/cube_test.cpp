
#include "vec.h"
#include "triangle.h"
#include "model.h"
#include <memory>

Vec3 cube_vertices[8] = {
    { .x = -1, .y = -1, .z = -1 }, // 1
    { .x = -1, .y =  1, .z = -1 }, // 2
    { .x =  1, .y =  1, .z = -1 }, // 3
    { .x =  1, .y = -1, .z = -1 }, // 4
    { .x =  1, .y =  1, .z =  1 }, // 5
    { .x =  1, .y = -1, .z =  1 }, // 6
    { .x = -1, .y =  1, .z =  1 }, // 7
    { .x = -1, .y = -1, .z =  1 }  // 8
};

Face cube_faces[6 * 2] = {
	Face({1, 2, 3}, {0, 0}, {0, 1}, {1, 1}),
	Face({1, 3, 4}, {0, 0}, {1, 1}, {1, 0}),
    // front
    //{ .a = 1, .b = 2, .c = 3, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    //{ .a = 1, .b = 3, .c = 4, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
	Face({4, 3, 5}, {0, 0}, {0, 1}, {1, 1}),
	Face({4, 5, 6}, {0, 0}, {1, 1}, {1, 0}),
    // right
    //{ .a = 4, .b = 3, .c = 5, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    //{ .a = 4, .b = 5, .c = 6, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    // back
	Face({6, 5, 7}, {0, 0}, {0, 1}, {1, 1}),
	Face({6, 7, 8}, {0, 0}, {1, 1}, {1, 0}),
    //{ .a = 6, .b = 5, .c = 7, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    //{ .a = 6, .b = 7, .c = 8, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    // left
	Face({8, 7, 2}, {0, 0}, {0, 1}, {1, 1}),
	Face({8, 2, 1}, {0, 0}, {1, 1}, {1, 0}),
    //{ .a = 8, .b = 7, .c = 2, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    //{ .a = 8, .b = 2, .c = 1, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
	Face({2, 7, 5}, {0, 0}, {0, 1}, {1, 1}),
	Face({2, 5, 3}, {0, 0}, {1, 1}, {1, 0}),

    // top
    //{ .a = 2, .b = 7, .c = 5, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    //{ .a = 2, .b = 5, .c = 3, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
	Face({6, 8, 1}, {0, 0}, {0, 1}, {1, 1}),
	Face({6, 1, 4}, {0, 0}, {1, 1}, {1, 0})
    // bottom
    //{ .a = 6, .b = 8, .c = 1, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    //{ .a = 6, .b = 1, .c = 4, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF }
};


std::unique_ptr<Model> load_cube_test(){
	auto cube = std::make_unique<Model>();

	for(int i = 0; i < 8; i++){
		cube->vertices.push_back(cube_vertices[i]);
	}

	for(int i = 0; i < 12; i++){
		cube->faces.push_back(cube_faces[i]);
	}

	cube->scale = Vec3(1, 1, 1);
	cube->rotation = Vec3(0, 0, 0);
	cube->translation = Vec3(0, 0, 0);

	return cube;
};
