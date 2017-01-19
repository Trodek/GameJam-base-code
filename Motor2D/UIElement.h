#ifndef _UIELEMENT_
#define _UIELEMENT_

#include "SDL\include\SDL.h"
#include "j1App.h"
#include "j1Render.h"

class j1Module;

enum UItypes {
	CheckBox, InputText, ScrollBar, Button, Label, Image, Window, Unknow
};

class UIElement {
public:
	UIElement(): type(Unknow){}
	UIElement(UItypes type, int pos_x, int pos_y, UIElement* parent, bool can_react = true, int width = 0, int height = 0): type(type),
		parent(parent),can_react(can_react), position({pos_x, pos_y, width, height})
	{
		if (parent != nullptr) {
			priority = parent->priority + 1;
			SetPos(pos_x, pos_y);
		}
	}
	virtual bool Update()
	{
		return true;
	}

	virtual void Draw() 
	{
		SDL_Rect viewport;
		if (parent != nullptr)
			viewport = { parent->GetPosition().x, parent->GetPosition().y, parent->position.w, parent->position.h };
		else
			viewport = App->render->viewport;
		App->render->SetViewPort(viewport);
		//check element is inside parent boundaries
		if (GetPosition().x + position.w > viewport.x + viewport.w) position.x = viewport.w - position.w;
		if (position.x < 0) position.x = 0;
		//DebugDraw();
		InnerDraw();
		App->render->ResetViewPort();
	}
	virtual void InnerDraw() {}

	virtual void CleanUp(){}

	UItypes GetType()const { return type; }
	
	void SetPos(int x, int y) {
		position.x = x; 
		position.y = y;
		
	}

	int GetPriority()const {
		return priority;
	}
	
	// Use only to make elements that always need to be draw last (example: console)
	void SetPriority(int new_priority) {
		priority = new_priority;
	}

	iPoint GetPosition()const {
		if (parent != nullptr) {
			return{ position.x + parent->GetPosition().x,position.y + parent->GetPosition().y };
		}
		else return{ position.x,position.y };
	}

	iPoint GetLocalPosition()const {
		return{ position.x,position.y };
	}

	UIElement* GetParent()const {
		return parent;
	}

	void DebugDraw(){
		App->render->DrawQuad(position, 255U, 0U, 0U, 255U, false, false);
	}

	virtual void Scroll(char dir, float percentage) {
		if (dir == 'h') {
			draw_offset.x = -position.w*percentage;
		}
		if (dir == 'v') {
			draw_offset.y = -position.h*percentage;
		}
	}

	void AddListener(j1Module* module) {
		if (listeners.find(module) == -1) {
			listeners.add(module);
		}
	}

	p2List_item<j1Module*>* GetFirstListener() {
		return listeners.start;
	}

	p2List_item<j1Module*>* GetLastListener() {
		return listeners.end;
	}

public:
	SDL_Rect position;
	bool can_react = true;
	bool mouse_over = false;
	bool can_move = false;
	iPoint last_position;
	bool to_delete = false;
	bool active = true;
	iPoint draw_offset = { 0,0 };

private:
	UItypes type;
	UIElement* parent = nullptr;
	int priority = 0;
	p2List<j1Module*> listeners;

private:

};

#endif