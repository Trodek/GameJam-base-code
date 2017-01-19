#ifndef _SPRITE2D_H_
#define _SPRITE2D_H_

#include "p2List.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct SDL_Rect;
class PhysBody;

class Sprite2D {
public:
	Sprite2D();
	Sprite2D(iPoint pos);

	virtual ~Sprite2D();

	bool LoadTexture(const char* path);
	void LoadAnimations(pugi::xml_node& node);

	void SetTexture(SDL_Texture* texture);
	void SetAnimation(AnimTypes anim);
	void SetPos(iPoint position);

	SDL_Texture* GetTexture() const;
	iPoint GetPos() const;
	Animation* GetAnim() const;

private:


public:
	
private:
	p2List<Animation*>	anims;
	int					current_anim = -1;
	SDL_Texture*		tex = nullptr;
	iPoint				pos;
	PhysBody*			physbody = nullptr;
	

};


#endif