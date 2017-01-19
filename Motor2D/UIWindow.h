#ifndef _UIWINDOW_
#define _UIWINDOW_

#include "UIImage.h"
#include "p2SString.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Gui.h"

class UIWindow : public UIImage {
public:
	//Constructor
	UIWindow(int x, int y, int w, int h, UIElement* parent) : UIImage(Window, x, y, parent, true, w, h)
	{
	}
	// Destructor
	~UIWindow() {}

	// Draw
	void InnerDraw()
	{
		if (visible){
			if (image_rect.w != 0 && image_rect.h != 0)
				App->render->Blit(App->gui->GetAtlas(), position.x + draw_offset.x, position.x + draw_offset.y, &image_rect, false);
			else
				App->render->DrawQuad({ position.x + draw_offset.x,position.y + draw_offset.y,position.w,position.h }, r, g, b, a, true, false);
		}
	}

	void SetFillColor(uint red, uint green, uint blue, uint alpha) {
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	void Scroll(char dir, float percentage) {
		if (dir == 'h') {
			draw_offset.x = (GetParent()->position.w - image_rect.w)*percentage;
		}
		if (dir == 'v') {
			draw_offset.y = (GetParent()->position.h - image_rect.h)*percentage;
		}
	}

public:
	bool visible = true;
private:
	uint r = 0;
	uint g = 0;
	uint b = 0;
	uint a = 0;
private:
};
#endif // !_UIBUTTON_
