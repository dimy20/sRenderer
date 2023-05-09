#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "vec.h"
#include "model.h"
#include "memory.h"
#include "darray.h"

Model * Obj_load(const char * filename){
	FILE * f = fopen(filename, "r");
	if(!f){
		fprintf(stderr, "Error: failed to open file %s\n", filename);
		return NULL;
	}

	//TODO: have a model_init function
	Model * model = RMALLOC(sizeof(Model));
	model->vertices = NULL;
	model->faces = NULL;
	model->scale = (Vec3){1, 1, 1};
	model->rotation = (Vec3){0, 0, 0};
	model->translation = (Vec3){0, 0, 0};


	char line[512];
	memset(line, 0, 512);

	while(fgets(line, 512, f)){
		if(strncmp(line, "v ", 2) == 0){
			Vec3 vertex;
			sscanf(line, "v %lf %lf %lf", &vertex.x, &vertex.y, &vertex.z);

			darray_push(model->vertices, vertex);
		};
		if(strncmp(line, "f ", 2) == 0){
			Face face;
			int texture_indices[3];
			int normal_indices[3];
			sscanf(line,"f %d/%d/%d %d/%d/%d %d/%d/%d",
					&face.indices[0], &texture_indices[0], &normal_indices[0],
					&face.indices[1], &texture_indices[1], &normal_indices[1],
					&face.indices[2], &texture_indices[2], &normal_indices[2]);

			face.color = 0xffffffff;

			darray_push(model->faces, face);
		}
	}

	fclose(f);
	return model;
};
