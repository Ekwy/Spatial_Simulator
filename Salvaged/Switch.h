#ifndef SDLENGINE_SWITCH_H
#define SDLENGINE_SWITCH_H

#include <SDL2/SDL_ttf.h>
#include <functional>
#include "Button.h"

class Switch : public Button {
private:
	//Indique si le bouton est pressé
	bool state;
	//Fonction définie manuellement, prenant en paramètre un VisualComponent2D*, étant appelée quand state = true
	std::function<void (VisualComponent2D*)> onUnClick;

public:
	Switch(std::string textureNameUnpressed, std::string textureNamePressed, int priority, bool state, SDL_Rect dstRect) : Button(textureNameUnpressed, textureNamePressed, priority, dstRect) {
		this->state = state;
	}

	Switch(std::string textureNameUnpressed, std::string textureNamePressed, int priority, SDL_Rect dstRect) : Button(textureNameUnpressed, textureNamePressed, priority, dstRect) {
		state = false;
	}

	Switch(std::string textureNameUnpressed, std::string textureNamePressed, int priority, bool state) : Button(textureNameUnpressed, textureNamePressed, priority) {
		this->state = state;
	}

	Switch(std::string textureNameUnpressed, std::string textureNamePressed, int priority) : Button(textureNameUnpressed, textureNamePressed, priority) {
		state = false;
	}

	//Permet de définir manuellement onUnClick
	void setOnUnClick(std::function<void (VisualComponent2D*)> onUnClick) {
		this->onUnClick = onUnClick;
	}

	void draw(SDL_Renderer* renderer, SDL_Point shift, int scale) {
        SDL_RenderCopy(renderer, (SDL_Texture*)SDLResourceManager::get(textureNames[(state ? 1 : 0)])->resource(), nullptr, global ? &dstRect : toGlobal(dstRect, shift, scale));
	}

    void notify(SDL_Event* event) {
        if (!hidden) {
			//Si le pointeur souris est dans dstRect
            if (SDL_PointInRect(new SDL_Point({event->motion.x, event->motion.y}), &dstRect) && event->type == SDL_MOUSEBUTTONUP) {
				//Assigne state à !state et return state
				if (state = !state) {
					if (onClick)
						onClick(this);
					if (onPress)
						onPress(this);
					currTextureName = 1;
				}else {
					if (onUnClick)
						onUnClick(this);
					if (onUnPress)
						onUnPress(this);
					currTextureName = 1;
				}
            }
        }
    }

	bool getState() {
		return state;
	}

	virtual VisualComponentType getType() {
        return SWITCH;
    }
};

#endif //SDLENGINE_SWITCH_H
