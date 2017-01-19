#ifndef _FIRSTSCENE_
#define _FIRSTSCENE_

#include "Scene.h"

class FirstScene : public Scene 
{
public:
	FirstScene();

	~FirstScene();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	void Draw();
	bool CleanUp();

	void UIReaction(UIElement* element, int react);
	void OnCommand(p2List<p2SString>& tokens);

	void OnCVar(p2List<p2SString>& tokens);

	void SaveCVar(p2SString& cvar_name, pugi::xml_node& node) const;

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
};

#endif