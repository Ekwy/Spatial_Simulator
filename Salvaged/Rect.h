#ifndef SDLENGINE_RECT_H
#define SDLENGINE_RECT_H

#include "VisualComponent2D.h"

class Rect : public VisualComponent2D {
private:
	bool filled;
	SDL_Color color;

public:
	Rect(SDL_Rect rect, int priority, bool filled, SDL_Color color) : VisualComponent2D(priority), filled(filled), color(color) {
		dstRect = rect;
	}

	void draw(SDL_Renderer* renderer, SDL_Point shift, int scale) {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		if (filled)
			SDL_RenderFillRect(renderer, global ? &dstRect : toGlobal(dstRect, shift, scale));
		else
			SDL_RenderDrawRect(renderer, global ? &dstRect : toGlobal(dstRect, shift, scale));
	}

	virtual VisualComponentType getType() {
		return RECT;
	}
};

#endif //SDLENGINE_RECT_H