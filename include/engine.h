#pragma once

#include "camera.h"
#include "fbuffer.h"
#include "display.h"
#include "resources.h"
#include "input.h"
#include "fps.h"

struct Engine{
	Engine(int window_w, int window_h);
	~Engine();
	void run();

	private:
		void update();
		void draw();

		Camera m_camera;
		std::unique_ptr<mate3d::Fbuffer> m_fb;
		std::unique_ptr<mate3d::display> m_display;
		Fps m_fps;
		Resources * m_resources;
		Input m_input;
};
