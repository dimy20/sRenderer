#pragma once

void * darray_evaluate(void * arr, int elem_size);
int darray_length(void * arr);
void darray_free(void * arr);

#define darray_push(arr, new_elem) do{				   \
	(arr) = darray_evaluate(arr, sizeof(new_elem)); \
	(arr)[darray_length(arr) - 1] = new_elem;        \
}while(0);
