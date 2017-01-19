#ifndef _UICHECKBOX_
#define _UICHECKBOX_

#include "UIButton.h"

class UICheckBox :public UIButton {
public:
	UICheckBox(int x, int y, UIElement* parent, int w, int h) :UIButton(CheckBox, x, y, w, h, parent) {

	}
	~UICheckBox(){}

	void InnerDraw()
	{
		if (image_rect.w == 0)
			Standard();
		App->render->Blit(App->gui->GetAtlas(), position.x + draw_offset.x, position.y + draw_offset.y, &image_rect, false);
		if (clicked) {
			App->render->Blit(App->gui->GetAtlas(), position.x + draw_offset.x + (position.w - GetClickedRect().w) / 2, position.y + draw_offset.y + (position.h - GetClickedRect().h) / 2, &GetClickedRect());
		}
	}

	void AddOptions(UICheckBox* option) {
		if (options.find(option) == -1) {
			options.add(option);
		}
		if (option->options.find(this) == -1) {
			option->options.add(this);
		}
	}

	void Clicked() {
		clicked = !clicked;
		if (is_option) {
			for (p2List_item<UICheckBox*>* item = options.start; item; item = item->next) {
				item->data->clicked = false;
			}
		}
	}

	bool GetIsClicked()const {
		return clicked;
	}

public:
	bool is_option = false;

private:
	bool clicked = false;
	p2List<UICheckBox*> options;

private:

};

#endif // !_UICHECKBOX_

