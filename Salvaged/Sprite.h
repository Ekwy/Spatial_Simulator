#ifndef SDLENGINE_SPRITE_H
#define SDLENGINE_SPRITE_H

#include <iostream>
#include "Image.h"
#include "SDLResourceManager.h"

struct TextureSpriteInfo {
	unsigned int totalFrames;
	unsigned int currFrame;
	unsigned int delay;
	unsigned int untilNext;
	bool playing;
	bool looping;
};

class Sprite : public Image {
private:
	void nextFrame() {
			if ((textureSpriteInfos[currTextureName].looping && (textureSpriteInfos[currTextureName].currFrame) == (textureSpriteInfos[currTextureName].totalFrames - 1))) {
				textureInfos[currTextureName].srcRect.x -= textureInfos[currTextureName].srcRect.w * (textureSpriteInfos[currTextureName].totalFrames - 1);
				textureSpriteInfos[currTextureName].currFrame = 0;
			}else if (textureSpriteInfos[currTextureName].currFrame != textureSpriteInfos[currTextureName].totalFrames - 1) {
				textureInfos[currTextureName].srcRect.x += textureInfos[currTextureName].srcRect.w;
				textureSpriteInfos[currTextureName].currFrame++;
			}

		}

public:
	//Information associée à chaque textures de textureNames
	std::map<int, TextureSpriteInfo> textureSpriteInfos;

	Sprite(std::string textureName, unsigned int totalFrames, int priority, int x, int y, unsigned int delay, unsigned int startFrame, bool playing, bool looping, SDL_Rect dstRect) : Image(textureName, priority, dstRect) {
		textureSpriteInfos[currTextureName] = {totalFrames, startFrame, delay, delay, playing, looping};

		dstRect.w = (textureInfos[currTextureName].srcRect.w /= totalFrames);
		dstRect.h = textureInfos[currTextureName].srcRect.h;

		textureInfos[currTextureName].srcRect.x += dstRect.w * startFrame;
	}

	//Constructeur pour ajouter les textures manuellement
	Sprite(std::string textureName, int priority, int x, int y, SDL_Rect dstRect) : Image(textureName, priority, dstRect) {}

	virtual VisualComponentType getType() {
		return SPRITE;
	}

	void draw(SDL_Renderer* renderer, SDL_Point shift, int scale) {
		SDL_RenderCopyEx(renderer, (SDL_Texture*)((SDLTexture*)SDLResourceManager::get(textureNames[currTextureName]))->resource(), &textureInfos[currTextureName].srcRect, global ? &dstRect : toGlobal(dstRect, shift, scale), textureInfos[currTextureName].angle, &textureInfos[currTextureName].center, textureInfos[currTextureName].flip);

		if (textureSpriteInfos[currTextureName].playing) {
			if (!--textureSpriteInfos[currTextureName].untilNext) {
				nextFrame();
				textureSpriteInfos[currTextureName].untilNext = textureSpriteInfos[currTextureName].delay;
			}
		}
	}

	void addTexture(int key, std::string textureName, TextureInfo textureInfo, TextureSpriteInfo textureSpriteInfo) {
		textureNames[key] = textureName;
		textureInfos[key] = textureInfo;
		textureSpriteInfos[key] = textureSpriteInfo;
	}

	TextureSpriteInfo getCurrentTextureSpriteInfo() {
		return textureSpriteInfos[currTextureName];
	}

	void setPlaying(bool playing) {
		textureSpriteInfos[currTextureName].playing = playing;
	}

	void setLooping(bool looping) {
		textureSpriteInfos[currTextureName].looping = looping;
	}

	void setCurrentFrame(unsigned int frame){
		textureSpriteInfos[currTextureName].currFrame = frame;
		textureInfos[currTextureName].srcRect.x = frame * textureInfos[currTextureName].srcRect.w;
	}
};
#endif //SDLENGINE_SPRITE_H
