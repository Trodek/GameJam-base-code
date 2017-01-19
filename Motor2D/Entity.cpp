#include "Entity.h"
#include "j1App.h"

Entity::Entity()
{
}

Entity::Entity(EntityType type, iPoint position, const char* entity_name): type(type)
{
	name.create(entity_name);

	info.SetPos(position);
	
	p2SString xml_file("%s.xml",name.GetString());
	pugi::xml_document doc;
	App->LoadXML(xml_file.GetString(), doc);
	pugi::xml_node config = doc.child("config");

	p2SString tex_path = config.child("texture").attribute("path").as_string();
	info.LoadTexture(tex_path.GetString());
	info.LoadAnimations(config);

}

Entity::~Entity()
{
}

bool Entity::Update(float dt)
{
	return true;
}

bool Entity::Draw()
{
	return true;
}

const char * Entity::GetName() const
{
	return name.GetString();
}
