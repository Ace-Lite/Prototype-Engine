#include <iostream>
#include <vector>
using namespace std;

// SKYDUSK: 24/07/2024
// This is very much prototype, not sure what to do here really.
// However I would focus on making it as conveneint for everyone, including us to use.
// Preferably one function/method for specific action in very consise way
// If someone thinks of better solution, go ahead!


// Not sure what to do here, I would have to wait for rendering first anyway
// I do not want to repeat DOOM situation where states are tied to animations.
// TODO: Discuss this
class Animation 
{
public:
	int lenght;
	vector<int*> frames;
};

class Entity // Entity without any logic (meant to be controlled by something else)
{
	public:
		float x, y, z; // Z should be used for 3D in case we went that direction
		float depth; // Depth in 2D for 3D objects

		float jaw, pitch, roll; // For 3D objects in both modes. (pitch for 2D sprites) and sprite flipping
		float scale_x, scale_y, scale_z; // scales (Z 3D only)

		bool visible = false; // Mostly for 2D, could possibly be used for 3D culling.
		
		float alpha;

		// Visuals
		Animation anim_data;
};

// Used for definitions
vector<int*> definitions;

class Object : public Entity // Entity with gamelogic
{
public:
	int physics_flags;

	// Object relations
	Object *target;
	vector<Object*>sub_objects; // thinking objects
	vector<Entity*>parts; // objects manipulated by object

	// Lua_data
	int definition_id; // this one is merely selection
	int embscript;
	int table;
};

vector<Entity*> entity_list;

class EntityManager
{
	public:
		void create()
		{
			Entity *entity;
			Object obj;

			entity = &obj;
			entity_list.push_back(entity); // probably not best solution, probably achives object slicing
		}


		void clear()
		{
			if (!entity_list.empty()) // merely making sure to have no memory leaks
				for (auto it = entity_list.begin(); it != entity_list.end(); ++it)
				{
					delete(*it);
				}

			entity_list.clear();
		}
};
