#ifndef SDLENGINE_LABEL_H
#define SDLENGINE_LABEL_H

#include <SDL2/SDL_ttf.h>
#include "VisualComponent2D.h"
#include "SDLResourceManager.h"

class Label : public VisualComponent2D {
protected:
	std::string str;
	SDL_Texture* texture;
	std::string fontName;
	SDL_Color color;
	bool blended;

public:
	Label(std::string str, std::string fontName, int priority, SDL_Color color, bool blended) : VisualComponent2D(priority) {
		this->str = str;
		this->fontName = fontName;
		this->color = color;
		this->blended = blended;

		updateTexture();
	}

	Label(std::string str, std::string fontName, int priority, SDL_Color color) : Label(str, fontName, priority, color, false) {}

	void draw(SDL_Renderer* renderer, SDL_Point shift, int scale) {
		SDL_RenderCopy(renderer, texture, nullptr, global ? &dstRect : toGlobal(dstRect, shift, scale));
	}

	void setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;

		SDL_DestroyTexture(texture);
		updateTexture();
	}

	std::string getStr() {
		return str;
	}

	void setStr(std::string str) {
		this->str = str;
		updateTexture();
	}

	//Retourne la largeur prise par le texte
	int getTextWidth() {
		int w;
		SDL_QueryTexture(texture, nullptr, nullptr, &w, nullptr);
		return w;
	}

	//Retourne la largeur prise par la string donnée
	int getTextWidth(std::string str) {
		int w;
		TTF_SizeText((TTF_Font*)SDLResourceManager::get(fontName)->resource(), str.c_str(), &w, nullptr);
		return w;
	}

	//Retourne la hauteur prise par le texte
	int getTextHeight() {
		int h;
		SDL_QueryTexture(texture, nullptr, nullptr, nullptr, &h);
		return h;
	}

	//Retourne la hauteur prise par la string donnée
	int getTextHeight(std::string str) {
		int h;
		TTF_SizeText((TTF_Font*)SDLResourceManager::get(fontName)->resource(), str.c_str(), nullptr, &h);
		return h;
	}

	void updateTexture() {
		SDL_Surface* surface;
		if (blended)
			surface = TTF_RenderUTF8_Blended((TTF_Font*)SDLResourceManager::get(fontName)->resource(), str.c_str(), color);
		else
			surface = TTF_RenderUTF8_Solid((TTF_Font*)SDLResourceManager::get(fontName)->resource(), str.c_str(), color);
		texture = SDL_CreateTextureFromSurface((SDL_Renderer*)SDLResourceManager::get(RENDERER_NAME)->resource(), surface);
		SDL_QueryTexture(texture, nullptr, nullptr, &dstRect.w, &dstRect.h);
		SDL_FreeSurface(surface);
	}

	virtual VisualComponentType getType() {
		return LABEL;
	}
};
#endif //SDLENGINE_LABEL_H
