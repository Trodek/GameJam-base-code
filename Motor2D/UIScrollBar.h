#ifndef _UISCROLLBAR_
#define _UISCROLLBAR_

#include "UIElement.h"
#include "UIImage.h"
#include "j1App.h"
#include "j1Gui.h"

class UIScrollBar :public UIElement {
public:
	UIScrollBar(int x, int y, int w, int h, UIElement* parent) : UIElement(ScrollBar, x, y, parent, true, w, h)	{	}

	~UIScrollBar(){}

	void SetBar(int rect_x, int rect_y, int w, int h) 
	{
		bar = (UIImage*)App->gui->CreateUIElement(Image, (position.w - w) / 2, (position.h - h) / 2, this, w, h);
		bar->SetRect({ rect_x,rect_y,w,h });
		bar->can_react = false;
	}

	void SetScroll(int rect_x, int rect_y, int w, int h)
	{
		scroll = (UIImage*)App->gui->CreateUIElement(Image, 0, 0, this, w, h);
		scroll->SetRect({ rect_x,rect_y,w,h });
		scroll->can_move = true;
	}

	bool Update() 
	{
		if (App->gui->focused_element == scroll) {
			if (position.w > position.h) {
				if (pos_diff != scroll->GetLocalPosition().x - bar->GetLocalPosition().x) {
					pos_diff = scroll->GetLocalPosition().x - bar->GetLocalPosition().x;
					target->Scroll('h', (float)pos_diff / (float)(bar->position.w - scroll->position.w)); //horizontal scrollbar
				}
			}
			else if (position.h > position.w) {
				if (pos_diff != scroll->GetLocalPosition().y - bar->GetLocalPosition().y) {
					pos_diff = scroll->GetLocalPosition().y - bar->GetLocalPosition().y;
					target->Scroll('v', -(float)pos_diff / (float)(bar->position.h - scroll->position.h)); //verticall scrollbar
				}
			}
		}
		return true;
	}

	void CleanUp() {
		App->gui->DeleteUIElement(bar);
		App->gui->DeleteUIElement(scroll);
	}

	void Active(bool set_active) {
		bar->active = set_active;
		scroll->active = set_active;
		this->active = set_active;
	}

private:
	UIImage* bar;
	UIImage* scroll;
	int pos_diff = 0;

public:

	UIElement* target;
};

#endif // !_UISCROLLBAR_

