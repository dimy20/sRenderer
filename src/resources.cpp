#include "resources.h"

using namespace mate3d;

Resources::Resources(){ TTF_Init(); };
Resources::~Resources(){  };
Resources * Resources::get_instance(){
	static Resources r;
	return &r;
};

void Resources::add_font(const std::string& font_id,
					             std::unique_ptr<mate3d::Font> && font){
	m_fonts[font_id] = std::move(font);
}

std::optional<std::reference_wrapper<Font>> Resources::get_font(const std::string& font_id){
	if(m_fonts.find(font_id) != m_fonts.end()){
		return *m_fonts[font_id];
	}
	return std::nullopt;
}
