#pragma once

#include "fbuffer.h"
void line(Fbuffer * fb, int x1, int y1, int x2, int y2, uint32_t color);
void rect(Fbuffer * fb, int x1, int y1, int w, int h, uint32_t color);
