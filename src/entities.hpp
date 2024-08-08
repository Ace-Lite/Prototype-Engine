#include <iostream>
#include <vector>
#include "gl_shader.hpp"
#include "gl_sprite.hpp"
#include "gl/glew.h"

struct vector2data
{
	float x, y;
};

struct vector3data
{
	float x, y, z;
};

// Not sure what to do here, I would have to wait for rendering first anyway
// I do not want to repeat DOOM situation where states are tied to animations.
// TODO: Discuss this
class Animation 
{
public:
	int lenght;
	vector<int*> frames;
};

class SpatialExistence // Entity without any logic (meant to be controlled by something else)
{
public:
	// NOTE: Feel free to change these into Vector3 values.
	vector3data pos = { 0.00f, 0.00f, 0.00f }; // Z should be used for 3D in case we went that direction 
	vector3data angles = { 0.00f, 0.00f, 0.00f };; // For 3D objects in both modes. (pitch for 2D sprites) and sprite flipping
	vector3data scale = { 1.0f, 1.0f, 1.0f }; // scales (Z 3D only)

	float dist(float x, float y, float z);
	float dist(vector3data aim);
	float dist(vector2data aim);

	float dir2D(float x, float y);
	float dir2D(vector3data aim);
	float dir2D(vector2data aim);
};

//
// These are classes regarding visible objects.
//

class Entity : public SpatialExistence // Entity without any logic (meant to be controlled by something else)
{
public:
	bool visible = false; // Mostly for 2D, could possibly be used for 3D culling.

	float alpha;

	int blockmap; // Chunk position

	// Visuals
	Shader* shader;
	Sprite* sprite;

	Animation anim_data;
};

class Object : public Entity // Entity with gamelogic
{
public:
	vector3data momentum;

	int physics_flags = 0;
	float gravity_scale = 1.0f;

	// Object relations
	Object* target;
	vector<Object*>sub_objects; // thinking objects
	vector<Entity*>parts; // objects manipulated by object

	// Lua_data
	int definition_id; // this one is merely selection
	int embscript;
	int table;
};

//
//	Misc
//

class Camera : public SpatialExistence {
	Object* target;
};

// Could be used for players
class Controller : public SpatialExistence {
	Camera* camera;

	// Lua stuff
	int events;
	int embscript;
};

//
//	Manager
//

class EntityManager {
	public:
		~EntityManager()
		{
			clear();
		}

		static Object* createObject();
		static Entity* createEntity();
		
		static void clear();
		static void iter_think();

		static size_t returnCount();
		static void draw();
};