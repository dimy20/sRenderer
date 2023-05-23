#include "resources.h"
#include "error.h"

using namespace mate3d;

Resources::Resources(){ DIE(TTF_Init() < 0 , TTF_GetError()); };
Resources::~Resources(){
	m_fonts.clear();
	TTF_Quit();
};
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
