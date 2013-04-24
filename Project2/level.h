#ifndef LEVEL
#define LEVEL

#include <vector>

class level
{
private:
	enum tile_state {TRANSPARENT, SOLID, START};
	typedef std::pair<tile_state, std::string> tile;
	std::vector<std::vector<tile> > tile_rows;

public:
	level(const std::string& name);
};

#endif