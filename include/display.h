#pragma once
#include <stdint.h>
void D_Init(int window_w, int window_h);
void D_Quit();
void D_present_pixels(const uint32_t * pixels);
