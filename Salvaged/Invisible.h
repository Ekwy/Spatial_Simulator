#ifndef SDLENGINE_INVISIBLE_H
#define SDLENGINE_INVISIBLE_H

#include "VisualComponent2D.h"

class Invisible : public VisualComponent2D {
public:
	Invisible(int x, int y, int w, int h) : VisualComponent2D(0) {
		dstRect = {x, y, w, h};
	}

	Invisible(SDL_Rect rect) : VisualComponent2D(0) {
		dstRect = rect;
	}

	void draw(SDL_Renderer* renderer, SDL_Point shift, int scale) {}

	virtual VisualComponentType getType() {
		return INVISIBLE;
	}
};

#endif //SDLENGINE_INVISIBLE_H
