#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include "p2List.h"
#include "j1Module.h"
#include "Entity.h"

class j1Entities : public j1Module
{
public:

	j1Entities();

	// Destructor
	virtual ~j1Entities();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Update Elements
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	Entity* CreateEntity(EntityType type, int pos_x, int pos_y);

	void DeleteEntity(Entity* del_entity);

private:

private:

	p2List<Entity*> entities;


public:

};


#endif // !_ENTITIES_H_

