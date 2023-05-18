#pragma once

#include <unordered_map>
#include <memory>
#include <optional>
#include "font.h"

struct Resources{
	Resources(const Resources& other) = delete;
	Resources& operator=(const Resources& other) = delete;
	~Resources();
	static Resources * get_instance();

	void add_font(const std::string& font_id, std::unique_ptr<mate3d::Font> && font);
	std::optional<std::reference_wrapper<mate3d::Font>> get_font(const std::string& font_id);
	private:
		Resources();
		std::unordered_map<std::string, std::unique_ptr<mate3d::Font>> m_fonts;
};
