#ifndef _UILABEL_
#define _UILABEL_

#include "UIElement.h"
#include "p2SString.h"
#include "j1App.h"
#include "j1Fonts.h"

struct _TTF_Font;

class UILabel : public UIElement {
public:

	UILabel() : UIElement(Label, 0, 0, nullptr) {	}
	UILabel(int pos_x, int pos_y, int w, int h, UIElement* parent) : UIElement(Label, pos_x, pos_y, parent, true, w, h){}
	UILabel(UItypes type, int pos_x, int pos_y, int w, int h, UIElement* parent) : UIElement(type, pos_x, pos_y, parent, true, w, h) {}

	void InnerDraw() {
		if (text_texture != nullptr)
			SDL_DestroyTexture(text_texture);
		text_texture = App->font->Print(text.c_str(),color,font);
		SDL_QueryTexture(text_texture, NULL, NULL, &texture_rect.w, &texture_rect.h);
		App->render->Blit(text_texture, position.x + draw_offset.x, position.y + draw_offset.y,&texture_rect,false);
	}

	void SetText(const char* txt, SDL_Color txt_color = {255,255,255,255}, _TTF_Font* txt_font = App->font->default) {
		text = txt;
		font = txt_font;
		color = txt_color;
	}

	void Scroll(char dir, float percentage) {
		if (dir == 'h') {
			draw_offset.x = -(texture_rect.w - GetParent()->position.w)*percentage;
		}
		if (dir == 'v') {
			draw_offset.y = -(texture_rect.h - GetParent()->position.h)*percentage;
		}
	}

public:
	std::string text = "";
	SDL_Texture* text_texture;
	SDL_Rect texture_rect = { 0, 0, 0, 0 };
	_TTF_Font* font = App->font->default;
	SDL_Color color = { 255,255,255,255 };

private:
	
};

#endif