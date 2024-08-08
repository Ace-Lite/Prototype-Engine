#include <iostream>
#include <string>
#include <vector>

#include "entities.hpp"

constexpr auto CHUNK_SIZE = 64.0f;

constexpr auto TILE_WIDTH = 64.0f;
constexpr auto TILE_HEIGHT = 32.0f;

class IsoMusicPlacement {
	std::string Peaceful;
	std::string Combat;
};

class IsoSpatial {
	void getInternalPos(int x, int y);

public:

	// to convert data
	int toIsoVector(int x, int y);
	int toNormVector(int x, int y);
};

class IsoBackground {
	int image;
};

class IsoTileDefitions {
	std::vector<int> data; // tile id
	std::vector<float> *elevation;

	IsoTileDefitions();
};

class IsoLayer : IsoSpatial {
	IsoBackground background;

	IsoTileDefitions exterior;
	IsoTileDefitions interior;

	// Music
	IsoMusicPlacement music;

	// Collision blockmap
	std::vector<int> collision;
	std::vector<float> elevation;
	std::vector<Object*> objects;

	IsoLayer();

public:

	int getCollision(float x, float y);
};

class IsoChunk {
	std::string tileset;
	std::vector<IsoLayer*> layers;

	// these are offsets
	int pos_x, pos_y;

	IsoChunk();

public:

	IsoLayer* getLayer(float z);
};

class IsoLevel : IsoSpatial {
	
	IsoLevel(std::string level) {
		width = 32; // TODO: write level loader
	}

	~IsoLevel() {
		width = 32; // TODO: write level loader
	}

public:

	// default variables
	// chunk - 32 x 32
	int width = 32;
	int height = 32;

	std::string name = "missing!";
	std::string alias = "missing!";

	std::vector<bool> flags; // flags for settings the level
	std::vector<bool> levelflags; // flags for level use for setpieces

	std::vector<IsoChunk*> chunks;

	IsoChunk* getChunk(float x, float y);
	IsoLayer* getLayer(float x, float y, float z);
};