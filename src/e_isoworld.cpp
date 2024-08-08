#include "e_isoworld.hpp"

//
//	ISO LAYER METHODS
//

int IsoLayer::getCollision(float x, float y) {}

//
//	ISO CHUNK METHODS
//

IsoLayer* IsoChunk::getLayer(float z)
{
	int floor = std::floor(z / 64.0f);

	if (layers.size() >= floor)
		return layers[floor];
	else
		return NULL;
}

//
//	ISO LEVEL METHODS
//

IsoChunk* IsoLevel::getChunk(float x, float y) {
	float coordinate = std::modf(x, (float*)width) + y * height;
	
	if (chunks.size() >= coordinate)
		return chunks[coordinate];
	else
		return NULL;
}

IsoLayer* IsoLevel::getLayer(float x, float y, float z) {
	IsoChunk* chunk = getChunk(x, y);

	if (chunk)
	{
		IsoLayer* layer = chunk->getLayer(z);

		if (layer)
			return layer;
	}
	
	return NULL;
}