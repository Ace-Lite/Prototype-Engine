#include <iostream>
#include <vector>
using namespace std;

// Some start, would require some library to do something with this in first place.
class tile_chunk
{
public:
	int width;
	int height;

	int depth;

	// lua script
	int script;

	int tileset;
	vector<int> placement;
	vector<int> flags;
};

class tile_world
{
	int setup;
	vector<tile_chunk*> chunks;
};
