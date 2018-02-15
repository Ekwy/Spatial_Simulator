#ifndef SDLENGINE_BUTTON_H
#define SDLENGINE_BUTTON_H

#include "VisualComponent2D.h"
#include "SDLTexture.h"

class Button : public VisualComponent2D {
protected:
	//Fonction à appeler quand le bouton est appuyé/désappuyé, principalement pour bouger des visComps
	std::function<void (VisualComponent2D*)> onPress;
	std::function<void (VisualComponent2D*)> onUnPress;

public:
	Button(std::string textureNameUnpressed, std::string textureNamePressed, int priority, SDL_Rect dstRect) : VisualComponent2D(priority, 2, textureNameUnpressed, textureNamePressed) {
		this->dstRect = dstRect;

		SDLEngine::registerEventObserver(SDL_MOUSEBUTTONDOWN, this);
		SDLEngine::registerEventObserver(SDL_MOUSEBUTTONUP, this);
	}

	Button(std::string textureNameUnpressed, std::string textureNamePressed, int priority, int x, int y) : VisualComponent2D(priority, 2, textureNameUnpressed, textureNamePressed) {
		dstRect.x = x;
		dstRect.y = y;
		SDL_QueryTexture((SDL_Texture*) SDLResourceManager::get(textureNames[0])->resource(), nullptr, nullptr, &dstRect.w, &dstRect.h);

		SDLEngine::registerEventObserver(SDL_MOUSEBUTTONDOWN, this);
		SDLEngine::registerEventObserver(SDL_MOUSEBUTTONUP, this);
	}

	Button(std::string textureNameUnpressed, std::string textureNamePressed, int priority) : Button(textureNameUnpressed, textureNamePressed, priority, 0, 0) {}

	~Button() {
		SDLEngine::unregisterEventObserver(SDL_MOUSEBUTTONDOWN, this);
		SDLEngine::unregisterEventObserver(SDL_MOUSEBUTTONUP, this);
	}

	void setOnPress(std::function<void (VisualComponent2D*)> onPress) {
		this->onPress = onPress;
	}

	std::function<void (VisualComponent2D*)> getOnPress() {
		return onPress;
	}

	void setOnUnPress(std::function<void (VisualComponent2D*)> onUnPress) {
		this->onUnPress = onUnPress;
	}

	std::function<void (VisualComponent2D*)> getUnOnPress() {
		return onUnPress;
	}

	void draw(SDL_Renderer* renderer, SDL_Point shift, int scale) {
		SDL_RenderCopy(renderer, (SDL_Texture*)SDLResourceManager::get(textureNames[currTextureName])->resource(), nullptr, global ? &dstRect : toGlobal(dstRect, shift, scale));
	}

	virtual void notify(SDL_Event* event) {
		if (!hidden) {
			if (SDL_PointInRect(new SDL_Point({event->motion.x, event->motion.y}), &dstRect)) {
				switch (event->type) {
					case SDL_MOUSEBUTTONUP:
						if (currTextureName) {
							if (onClick)
								onClick(this);
							if (onUnPress)
								onUnPress(this);
							currTextureName = 0;
						}
						break;
					case SDL_MOUSEMOTION:
						if (onMouseOver && !mouseIsOver)
							onMouseOver(this);
						mouseIsOver = true;
						break;
					case SDL_MOUSEBUTTONDOWN:
						if (onPress)
							onPress(this);
						currTextureName = 1;
						break;
					default:;

				}
			}else {
				switch (event->type) {
					case SDL_MOUSEBUTTONUP:
						if (currTextureName) {
							if (onUnPress)
								onUnPress(this);
							currTextureName = 0;
						}
						break;
				}
				if (mouseIsOver && onMouseAway)
					onMouseAway(this);
				mouseIsOver = false;
			}
		}
	}

	virtual VisualComponentType getType() {
		return BUTTON;
	}
};

#endif //SDLENGINE_BUTTON_H
