#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UIElement.h"
#include "UILabel.h"
#include "UIImage.h"
#include "UIButton.h"
#include "UIWindow.h"
#include "UIInputText.h"
#include "UIScrollBar.h"
#include "UICheckBox.h"


j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	bool ret = true;

	focus_changed = false;

	ClearUIElements();

	for (p2List_item<UIElement*>* item = UIelements.start; item != nullptr; item = item->next) {
		if (item->data->active) {
			ret = item->data->Update();
			draw_queue.Push(item->data, item->data->GetPriority());
		}
	}

	for (int i = 0; i < draw_queue.Count(); i++) {
		UIElement* tmp = *draw_queue.Peek(i);
		if (tmp->can_react) {
			react_stack.push(tmp);
		}
	}

	return ret;
}

bool j1Gui::Update(float dt)
{
	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN || App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
		if (focused_element != nullptr){
			if (!(mouse.x > focused_element->GetPosition().x && mouse.x<(focused_element->GetPosition().x + focused_element->position.w) && mouse.y > focused_element->GetPosition().y && mouse.y < (focused_element->GetPosition().y + focused_element->position.h))) {
				focused_element = nullptr;
			}
		}
	}

	UIElement* item;
	while (react_stack.pop(item)) {
		Reaction react = None;
		if (mouse.x > item->GetPosition().x && mouse.x<(item->GetPosition().x + item->position.w) && mouse.y > item->GetPosition().y && mouse.y < (item->GetPosition().y + item->position.h)) {
			if (!item->mouse_over) {
				item->mouse_over = true;
				react = MouseEnter;
			}
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
				react = LeftClick;
				focused_element = item;
			}
			if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) {
				react = RightClick;
				focused_element = item;
			}
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				react = LeftClickUp;
			}
			if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP) {
				react = RightClickUp;
			}
		}
		else {
			if (item->mouse_over) {
				item->mouse_over = false;
				react = MouseLeave;
			}
		}
		if (focused_element == item) {
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
				react = LeftArrow;
			}
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
				react = RightArrow;
			}
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
				react = UpArrow;
			}
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
				react = DownArrow;
			}
			if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN) {
				if(!focus_changed)
					focused_element = GetNextFocus();
			}
			if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
				react = Return;
			}
		}
		if (react != None) {
			for (p2List_item<j1Module*>* module = item->GetFirstListener(); module; module = module->next) {
				module->data->UIReaction(item, react);
			}
			break;
		}
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && (mouse.x != last_mouse.x || mouse.y != last_mouse.y)) {
		if (focused_element != nullptr && focused_element->can_move) {
			int x_motion = mouse.x - last_mouse.x, y_motion = mouse.y - last_mouse.y;
			focused_element->SetPos(focused_element->GetLocalPosition().x + x_motion, focused_element->GetLocalPosition().y + y_motion);
		}
	}
	last_mouse = mouse;
	react_stack.clear();

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	UIElement* tmp;
	while (draw_queue.Pop(tmp))
		tmp->Draw();
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	for (p2List_item<UIElement*>* item = UIelements.start; item != nullptr; item = item->next) {
		RELEASE(item->data);
	}

	return true;
}

void j1Gui::UIReaction(UIElement * element, int react)
{
	Reaction reaction = static_cast<Reaction>(react);

	switch (reaction)
	{
	case MouseEnter:
		if (element->GetType() == Button) {
			UIButton* button = dynamic_cast<UIButton*>(element);
			button->Highlight();
		}
		if (element->GetType() == CheckBox) {
			UICheckBox* box = dynamic_cast<UICheckBox*>(element);
			box->Highlight();
		}
		break;
	case MouseLeave:
		if (element->GetType() == Button) {
			UIButton* button = dynamic_cast<UIButton*>(element);
			button->Standard();
		}
		if (element->GetType() == CheckBox) {
			UICheckBox* box = dynamic_cast<UICheckBox*>(element);
			box->Standard();
		}
		break;
	case RightClick:
		if (element->GetType() == Button) {
			UIButton* button = dynamic_cast<UIButton*>(element);
			button->Clicked();
		}
		if (element->GetType() == CheckBox) {
			UICheckBox* box = dynamic_cast<UICheckBox*>(element);
			box->Clicked();
		}
		break;
	case LeftClick:
		if (element->GetType() == Button) {
			UIButton* button = dynamic_cast<UIButton*>(element);
			button->Clicked();
		}
		if (element->GetType() == CheckBox) {
			UICheckBox* box = dynamic_cast<UICheckBox*>(element);
			box->Clicked();
		}
		break;
	case RightClickUp:
		if (element->GetType() == Button) {
			UIButton* button = dynamic_cast<UIButton*>(element);
			button->Highlight();
		}
		if (element->GetType() == CheckBox) {
			UICheckBox* box = dynamic_cast<UICheckBox*>(element);
			box->Highlight();
		}
		break;
	case LeftClickUp:
		if (element->GetType() == Button) {
			UIButton* button = dynamic_cast<UIButton*>(element);
			button->Highlight();
		}
		if (element->GetType() == CheckBox) {
			UICheckBox* box = dynamic_cast<UICheckBox*>(element);
			box->Highlight();
		}
		break;
	case Tab:
		break;
	case None:
		break;
	default:
		break;
	}
}

// getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

UIElement * j1Gui::CreateUIElement(UItypes type, int pos_x, int pos_y, UIElement* parent, int w, int h)
{
	UIElement* element = nullptr;

	switch (type)
	{
	case CheckBox:
		element = new UICheckBox(pos_x, pos_y, parent, w, h);
		break;
	case InputText:
		element = new UIInputText(pos_x, pos_y,w,h,parent);
		break;
	case Button:
		element = new UIButton(pos_x, pos_y, w, h, parent);
		break;
	case Label:
		element = new UILabel(pos_x, pos_y, w, h, parent);
		break;
	case Image:
		element = new UIImage(pos_x, pos_y, w, h, parent);
		break;
	case Window:
		element = new UIWindow(pos_x, pos_y, w, h, parent);
		break;
	case ScrollBar:
		element = new UIScrollBar(pos_x, pos_y, w, h, parent);
		break;
	case Unknow:
		break;
	default:
		break;
	}

	if (element != nullptr) {
		element->AddListener(this);
		UIelements.add(element);
	}

	return element;
}

void j1Gui::DeleteUIElement(UIElement * element)
{
	element->to_delete = true;
}

void j1Gui::ClearUIElements()
{
	for (p2List_item<UIElement*>* item = UIelements.start; item != nullptr; item = item->next) {
		if (item->data->to_delete == true) {
			item->data->CleanUp();
			RELEASE(item->data);
			UIelements.del(item);
			if (item->data == focused_element)
				focused_element = nullptr;
		}

	}
}

UIElement * j1Gui::GetNextFocus()
{
	UIElement* new_focus = nullptr;

	for (p2List_item<UIElement*>* item = UIelements.end; item; item = item->prev) {
		if (focused_element->GetPriority() == item->data->GetPriority() && item->data != focused_element && item->data->can_react) {
			new_focus = item->data;
		}
		else if (item->data == focused_element && new_focus != nullptr) break;

	}
	focus_changed = true;
	return new_focus;
}

// class Gui ---------------------------------------------------

