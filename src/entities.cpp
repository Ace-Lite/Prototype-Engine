#include <iostream>
#include <vector>
#include "entities.hpp"

#include "gl_init.hpp"
#include "gl_shader.hpp"
#include "gl_sprite.hpp"
#include "gl/glew.h"

// SKYDUSK: 24/07/2024
// This is very much prototype, not sure what to do here really.
// However I would focus on making it as conveneint for everyone, including us to use.
// Preferably one function/method for specific action in very consise way
// If someone thinks of better solution, go ahead!

#define DEFAULT_GRAVITY 9.81f
#define BLOCKMAP_DIM 256.00f

// Gravitational flags
#define GF_COLLISION	1 << 0
#define GF_GRAVITY		1 << 1

// Used for definitions
std::vector<int*> definitions;
std::vector<Object*> object_list;
std::vector<Entity*> entity_list;

// prototype blockmap, refactor when world is made.
static std::vector<std::vector<Object*>> blockmap; //this setup is ridiculous.

static GLuint incideQuad[] = { 0, 1, 2, 2, 3, 0 };
extern Sprite* errorSprite;

Object* EntityManager::createObject()
{
	Object *obj = new Object;
	object_list.push_back(obj);
	entity_list.push_back(obj);

	return obj;
}

Entity* EntityManager::createEntity()
{
	Entity* ent = new Entity;
	entity_list.push_back(ent); // probably not best solution, probably achives object slicing
		
	return ent;
}

void EntityManager::clear()
{		
	auto itr = entity_list.begin();
	while (itr != entity_list.end())
	{
		itr = entity_list.erase(itr);
		++itr;
	}

	object_list.clear();
	entity_list.clear();
}

void EntityManager::iter_think()
{
	if (!object_list.empty()) // merely making sure to have no memory leaks
	{
		for (auto it = object_list.begin(); it != object_list.end(); ++it)
		{

			if ((*it)->physics_flags & GF_GRAVITY)
				continue;

			// Collision Checks
			if ((*it)->physics_flags & GF_COLLISION)
				continue;

		}
	}
}

size_t EntityManager::returnCount()
{
	return entity_list.size();
}

void EntityManager::draw()
{
	if (!entity_list.empty()) // merely making sure to have no memory leaks
	{
		for (auto it = entity_list.begin(); it != entity_list.end(); ++it)
		{
			if (!(*it)->sprite)
				continue;

			vector3data pos = (*it)->pos;
			vector3data sca = (*it)->scale;

			float alpha = (*it)->alpha;

			vector2data dim = { 
				(float)(*it)->sprite->width * sca.x, // width
				(float)(*it)->sprite->height * sca.y  // height
			};

			vector2data vector1 = { pos.x			, pos.y			};
			vector2data vector2 = { pos.x + dim.x	, pos.y			};
			vector2data vector3 = { pos.x + dim.x	, pos.y + dim.y };
			vector2data vector4 = { pos.x			, pos.y + dim.y };
					
			GLfloat verticles[] = {
			// coordinates		|	 colors RGBA	| texture coordinates
			vector1.x, vector1.y, 1.0, 1.0, 1.0, alpha, 0.f, 0.f,
			vector2.x, vector2.y, 1.0, 1.0, 1.0, alpha, 1.f, 0.f,
			vector3.x, vector3.y, 1.0, 1.0, 1.0, alpha, 1.f, 1.f,
			vector4.x, vector4.y, 1.0, 1.0, 1.0, alpha, 0.f, 1.f,
			};

			// bindings
			if ((*it)->shader)
				(*it)->shader->activate();
			else
				glUseProgram(0);

			if ((*it)->sprite)
				(*it)->sprite->bind();
			else
				errorSprite->bind();

			// initizing & rendering
			glBufferData(GL_ARRAY_BUFFER, sizeof(verticles), verticles, GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(incideQuad), incideQuad, GL_STATIC_DRAW);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}
}
