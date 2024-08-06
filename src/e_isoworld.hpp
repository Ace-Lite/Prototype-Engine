#include <iostream>
#include <string>
#include <vector>

#include "entities.hpp"

using namespace std;

class IsoMusicPlacement {
	string Peaceful;
	string Combat;
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
	vector<int> data; // tile id
	vector<float> *elevation;

	IsoTileDefitions();
};

class IsoLayer : IsoSpatial {
	IsoBackground background;

	IsoTileDefitions exterior;
	IsoTileDefitions interior;

	// Music
	IsoMusicPlacement music;

	// Collision blockmap
	vector<int> collision;
	vector<float> elevation;
	vector<Object*> objects;

	IsoLayer();

public:

	int getCollision(float x, float y);
};

class IsoChunk {
	string tileset;
	vector<IsoLayer*> layers;

	// these are offsets
	int pos_x, pos_y;

	IsoChunk();

public:

	IsoLayer* getLayer(float z);
};

class IsoLevel : IsoSpatial {
	// default variables
	// chunk - 32 x 32
	int width = 32;
	int height = 32;

	string name = "missing!";
	string alias = "missing!";	

	vector<bool> flags; // flags for settings the level
	vector<bool> levelflags; // flags for level use for setpieces

	vector<IsoChunk*> chunks;
	
	IsoLevel(string level) {
		width = 32; // TODO: write level loader
	}

	~IsoLevel() {
		width = 32; // TODO: write level loader
	}

public:

	IsoChunk* getChunk(float x, float y);
};