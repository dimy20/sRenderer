#include "primitives2d.h"
#include <stdlib.h>
#include <stdbool.h>

#define SWAP(type, a, b) do { \
    type temp = a; \
    a = b; \
    b = temp; \
} while (0)

#define MIN(a, b) (a) < (b) ? (a) : (b)

void line(Fbuffer * fb, int x1, int y1, int x2, int y2, uint32_t color){

	int dx = abs(x1 - x2);
	int dy = abs(y1 - y2);

	bool transposed = false;

	if(dy > dx){ // transpose the line
		SWAP(int, x1, y1);
		SWAP(int, x2, y2);
		SWAP(int, dx, dy);
		transposed  = true;
	}

	// make sure left to write
	if(x1 > x2){
		SWAP(int, x1, x2);
		SWAP(int, y1, y2);
	}

	int offset_step = 2 * dy; // avoid use of float
	int offset = 0;
	int y_step = y1 < y2 ? 1 : -1;
	int y = y1;

	if(transposed){
		for(int x = x1; x <= x2; x++){
			Fbuffer_set_pixel(fb, y, x, color);
			offset += offset_step;
			if(offset > dx){
				y += y_step;
				offset -= dx * 2;
			}
		}
	}else{
		for(int x = x1; x <= x2; x++){
			Fbuffer_set_pixel(fb, x, y, color);
			offset += offset_step;
			if(offset > dx){
				y += y_step;
				offset -= dx * 2;
			}
		}
	}
};

void rect(Fbuffer * fb, int x1, int y1, int w, int h, uint32_t color){
   if(w < 0 || h < 0) return;

	if(x1 >= fb->w || y1 >= fb->h || x1 < 0 || y1 < 0){
		return;
	}

	int start_x = x1;
	int end_x = MIN(x1 + w, fb->w - 1);

	int min_y = y1;
	int max_y = MIN(y1 + h, fb->h - 1);

	for(int y = min_y; y <= max_y; y++){
		line(fb, start_x, y, end_x, y, color);
	}
};
