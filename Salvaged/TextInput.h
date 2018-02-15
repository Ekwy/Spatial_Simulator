#ifndef SDLENGINE_TEXTINPUT_H
#define SDLENGINE_TEXTINPUT_H

#include <string>
#include "Label.h"

class TextInput : public Label {
private:
	bool focused;
	SDL_Rect* customRect;
	std::function<void (VisualComponent2D*)> onFocus;
	std::function<void (VisualComponent2D*)> onUnfocus;
	std::function<void (VisualComponent2D*)> onEdit;

public:
	TextInput(std::string str, std::string fontName, int priority, SDL_Color color, bool blended) : Label(str, fontName, priority, color, blended) {
		focused = false; //savoir si on écrit ou pas

		SDLEngine::registerEventObserver(SDL_MOUSEBUTTONUP, this);
		SDLEngine::registerEventObserver(SDL_TEXTINPUT, this);
		SDLEngine::registerEventObserver(SDL_KEYUP, this);
	}

	TextInput(std::string str, std::string fontName, int priority, SDL_Color color) : TextInput(str, fontName, priority, color, false) {}

	~TextInput() {
		SDLEngine::unregisterEventObserver(SDL_MOUSEBUTTONUP, this);
		SDLEngine::unregisterEventObserver(SDL_TEXTINPUT, this);
		SDLEngine::unregisterEventObserver(SDL_KEYUP, this);
	}

	void setCustomRect(SDL_Rect* customRect) {
		this->customRect = customRect;
	}

	void setOnFocus(std::function<void (VisualComponent2D*)> onFocus) {
		this->onFocus = onFocus;
	}

	std::function<void (VisualComponent2D*)> getOnFocus() {
		return onFocus;
	}

	void setOnUnfocus(std::function<void (VisualComponent2D*)> onUnfocus) {
		this->onUnfocus = onUnfocus;
	}

	std::function<void (VisualComponent2D*)> getOnUnfocus() {
		return onUnfocus;
	}

	void setOnEdit(std::function<void (VisualComponent2D*)> onEdit) {
		this->onEdit = onEdit;
	}

	std::function<void (VisualComponent2D*)> getOnEdit() {
		return onEdit;
	}

	void notify(SDL_Event* event) {
		//si curseur a l'interieur du rect
		switch (event->type) {
			case SDL_TEXTINPUT:
				if (focused) {
					str += event->text.text;
					if (onEdit)
						onEdit(this);
					updateTexture();
				}
				break;
			case SDL_KEYUP:
				if (event->key.keysym.sym == SDLK_BACKSPACE && focused) {
					str = str.substr(0, str.size() - 1);
					if (onEdit)
						onEdit(this);
					updateTexture();
				}
				break;
			default:;
		}
		if (SDL_PointInRect(new SDL_Point({event->motion.x, event->motion.y}), (customRect ? customRect : &dstRect))) {
			switch (event->type) {
				case SDL_MOUSEBUTTONUP:
					if (onClick)
						onClick(this);
					if (onFocus)
						onFocus(this);
					focused = true;
					SDL_StartTextInput();
					break;
				case SDL_MOUSEMOTION:
					if (onMouseOver && !mouseIsOver)
						onMouseOver(this);
					mouseIsOver = true;
				default:;
			}
		}else {
			switch (event->type) {
				case SDL_MOUSEBUTTONUP:
					if (onUnfocus)
						onUnfocus;
					focused = false;
					break;
				case SDL_MOUSEMOTION:
					if (mouseIsOver && onMouseAway)
						onMouseAway(this);
					mouseIsOver = false;
				default:;
			}
		}
	}

	virtual VisualComponentType getType() {
		return TEXT_INPUT;
	}
};
#endif //SDLENGINE_TEXTINPUT_H
