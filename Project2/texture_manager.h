#ifndef TEXTURE_MANAGER
#define TEXTURE_MANAGER

#include <map>
#include <vector>
#include "RGBpixmap.h"

typedef std::vector<RGB_uchar> texture;

class texture_manager
{
private:
	texture_manager() {}
	static std::map<std::string, texture> textures;

public:
	static void load_texture(const std::string& name, const int& width, const int& height);
	static void delete_texture(const std::string& name);
	static void render_texture(const std::string& name);
};

#endif TEXTURE_MANAGER