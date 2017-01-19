#ifndef _UIINPUTTEXT_
#define _UIINPUTTEXT_

#include "UILabel.h"
#include "j1Gui.h"
#include "p2SString.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Input.h"

#define CURSOR_W 2

class UIInputText : public UILabel {
public:
	UIInputText(int x, int y, int w, int h, UIElement* parent) : UILabel(InputText, x, y, w, h, parent){}
	~UIInputText(){}

	void InnerDraw() {
		if (text_texture != nullptr)
			SDL_DestroyTexture(text_texture);
		if (!text.empty()) {
			text_texture = App->font->Print(text.c_str(), color, font);
			SDL_QueryTexture(text_texture, NULL, NULL, &texture_rect.w, &texture_rect.h);

			if (App->gui->focused_element == this) {
				int w, h;
				App->font->CalcSize(text.substr(0, cursor_position).c_str(), w, h, font);
				if (w + draw_offset.x > position.w - CURSOR_W) {
					int last_leter_w;
					App->font->CalcSize(text.substr(cursor_position - 1, cursor_position).c_str(), last_leter_w, h, font);
					draw_offset.x -= last_leter_w;
				}
				if (w + draw_offset.x < 0) {
					int last_leter_w;
					if (App->font->CalcSize(text.substr(cursor_position, cursor_position + 1).c_str(), last_leter_w, h, font))
						draw_offset.x += last_leter_w;
				}

				App->render->DrawQuad({ position.x + w + draw_offset.x, position.y, CURSOR_W, position.h }, 255U, 255U, 255U, 255U, true, false);
			}

			App->render->Blit(text_texture, position.x + draw_offset.x, position.y + draw_offset.y, &texture_rect, false);
		}
	}
	bool Update() {
		if (App->gui->focused_element == this) {
			App->input->StartInputText(&text, &cursor_position);
			if (!used) {
				used = true;
				text.clear();
			}
			else {
				if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
					if(cursor_position>0) cursor_position--;
				}
				if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
					if (cursor_position<text.length()) cursor_position++;
				}
				if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN) {
					if (cursor_position > 0) {
						int w, h;
						App->font->CalcSize(text.substr(cursor_position - 1, 1).c_str(), w, h);
						text.erase(cursor_position - 1, 1);
						cursor_position--;
						if (draw_offset.x < 0)
							draw_offset.x += w;
					}
				}
				if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN) {
					text.erase(cursor_position, 1);
				}
				if (App->input->GetKey(SDL_SCANCODE_HOME) == KEY_DOWN) {
					cursor_position = 0;
				}
				if (App->input->GetKey(SDL_SCANCODE_END) == KEY_DOWN) {
					cursor_position = text.length();
				}

			}

		}
		else {
			if (used) {
				if(App->input->input_text == &text)
					App->input->EndInputText();
				if (text.length() < 1) {
					text = default_text;
					used = false;
				}
			}
		}
		return true;
	}

	void SetDefaultText(char* text, SDL_Color txt_color = { 255,255,255,255 }, _TTF_Font* txt_font = App->font->default) {
		default_text = text;
		font = txt_font;
		color = txt_color;
	}

	void ClearText() {
		text = "";
		cursor_position = 0;
	}

	void SetInputText(std::string txt){
		text = txt;
		cursor_position = txt.length();
	}

private:
	int cursor_position;
	std::string default_text;
	bool used = true;
};

#endif