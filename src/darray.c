#include "memory.h"

#define DEFAULT_CAP 10

typedef struct{
	int size;
	int cap;
}darray_header;

#define HEADER_OFFSET(arr) ((char *)arr - sizeof(darray_header))
#define BASE_OFFSET(header) ((char *)header) + sizeof(darray_header)

void * darray_evaluate(void * arr, int elem_size){
	if(arr == NULL){
		size_t size = sizeof(darray_header) + (DEFAULT_CAP * elem_size);
		arr = RMALLOC(size);

		darray_header * header = (darray_header *)(arr);
		header->size = 1;
		header->cap = DEFAULT_CAP;

		return BASE_OFFSET(arr);
	};

	darray_header * header = (darray_header *)HEADER_OFFSET(arr);

	// guard for growth
	if(++header->size == header->cap){
		int elem_cap = header->cap * 2;
		int bytes_cap  = (elem_size * elem_cap) + sizeof(darray_header);

		header = (darray_header *)realloc((void *)header, bytes_cap);
		header->cap = elem_cap;

		arr = BASE_OFFSET(header);
	}

	return arr;
};

int darray_length(void * arr){
	if(arr == NULL) return 0;
	darray_header * header = (darray_header *)HEADER_OFFSET(arr);
	return header->size;
}

void darray_free(void * arr){
	free(HEADER_OFFSET(arr));
};

