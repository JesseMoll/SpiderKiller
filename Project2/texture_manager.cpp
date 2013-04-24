#include "texture_manager.h"
#include "exceptions.h"

std::map<std::string, texture> texture_manager::textures;

void texture_manager::load_texture(const std::string& name, const int& width, const int& height)
{
	if (texture_manager::textures.count(name) == 1)
		return;

	RGBpixmap rgb_pixmap(width, height);
	if (!rgb_pixmap.readBMPFile(name))
		throw file_read_error_exception(name);

	texture pixel_array;
	for (int row = 0; row < width; ++row)
	{
		for (int col = 0; col < height; ++col)
		{
			RGBpixel pixel = rgb_pixmap.getPixel(col, row);
			pixel_array.push_back(pixel.r);
			pixel_array.push_back(pixel.g);
			pixel_array.push_back(pixel.b);
		}
	}

	texture_manager::textures.insert(std::pair<std::string, texture>(name, pixel_array));
}

void texture_manager::delete_texture(const std::string& name)
{
	texture_manager::textures.erase(name);
}