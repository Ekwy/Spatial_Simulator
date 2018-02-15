#ifndef SOURCE_KEYLIST_H
#define SOURCE_KEYLIST_H

#include <functional>
#include <list>
#include "Panel.h"
#include "Piece.h"
#include "Rect.h"
#include "Button.h"
#include "Label.h"
#include "Observer.h"
#include "GameData.h"

/// \brief Panel contenant les touches utilisées et les fonctions attachées

class KeyList : public Panel, public Observer<SDL_Event*> {
private:
	Panel* dragged;	///< Pointeur vers le Panel tenue par le curseur
	Panel* functionToRemove;	///< Pointeur vers le FunctionPanel à retirer
	Panel* keyToRemove;	///< Pointeur vers le KeyPanel à retirer
	std::pair<int, int> oldPos;	///< Ancienne position du Panel tenue par le curseur

	/// \brief Panel représentant une fonction
	struct FunctionPanel : public Panel {
		KeyList& keyList;	///< Référence vers KeyList
		const std::string name;	///< Nom de la fonction
		Button background;	///< Fond et bouton du FunctionPanel
		Label label;	///< Label du FunctionPanel

		/// \param numFunctionPanel Nombre de FunctionPanel avant celui-ci
		FunctionPanel(const std::string& name, const unsigned& numFunctionPanel, KeyList& keyList)
				: Panel(0, List::INDENT, (numFunctionPanel + 1) * (List::NODE_HEIGHT + List::SPACE_BETWEEN)),
				  name(name),
				  background(SDLGLContext::loadTexture("Textures2D/violet.png"), SDLGLContext::loadTexture("Textures2D/violetPale.png"), 0, 0, 0, List::WIDTH - 2 * List::MARGIN - List::INDENT, List::NODE_HEIGHT),
				  label(1, 0, 0, name, "Fonts/OpenSans/OpenSans-Regular.ttf", 24, {0, 0, 0, 255}),
				  keyList(keyList) {
			addComponent("background", &background);
			addComponent("label", &label);

			background.onPress = [this, &keyList](VisualComponent2D& visComp) {
				EventDispatcher::registerEventObserver(SDL_MOUSEMOTION, &keyList);
				keyList.oldPos.first = this->pos.x;
				keyList.oldPos.second = this->pos.y;
				keyList.dragged = this;
			};

			background.onUnPress = [this, &keyList](VisualComponent2D& visComp) {
				EventDispatcher::unregisterEventObserver(SDL_MOUSEMOTION, &keyList);

				this->pos.setLocalPos(keyList.oldPos.first, keyList.oldPos.second);
				keyList.dragged = nullptr;

				if (!keyList.pos.containsPoint(List::WIDTH, keyList.background.h, EventDispatcher::sdlEvent.button.x, EventDispatcher::sdlEvent.button.y))
					keyList.functionToRemove = this;
			};
		}
	};

	/// \brief Panel représentant une touche
	struct KeyPanel : public Panel {
		KeyList& keyList;	///< Référence vers KeyList
		const std::string name;	///< Nom du KeyPanel, différent du nom de la touche
		const unsigned scancode;	///< Scancode de la touche
		Button background;	///< Fond et bouton du KeyPanel
		Label label;	///< Label du KeyPanel
		std::list<FunctionPanel> funcs;	///< Liste de FunctionPanel

		/// \param numKeyPanel Nombre de KeyPanel avant celui-ci
		KeyPanel(const std::string& name, const unsigned& scancode, const unsigned& numKeyPanel, KeyList& keyList)
				: Panel(0, List::MARGIN, List::MARGIN + numKeyPanel * (List::NODE_HEIGHT + List::SPACE_BETWEEN)),
				  name(name),
				  scancode(scancode),
				  background(SDLGLContext::loadTexture("Textures2D/blue.png"), SDLGLContext::loadTexture("Textures2D/bluePale.png"), 0, 0, 0, List::WIDTH - 2 * List::MARGIN, List::NODE_HEIGHT),
				  label(1, 0, 0, getKeyName(scancode), "Fonts/OpenSans/OpenSans-Regular.ttf", 24, {0, 0, 0, 255}),
				  keyList(keyList) {
			addComponent("background", &background);
			addComponent("label", &label);

			background.onPress = [this, &keyList](VisualComponent2D& visComp) {
				EventDispatcher::registerEventObserver(SDL_MOUSEMOTION, &keyList);
				keyList.oldPos.first = this->pos.x;
				keyList.oldPos.second = this->pos.y;
				keyList.dragged = this;
			};

			background.onUnPress = [this, &keyList](VisualComponent2D& visComp) {
				EventDispatcher::unregisterEventObserver(SDL_MOUSEMOTION, &keyList);

				this->pos.setLocalPos(keyList.oldPos.first, keyList.oldPos.second);
				keyList.dragged = nullptr;

				if (!keyList.pos.containsPoint(List::WIDTH, keyList.background.h, EventDispatcher::sdlEvent.button.x, EventDispatcher::sdlEvent.button.y))
					keyList.keyToRemove = this;
			};
		}

		/// \brief Ajoute un FunctionPanel au nom name
		void addFunction(const std::string& name) {
			funcs.emplace_back(name, funcs.size(), keyList);
			addPanel(name, &funcs.back());
		}

		/// \brief Déplace les FunctionPanel suivant it de 1 vers le haut
		void moveElements(std::list<FunctionPanel>::iterator it) {
			it++;
			if (it != funcs.end()) {
				int offset = List::NODE_HEIGHT + List::SPACE_BETWEEN;
				keyList.background.h -= offset;
				keyList.addKeyPanel.pos.y -= offset;

				for (; it != funcs.end(); it++)
					it->pos.y -= offset;
			}
		}
	};

	Rect background;	///< Fond de la liste
	static const std::list<std::pair<std::string, unsigned int>> keys;    ///< Liste de std::pair de noms et de scancodes pour chaque touches
	std::list<KeyPanel> keyPanels;	///< Liste de KeyPanel
	bool registered;	///< Vrai si une touche a été appuyée et KeyList doit être désinscrit de SDL_KEYDOWN
	Panel addKeyPanel;	///< Panel contenant le bouton addKey
	Button addKeyButton;	///< Bouton de addKey
	Label addKeyLabel;	///< Label de addKey

public:
	std::map<unsigned int, std::map<std::string, std::function<void(Piece&)>*>>* keyFunctions;	///< Pointeur vers Cockpit::keyFunctions

	/// \param priority Valeur initiale de priority.
	/// \param x Valeur initiale de pos.x
	/// \param y Valeur initiale de pos.y
	/// \param rememberFunctions Vrai les touches et fonctions de PlayerData::commands doivent être ajoutées
	KeyList(const int& priority, const int& x, const int& y, const bool& rememberFunctions)
			: Panel(priority, x, y),
			  background(0, 0, 0, List::WIDTH, 2 * (List::MARGIN + List::NODE_HEIGHT) + List::SPACE_BETWEEN, false, Vector3d(.5, .5, .5), false),
			  registered(false),
			  dragged(nullptr),
			  functionToRemove(nullptr),
			  keyToRemove(nullptr),
			  addKeyPanel(1, List::MARGIN, background.h - List::MARGIN - List::NODE_HEIGHT),
			  addKeyButton(SDLGLContext::loadTexture("Textures2D/green.png"), SDLGLContext::loadTexture("Textures2D/greenPale.png"), 0, 0, 0, List::WIDTH - 2 * List::MARGIN, List::NODE_HEIGHT),
			  addKeyLabel(1, 0, 0, "Add key", "Fonts/OpenSans/OpenSans-Regular.ttf", 24, {0, 0, 0, 255}) {
		addComponent("background", &background);

		addPanel("addKey", &addKeyPanel);
		panels["addKey"]->addComponent("background", &addKeyButton);
		panels["addKey"]->addComponent("label", &addKeyLabel);

		panels["addKey"]->getComponent("background")->onClick = [this](VisualComponent2D& visComp) {
			EventDispatcher::registerEventObserver(SDL_KEYDOWN, this);
			this->addKeyLabel.setStr("Press a key");
		};

		if (rememberFunctions)
			for (const std::pair<unsigned, std::vector<std::string>>& funcs : PlayerData::commands) {
				std::string keyName(getKeyName(funcs.first));
				keyPanels.emplace_back(std::to_string(keyPanels.size()) + keyName, funcs.first, getNodeNum(), *this);
				addPanel(keyPanels.back().name, &keyPanels.back());
			}
	}

	~KeyList() {
		PlayerData::commands.clear();

		for (const KeyPanel& keyPanel : keyPanels) {
			if (PlayerData::commands.find(keyPanel.scancode) == PlayerData::commands.end())
				PlayerData::commands[keyPanel.scancode].reserve(keyPanel.funcs.size());

			for (const FunctionPanel& functionPanel : keyPanel.funcs)
				PlayerData::commands.at(keyPanel.scancode).push_back(functionPanel.name);
		}
	}

	/// \brief Mets à jour la KeyList
	void update() {
		if (registered) {
			EventDispatcher::unregisterEventObserver(SDL_KEYDOWN, this);
			registered = false;
		}

		if (functionToRemove) {
			bool found = false;
			for (std::list<KeyPanel>::iterator it = keyPanels.begin(); it != keyPanels.end() && !found; it++)
				for (std::list<FunctionPanel>::iterator jt = it->funcs.begin(); jt != it->funcs.end() && !found; jt++)
					if (&(*jt) == functionToRemove) {
						it->moveElements(jt);
						moveElements(it, true);
						it->removePanel(jt->name);
						(*keyFunctions)[it->scancode].erase(jt->name);
						it->funcs.erase(jt);
						functionToRemove = nullptr;
						found = true;
					}
		}else if (keyToRemove) {
			bool found = false;
			for (std::list<KeyPanel>::iterator it = keyPanels.begin(); it != keyPanels.end() && !found; it++)
				if (&(*(it)) == keyToRemove) {
					moveElements(it, true, 1 + (unsigned) it->funcs.size());
					panels.erase(it->name);
					keyPanels.erase(it);
					keyToRemove = nullptr;
					found = true;
				}
		}
	}

	/// \brief Ajoute une fonction à une touche selon la position du curseur
	/// \param name Nom de la fonction
	/// \param func Pointeur vers la fonction
	void addFunction(const std::string& name, std::function<void(Piece&)>* func) {
		bool found(false);
		for (std::list<KeyPanel>::iterator it = keyPanels.begin(); it != keyPanels.end() && !found; it++)
			if (it->background.pos.containsPoint(it->background.w, it->background.h, EventDispatcher::sdlEvent.button.x, EventDispatcher::sdlEvent.button.y))
				if ((*keyFunctions)[it->scancode].find(name) == (*keyFunctions)[it->scancode].end()) {
					(*keyFunctions)[it->scancode].emplace(name, func);
					it->addFunction(name);

					moveElements(it);
					found = true;
				}
	}

	/// \brief Ajoute une fonction à une touche selon la position du curseur
	/// \param name Nom de la fonction
	/// \param func Pointeur vers la fonction
	void addFunction(const std::string& name, std::function<void(Piece&)>* func, const unsigned& scancode) {
		bool found(false);
		for (std::list<KeyPanel>::iterator it = keyPanels.begin(); it != keyPanels.end() && !found; it++)
			if (it->scancode == scancode)
				if ((*keyFunctions)[it->scancode].find(name) == (*keyFunctions)[it->scancode].end()) {
					(*keyFunctions)[it->scancode].emplace(name, func);
					it->addFunction(name);

					moveElements(it);
					found = true;
				}
	}

	/// \brief Déplace les panneaux à partir de it + 1 de 1
	/// \param it Iterateur vers le panneau avant ceux à déplacer
	/// \param up Vrai pour déplacer vers le haut
	void moveElements(std::list<KeyPanel>::iterator it, const bool& up = false, const unsigned& numElements = 1) {
		it++;
		int offset = (up ? -numElements : numElements) * (List::NODE_HEIGHT + List::SPACE_BETWEEN);
		background.h += offset;
		addKeyPanel.pos.y += offset;

		for (; it != keyPanels.end(); it++)
			it->pos.y += offset;
	}

	/// \return Le nom de la touche associé à scancode
	static const std::string& getKeyName(const unsigned int& scancode) {
		return std::find_if(keys.begin(), keys.end(), [&scancode](const std::pair<std::string, unsigned int> key) {
			return key.second == scancode;
		})->first;
	}

	/// \return Le scancode associé à name
	static const unsigned int& getKeyScancode(const std::string& name) {
		return std::find_if(keys.begin(), keys.end(), [&name](const std::pair<std::string, unsigned int> key) {
			return key.first == name;
		})->second;
	}

	/// \return Le nombre d'éléments dans la liste
	unsigned int getNodeNum() const {
		unsigned int num(0);

		for (const auto& keyPanel : keyPanels)
			num += 1 + keyPanel.funcs.size();

		return num;
	}

	/// \brief Retire les FunctionPanel associés à name
	void removeFunction(const std::string& name) {
		for (std::list<KeyPanel>::iterator it = keyPanels.begin(); it != keyPanels.end(); it++) {
			std::list<FunctionPanel>::iterator jt = std::find_if(it->funcs.begin(), it->funcs.end(), [&name](const FunctionPanel& funcPanel) {
				return funcPanel.name == name;
			});
			if (jt != it->funcs.end()) {
				it->moveElements(jt);
				moveElements(it, true);
				it->removePanel(jt->name);
				(*keyFunctions)[it->scancode].erase(jt->name);
				it->funcs.erase(jt);
			}
		}
	}

	/// \brief Notification de l'action en cours
	/// \param event Structure contenant l'information sur l'action de l'utilisateur
	void notify(SDL_Event* event) {
		if (event->type == SDL_MOUSEMOTION && dragged)
			dragged->move(event->motion.xrel, event->motion.yrel);
		else if (event->type == SDL_KEYDOWN) {
			unsigned scancode = event->key.keysym.scancode;

			if (scancode != SDL_SCANCODE_TAB) {
				if (std::find_if(keyPanels.begin(), keyPanels.end(), [&scancode](const KeyPanel& keyPanel) { return scancode == keyPanel.scancode; }) == keyPanels.end()) {
					keyPanels.emplace_back(std::to_string(keyPanels.size()) + getKeyName(scancode), scancode, getNodeNum(), *this);
					addPanel(keyPanels.back().name, &keyPanels.back());

					unsigned offset = List::NODE_HEIGHT + List::SPACE_BETWEEN;
					background.h += offset;
					addKeyPanel.pos.y += offset;
				}

				addKeyLabel.setStr("Add key");
				registered = true;
			}
		}
	}
};

const std::list<std::pair<std::string, unsigned int>> KeyList::keys = {
		{"A", 4},
		{"B", 5},
		{"C", 6},
		{"D", 7},
		{"E", 8},
		{"F", 9},
		{"G", 10},
		{"H", 11},
		{"I", 12},
		{"J", 13},
		{"K", 14},
		{"L", 15},
		{"M", 16},
		{"N", 17},
		{"O", 18},
		{"P", 19},
		{"Q", 20},
		{"R", 21},
		{"S", 22},
		{"T", 23},
		{"U", 24},
		{"V", 25},
		{"W", 26},
		{"X", 27},
		{"Y", 28},
		{"Z", 29},
		{"1", 30},
		{"2", 31},
		{"3", 32},
		{"4", 33},
		{"5", 34},
		{"6", 35},
		{"7", 36},
		{"8", 37},
		{"9", 38},
		{"0", 39},
		{"RETURN", 40},
		{"ESCAPE", 41},
		{"BACKSPACE", 42},
		{"TAB", 43},
		{"SPACE", 44},
		{"MINUS", 45},
		{"EQUALS", 46},
		{"LEFTBRACKET", 47},
		{"RIGHTBRACKET", 48},
		{"BACKSLASH", 49},
		{"NONUSHASH", 50},
		{"SEMICOLON", 51},
		{"APOSTROPHE", 52},
		{"GRAVE", 53},
		{"COMMA", 54},
		{"PERIOD", 55},
		{"SLASH", 56},
		{"CAPSLOCK", 57},
		{"F1", 58},
		{"F2", 59},
		{"F3", 60},
		{"F4", 61},
		{"F5", 62},
		{"F6", 63},
		{"F7", 64},
		{"F8", 65},
		{"F9", 66},
		{"F10", 67},
		{"F11", 68},
		{"F12", 69},
		{"PRINTSCREEN", 70},
		{"SCROLLLOCK", 71},
		{"PAUSE", 72},
		{"INSERT", 73},
		{"HOME", 74},
		{"PAGEUP", 75},
		{"DELETE", 76},
		{"END", 77},
		{"PAGEDOWN", 78},
		{"RIGHT", 79},
		{"LEFT", 80},
		{"DOWN", 81},
		{"UP", 82},
		{"LCTRL", 224},
		{"LSHIFT", 225},
		{"LALT", 226},
		{"LGUI", 227},
		{"RCTRL", 228},
		{"RSHIFT", 229},
		{"RALT", 230},
		{"RGUI", 231}
};

#endif //SOURCE_KEYLIST_H
