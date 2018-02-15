#ifndef SDLENGINE_GUI_H
#define SDLENGINE_GUI_H

#include <map>
#include <functional>
#include "VisualComponent2D.h"
#include "Sprite.h"
#include "Label.h"
#include "TextInput.h"
#include "Button.h"
#include "Switch.h"

class GUI {
protected:
	std::map<unsigned int, std::map<std::string, VisualComponent2D*>> visualComponents;
	std::map<unsigned int, std::map<std::string, std::pair<int, int>>> originalPositions;

public:
	~GUI() {
		SDLRenderer* renderer = ((SDLRenderer*)SDLResourceManager::get(RENDERER_NAME));

		for (std::pair<unsigned int, std::map<std::string, VisualComponent2D*>> it : visualComponents)
			for (std::pair<std::string, VisualComponent2D*> jt : visualComponents[it.first]) {
				renderer->removeVisualComponent(jt.first);
				delete jt.second;
			}
	}

	void setOriginalPositions() {
		for (std::pair<unsigned int, std::map<std::string, VisualComponent2D*>> it : visualComponents)
			for (std::pair<std::string, VisualComponent2D*> jt : visualComponents[it.first])
				originalPositions[it.first][jt.first] = std::pair<int, int>(jt.second->getDstRect()->x, jt.second->getDstRect()->y);
	}

	void addVisCompsToRenderer() {
		SDLRenderer* renderer = ((SDLRenderer*)SDLResourceManager::get(RENDERER_NAME));

		for (std::pair<unsigned int, std::map<std::string, VisualComponent2D*>> it : visualComponents)
			for (std::pair<std::string, VisualComponent2D*> jt : visualComponents[it.first])
				renderer->addVisualComponent(jt.first, jt.second);
	}

	void addVisCompsToRenderer(unsigned int group) {
		SDLRenderer* renderer = ((SDLRenderer*)SDLResourceManager::get(RENDERER_NAME));

		for (std::pair<std::string, VisualComponent2D*> it : visualComponents[group])
			renderer->addVisualComponent(it.first, it.second);
	}

	void removeVisCompsFromRenderer() {
		SDLRenderer* renderer = ((SDLRenderer*)SDLResourceManager::get(RENDERER_NAME));

		for (std::pair<unsigned int, std::map<std::string, VisualComponent2D*>> it : visualComponents)
			for (std::pair<std::string, VisualComponent2D*> jt : visualComponents[it.first])
				renderer->removeVisualComponent(jt.first);
	}

	void removeVisCompsFromRenderer(unsigned int group) {
		SDLRenderer* renderer = ((SDLRenderer*)SDLResourceManager::get(RENDERER_NAME));

		for (std::pair<std::string, VisualComponent2D*> it : visualComponents[group])
			renderer->removeVisualComponent(it.first);
	}

	//Cache le groupe et l'arrête de détecter les events
	void hideGroup(unsigned int group) {
		removeVisCompsFromRenderer(group);
		for (std::pair<std::string, VisualComponent2D*> it : visualComponents[group])
			it.second->hidden = true;
	}

	//Cache le groupe et lui permet de détecter les events
	void showGroup(unsigned int group) {
		addVisCompsToRenderer(group);
		for (std::pair<std::string, VisualComponent2D*> it : visualComponents[group])
			it.second->hidden = false;
	}

	void applyToAll(std::function<void (VisualComponent2D*)> func) {
		for (std::pair<unsigned int, std::map<std::string, VisualComponent2D*>> it : visualComponents)
			for (std::pair<std::string, VisualComponent2D*> jt : visualComponents[it.first])
				func(jt.second);
	}

	void applyToGroup(unsigned int group, std::function<void (VisualComponent2D*)> func) {
		for (std::pair<std::string, VisualComponent2D*> it : visualComponents[group])
			func(it.second);
	}

	VisualComponent2D* getVisualComponent(std::string name) {
		for (std::pair<unsigned int, std::map<std::string, VisualComponent2D*>> it : visualComponents)
			if (it.second.find(name) != it.second.end())
				return it.second[name];
	}

	VisualComponent2D* getVisualComponent(unsigned int group, std::string name) {
		return visualComponents[group][name];
	}
};

#endif //SDLENGINE_GUI_H
