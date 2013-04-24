#include <fstream>
#include <sstream>
#include "level.h"
#include "exceptions.h"
#include "texture_manager.h"

level::level(const std::string& name)
{
	std::ifstream file_stream(name.c_str());
	if (!file_stream.is_open())
		throw file_open_error_exception(name);

	std::string line;
	int texture_width;
	int texture_height;
	std::vector<tile>::size_type row_length;
	std::getline(file_stream, line);
	std::istringstream string_stream(line);
	if (!(string_stream >> texture_width >> texture_height >> row_length))
		throw file_read_error_exception(name);

	std::vector<tile> current_row;
	while (std::getline(file_stream, line))
	{
		level::tile_state current_state;
		switch (line[0])
		{
		case '0':
			current_state = level::CLEAR;
			break;
		
		case '1':
			current_state = level::SOLID;
			break;

		case '2':
			current_state = level::START;
			break;

		default:
			throw file_read_error_exception(name);
		}

		std::string texture_name = line.substr(1);
		texture_manager::load_texture(texture_name, texture_width, texture_height);
		current_row.push_back(tile(current_state, texture_name));
		if (current_row.size() == row_length)
		{
			this->tile_rows.push_back(current_row);
			current_row.clear();
		}
	}

	file_stream.close();
}