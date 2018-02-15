#ifndef VISUALCOMPONENT2D_H
#define VISUALCOMPONENT2D_H

#include <map>
#include <functional>
#include "VisualComponent.h"
#include "SDLEngine.h"

class VisualComponent2D : public VisualComponent, public Observer<SDL_Event*> {
protected:
	bool mouseIsOver;
	std::function<void (VisualComponent2D*)> onClick;
	std::function<void (VisualComponent2D*)> onMouseOver;
	std::function<void (VisualComponent2D*)> onMouseAway;

public:
	std::map<int, std::string> textureNames;
	int currTextureName;
	SDL_Rect dstRect;
	int priority;
	bool hidden;
	bool global;

	VisualComponent2D(int priority)  {
		this->priority = priority;

		mouseIsOver = hidden = dstRect.x = dstRect.y = dstRect.w = dstRect.h = global = currTextureName = 0;
	}

	VisualComponent2D(int priority, int argc, ...) : VisualComponent2D(priority) {
		va_list argv;
		va_start(argv, argc);
		for (unsigned int i = 0; i < argc; ++i)
			textureNames[i] = va_arg(argv, std::string);
		va_end(argv);
	}

	VisualComponent2D(int priority, std::string textureName) : VisualComponent2D(priority) {
		textureNames[0] = textureName;
	}

	virtual ~VisualComponent2D() {
		if (onClick)
			SDLEngine::unregisterEventObserver(SDL_MOUSEBUTTONUP, this);
		if (onMouseOver || onMouseAway)
			SDLEngine::unregisterEventObserver(SDL_MOUSEMOTION, this);
	}

	SDL_Rect* getDstRect() {
		return &dstRect;
	}

	void setOnClick(std::function<void (VisualComponent2D*)> onClick) {
		SDLEngine::registerEventObserver(SDL_MOUSEBUTTONUP, this);
		this->onClick = onClick;
	}

	std::function<void (VisualComponent2D*)> getOnClick() {
		return onClick;
	}

	void setOnMouseOver(std::function<void (VisualComponent2D*)> onMouseOver) {
		SDLEngine::registerEventObserver(SDL_MOUSEMOTION, this);
		this->onMouseOver = onMouseOver;
	}

	std::function<void (VisualComponent2D*)> getOnMouseOver() {
		return onMouseOver;
	}

	void setOnMouseAway(std::function<void (VisualComponent2D*)> onMouseAway) {
		SDLEngine::registerEventObserver(SDL_MOUSEMOTION, this);
		this->onMouseAway = onMouseAway;
	}

	std::function<void (VisualComponent2D*)> getOnMouseAway() {
		return onMouseAway;
	}

	void setPos(int x, int y) {
		dstRect.x = x;
		dstRect.y = y;
	}

	void moveBy(int dx, int dy) {
		dstRect.x += dx;
		dstRect.y += dy;
	}

	void setSize(int width, int height) {
		dstRect.w = width;
		dstRect.h = height;
	}

	void increaseScaleBy(int dw, int dh) {
		dstRect.w += dw;
		dstRect.h += dh;
	}

	void scaleBy(int w, int h) {
		dstRect.w *= w;
		dstRect.h *= h;
	}

	virtual void setStr(std::string str) {

	}

	virtual std::string getStr() {

	}

	virtual void draw(SDL_Renderer* renderer, SDL_Point shift, int scale) = 0;

	virtual void notify(SDL_Event* event) {
		if (!hidden) {
			if (SDL_PointInRect(new SDL_Point({event->motion.x, event->motion.y}), &dstRect)) {
				switch (event->type) {
					case SDL_MOUSEBUTTONUP:
						if (onClick)
							onClick(this);
						break;
					case SDL_MOUSEMOTION:
						if (onMouseOver && !mouseIsOver)
							onMouseOver(this);
						mouseIsOver = true;
						break;
					default:;
				}
			}else if (event->type == SDL_MOUSEMOTION){
				if (mouseIsOver && onMouseAway)
					onMouseAway(this);
				mouseIsOver = false;
			}
		}
	}
};

#endif
