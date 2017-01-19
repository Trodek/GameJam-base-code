#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "UIImage.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UIInputText.h"
#include "j1Scene.h"
#include "UIWindow.h"
#include "UIScrollBar.h"
#include "UICheckBox.h"
#include "j1Console.h"
#include "FirstScene.h"
#include "Scene.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	first_scene = new FirstScene();

	if (current_scene == nullptr)
	{
		current_scene = first_scene;
		current_scene->Start();
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	if (current_scene != nullptr)
		current_scene->PreUpdate();

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(current_scene!= nullptr)
		current_scene->Update(dt);

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (current_scene != nullptr)
		current_scene->PostUpdate();

	// Jeje
	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN)
		App->console->Activate();

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing current scene");

	if (current_scene != nullptr)
		current_scene->CleanUp();

	return true;
}

void j1Scene::UIReaction(UIElement * element, int react)
{
	current_scene->UIReaction(element, react);
}

void j1Scene::ChangeScene(Scene * new_scene)
{
	current_scene->CleanUp();
	current_scene = new_scene;
	current_scene->Start();
}
