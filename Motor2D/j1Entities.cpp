#include "j1Entities.h"
#include "p2Log.h"

j1Entities::j1Entities()
{
	name.create("entities");
}

j1Entities::~j1Entities()
{
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next) {
		RELEASE(entity->data);
	}
	entities.clear();
}

bool j1Entities::Awake(pugi::xml_node &)
{
	return true;
}

bool j1Entities::Start()
{
	return true;
}

bool j1Entities::PreUpdate()
{
	return true;
}

bool j1Entities::Update(float dt)
{
	bool ret = true;

	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next) {
		ret = entity->data->Update(dt);
		ret = entity->data->Draw();
		if (!ret) {
			LOG("Error updating entity: %s", entity->data->GetName());
			break;
		}
	}

	return true;
}

bool j1Entities::PostUpdate()
{
	return true;
}

bool j1Entities::CleanUp()
{
	return true;
}

Entity * j1Entities::CreateEntity(EntityType type, int pos_x, int pos_y)
{
	static_assert(NoType == 0, "CreateEntity code need update");

	Entity* tmp = nullptr;

	switch (type)
	{
	case NoType:
		break;
	default:
		break;
	}
	
	if (tmp != nullptr)
		entities.add(tmp);

	return tmp;
}

void j1Entities::DeleteEntity(Entity * del_entity)
{
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next) {
		if (entity->data == del_entity) {
			RELEASE(entity->data);
			entities.del(entity);
			break;
		}
	}
}