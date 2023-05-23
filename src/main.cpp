#include <stdio.h>

#define WINDOW_W 800
#define WINDOW_H 600
#include "renderer.h"

int main(){
	renderer::init(WINDOW_W, WINDOW_H);
	renderer::run();
	renderer::quit();
};
