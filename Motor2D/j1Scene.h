#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class Scene;
class FirstScene;

class UIImage;
class UILabel;
class UIButton;
class UIWindow;
class UIInputText;
class UIScrollBar;
class UICheckBox;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void UIReaction(UIElement* element, int react);

	void ChangeScene(Scene* new_scene);

	Scene* current_scene = nullptr;

	FirstScene* first_scene = nullptr;

private:
	SDL_Texture* debug_tex;

	UIImage* banner;
	UILabel* text;
	UILabel* text2;
	UIButton* button;
	UIButton* button2;
	UIWindow* window;
	UIInputText* input_text;
	UIScrollBar* horizontal;
	UIScrollBar* vertical;
	UICheckBox* option1;
	UICheckBox* option2;
	UICheckBox* multi_option1;
	UICheckBox* multi_option2;
	UIWindow* vis;
};

#endif // __j1SCENE_H__