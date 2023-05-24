#include "geometry.h"
#include "world_obj.h"

static const Vec3 origin(0, 0, 0);
// screen space translations
#define WINDOW_W 800
#define WINDOW_H 600
static const int screen_y = WINDOW_H / 2;
static const int screen_x = WINDOW_W / 2;

// just for now
extern Mat4 view_matrix;
extern Mat4 perspective_proj_mat;
extern WorldObj obj;

// performs backface culling test on transformed face vertices
bool pipeline::backface_culled(const Vec3 * transformed_vertices, Vec3 * normal){
	 //backface culling test
	Vec3 vec_a = transformed_vertices[0];
	Vec3 vec_b = transformed_vertices[1];
	Vec3 vec_c = transformed_vertices[2];

	Vec3 vec_ab = vec_b - vec_a;
	Vec3 vec_ac = vec_c - vec_a;

	*normal = normalize(cross(vec_ab, vec_ac));
	Vec3 camera_dir = normalize(origin - vec_a);

	return dot(*normal, camera_dir) < 0;
};

// applies transformations to a face vertices, and ouputs an array of 3 transformed vertices
void pipeline::transformations(const Face& face,
							  Vec3 * transformed_vertices,
							  const Mat4& world_matrix){
	std::fill(transformed_vertices, transformed_vertices + 3, Vec3());
	Vec4 augmented_vertices[3];

	for(int j = 0; j < 3; j++){
		augmented_vertices[j] = vec4_from_vec3(obj.model->vertices[face.indices[j] - 1]);
		augmented_vertices[j] = view_matrix * world_matrix * augmented_vertices[j];
		transformed_vertices[j] = vec3_from_vec4(augmented_vertices[j]);
	};
};


Vec2f weak_project(const Vec3& p, double fov_factor){
	return Vec2f((p.x * fov_factor) / p.z, (p.y * fov_factor) / p.z);
};

Vec4 perspective_project_vertex(const Mat4& perspective_matrix, const Vec4& transformed_vertex){
	Vec4 projected_vertex = perspective_matrix * transformed_vertex;

	/* Perform perspective division:
	   original Z depth, World z value of v is kept in W component of 
	   projected_v, we can use this value to perform perspective division. */
	if(projected_vertex.w != 0.0){
		projected_vertex.x /= projected_vertex.w;
		projected_vertex.y /= projected_vertex.w;
		projected_vertex.z /= projected_vertex.w;
	};

	return projected_vertex;
};

void pipeline::projection(const Vec3 * transformed_vertices, Proj_triangle& projected_triangle){
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
