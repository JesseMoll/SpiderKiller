#include "texture_manager.h"
#include "exceptions.h"

std::map<std::string, texture> texture_manager::textures;
std::map<std::string, GLuint> texture_manager::texture_names;

void texture_manager::load_texture(const std::string& name, const int& width, const int& height)
{
	if (texture_manager::textures.count(name) == 1)
		return;

	RGBpixmap rgb_pixmap(height, width);
	if (!rgb_pixmap.readBMPFile(name))
		throw file_read_error_exception(name);

	texture pixel_array;
	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			RGBpixel pixel = rgb_pixmap.getPixel(col, row);
			//Is the pixel transparent?
			if(pixel.r == 0 && pixel.g == 255 && pixel.b == 255)
			{
				//Make it black and transparent (otherwise, cyan bleeds through
				pixel_array.push_back(0);
				pixel_array.push_back(0);
				pixel_array.push_back(0);
				pixel_array.push_back(0);
			}
			else
			{
				//Not sure, but the rgb order seems wrong... TODO
				pixel_array.push_back(pixel.r);
				pixel_array.push_back(pixel.g);
				pixel_array.push_back(pixel.b);
				//Opaque (alpha channel is transparency)
				pixel_array.push_back(255);
			}
		}
	}

	//Generate an empty texture
	glGenTextures (1,&texture_manager::texture_names[name]);
	//Set active texture to the newly created texture
	glBindTexture (GL_TEXTURE_2D,texture_manager::texture_names[name]);
	//Fills the texture in video memory using the pixel_array
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &pixel_array[0]);

	//Not sure if we need to hold on to the array (openGL stores the texture internally)
	texture_manager::textures.insert(std::pair<std::string, texture>(name, pixel_array));
}

void texture_manager::delete_texture(const std::string& name)
{
	//make GL delete the texture
	glDeleteTextures(1, &texture_names[name]);

	texture_manager::textures.erase(name);
	texture_manager::texture_names.erase(name);
}

GLuint texture_manager::get_texture_name(const std::string& name)
{
	return texture_names[name];
}

texture texture_manager::get_texture(const std::string& name)
{
	if(textures.count(name) != 1)
		throw(texture_not_found(name));
	return textures[name];
}