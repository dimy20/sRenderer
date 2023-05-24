#include <stdio.h>

#define WINDOW_W 800
#define WINDOW_H 600
#include "engine.h"

int main(){
	Engine engine(WINDOW_W, WINDOW_H);
	engine.run();
};
