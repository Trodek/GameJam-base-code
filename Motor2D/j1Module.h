// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __j1MODULE_H__
#define __j1MODULE_H__

#include "p2SString.h"
#include "p2List.h"
#include "PugiXml\src\pugixml.hpp"

class j1App;
class UIElement;
class PhysBody;

class j1Module
{
public:

	j1Module() : active(false)
	{}

	virtual ~j1Module()
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	virtual void UIReaction(UIElement* element, int react) {

	}
	virtual void OnCommand(p2List<p2SString>& tokens) {

	}

	virtual void OnCVar(p2List<p2SString>& tokens) {

	}

	virtual void SaveCVar(p2SString& cvar_name, pugi::xml_node& node) const {

	}

	virtual void OnCollision(PhysBody* bodyA, PhysBody* bodyB)
	{

	}


public:

	p2SString	name;
	bool		active;

};

#endif // __j1MODULE_H__