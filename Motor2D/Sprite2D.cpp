#include "Sprite2D.h"
#include "p2Log.h"


Sprite2D::Sprite2D()
{
	pos.x = 0; pos.y = 0;
}

Sprite2D::Sprite2D(iPoint pos) : pos(pos)
{
}

Sprite2D::~Sprite2D()
{
	for (p2List_item<Animation*>* anim = anims.start; anim; anim = anim->next) {
		RELEASE(anim->data);
	}
	anims.clear();
}

// Load image at path as the texture for this sprite. Return false on fail.
bool Sprite2D::LoadTexture(const char * path)
{
	bool ret = true;

	tex = App->tex->Load(path);
	if (tex == nullptr) {
		ret = false;
		LOG("Error loading image: %s", path);
	}

	return ret;
}

//Set texture as the texture for this sprite
void Sprite2D::SetTexture(SDL_Texture * texture)
{
	if (texture != nullptr)
		tex = texture;
}

void Sprite2D::LoadAnimations(pugi::xml_node & node)
{
	for (pugi::xml_node anim = node.child("anim"); anim != NULL; anim = anim.next_sibling("anim")) {
		p2List<SDL_Rect> anim_rects;
		float speed = anim.attribute("speed").as_float(1.0f);
		int type = anim.attribute("type").as_int(-1);
		bool loop = anim.attribute("loop").as_bool(true);
		for (pugi::xml_node frame = anim.child("frame"); frame != NULL; frame = frame.next_sibling("frame")) {
			SDL_Rect new_frame = { frame.attribute("x").as_int(0),frame.attribute("y").as_int(0),frame.attribute("w").as_int(0),frame.attribute("h").as_int(0) };
			anim_rects.add(new_frame);
		}
		if (type != -1)
			anims.add(new Animation(anim_rects, speed, static_cast<AnimTypes>(type), loop));
	}
}

void Sprite2D::SetAnimation(AnimTypes anim)
{
	for (int i = 0; i < anims.count(); i++) {
		if (anims[i]->type == anim) {
			current_anim = i;
			break;
		}
	}
}

// Change Sprite2D position
void Sprite2D::SetPos(iPoint position)
{
	pos = position;
}

// Return the texture stored on this Sprite2D
SDL_Texture * Sprite2D::GetTexture() const
{
	return tex;
}

// Return Sprite2D position
iPoint Sprite2D::GetPos() const
{
	return pos;
}


// Return a pointer to current animation
Animation * Sprite2D::GetAnim() const
{
	return anims[current_anim];
}