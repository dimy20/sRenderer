#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory>

#include "vec.h"
#include "model.h"

std::unique_ptr<Model> load_obj(const char * filename){
	FILE * f = fopen(filename, "r");
	if(!f){
		fprintf(stderr, "Error: failed to open file %s\n", filename);
		return NULL;
	}

	std::unique_ptr<Model> model = std::make_unique<Model>();

	char line[512];
	memset(line, 0, 512);

	while(fgets(line, 512, f)){
		if(strncmp(line, "v ", 2) == 0){
			Vec3 vertex;
			sscanf(line, "v %lf %lf %lf", &vertex.x, &vertex.y, &vertex.z);
			model->vertices.push_back(vertex);
		}else if(std::strncmp(line, "vt ", 3) == 0){
			Tex2_coord uv;
			sscanf(line, "vt %lf %lf", &uv.u, &uv.v);
			model->uv_coords.push_back(uv);
		}else if(strncmp(line, "f ", 2) == 0){
			Face face;
			sscanf(line,"f %d/%d/%d %d/%d/%d %d/%d/%d",
					&face.indices[0], &face.uv_indices[0], &face.normal_indices[0],
					&face.indices[1], &face.uv_indices[1], &face.normal_indices[1],
					&face.indices[2], &face.uv_indices[2], &face.normal_indices[2]);

			face.color = 0xffffffff;

			model->faces.push_back(face);
		}
	}

	fclose(f);
	return model;
};
