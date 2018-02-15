#ifndef IMAGE_H
#define IMAGE_H

#include "SDLTexture.h"
#include "VisualComponent2D.h"
#include "Sprite.h"

#ifdef _WIN32
#include <SDL2/SDL_image.h>

#endif

#ifdef __APPLE__
#include <SDL2_image/SDL_image.h>
#endif

struct TextureInfo {
	SDL_Rect srcRect;
	SDL_Point center;
	double angle; // en degré
	SDL_RendererFlip flip;
};

class Image : public VisualComponent2D {
public:
	//Information associée à chaque textures de textureNames
	std::map<int, TextureInfo> textureInfos;

	Image(std::string textureName, int priority, int x, int y) : VisualComponent2D(priority, textureName) {
		setPos(x, y);

		SDL_QueryTexture(static_cast<SDL_Texture*>(SDLResourceManager::get(textureNames[currTextureName])->resource()), nullptr, nullptr, &dstRect.w, &dstRect.h);
		textureInfos[currTextureName] = {{0, 0, dstRect.w, dstRect.h}, {dstRect.w/2, dstRect.h/2}, 0, SDL_FLIP_NONE};
	}

	Image(std::string textureName, int priority) : Image(textureName, priority, 0, 0) {}

	Image(std::string textureName, int priority, SDL_Rect dstRect) : VisualComponent2D(priority, textureName) {
		this->dstRect = dstRect;

		textureInfos[currTextureName] = {{0, 0, 0, 0}, {dstRect.w/2, dstRect.h/2}, 0, SDL_FLIP_NONE};
		SDL_QueryTexture(static_cast<SDL_Texture*>(SDLResourceManager::get(textureNames[currTextureName])->resource()), nullptr, nullptr, &textureInfos[currTextureName].srcRect.w, &textureInfos[currTextureName].srcRect.h);
	}

	int getGlobalCenterX() {
		return textureInfos[currTextureName].center.x + dstRect.x;
	}

	int getGlobalCenterY() {
		return textureInfos[currTextureName].center.y + dstRect.y;
	}

	void setSrcRectY(int y){
		textureInfos[currTextureName].srcRect.y = y;
	}

	void setSrcRectX(int x){
		textureInfos[currTextureName].srcRect.x = x;
	}

	void addTexture(int key, std::string textureName, TextureInfo textureInfo) {
		textureNames[key] = textureName;
		textureInfos[key] = textureInfo;
	}

	void addTexture(int key, std::string textureName) {
		addTexture(key, textureName, {{0, 0, dstRect.w, dstRect.h}, {dstRect.w/2, dstRect.h/2}, 0, SDL_FLIP_NONE});
	}

	TextureInfo getCurrentTextureInfo() {
		return textureInfos[currTextureName];
	}

	void setSrcRect(SDL_Rect srcRect) {
		textureInfos[currTextureName].srcRect = srcRect;
	}

	void setFlip(SDL_RendererFlip flip) {
		textureInfos[currTextureName].flip = flip;
	}

	void setAllFlip(SDL_RendererFlip flip) {
		for (std::pair<const int, TextureInfo>& it : textureInfos)
			it.second.flip = flip;
	}

	void setAngle(double angle) {
		textureInfos[currTextureName].angle = angle;
	}

	void setAllAngles(double angle) {
		for (std::pair<const int, TextureInfo>& it : textureInfos)
			it.second.angle = angle;
	}

	void draw(SDL_Renderer* renderer, SDL_Point shift, int scale) {
		SDL_RenderCopyEx(renderer, (SDL_Texture *) ((SDLTexture *) SDLResourceManager::get(textureNames[currTextureName]))->resource(), &textureInfos[currTextureName].srcRect, global ? &dstRect : toGlobal(dstRect, shift, scale), textureInfos[currTextureName].angle, &textureInfos[currTextureName].center, textureInfos[currTextureName].flip);
	}

	virtual VisualComponentType getType() {
		return IMAGE;
	}
};

#endif
