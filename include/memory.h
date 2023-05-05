#pragma once

#include <stdlib.h>
#include "error.h"

#define RMALLOC(size) ({ \
    void * ptr = malloc(size); \
	DIE(!ptr, "Failed to allocate memory.");\
    ptr; \
})
