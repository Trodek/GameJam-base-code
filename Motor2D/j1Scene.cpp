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
	// UIElements Creation

	window = (UIWindow*)App->gui->CreateUIElement(Window, 30, 30,nullptr, 421, 453);
	window->SetRect({ 32,542,421,453 });
	window->AddListener(this);

	vis = (UIWindow*)App->gui->CreateUIElement(Window, 50, 50, window, 50, 50);
	vis->visible = false;
	vis->AddListener(this);

	banner = (UIImage*)App->gui->CreateUIElement(Image, 0, 0, vis);
	banner->SetRect({ 485, 829, 328, 103 });
	banner->can_react = false;
	banner->active = false;

	vertical = (UIScrollBar*)App->gui->CreateUIElement(ScrollBar, 40, 50, window, 15, 154);
	vertical->can_move = false;
	vertical->SetBar(974, 788, 8, 154);
	vertical->SetScroll(843, 330, 15, 10);
	vertical->target = banner;

	horizontal = (UIScrollBar*)App->gui->CreateUIElement(ScrollBar, 60, 80, window, 154, 15);
	horizontal->can_move = false;
	horizontal->SetBar(974, 788, 154, 8);
	horizontal->SetScroll(843, 330, 10, 15);
	horizontal->target = banner;

	button2 = (UIButton*)App->gui->CreateUIElement(Button, 70, 150, window);
	button2->SetRects({ 648,173,218,57 }, { 6,117,218,57 }, { 417,173,218,57 });
	button2->AddListener(this);

	text2 = (UILabel*)App->gui->CreateUIElement(Label, 50, 6, button2, 90, 20);
	text2->SetText("_______________");
	text2->AddListener(this);

	button = (UIButton*)App->gui->CreateUIElement(Button, 10, 250, window);
	button->SetRects({ 648,173,218,57 }, { 6,117,218,57 }, { 417,173,218,57 });
	button->AddListener(this);

	text = (UILabel*)App->gui->CreateUIElement(Label, 5, 2, button, 90, 20);
	text->SetText("HOLA RIC :)");
	text->AddListener(this);
	text->can_move = false;

	input_text = (UIInputText*)App->gui->CreateUIElement(InputText, 10, 20, window, 70, 20);
	input_text->SetDefaultText("Hello World");
	input_text->AddListener(this);
	input_text->can_move = false;

	option1 = (UICheckBox*)App->gui->CreateUIElement(CheckBox, 400, 10);
	option1->SetRects({ 977,432,20,21 }, { 858,89,20,21 }, { 391,168,12,11 });
	option1->is_option = true;
	option1->can_move = false;

	option2 = (UICheckBox*)App->gui->CreateUIElement(CheckBox, 400, 33);
	option2->SetRects({ 977,432,20,21 }, { 858,89,20,21 }, { 391,168,12,11 });
	option2->is_option = true;
	option2->can_move = false;
	option2->AddOptions(option1);

	multi_option1 = (UICheckBox*)App->gui->CreateUIElement(CheckBox, 450, 10);
	multi_option1->SetRects({ 992,847,20,20 }, { 992,814,20,20 }, { 391,168,12,11 });
	multi_option1->can_move = false;

	multi_option2 = (UICheckBox*)App->gui->CreateUIElement(CheckBox, 450, 33);
	multi_option2->SetRects({ 992,847,20,20 }, { 992,814,20,20 }, { 391,168,12,11 });
	multi_option2->can_move = false;
	multi_option2->AddOptions(multi_option1);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		App->console->Activate();

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::UIReaction(UIElement * element, int react)
{
	Reaction reaction = static_cast<Reaction>(react);

	switch (reaction)
	{
	case MouseEnter:
		if (element == text) {
			text->SetText("Mouse Over");
		}
		if (element == text2) {
			text2->SetText("SIMON NOOB");
		}
		break;
	case MouseLeave:
		if (element == text) {
			text->SetText("Hola Ric :)");
		}
		break;
	case RightClick:
		if (element == text) {
			text->SetText("Right Click");
		}
		break;
	case LeftClick:
		if (element == text) {
			text->SetText("Left Click");
		}
		if (element == button2) {
			banner->active = true;
		}
		break;
	case LeftClickUp:
		break;
	case RightClickUp:
		break;
	case Tab:
		break;
	case RightArrow:
		break;
	case LeftArrow:
		break;
	case UpArrow:
		break;
	case DownArrow:
		break;
	default:
		break;
	}
}
