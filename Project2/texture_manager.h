#ifndef TEXTURE_MANAGER
#define TEXTURE_MANAGER

#include <map>
#include <vector>
#include "RGBpixmap.h"

typedef std::vector<RGB_uchar> texture;

class texture_manager
{
private:
public:
	texture_manager() {}
	static std::map<std::string, texture> textures;
//So OpenGL can access the texture(I just wanted to see if this worked, feel free storing the glTexture numbers elsewhere)
//sorry for butting in, but I really like seeing something tangible
	static std::map<std::string, GLuint> texture_names;
public:
	static void load_texture(const std::string& name, const int& width, const int& height);
	static void delete_texture(const std::string& name);
	static void render_texture(const std::string& name);
	static GLuint get_texture_name(const std::string& name);
	static texture get_texture(const std::string& name);

};

#endif TEXTURE_MANAGER