#include <stdlib.h>
#include "model.h"
#include "darray.h"

void Model_destroy(Model * model){
	if(model != NULL){
		darray_free(model->vertices);
		darray_free(model->faces);
		free(model);
	}
}
