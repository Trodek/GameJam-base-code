#ifndef _UISTATICIMAGE_
#define _UISTATICIMAGE_

#include "UIElement.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1App.h"

class UIStaticImage : public UIElement {
public:
	
	UIStaticImage() : UIElement(StaticImage, 0, 0){}
	UIStaticImage(int pos_x, int pos_y) : UIElement(StaticImage, pos_x, pos_y){}
	UIStaticImage(SDL_Rect image_rect, int pos_x, int pos_y): UIElement(StaticImage, pos_x, pos_y), image_rect(image_rect){}

	void Draw() const{
		App->render->Blit(App->gui->GetAtlas(), -App->render->camera.x + position.x, -App->render->camera.y + position.y, &image_rect);
	}

	void SetRect(SDL_Rect img_rect) {
		image_rect = img_rect;
	}

public:
private:
	SDL_Rect image_rect = { 0,0,0,0 };
private:

};

#endif
