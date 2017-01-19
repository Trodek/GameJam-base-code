#ifndef _UIBUTTON_
#define _UIBUTTON_

#include "UIImage.h"
#include "p2SString.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Gui.h"

class UIButton : public UIImage {
public:
	//Constructor
	UIButton(int x, int y, int w, int h,UIElement* parent) : UIImage(Button,x,y,parent,true,w,h)
	{
	}
	UIButton(UItypes type, int x, int y, int w, int h, UIElement* parent) : UIImage(type, x, y, parent, true, w, h)
	{
	}
	// Destructor
	~UIButton(){}

	// Draw
	void InnerDraw() 
	{
		if (image_rect.w == 0)
			Standard();
		App->render->Blit(App->gui->GetAtlas(), position.x + draw_offset.x, position.y + draw_offset.y, &image_rect,false);
	}

	//Set Draw Rect
	void SetRects(SDL_Rect std_rect,SDL_Rect hl_rect, SDL_Rect click_rect) {
		standard_rect = std_rect;
		highlight_rect = hl_rect;
		clicked_rect = click_rect;
		if (position.w == 0 && position.h == 0) {
			position.w = std_rect.w;
			position.h = std_rect.h;
		}
	}
	void Standard() {
		image_rect = standard_rect;
	}
	void Highlight() {
		image_rect = highlight_rect;
	}
	virtual void Clicked() {
		image_rect = clicked_rect;
	}

	SDL_Rect GetClickedRect() const{
		return clicked_rect;
	}

public:
	
private:
	
private:
	SDL_Rect standard_rect = { 0,0,0,0 };
	SDL_Rect highlight_rect = { 0,0,0,0 };
	SDL_Rect clicked_rect = { 0,0,0,0 };
};
#endif // !_UIBUTTON_