#ifndef _SCENE_
#define _SCENE_

#include "p2SString.h"
#include "p2List.h"
#include "PugiXml\src\pugixml.hpp"

class UIElement;
class PhysBody;
class Scene 
{
public:
	Scene() {};

	virtual ~Scene() {};

	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual void Draw() {};
	virtual bool CleanUp() { return true; };

	virtual void UIReaction(UIElement* element, int react) {};
	virtual void OnCommand(p2List<p2SString>& tokens) {};

	virtual void OnCVar(p2List<p2SString>& tokens) {};

	virtual void SaveCVar(p2SString& cvar_name, pugi::xml_node& node) const {};

	virtual void OnCollision(PhysBody* bodyA, PhysBody* bodyB) {};

};

#endif